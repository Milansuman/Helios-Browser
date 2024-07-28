#include "DialogApi.h"

DialogApi::DialogApi(QObject *parent): QObject(parent){}

void DialogApi::closeDialog(){
    emit this->closeDialogRequested();
}

DialogApi::~DialogApi() = default;