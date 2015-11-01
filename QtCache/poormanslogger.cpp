#include "poormanslogger.h"
#include <stdio.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <stdio.h>

PoorMansLogger::PoorMansLogger(const QString& filepath)
{
    FH = open(qPrintable(filepath), O_WRONLY | O_CREAT);
    dup2(FH, fileno(stderr));
    dup2(FH, fileno(stdout));
}

PoorMansLogger::~PoorMansLogger()
{
    close(FH);
}

