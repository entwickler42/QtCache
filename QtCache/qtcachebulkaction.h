#ifndef QTCACHEBULKACTION_H
#define QTCACHEBULKACTION_H

#include "qtcache.h"
#include "qtcacheprogressreporter.h"

QTCACHENAMESPACEBEGIN

class QTCACHESHARED_EXPORT BulkAction
        : public ProgressReporter
{
    Q_OBJECT

public:
    explicit BulkAction(QtCache* cache, QObject *parent = 0);

    bool isAborted() const { return m_aborted; }
    QtCache* cache() const { return m_cache; }

signals:
    void aborted();
    void finished();

public slots:
    void abort() { setAbort(true); }
    virtual void run() = 0;

protected:
    void setAbort(bool abort) { m_aborted = abort; }

private:
    QtCache* m_cache = NULL;
    bool m_aborted = false;
};

QTCACHENAMESPACEEND

#endif // QTCACHEBULKACTION_H
