#ifndef QTCACHEBULKACTION_H
#define QTCACHEBULKACTION_H

#include "qtcache.h"

QTCACHENAMESPACEBEGIN

class QTCACHESHARED_EXPORT BulkAction
        : public QObject
{
    Q_OBJECT

public:
    explicit BulkAction(QtCache* cache, QObject *parent = 0);

    bool isAborted() const { return m_aborted; }
    QtCache* cache() const { return m_cache; }
    const Progress& currentProgress() const { return m_current_progress; }

signals:
    void aborted();
    void finished();
    void error(std::exception& ex, QtC::Progress&);
    void progress(QtC::Progress&);

public slots:
    void abort() { setAbort(true); }

protected:
    void setAbort(bool abort) { m_aborted = abort; }
    Progress& currentProgress() { return m_current_progress; }

    void reportProcessBegin(Progress& p);
    void reportProgress(Progress& p);
    void reportProcessEnd(Progress& p);

private:
    Progress m_current_progress;
    QtCache* m_cache = NULL;
    bool m_aborted = false;
};

QTCACHENAMESPACEEND

#endif // QTCACHEBULKACTION_H
