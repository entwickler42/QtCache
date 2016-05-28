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
    Progress& currentProgress() { return m_current_progress; }

    void link(const ProgressReporter* source)
    {
        connect(source, SIGNAL(error(const std::exception&,QtC::Progress&)), this, SLOT(reportError(const std::exception&,QtC::Progress&)));
        connect(source, SIGNAL(progressBegin(QtC::Progress&)), this, SLOT(reportProcessBegin(QtC::Progress&)));
        connect(source, SIGNAL(progress(QtC::Progress&)), this, SLOT(reportProgress(QtC::Progress&)));
        connect(source, SIGNAL(progressEnd(QtC::Progress&)), this, SLOT(reportProcessEnd(QtC::Progress&)));
    }

    void unlink(const ProgressReporter* source)
    {
        disconnect(source, SIGNAL(error(const std::exception&,QtC::Progress&)), this, SLOT(reportError(const std::exception&,,QtC::Progress&)));
        disconnect(source, SIGNAL(progressBegin(QtC::Progress&)), this, SLOT(reportProcessBegin(QtC::Progress&)));
        disconnect(source, SIGNAL(progress(QtC::Progress&)), this, SLOT(reportProgress(QtC::Progress&)));
        disconnect(source, SIGNAL(progressEnd(QtC::Progress&)), this, SLOT(reportProcessEnd(QtC::Progress&)));
    }

signals:
    void error(const std::exception& ex, QtC::Progress&);
    void progressBegin(QtC::Progress&);
    void progress(QtC::Progress&);
    void progressEnd(QtC::Progress&);

protected slots:
    void reportError(const std::exception&, QtC::Progress&);
    void reportProcessBegin(QtC::Progress&);
    void reportProgress(QtC::Progress&);
    void reportProcessEnd(QtC::Progress&);

protected:
    virtual void emitError(const std::exception&, QtC::Progress&);
    virtual void emitProgressBegin(QtC::Progress&);
    virtual void emitProgress(QtC::Progress&);
    virtual void emitProgressEnd(QtC::Progress&);

private:
    Progress m_current_progress;

    template <class T> void forwardCallToPluginDirector(void (T::*method)(Progress&), Progress& p);
};

QTCACHENAMESPACEEND

#endif // QTCACHEPROGRESSTRANSMITTER_H
