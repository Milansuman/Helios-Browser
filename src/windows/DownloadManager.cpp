#include "DownloadManager.h"

#include <QFontDatabase>
#include <QFont>
#include <QListWidgetItem>
#include <QProgressBar>
#include <QPainter>

DownloadManager::DownloadManager(QWidget *parent) : QDialog(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setFixedWidth(300);

    QFontDatabase::addApplicationFont(":/fonts/SFUIText-Bold.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(0).at(0);
    QFont *font = new QFont(fontFamily, -1, QFont::Bold);

    this->layout = new QStackedLayout(this);
    this->mainPage = new QWidget();
    this->dialogLayout = new QVBoxLayout(this->mainPage);
    this->dialogLayout->setContentsMargins(15, 15, 15, 15);

    downloadList = new QListWidget(this);
    downloadList->setFont(*font);
    downloadList->setStyleSheet(
        "QListWidget {"
        "   background-color: rgb(228, 228, 228);"
        "   border-radius:20px"
        "   color: white;"
        "}");

    this->dialogLayout->addWidget(downloadList);
    setLayout(this->dialogLayout);

    this->layout->addWidget(this->mainPage);
}

void DownloadManager::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setBrush(QBrush(QColor(30, 30, 30, 230)));
    painter.setPen(Qt::NoPen);

    painter.drawRoundedRect(rect(), 10, 10);
}

void DownloadManager::addDownload(const QString &filename, int progress)
{
    QListWidgetItem *item = new QListWidgetItem(downloadList);
    downloadList->addItem(item);

    QWidget *itemWidget = new QWidget(downloadList);
    QVBoxLayout *itemLayout = new QVBoxLayout(itemWidget);

    QLabel *filenameLabel = new QLabel(filename, itemWidget);
    filenameLabel->setStyleSheet("color: white;");

    QProgressBar *progressBar = new QProgressBar(itemWidget);
    progressBar->setValue(progress);
    progressBar->setStyleSheet(
        "QProgressBar {"
        "   border: 1px solid white;"
        "   border-radius: 5px;"
        "   text-align: center;"
        "}"
        "QProgressBar::chunk {"
        "   background-color: #0147FF;"
        "   border-radius: 5px;"
        "}");

    itemLayout->addWidget(filenameLabel);
    itemLayout->addWidget(progressBar);
    itemWidget->setLayout(itemLayout);

    item->setSizeHint(itemWidget->sizeHint());
    downloadList->setItemWidget(item, itemWidget);
}

void DownloadManager::updateDownloadProgress(const QString &filename, int progress)
{
    for (int i = 0; i < downloadList->count(); ++i)
    {
        QListWidgetItem *item = downloadList->item(i);
        QWidget *itemWidget = downloadList->itemWidget(item);
        if (itemWidget)
        {
            QLabel *filenameLabel = itemWidget->findChild<QLabel *>();
            if (filenameLabel && filenameLabel->text() == filename)
            {
                QProgressBar *progressBar = itemWidget->findChild<QProgressBar *>();
                if (progressBar)
                {
                    progressBar->setValue(progress);
                    break;
                }
            }
        }
    }
}

void DownloadManager::open()
{
    this->move(this->parentWidget()->mapToGlobal(QPoint(this->parentWidget()->width() / 2 - this->width() / 2, 10)));
    QDialog::open();
}

DownloadManager::~DownloadManager()
{
    delete this->dialogLayout;
    delete downloadList;
}