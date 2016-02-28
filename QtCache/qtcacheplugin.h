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

#ifndef QTCACHEPLUGIN_H
#define QTCACHEPLUGIN_H

#include "qtcache_global.h"
#include <qtcache.h>
#include <QObject>

class QSettings;
class QCommandLineParser;

QTCACHENAMESPACEBEGIN

class QTCACHESHARED_EXPORT Plugin :
        public ProgressReporter
{
    friend class PluginDirector;

    Q_OBJECT

public:
    typedef QtC::Plugin*(*FactoryFunction)(QObject*);
    static const char* FactoryFunctionSymbol;

    explicit Plugin(QObject *parent = 0);
    virtual ~Plugin();

    virtual QString name() const = 0;
    virtual QString description() const { return ""; }
    virtual int version() const { return 0x000000; }

protected slots:
    virtual void onInitialize() {}
    virtual void onDeinitialize() {}

    virtual void onError(std::exception&, Progress&) {}
    virtual void onProgressBegin(Progress&) {}
    virtual void onProgress(Progress&) {}
    virtual void onProgressEnd(Progress&) {}

    virtual void onParseCommandlineOptionsBegin(QCommandLineParser&) {}
    virtual void onParseCommandlineOptionsEnd(QCommandLineParser&) {}
    virtual void onLoadApplicationSettingsBegin(QSettings&) {}
    virtual void onLoadApplicationSettingsEnd(QSettings&) {}
    virtual void onSaveApplicationSettingsBegin(QSettings&) {}
    virtual void onSaveApplicationSettingsEnd(QSettings&) {}
};

template<class T>
static T* createPluginInstance(QObject* parent)
{
    return new T(parent);
}

QTCACHENAMESPACEEND

#endif // QTCACHEPLUGIN_H
