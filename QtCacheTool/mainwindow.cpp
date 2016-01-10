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
#include <qtcachepluginobserver.h>
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
    dlg->setUciEnabled(false);
    dlg->setFormat(QtC::CacheConnectionDialog::NAMESPACE_FLAG);

    ui->outputDirectory->setText(conf->DefaultExportDirectory());
    ui->includeFilter->addItems(loadFilters());

    loadSettings();
    parseCommandlineOptions();

    connect(QtC::QtCache::instance(), SIGNAL(progress(QtC::Progress&)), this, SLOT(cacheProgress(QtC::Progress&)));

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

    for(int i=0; i<cache()->pluginObserver()->plugins().count(); i++){
        QtC::Plugin* plugin = cache()->pluginObserver()->plugins().at(i);
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
    dlg->load(conf->config());
}

void MainWindow::saveSettings()
{
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
        dlg->save(conf->config());
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
        i->setIcon(QIcon(":/QtCacheTool/FILE_IDLE"));
        i->setToolTip("");
    }
    setBuisyUI();
    abortTask = false;
    bulk_import_active = true;
    try{
        preImportHook();

        QString qspec = ui->compile->isChecked() ? ui->qspec->text() : QString("");
        QStringList import_files;
        for(int i=0; i<ui->listWidget->count(); i++){
            QListWidgetItem* item = ui->listWidget->item(i);
            import_files.append(item->text());
        }

        QtC::BulkImport import(cache());
        connect(&import, SIGNAL(aborted()), this, SLOT(bulkImportAborted()));
        connect(&import, SIGNAL(finished()), this, SLOT(bulkImportFinished()));
        connect(&import, SIGNAL(error(std::exception&, QtC::Progress&)), this, SLOT(bulkImportError(std::exception&, QtC::Progress&)));
        connect(&import, SIGNAL(progress(QtC::Progress&)), this, SLOT(bulkImportProgress(QtC::Progress&)));
        import.compileEarly = ui->compileEarly->isChecked();
        import.load(import_files, qspec);

        postImportHook();
    }catch(std::exception& ex){
        ui->statusBar->showMessage(tr("Import failed!"));
        QMessageBox::critical(this, tr("Exception"), ex.what());
    }
    bulk_import_active = false;
    setIdleUI();
}

void MainWindow::bulkImportAborted()
{
    ui->statusBar->showMessage(tr("Bulkimport was aborted!"));
}

void MainWindow::bulkImportFinished()
{
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(0);
    ui->statusBar->showMessage(tr("Bulkimport has finished!"));
}

void MainWindow::bulkImportError(std::exception& ex, QtC::Progress& p)
{
    bulkImportProgress(p);
    setListViewItem(p.tag().toString(), ":/QtCacheTool/FILE_ERROR", ex.what());
    if (!ui->ignoreImportErrors->isChecked()){
        int rval = QMessageBox::warning(this, tr("Exception"), ex.what(),
                                        QMessageBox::Cancel|QMessageBox::Ignore,
                                        QMessageBox::Ignore);
        abortTask = rval == QMessageBox::Cancel;
    }
}

void MainWindow::bulkImportProgress(QtC::Progress& p)
{
    bool process_events = ui->progressBar->value() != p.percent();

    QStringList tags;
    switch (p.type()) {
    case QtC::Progress::BULK_READ:
        setListViewItem(p.tag().toString(), ":/QtCacheTool/FILE_OK");
        ui->statusBar->showMessage(tr("Reading %1").arg(p.tag().toString()));
        break;
    case QtC::Progress::BULK_UPLOAD:
        setListViewItem(p.tag().toString(), ":/QtCacheTool/FILE_OK");
        ui->statusBar->showMessage(tr("Uploading %1").arg(p.tag().toString()));
        break;
    case QtC::Progress::BULK_COMPILE:
        tags = p.tag().toStringList();
        setListViewItem(tags.at(1), ":/QtCacheTool/FILE_OK");
        ui->statusBar->showMessage(tr("Compiling %1").arg(tags.at(0)));
        break;
    }
    if(abortTask){
        QtC::BulkImport* import = static_cast<QtC::BulkImport*>(sender());
        import->abort();
    }

    if (process_events){
        ui->progressBar->setMaximum(100);
        ui->progressBar->setValue(p.percent());
        QApplication::processEvents();
    }
}

void MainWindow::cacheProgress(QtC::Progress& p)
{
    bool process_events = ui->progressBar->value() != p.percent();

    if (!bulk_import_active){
        QStringList tags;
        switch(p.type()){
        case QtC::Progress::CONNECT:
            ui->statusBar->showMessage(tr("Connecting Caché"));
            break;
        case QtC::Progress::DISCONNECT:
            ui->statusBar->showMessage("Disconnecting Caché");
            break;
        case QtC::Progress::QUERY_NS:
            ui->statusBar->showMessage(tr("Receiving namespace list"));
            break;
        case QtC::Progress::QUERY_OBJECTS:
            ui->statusBar->showMessage(tr("Receiving object list"));
            break;
        case QtC::Progress::OBJECT_COMPILE:
            tags = p.tag().toStringList();
            setListViewItem(tags.at(0), ":/QtCacheTool/FILE_OK");
            ui->statusBar->showMessage(tr("Compiling %1").arg(tags.at(0)));
            break;
        case QtC::Progress::XMLFILE_IMPORT:
            tags = p.tag().toStringList();
            if (p.percent() == 0){
                setListViewItem(tags.at(0), ":/QtCacheTool/FILE_OK");
            }
            ui->statusBar->showMessage(tr("Importing %1").arg(tags.at(0)));
            break;
        case QtC::Progress::XMLFILE_EXPORT:
            tags = p.tag().toStringList();
            if (p.percent() == 0){
                setListViewItem(tags.at(0), ":/QtCacheTool/FILE_OK");
            }
            ui->statusBar->showMessage(tr("Exporting %1").arg(tags.at(0)));
            break;
        default:
            ui->statusBar->showMessage(tr("Idle"));
            break;
        }
        process_events = true;
    }

    if (process_events){
        ui->progressBar->setMaximum(100);
        ui->progressBar->setValue(p.percent());
        QApplication::processEvents();
    }
}

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
    abortTask = true;
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
        QtC::QtCache::ObjectFilterType filterType =
                ui->regularExpression->isChecked() ? QtC::QtCache::REGEXP : QtC::QtCache::PATTERN;
        QStringList ls = cache()->listObjects(filter, filterType);
        ui->progressBar->setMaximum(ls.count());
        abortTask = false;
        for(int i=0; !abortTask && i<ls.count(); i++){
            const QString& s = ls.at(i);
            try{
                ui->statusBar->showMessage(tr("Exporting %1").arg(s));
                ui->progressBar->setValue(i+1);
                QCoreApplication::processEvents();
                cache()->exportXmlFile(ui->outputDirectory->text(), s);
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
        saveFilters(QStringList() << *ui->includeFilter);
    }
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
    if(p.isSet(autoConnect)){
        conf->setAutoConnect(true);
    }
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

