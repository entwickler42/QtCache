#ifndef POORMANSLOGGER_H
#define POORMANSLOGGER_H

#include "qtcache_global.h"
#include <QString>


class QTCACHESHARED_EXPORT PoorMansLogger
{
public:
    PoorMansLogger(const QString& filepath = "logfile.log");
    ~PoorMansLogger();

private:
    int FH;
};

#endif // POORMANSLOGGER_H
