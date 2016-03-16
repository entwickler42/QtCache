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
#include <qtcachebulkimport.h>
#include <qtcachebulkexport.h>
#include <qtcacheplugindirector.h>
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
    dlg(new QtC::CacheConnectionDialog(this))
{
    ui->setupUi(this);
    dlg->setUci(QStringList() << "%SYS");
    dlg->setUciVisible(false);
    dlg->setFormat(QtC::CacheConnectionDialog::NAMESPACE_FLAG);

    ui->outputDirectory->setText(conf->DefaultExportDirectory());
    ui->includeFilter->addItems(loadObjectFilters());
    ui->listWidgetContentFilter->addItems(loadContentFilters());

    subscripe(QtC::QtCache::instance());

    loadSettings();
    parseCommandlineOptions();

    if(conf->AutoConnect()){
        try{
            cache()->connect(
                        dlg->connectionString(),
                        dlg->username(),
                        dlg->password());
            onServerConnected();
        }catch(std::exception& ex){
            QMessageBox::critical(this, tr("Exception"), ex.what());
        }
    }

    createPluginTable();
}

MainWindow::~MainWindow()
{
    saveSettings();
    delete dlg;
    delete ui;
}

void MainWindow::createPluginTable()
{
    const int ColumnName = 0;
    const int ColumnDesc = 1;

    ui->tablePlugins->insertColumn(ColumnName);
    ui->tablePlugins->insertColumn(ColumnDesc);

    ui->tablePlugins->setHorizontalHeaderItem(ColumnName, new QTableWidgetItem("Name"));
    ui->tablePlugins->setHorizontalHeaderItem(ColumnDesc, new QTableWidgetItem("Beschreibung"));

    const QList<QtC::Plugin*>& plugins = cache()->plugins()->loaded();
    for(int i=0; i<plugins.count(); i++){
        QtC::Plugin* plugin = plugins.at(i);
        ui->tablePlugins->insertRow(i);
        QTableWidgetItem* nameItem = new QTableWidgetItem(plugin->name());
        QTableWidgetItem* descItem = new QTableWidgetItem(plugin->description());
        //!\todo actually toggle plugins enabled
        //  nameItem->setCheckState(Qt::Checked);
        nameItem->setFlags(nameItem->flags() ^ Qt::ItemIsEditable);
        descItem->setFlags(descItem->flags() ^ Qt::ItemIsEditable);
        ui->tablePlugins->setItem(i, ColumnName, nameItem);
        ui->tablePlugins->setItem(i, ColumnDesc, descItem);
    }
    ui->tablePlugins->resizeColumnsToContents();
    ui->tablePlugins->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::loadSettings()
{
    cache()->plugins()->onLoadApplicationSettingsBegin(*conf->config());
    ui->compileEarly->setChecked(conf->CompileEarly());
    ui->compile->setChecked(conf->Compile());
    ui->qspec->setText(conf->QSPEC());
    QString outputDirectory = conf->DefaultExportDirectory();
    if (!QDir(outputDirectory).exists()){
        conf->setDefaultExportDirectory(QDir::currentPath());
    }
    ui->preImportHook->setText(conf->PreImportHook());
    ui->enablePreImportHook->setChecked(!conf->PreImportHook().isEmpty());
    ui->postImportHook->setText(conf->PostImportHook());
    ui->enablePostImportHook->setChecked(!conf->PostImportHook().isEmpty());
    dlg->load(conf);
    cache()->plugins()->onLoadApplicationSettingsEnd(*conf->config());
}

void MainWindow::saveSettings()
{
    cache()->plugins()->onSaveApplicationSettingsBegin(*conf->config());
    conf->setCompileEarly(ui->compileEarly->isChecked());
    conf->setCompile(ui->compile->isChecked());
    conf->setQSPEC(ui->qspec->text());
    conf->setPreImportHook(ui->enablePreImportHook->isChecked() ? ui->preImportHook->text() : "");
    conf->setPostImportHook(ui->enablePostImportHook->isChecked() ? ui->postImportHook->text() : "");
    if (!ui->targetUCI->currentText().isEmpty()){
        conf->setPreferedUCI(ui->targetUCI->currentText());
    }
    conf->setPostImportHook(ui->postImportHook->text());
    conf->setPreImportHook(ui->preImportHook->text());
    cache()->plugins()->onSaveApplicationSettingsEnd(*conf->config());
}

void MainWindow::showEvent(QShowEvent*)
{
    restoreGeometry(conf->config()->value("MainWindow/Geometry").toByteArray());
    ui->statusBar->showMessage(tr("Welcome to the Qt Caché Tool"));
    if (cache()->isConnected()){
        ui->statusBar->showMessage(tr("Connection established!"));
    }
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

void MainWindow::onServerConnected()
{
    try{
        ui->connectionString->setText(dlg->connectionString());
        ui->targetUCI->clear();
        dlg->save(conf);
        QStringList ls = cache()->listNamespaces(
                    conf->config()->value("ExcludePercentUCI", true).toBool());
        if (ls.count()){
            ui->targetUCI->clear();
            ui->targetUCI->addItems(ls);
            if (!conf->PreferedUCI().isEmpty()){
                int idx = ui->targetUCI->findText(conf->PreferedUCI());
                if(idx > -1) {
                    ui->targetUCI->setCurrentIndex(idx);
                }
            }
        }else{
            throw QtC::QtCacheException(cache()->lastStatus());
        }
        ui->statusBar->showMessage(
                    cache()->isConnected() ?
                        tr("Sucessfully connected to %1").arg(dlg->server()) :
                        tr("Failed to connected to %1").arg(dlg->server())
                        );
        ui->connectionString->setToolTip(tr("JobId: %1").arg(cache()->jobId()));
    }catch(std::exception& ex){
        QMessageBox::critical(this, tr("Exception"), ex.what());
    }
}

void MainWindow::on_selectServer_pressed()
{
    if (dlg->exec() == QDialog::Accepted){
        try{
            cache()->connect(
                        dlg->connectionString(),
                        dlg->username(),
                        dlg->password());
            onServerConnected();
        }catch(std::exception& ex){
            QMessageBox::critical(this, tr("Exception"), ex.what());
        }
    }
}

void MainWindow::on_addFiles_pressed()
{
    try{
        QFileDialog dlg;
        dlg.setDirectory(conf->config()->value("DefaultImportDirectory", QDir::currentPath()).toString());
        dlg.setNameFilters(QtC::QtCacheUi::defaultNameFilters());
        dlg.setFileMode(QFileDialog::ExistingFiles);
        if (dlg.exec() == QFileDialog::Accepted){
            foreach(const QString& s, dlg.selectedFiles()){
                loadImportFile(dlg.directory().absoluteFilePath(s));
            }
            conf->config()->setValue("DefaultImportDirectory", dlg.directory().absolutePath());
        }
    }catch(std::exception &ex){
        QMessageBox::critical(this, tr("Exception"), ex.what());
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
    item->setIcon(QIcon(":/QtCacheTool/FILE_IDLE"));
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

void MainWindow::runInteractive(QtC::BulkAction* bulkop)
{
    if (!cache()->isConnected()){
        QMessageBox::information(this, tr("Information"), tr("Cachè connection has not been established yet!"));
        return;
    }

    setBuisyUI();
    m_abort_task = false;

    subscripe(bulkop);
    connect(bulkop, SIGNAL(aborted()), this, SLOT(onBulkActionAborted()));
    connect(bulkop, SIGNAL(finished()), this, SLOT(onBulkActionFinished()));

    try{
        bulkop->run();
    }catch(std::exception& ex){
        LOG_EXCEPTION(ex);
        ui->statusBar->showMessage(tr("Export failed!"));
        QMessageBox::critical(this, tr("Exception"), ex.what());
    }catch(...){
        LOG_UNKNOWN_EXCEPTION;
        ui->statusBar->showMessage(tr("Export failed!"));
        QMessageBox::critical(this, tr("Exception"),tr("Unknown Error..."));
    }

    subscripe(bulkop);
    disconnect(bulkop, SIGNAL(aborted()), this, SLOT(onBulkActionAborted()));
    disconnect(bulkop, SIGNAL(finished()), this, SLOT(onBulkActionFinished()));

    setIdleUI();
}

void MainWindow::on_exportFiles_pressed()
{
    QtC::BulkExport bulkop(cache());
    QString filter = ui->includeFilterEnabled->isChecked() ? ui->includeFilter->currentText() : "";
    QtC::QtCache::ObjectFilterType filterType = ui->regularExpression->isChecked() ? QtC::QtCache::REGEXP : QtC::QtCache::PATTERN;
    bulkop.filter = filter;
    bulkop.filterType = filterType;
    bulkop.contentFilter = loadContentFilters();
    bulkop.outputDirectory = QDir(ui->outputDirectory->text());
    runInteractive(&bulkop);
}

void MainWindow::on_importFiles_pressed()
{
    if(ui->listWidget->count() == 0) {
        QMessageBox::information(this, tr("Information"), tr("List of files is empty!"));
        return;
    }
    foreach(QListWidgetItem* i, ui->listWidget->findItems("*", Qt::MatchWildcard)){
        i->setIcon(QIcon(":/QtCacheTool/FILE_IDLE"));
        i->setToolTip("");
    }

    preImportHook();

    QtC::BulkImport bulkop(cache());
    QString qspec = ui->compile->isChecked() ? ui->qspec->text() : QString("");
    QStringList filepaths;
    for(int i=0; i<ui->listWidget->count(); i++){
        QListWidgetItem* item = ui->listWidget->item(i);
        filepaths.append(item->text());
    }
    bulkop.compileEarly = ui->compileEarly->isChecked();
    bulkop.filepaths = filepaths;
    bulkop.qspec = qspec;
    runInteractive(&bulkop);

    postImportHook();
}

void MainWindow::onBulkActionAborted()
{
    ui->statusBar->showMessage(tr("Bulkimport was aborted!"));
}

void MainWindow::onBulkActionFinished()
{
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(0);
    ui->statusBar->showMessage(tr("Bulkimport has finished!"));
}

void MainWindow::onProgressError(std::exception& ex, QtC::Progress& prog)
{
    setListViewItem(prog.tag().toString(), ":/QtCacheTool/FILE_ERROR", ex.what());
    if (!m_ignore_errors){
        int rval = QMessageBox::warning(this, tr("Exception"), ex.what(),
                                        QMessageBox::Cancel|QMessageBox::Ignore,
                                        QMessageBox::Ignore);
        m_abort_task = rval == QMessageBox::Cancel;
    }

    QApplication::processEvents();
}

void MainWindow::onProgressBegin(QtC::Progress& prog)
{
    switch(prog.type()){
    case QtC::Progress::CONNECT:
        ui->statusBar->showMessage(tr("Connecting Caché"));
        break;
    case QtC::Progress::DISCONNECT:
        ui->statusBar->showMessage("Disconnecting Caché...");
        break;
    case QtC::Progress::QUERY_NS:
        ui->statusBar->showMessage(tr("Receiving namespace list..."));
        break;
    case QtC::Progress::QUERY_OBJECTS:
        ui->statusBar->showMessage(tr("Receiving object list..."));
        break;
    default:
        return;
    }

    if(m_abort_task){
        QtC::BulkAction* bulkop = qobject_cast<QtC::BulkAction*>(sender());
        if(bulkop) { bulkop->abort(); }
        prog.abort();
    }

    QApplication::processEvents();
}

void MainWindow::onProgress(QtC::Progress& prog)
{
    QStringList tags;
    switch (prog.type()) {
    case QtC::Progress::BULK_SAVE:
        tags = prog.tag().toStringList();
        ui->statusBar->showMessage(tr("Exporing %1").arg(tags.at(1)));
        break;
    case QtC::Progress::BULK_READ:
        setListViewItem(prog.tag().toString(), ":/QtCacheTool/FILE_OK");
        ui->statusBar->showMessage(tr("Reading %1").arg(prog.tag().toString()));
        break;
    case QtC::Progress::BULK_UPLOAD:
        setListViewItem(prog.tag().toString(), ":/QtCacheTool/FILE_OK");
        ui->statusBar->showMessage(tr("Uploading %1").arg(prog.tag().toString()));
        break;
    case QtC::Progress::BULK_COMPILE:
        tags = prog.tag().toStringList();
        setListViewItem(tags.at(1), ":/QtCacheTool/FILE_OK");
        ui->statusBar->showMessage(tr("Compiling %1").arg(tags.at(0)));
        break;
    case QtC::Progress::BULK_COMPILE_EARLY:
        tags = prog.tag().toStringList();
        setListViewItem(tags.at(1), ":/QtCacheTool/FILE_OK");
        ui->statusBar->showMessage(tr("Uploading and compiling %1").arg(tags.at(0)));
        break;
    default:
        break;
    }

    if(m_abort_task){
        QtC::BulkAction* bulkop = qobject_cast<QtC::BulkAction*>(sender());
        if(bulkop) { bulkop->abort(); }
        prog.abort();
    }

    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(prog.percent());
    QApplication::processEvents();
}

void MainWindow::onProgressEnd(QtC::Progress&)
{}

void MainWindow::setListViewItem(const QString& filename, const QString& iconpath, const QString& toolTip)
{
    QList<QListWidgetItem*> items = ui->listWidget->findItems(filename, Qt::MatchExactly);
    foreach(QListWidgetItem* i, items) {
        i->setToolTip(toolTip);
        i->setIcon(QIcon(iconpath));
        ui->listWidget->scrollToItem(i);
    }
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
    m_abort_task = true;
}

void MainWindow::on_selectOutputDirectory_pressed()
{
    QFileDialog dlg;
    dlg.setDirectory(conf->DefaultExportDirectory());
    dlg.setOption(QFileDialog::ShowDirsOnly);
    dlg.setFileMode(QFileDialog::Directory);
    if (dlg.exec() == QDialog::Accepted){
        ui->outputDirectory->setText(dlg.directory().absolutePath());
        conf->setDefaultExportDirectory(dlg.directory().absolutePath());
    }
}

void MainWindow::on_saveCurrentFilter_pressed()
{
    int index = ui->includeFilter->currentIndex();
    if (ui->includeFilter->currentText() != ui->includeFilter->itemText(index)){
        ui->includeFilter->addItem(ui->includeFilter->currentText());
    }
    saveObjectFilters(QStringList() << *ui->includeFilter);
}

void MainWindow::on_removeCurrentFilter_pressed()
{
    int index = ui->includeFilter->currentIndex();
    if (QMessageBox::question(this,
                              tr("conf->config() delete"),
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
        saveObjectFilters(QStringList() << *ui->includeFilter);
    }
}

QStringList MainWindow::loadObjectFilters() const
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

void MainWindow::saveObjectFilters(const QStringList& ls) const
{
    conf->config()->remove("ExportFilter");
    conf->config()->beginWriteArray("ExportFilter");
    for(int i=0; i<ls.count(); i++){
        conf->config()->setArrayIndex(i);
        conf->config()->setValue("RegExp", ls.at(i));
    }
    conf->config()->endArray();
}


QStringList MainWindow::loadContentFilters() const
{
    QStringList ls;
    int count = conf->config()->beginReadArray("ContentFilter");
    for(int i=0; i<count; i++){
        conf->config()->setArrayIndex(i);
        ls << conf->config()->value("RegExp").toString();
    }
    conf->config()->endArray();
    return ls;
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
    QCommandLineOption autoConnect("a", tr("Automatically connect during startup"));
    p.addOption(autoConnect);
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

    cache()->plugins()->onParseCommandlineOptionsBegin(p);

    p.process(QApplication::instance()->arguments());

    if (p.isSet(uci)){
        conf->setPreferedUCI(p.value(uci));
    }
    if(p.isSet(autoConnect)){
        conf->setAutoConnect(true);
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

    cache()->plugins()->onParseCommandlineOptionsEnd(p);
}

void MainWindow::setBuisyUI()
{
    ui->targetUCI->setEnabled(false);
    ui->addFiles->setEnabled(false);
    ui->removeFiles->setEnabled(false);
    ui->importFiles->setEnabled(false);
    ui->selectServer->setEnabled(false);
    ui->tabExport->setEnabled(false);
    ui->abortTask->setEnabled(true);
}

void MainWindow::setIdleUI() {
    ui->targetUCI->setEnabled(true);
    ui->addFiles->setEnabled(true);
    ui->removeFiles->setEnabled(true);
    ui->importFiles->setEnabled(true);
    ui->selectServer->setEnabled(true);
    ui->tabExport->setEnabled(true);
    ui->abortTask->setEnabled(false);

}

