#pragma once

#include <QWidget>
#include <QHBoxLayout>

#include "IconButton.h"
#include "AddressBox.h"

#include "SplitTabMenu.h"

class TabTitleBar : public QWidget {
    Q_OBJECT
private:
    QHBoxLayout *layout;
    IconButton *backButton, *forwardButton, *copyLinkButton, *siteSettingsButton, *closeButton;
    SplitTabMenu *splitTabMenu;
    AddressBox *addressBox;
public:
    TabTitleBar(QWidget *parent=nullptr);
    ~TabTitleBar();
};