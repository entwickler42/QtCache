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
    bool connected = cache()->connect(
                dlg->connectionString(),
                dlg->username(),
                dlg->password()
                );
    if (!connected){
        QMessageBox::warning(this, tr("Connection Error"), tr("Failed to establish connection to caché!"));
    }
}

void MainWindow::on_importFiles_pressed()
{
    cache()->execute("S ^ySBR(0)=1");
}
