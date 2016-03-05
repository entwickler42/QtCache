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

#ifndef CACHECONNECTIONDIALOG_H
#define CACHECONNECTIONDIALOG_H

#include <QDialog>
#include "qtcacheui_global.h"

namespace Ui {
    class CacheConnectionDialog;
}

class QSettings;

QTCACHENAMESPACEBEGIN

class QTCACHEUISHARED_EXPORT CacheConnectionDialog :
        public QDialog
{
    Q_OBJECT

public:
    enum ConnectionStringFormat
    {
        PROTOCOL_FLAG    = 0x01,
        CREDENTIALS_FLAG = 0x02,
        NAMESPACE_FLAG   = 0x04,
    };

    explicit CacheConnectionDialog(QWidget *parent = 0);
    ~CacheConnectionDialog();

    QString connectionString() const;
    QString server() const;
    QString port() const;
    QString uci() const;
    QString username() const;
    QString password() const;
    int format() const;

    void setServer(const QString&);
    void setPort(const QString&);
    void setUci(const QStringList&);
    void setUsername(const QString&);
    void setPassword(const QString&);
    void setUciVisible(bool);
    void setFormat(int format);
    void setShowPasswordVisible(bool allow);

    template <class T> void save(T* conf)
    {
        conf->setUser(username());
        conf->setPasswd(password());
        conf->setServer(server());
        conf->setPort(port());
    }

    template <class T> void load(T* conf)
    {
        setUsername(conf->User());
        setPassword(conf->Passwd());
        setServer(conf->Server());
        setPort(conf->Port());
    }

protected:
    virtual void showEvent(QShowEvent* ev);

private slots:
    void on_showPassword_toggled(bool);

private:
    Ui::CacheConnectionDialog *ui;
    int constrformat = 0;
};

QTCACHENAMESPACEEND

#endif // CACHECONNECTIONDIALOG_H
