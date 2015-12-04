#ifndef BULKIMPORT_H
#define BULKIMPORT_H

#include <QObject>
#include "qtcache.h"

class BulkImport
        : public QObject
{
    Q_OBJECT

public:
    explicit BulkImport(QtCache* cache, QObject *parent = 0);

signals:
    void finished();
    void error(std::exception& ex);
    void loadingFile(const QString& filename, int pos, int max);
    void uploadingFile(const QString& filename, int pos, int max);
    void compilingFile(const QString& filename, int pos, int max);

public slots:
    void load(const QStringList& filepaths, const QString& qspec = "");
    void abort()
    {
        m_abort_import = true;
    }

protected:
    void load(const QString& filepath, const QString& qspec = "");

private:
    QtCache* m_cache = NULL;
    bool m_abort_import = false;
};

#endif // BULKIMPORT_H
