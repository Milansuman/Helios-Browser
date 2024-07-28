#include "TerminalApi.h"

#include <cstdlib>

#ifdef __linux__
#include <pty.h>
#endif

TerminalApi::TerminalApi(QObject *parent): QObject(parent){
    #ifdef __linux__

    QString shell = QString(std::getenv("SHELL"));

    #endif
}

TerminalApi::~TerminalApi() = default;