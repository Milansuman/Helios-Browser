#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QEnterEvent>
#include <QEvent>

#include "IconButton.h"
#include "AddressBox.h"
#include "SplitTabMenu.h"

class TabTitleBar : public QWidget {
    Q_OBJECT
private:
    QHBoxLayout *layout;
    IconButton *reloadButton, *backButton, *forwardButton, *copyLinkButton, *siteSettingsButton, *closeButton;
    SplitTabMenu *splitTabMenu;
    AddressBox *addressBox;
public:
    TabTitleBar(QWidget *parent=nullptr);
    void setTitle(QString title);
    void setIsBlack(bool isBlack);
    ~TabTitleBar();
protected:
    void enterEvent(QEnterEvent *event);
    void leaveEvent(QEvent *event);
signals:
    void reloadRequested();
    void previousPageRequested();
    void nextPageRequested();
    void closeTabRequested();
    void splitTabLeftRequested();
    void splitTabRightRequested();
    void splitTabFlipRequested();
    void copyLinkRequested();
    void searchRequested();
    void siteSettingsRequested();
};