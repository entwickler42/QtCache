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
#include <poormanslogger.h>
#include <QApplication>
#include <QSettings>
#include <QTranslator>
#include <QDir>


void i18n(QApplication* application)
{
    QSettings conf("QtCacheTool.ini", QSettings::IniFormat);
    QString name = conf.value("Locale", QLocale::system().name()).toString();
    QDir i18n(":/i18n");
    foreach(const QString& s, i18n.entryList(QStringList() << "*.qm")){
        if (s.contains(name)){
            QString filename = s;
            filename.remove(".qm");
            QString directory = i18n.absolutePath();
            QTranslator* trn = new QTranslator(application);
            if (trn->load(filename, directory)){
                application->installTranslator(trn);
            }else{
                delete trn;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    PoorMansLogger(QString("%1.log").arg("errors"));

    QCoreApplication::setApplicationName("Qt Caché Tool");
    QCoreApplication::setOrganizationName("Entwickler42");
    QCoreApplication::setOrganizationDomain("http://github.org/entwickler42");
    QCoreApplication::setApplicationVersion("1.0.3");

    QApplication app(argc, argv);
    i18n(&app);

    MainWindow w;
    w.show();

    return app.exec();
}
