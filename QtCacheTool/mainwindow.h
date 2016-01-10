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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qtcache.h>
#include "qtcachetoolsettings.h"

namespace Ui {
    class MainWindow;
}

namespace QtC {
    class CacheConnectionDialog;
    class BulkImportProgress;
}

class QSettings;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    virtual void showEvent(QShowEvent*);
    virtual void closeEvent(QCloseEvent *);

    QtC::QtCache* cache() const
    {
        return QtC::QtCache::instance();
    }

    void parseCommandlineOptions();
    void loadImportDirectory(const QString& path);
    void loadImportFile(const QString& filepath);

    void setBuisyUI();
    void setIdleUI();

private slots:
    void on_targetUCI_currentIndexChanged(const QString&);
    void on_qspec_editingFinished();
    void on_selectServer_pressed();
    void on_addFiles_pressed();
    void on_removeFiles_pressed();
    void on_importFiles_pressed();
    void on_abortTask_pressed();
    void on_selectOutputDirectory_pressed();
    void on_exportFiles_pressed();
    void on_saveCurrentFilter_pressed();
    void on_removeCurrentFilter_pressed();

    void cacheProgress(QtC::Progress& progress);

    void bulkImportAborted();
    void bulkImportFinished();
    void bulkImportError(std::exception& ex, QtC::Progress&);
    void bulkImportProgress(QtC::Progress& progress);

private:
    Ui::MainWindow *ui;
    QtCacheToolSettings* conf;
    QtC::CacheConnectionDialog* dlg;
    bool abortTask = false;
    bool bulk_import_active = false;

    QStringList loadFilters() const;
    void saveFilters(const QStringList&) const;

    void onServerConnected();
    void preImportHook();
    void postImportHook();
    void setListViewItem(const QString& filename, const QString& iconpath, const QString& toolTip = "");
    void saveSettings();
    void loadSettings();

    void createPluginTable();
};

#endif // MAINWINDOW_H
