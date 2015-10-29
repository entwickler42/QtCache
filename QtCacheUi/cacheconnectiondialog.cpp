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

#include "cacheconnectiondialog.h"
#include "ui_cacheconnectiondialog.h"

#include <QMessageBox>

CacheConnectionDialog::CacheConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CacheConnectionDialog)
{
    ui->setupUi(this);
}

CacheConnectionDialog::~CacheConnectionDialog()
{
    delete ui;
}

bool CacheConnectionDialog::unsafe() const
{
    return m_unsafe;
}

void CacheConnectionDialog::setUnsafe(bool mode)
{
    m_unsafe = mode;
}

QString CacheConnectionDialog::server() const
{
    return ui->address->text();
}

QString CacheConnectionDialog::port() const
{
    return ui->port->text();
}

QString CacheConnectionDialog::uci() const
{
    return ui->uci->currentText();
}

QString CacheConnectionDialog::username() const
{
    return ui->username->text();
}

QString CacheConnectionDialog::password() const
{
    return ui->password->text();
}

void CacheConnectionDialog::setServer(const QString& text)
{
    ui->address->setText(text);
}

void CacheConnectionDialog::setPort(const QString& text)
{
    ui->port->setValue(text.toInt());
}

void CacheConnectionDialog::setUci(const QStringList& items)
{
    ui->uci->clear();
    ui->uci->addItems(items);
}

void CacheConnectionDialog::setUsername(const QString& text)
{
    ui->username->setText(text);
}

void CacheConnectionDialog::setPassword(const QString& text)
{
    ui->password->setText(text);
}

void CacheConnectionDialog::setUciEnabled(bool enabled)
{
    ui->uci->setEnabled(enabled);
}

QString CacheConnectionDialog::connectionString() const
{
    QString cn = QString("CN_IPTCP:%1[%2]").arg(
                ui->address->text(),
                ui->port->text()
                );
    /*
    if (ui->uci->currentText() != "" ){
        cn.append(QString(":\"%1\"").arg(ui->uci->currentText()));
    }
    */
    if (unsafe() && ui->username->text() != ""){
        cn.append(QString(":%1").arg(ui->username->text()));
        cn.append(QString(":@%1").arg(ui->password->text()));
    }

    return cn;
}

void CacheConnectionDialog::on_showPassword_toggled(bool checked)
{
    ui->password->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
}

void CacheConnectionDialog::on_test_pressed()
{
    QMessageBox::warning(this, tr("IMP"), tr("Implementation missing"));
}

void CacheConnectionDialog::showEvent(QShowEvent* ev)
{
    QDialog::showEvent(ev);
    ui->address->selectAll();
}
