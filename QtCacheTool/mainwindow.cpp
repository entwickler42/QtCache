/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <qtcache.h>
#include <qtcacheui.h>
#include <qtcacheexception.h>
#include <cacheconnectiondialog.h>
#include <QCommandLineParser>
#include <QStringList>
#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>
#include <QRegExp>

QStringList operator << (QStringList ls, const QComboBox& cb)
{
    for(int i=0; i<cb.count(); i++){
        ls << cb.itemText(i);
    }
    return ls;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    conf(new QtCacheToolSettings(this)),
    dlg(new CacheConnectionDialog(this))
{
    ui->setupUi(this);
    ui->qspec->setText(conf->QSPEC());
    dlg->setUci(QStringList() << "%SYS");
    dlg->setUciEnabled(false);
    dlg->setFormat(CacheConnectionDialog::NAMESPACE_FLAG);
    dlg->load(conf->config());
    QString outputDirectory = conf->config()->value("DefaultExportDirectory", QDir::currentPath()).toString();
    if (!QDir(outputDirectory).exists()){
        outputDirectory = QDir::currentPath();
    }
    ui->outputDirectory->setText(outputDirectory);
    ui->includeFilter->addItems(loadFilters());

    ui->preImportHook->setText(conf->preImportHook());
    ui->enablePreImportHook->setChecked(!conf->preImportHook().isEmpty());

    ui->postImportHook->setText(conf->postImportHook());
    ui->enablePostImportHook->setChecked(!conf->postImportHook().isEmpty());

    parseCommandlineOptions();
    connect(QtCache::instance(), SIGNAL(reportProgress(QString,qint64,qint64)), this, SLOT(reportProgress(QString,qint64,qint64)));
}

MainWindow::~MainWindow()
{
    saveSettings();
    delete dlg;
    delete ui;
}

void MainWindow::saveSettings()
{
    conf->setPreImportHook(ui->enablePreImportHook->isChecked() ? ui->preImportHook->text() : "");
    conf->setPostImportHook(ui->enablePostImportHook->isChecked() ? ui->postImportHook->text() : "");
    if (!ui->targetUCI->currentText().isEmpty()){
        conf->setPreferedUCI(ui->targetUCI->currentText());
    }
}

void MainWindow::showEvent(QShowEvent*)
{
    restoreGeometry(conf->config()->value("MainWindow/Geometry").toByteArray());
    ui->statusBar->showMessage(tr("Welcome to the Qt Caché Tool"));
}

void MainWindow::closeEvent(QCloseEvent *)
{
    conf->config()->setValue("MainWindow/Geometry", saveGeometry());
}

void MainWindow::on_targetUCI_currentIndexChanged(const QString& text)
{
    cache()->setTargetUci(text);
    ui->statusBar->showMessage(tr("Selected target UCI: %1").arg(text));
}

void MainWindow::on_qspec_editingFinished()
{
    conf->config()->setValue("QSPEC", ui->qspec->text());
}

void MainWindow::on_selectServer_pressed()
{
    if (dlg->exec() == QDialog::Accepted){
        ui->connectionString->setText(dlg->connectionString());
        ui->targetUCI->clear();

        try{
            cache()->connect(
                        dlg->connectionString(),
                        dlg->username(),
                        dlg->password());
            dlg->save(conf->config());
            QStringList ls = cache()->listNamespaces(
                        conf->config()->value("ExcludePercentUCI", true).toBool());
            if (ls.count()){
                ui->targetUCI->clear();
                ui->targetUCI->addItems(ls);
                if (!conf->preferedUCI().isEmpty()){
                    int idx = ui->targetUCI->findText(conf->preferedUCI());
                    if(idx > -1) {
                        ui->targetUCI->setCurrentIndex(idx);
                    }
                }
            }else{
                throw QtCacheException(cache()->lastStatus());
            }
            ui->statusBar->showMessage(
                        cache()->isConnected() ?
                            tr("Sucessfully connected to %1").arg(dlg->server()) :
                            tr("Failed to connected to %1").arg(dlg->server())
                            );
            ui->connectionString->setToolTip(tr("JobId: %1").arg(cache()->jobId()));
        }catch(std::exception& ex){
            QMessageBox::critical(this, tr("Exception"), ex.what());
        }catch(...){
            QMessageBox::critical(this, tr("Exception"), tr("Unknown exception occured!"));
        }
    }
}

void MainWindow::on_addFiles_pressed()
{
    try{
        QFileDialog dlg;
        dlg.setDirectory(conf->config()->value("DefaultImportDirectory", QDir::currentPath()).toString());
        dlg.setNameFilters(QtCacheUi::defaultNameFilters());
        dlg.setFileMode(QFileDialog::ExistingFiles);
        if (dlg.exec() == QFileDialog::Accepted){
            foreach(const QString& s, dlg.selectedFiles()){
                loadImportFile(dlg.directory().absoluteFilePath(s));
            }
            conf->config()->setValue("DefaultImportDirectory", dlg.directory().absolutePath());
        }
    }catch(std::exception &ex){
        QMessageBox::critical(this, tr("Exception"), ex.what());
    }catch(...){
        QMessageBox::critical(this, tr("Exception"), tr("Unknown exception occured!"));
    }
}

void MainWindow::loadImportDirectory(const QString& path)
{
    foreach(const QString& i, QDir(path).entryList(QDir::Files)){
        loadImportFile(QDir(path).absoluteFilePath(i));
    }
}

void MainWindow::loadImportFile(const QString& filepath)
{
    QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
    item->setIcon(QIcon(":/QtCacheTool/ImportFile"));
    item->setText(filepath);
}

void MainWindow::on_removeFiles_pressed()
{
    if (ui->listWidget->selectedItems().count() == 0){
        QMessageBox::information(this, tr("Information"), tr("No items selected!"));
    }else{
        foreach(QListWidgetItem* i, ui->listWidget->selectedItems()){
            delete i;
        }
    }
}

void MainWindow::on_importFiles_pressed()
{
    if (!cache()->isConnected()){
        QMessageBox::information(this, tr("Information"), tr("Cachè connection has not been established yet!"));
        return;
    }
    if(ui->listWidget->count() == 0) {
        QMessageBox::information(this, tr("Information"), tr("List of files is empty!"));
        return;
    }
    foreach(QListWidgetItem* i, ui->listWidget->findItems("*", Qt::MatchWildcard)){
        i->setIcon(QIcon(":/QtCacheTool/ImportFile"));
        i->setToolTip("");
    }
    setBuisyUI();
    try{
        preImportHook();
        abortTask = false;
        ui->progressBar->setMaximum(ui->listWidget->count());

        QString qspec = ui->compile->isChecked() ? ui->qspec->text() : QString("");

        for(int i=0; !abortTask && i<ui->listWidget->count(); i++){
            QListWidgetItem* item = ui->listWidget->item(i);
            ui->statusBar->showMessage(tr("Importing %1").arg(item->text()));
            try{
                cache()->importObject(item->text(), qspec);
                item->setIcon(QIcon(":/QtCacheTool/ImportFileOk"));
                QCoreApplication::processEvents();
            }catch(std::exception& ex){
                QString err = tr("%1\n%2").arg(ex.what(), cache()->errorLog());
                item->setToolTip(err);
                item->setIcon(QIcon(":/QtCacheTool/ImportFileError"));
                if (!ui->ignoreImportErrors->isChecked()){
                    int rval = QMessageBox::warning(this, tr("Exception"), err,
                                                    QMessageBox::Cancel|QMessageBox::Ignore,
                                                    QMessageBox::Ignore);
                    abortTask = rval == QMessageBox::Cancel;
                }
            }
            ui->progressBar->setValue(i+1);
        }
        postImportHook();
        if (abortTask){
            ui->statusBar->showMessage(tr("Import aborted!"));
            ui->progressBar->setValue(0);
        }else{
            ui->statusBar->showMessage(tr("Import finished!"));
        }
    }catch(std::exception& ex){
        ui->statusBar->showMessage(tr("Import failed!"));
        QMessageBox::critical(this, tr("Exception"), ex.what());
    }
    setIdleUI();
}

void MainWindow::preImportHook()
{
    if (ui->enablePreImportHook->isChecked() && !ui->preImportHook->text().isEmpty()){
        try{
            cache()->execute(ui->preImportHook->text());
        }catch(std::exception& ex){
            int rval = QMessageBox::warning(this,
                                            tr("Exception"),
                                            tr("Failed to execute pre import hook:\n\n%1").arg(ex.what()),
                                            QMessageBox::Cancel|QMessageBox::Ignore,
                                            QMessageBox::Ignore);
            if (rval == QMessageBox::Cancel) throw;
        }
    }
}

void MainWindow::postImportHook()
{
    if (ui->enablePostImportHook->isChecked() && !ui->postImportHook->text().isEmpty()){
        try{
            cache()->execute(ui->postImportHook->text());
        }catch(std::exception& ex){
            int rval = QMessageBox::warning(this,
                                            tr("Exception"),
                                            tr("Failed to execute post import hook:\n\n%1").arg(ex.what()),
                                            QMessageBox::Cancel|QMessageBox::Ignore,
                                            QMessageBox::Ignore);
            if (rval == QMessageBox::Cancel) throw;
        }
    }
}

void MainWindow::on_abortTask_pressed()
{
    abortTask = true;
}

void MainWindow::on_selectOutputDirectory_pressed()
{
    QFileDialog dlg;
    dlg.setDirectory(conf->config()->value("DefaultExportDirectory", QDir::currentPath()).toString());
    dlg.setOption(QFileDialog::ShowDirsOnly);
    dlg.setFileMode(QFileDialog::Directory);
    if (dlg.exec() == QDialog::Accepted){
        ui->outputDirectory->setText(dlg.directory().absolutePath());
        conf->config()->setValue("DefaultExportDirectory", dlg.directory().absolutePath());
    }
}

void MainWindow::on_exportFiles_pressed()
{
    if (!cache()->isConnected()){
        QMessageBox::information(this, tr("Information"), tr("Cachè connection has not been established yet!"));
        return;
    }
    setBuisyUI();
    try{
        ui->statusBar->showMessage(tr("Receiving list of cachè objects..."));
        QString filter = ui->includeFilterEnabled->isChecked() ? ui->includeFilter->currentText() : "";
        QStringList ls = cache()->listObjects(filter);
        ui->progressBar->setMaximum(ls.count());
        abortTask = false;
        for(int i=0; !abortTask && i<ls.count(); i++){
            const QString& s = ls.at(i);
            try{
                ui->statusBar->showMessage(tr("Exporting %1").arg(s));
                ui->progressBar->setValue(i+1);
                QCoreApplication::processEvents();
                cache()->exportObject(ui->outputDirectory->text(), s);
            }catch(std::exception& ex){
                PML::LOG << tr("Failed to export %1\n%2").arg(s, ex.what());
                if (!ui->ignoreExportErrors){
                    QString err = tr("%1\n%2").arg(ex.what(), cache()->errorLog());
                    int rval = QMessageBox::warning(this, tr("Exception"),err,
                                                    QMessageBox::Cancel|QMessageBox::Ignore,
                                                    QMessageBox::Ignore);
                    abortTask = rval != QMessageBox::Ignore;
                }
            }
        }
        if (abortTask){
            ui->statusBar->showMessage(tr("Export aborted!"));
            ui->progressBar->setValue(0);
        }else{
            ui->statusBar->showMessage(tr("Export finished!"));
        }
    }catch(std::exception& ex){
        QMessageBox::critical(this, tr("Exception"), ex.what());
    }
    setIdleUI();
}

void MainWindow::on_saveCurrentFilter_pressed()
{
    int index = ui->includeFilter->currentIndex();
    if (ui->includeFilter->currentText() != ui->includeFilter->itemText(index)){
        ui->includeFilter->addItem(ui->includeFilter->currentText());
    }
    saveFilters(QStringList() << *ui->includeFilter);
}

void MainWindow::on_removeCurrentFilter_pressed()
{
    int index = ui->includeFilter->currentIndex();
    if (QMessageBox::question(this,
                              tr("conf->config()irm delete"),
                              tr("Do you really want to remove this filter from the list?\n\n%1").arg(
                                  ui->includeFilter->currentText()
                                  )
                              ) != QMessageBox::Yes){
        return;
    }
    if (ui->includeFilter->currentText() != ui->includeFilter->itemText(index)){
        ui->includeFilter->setCurrentIndex(index);
    }else{
        ui->includeFilter->removeItem(index);
        saveFilters(QStringList() << *ui->includeFilter);
    }
}

void MainWindow::reportProgress(const QString& message, qint64 pos, qint64 end)
{
    ui->progressBar2->setMaximum(end);
    ui->progressBar2->setValue(pos);
    ui->statusBar->showMessage(message);
    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
}

QStringList MainWindow::loadFilters() const
{
    QStringList ls;
    int count = conf->config()->beginReadArray("ExportFilter");
    for(int i=0; i<count; i++){
        conf->config()->setArrayIndex(i);
        ls << conf->config()->value("RegExp").toString();
    }
    conf->config()->endArray();
    return ls;
}

void MainWindow::saveFilters(const QStringList& ls) const
{
    conf->config()->remove("ExportFilter");
    conf->config()->beginWriteArray("ExportFilter");
    for(int i=0; i<ls.count(); i++){
        conf->config()->setArrayIndex(i);
        conf->config()->setValue("RegExp", ls.at(i));
    }
    conf->config()->endArray();
}

void MainWindow::parseCommandlineOptions()
{
    QCommandLineParser p;
    p.addHelpOption();
    p.addVersionOption();
    QCommandLineOption server("s", tr("Cachè instance to connect to"), "Server", "127.0.0.1");
    p.addOption(server);
    QCommandLineOption port("p", tr("Cachè instance TCP port"), "Port", "1972");
    p.addOption(port);
    QCommandLineOption uci("n", tr("Cachè namespace to work on"), "UCI", "USER");
    p.addOption(uci);
    QCommandLineOption username("u", tr("Username used for authentication"), "Username");
    p.addOption(username);
    QCommandLineOption password("p", tr("Password used for authentication"), "Password");
    p.addOption(password);
    QCommandLineOption compile("c", tr("Compile imported objects"));
    p.addOption(compile);
    QCommandLineOption compileFlags("cf", tr("Compile flags"), "flags");
    p.addOption(compileFlags);
    QCommandLineOption importDirectory("i", tr("Directory of Cachè Object to be imported"), "ImportDirectory");
    p.addOption(importDirectory);
    QCommandLineOption outputDirectory("e", tr("Directory used for the Cachè Object export"), "OutputDirectory");
    p.addOption(outputDirectory);
    QCommandLineOption objectFilter("f", tr("Object filter used when exporting"), "Filter", "(?i).+int$;(?i).+mac$;(?i).+cls$");
    p.addOption(objectFilter);
    QCommandLineOption preImportHook("pre-import-hook", tr("Cachè Object script to be executed before an import"), "COS");
    p.addOption(preImportHook);
    QCommandLineOption postImportHook("post-import-hook", tr("Cachè Object script to be executed after an import"), "COS");
    p.addOption(postImportHook);

    p.process(QApplication::instance()->arguments());

    if (p.isSet(uci)){
        conf->setPreferedUCI(p.value(uci));
    }
    if (p.isSet(compile)){
        ui->compile->setChecked(true);
    }
    if (p.isSet(compileFlags)){
        ui->qspec->setText(p.value(compileFlags));
    }
    if (p.isSet(importDirectory)){
        foreach(const QString& i, p.values(importDirectory)){
            loadImportDirectory(i);
        }
    }
    if(p.isSet(outputDirectory)){
        ui->outputDirectory->setText(p.value(outputDirectory));
    }
    if(p.isSet(preImportHook)){
        ui->preImportHook->setText(p.value(preImportHook));
        ui->enablePreImportHook->setChecked(true);
    }
    if(p.isSet(postImportHook)){
        ui->postImportHook->setText(p.value(postImportHook));
        ui->enablePostImportHook->setChecked(true);
    }
}

void MainWindow::setBuisyUI()
{
    ui->targetUCI->setEnabled(false);
    ui->addFiles->setEnabled(false);
    ui->removeFiles->setEnabled(false);
    ui->selectServer->setEnabled(false);
    ui->tabExport->setEnabled(false);
    ui->tabImport->setEnabled(false);
    ui->abortTask->setEnabled(true);
}

void MainWindow::setIdleUI() {
    ui->targetUCI->setEnabled(true);
    ui->addFiles->setEnabled(true); ui->removeFiles->setEnabled(true);
    ui->selectServer->setEnabled(true); ui->tabExport->setEnabled(true);
    ui->tabImport->setEnabled(true); ui->abortTask->setEnabled(false);
}

