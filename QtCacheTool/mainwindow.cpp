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
#include <cacheconnectiondialog.h>
#include <QMessageBox>
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dlg = new CacheConnectionDialog(this);
    dlg->setUci(QStringList() << "%SYS");
    dlg->setUciEnabled(false);
    dlg->setFormat(CN_FORMAT_NAMESPACE);

    dlg->setUsername("sbr");
    dlg->setPassword("Logis#2015!!!");
    dlg->setServer("150.10.1.112");
    dlg->setPort("56773");

    m_QtCache = QtCache::instance();
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

void MainWindow::on_importFiles_pressed()
{
    try{
        QFileDialog dlg;
        dlg.setFileMode(QFileDialog::ExistingFiles);
        if (dlg.exec() == QFileDialog::Accepted){
            ui->listWidget->addItems(dlg.selectedFiles());
        }
    }catch(std::exception &ex){
        QMessageBox::critical(this, tr("Exception"), ex.what());
    }catch(...){
        QMessageBox::critical(this, tr("Exception"), tr("Unknown exception occured!"));
    }
}
