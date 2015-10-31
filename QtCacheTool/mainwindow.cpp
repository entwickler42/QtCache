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
#include <qtcacheexception.h>
#include <cacheconnectiondialog.h>
#include <QMessageBox>
#include <QFileDialog>
#include <QThread>
#include <QSettings>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    conf(new QSettings("QtCacheTool.ini", QSettings::IniFormat)),
    dlg(new CacheConnectionDialog(this))
{
    ui->setupUi(this);

    dlg->setUci(QStringList() << "%SYS");
    dlg->setUciEnabled(false);
    dlg->setFormat(CacheConnectionDialog::NAMESPACE_FLAG);
    dlg->load(conf);
}

MainWindow::~MainWindow()
{
    delete dlg;
    delete ui;
}

void MainWindow::showEvent(QShowEvent*)
{
    ui->statusBar->showMessage("Welcome to the Qt Caché Tool");
}

void MainWindow::on_selectServer_pressed()
{
    if (dlg->exec() == QDialog::Accepted){
        ui->connectionString->setText(dlg->connectionString());
        ui->uci->clear();
        ui->uci->addItem(dlg->uci());
    }
    try{
        cache()->connect(
                    dlg->connectionString(),
                    dlg->username(),
                    dlg->password());
        dlg->save(conf);
        QStringList ls = cache()->listNamespaces();
        if (ls.count()){
            ui->uci->clear();
            ui->uci->addItems(ls);
        }else{
            throw std::exception(qPrintable(cache()->lastStatus()));
        }
    }catch(std::exception& ex){
        QMessageBox::critical(this, tr("Exception"), ex.what());
    }catch(...){
        QMessageBox::critical(this, tr("Exception"), tr("Unknown exception occured!"));
    }

    ui->statusBar->showMessage(
                cache()->isConnected() ?
                    tr("Sucessfully connected to %1").arg(dlg->server()) :
                    tr("Failed to connected to %1").arg(dlg->server())
                    );
}

void MainWindow::on_addFiles_pressed()
{
    try{
        QFileDialog dlg;
        dlg.setDirectory(conf->value("DefaultImportDirectory", QDir::currentPath()).toString());
        dlg.setNameFilters(QStringList()
                           << "Caché Export (*.xml *.mac *.int *.cls)"
                           << "Any Files (*)");
        dlg.setFileMode(QFileDialog::ExistingFiles);
        if (dlg.exec() == QFileDialog::Accepted){
            ui->listWidget->addItems(dlg.selectedFiles());
            conf->setValue("DefaultImportDirectory", dlg.directory().absolutePath());
        }
    }catch(std::exception &ex){
        QMessageBox::critical(this, tr("Exception"), ex.what());
    }catch(...){
        QMessageBox::critical(this, tr("Exception"), tr("Unknown exception occured!"));
    }
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
    setBuisyUI();
    try{
        QString qspec = ui->compile->isChecked() ? ui->qspec->text() : QString("");
        abortImort = false;
        ui->progressBar->setMaximum(ui->listWidget->count());
        for(int i=0; !abortImort && i<ui->listWidget->count(); i++){
            QListWidgetItem* item = ui->listWidget->item(i);
            ui->statusBar->showMessage(tr("Importing %1").arg(item->text()));
            ui->progressBar->setValue(i+1);
            try{
                cache()->importFile(ui->uci->currentText(), item->text(), qspec);
            }catch(QtCacheException& ex){
                QMessageBox::critical(this,
                                      tr("Exception"),
                                      tr("%1\n%2").arg(ex.what(), cache()->errorLog()));
            }
            QCoreApplication::processEvents();
        }
        if (abortImort){
            ui->statusBar->showMessage(tr("Import aborted!"));
            ui->progressBar->setValue(0);
        }else{
            ui->statusBar->showMessage(tr("Import finished!"));
        }
    }catch(std::exception& ex){
        QMessageBox::critical(this, tr("Exception"), ex.what());
    }
    setIdleUI();
}

void MainWindow::on_abortTask_pressed()
{
    abortImort = true;
}

void MainWindow::setBuisyUI()
{
    ui->uci->setEnabled(false);
    ui->addFiles->setEnabled(false);
    ui->removeFiles->setEnabled(false);
    ui->selectServer->setEnabled(false);
    ui->tabExport->setEnabled(false);
    ui->tabImport->setEnabled(false);
    ui->abortTask->setEnabled(true);
}

void MainWindow::setIdleUI()
{
    ui->uci->setEnabled(true);
    ui->addFiles->setEnabled(true);
    ui->removeFiles->setEnabled(true);
    ui->selectServer->setEnabled(true);
    ui->tabExport->setEnabled(true);
    ui->tabImport->setEnabled(true);
    ui->abortTask->setEnabled(false);
}
