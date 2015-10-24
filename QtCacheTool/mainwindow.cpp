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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dlg = new CacheConnectionDialog(this);
    dlg->setUci(QStringList() << "%SYS");
    dlg->setUciEnabled(false);
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
}

void MainWindow::on_importFiles_pressed()
{
    QtCache* cache = QtCache::instance();
    cache->connect(ui->connectionString->text(), dlg->uci());
}
