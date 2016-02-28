#ifndef QTCACHEPROGRESSTRANSMITTER_H
#define QTCACHEPROGRESSTRANSMITTER_H

#include "qtcacheexception.h"
#include "qtcacheprogress.h"

QTCACHENAMESPACEBEGIN

class QTCACHESHARED_EXPORT ProgressReporter  :
        public QObject
{
    Q_OBJECT

public:
    ProgressReporter(QObject* parent = 0);
    virtual ~ProgressReporter();

    const Progress& currentProgress() const { return m_current_progress; }

signals:
    void error(std::exception& ex, QtC::Progress&);
    void progressBegin(QtC::Progress&);
    void progress(QtC::Progress&);
    void progressEnd(QtC::Progress&);

protected:
    void reportError(std::exception& ex, Progress& p);
    void reportProcessBegin(Progress& p);
    void reportProgress(Progress& p);
    void reportProcessEnd(Progress& p);

    virtual void emitError(std::exception& ex, Progress& p);
    virtual void emitProgressBegin(Progress& p);
    virtual void emitProgress(Progress& p);
    virtual void emitProgressEnd(Progress& p);

private:
    Progress m_current_progress;
};

QTCACHENAMESPACEEND

#endif // QTCACHEPROGRESSTRANSMITTER_H
