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

#ifndef BULKIMPORT_H
#define BULKIMPORT_H

#include "qtcachebulkaction.h"

QTCACHENAMESPACEBEGIN

class QTCACHESHARED_EXPORT BulkImport
        : public BulkAction
{
    Q_OBJECT

public:
    bool compileEarly = false;
    QString qspec = "";
    QStringList filepaths;

    explicit BulkImport(QtCache* cache, QObject *parent = 0);

public slots:
    void run();

private:
    void load(const QStringList& filepaths, const QString& qspec = "");
    void loadCompileEarly(const QStringList& filepaths, const QString& qspec = "");
    void loadCompileLate(const QStringList& filepaths, const QString& qspec = "");
};

QTCACHENAMESPACEEND

#endif // BULKIMPORT_H
