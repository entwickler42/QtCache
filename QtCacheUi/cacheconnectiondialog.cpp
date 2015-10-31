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

#include <QSettings>


CacheConnectionDialog::CacheConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CacheConnectionDialog),
    constrformat(PROTOCOL_FLAG|CREDENTIALS_FLAG|NAMESPACE_FLAG)
{
    ui->setupUi(this);
}

CacheConnectionDialog::~CacheConnectionDialog()
{
    delete ui;
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

void CacheConnectionDialog::setFormat(int format)
{
    constrformat = format;
}

int CacheConnectionDialog::format() const
{
    return constrformat;
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
    QString cn = (constrformat & PROTOCOL_FLAG) == PROTOCOL_FLAG ? "CN_IPTCP:" : "";

    cn = QString("%1%2[%3]").arg(
                cn,
                ui->address->text(),
                ui->port->text()
                );

    if ((constrformat & NAMESPACE_FLAG) == NAMESPACE_FLAG &&
            ui->uci->currentText() != "" ){
        cn.append(QString(":%1").arg(ui->uci->currentText()));
    }

    if ((constrformat & CREDENTIALS_FLAG) == CREDENTIALS_FLAG &&
         ui->username->text() != "") {
        cn.append(QString(":%1").arg(ui->username->text()));
        cn.append(QString(":@%1").arg(ui->password->text()));
    }

    return cn;
}

void CacheConnectionDialog::save(QSettings* conf)
{
    conf->setValue("user", username());
    conf->setValue("passwd", password());
    conf->setValue("server", server());
    conf->setValue("port", port());
}

void CacheConnectionDialog::load(QSettings* conf)
{
    setUsername(conf->value("user").toString());
    setPassword(conf->value("passwd").toString());
    setServer(conf->value("server", "127.0.0.1").toString());
    setPort(conf->value("port", "1972").toString());
}

void CacheConnectionDialog::on_showPassword_toggled(bool checked)
{
    ui->password->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
}

void CacheConnectionDialog::showEvent(QShowEvent* ev)
{
    QDialog::showEvent(ev);
    ui->address->selectAll();
}
