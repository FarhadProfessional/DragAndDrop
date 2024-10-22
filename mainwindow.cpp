#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDragEnterEvent>
#include <QEvent>
#include <QMimeData>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAcceptDrops(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    const QMimeData *mimeData = event->mimeData();

    if (mimeData)
    {
        if (mimeData->hasUrls())
        {
            QList<QUrl> urlList = mimeData->urls();
            for (int i = 0; i < urlList.size(); ++i)
            {
                qDebug()<<urlList.at(i).toLocalFile();
            }
            event->acceptProposedAction();
            setFileInfo(QFile(urlList.at(0).toLocalFile()));
        }
    }
}

void MainWindow::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
    clearFileInfo();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();

    if (mimeData)
    {
        if (mimeData->hasUrls())
        {
            event->acceptProposedAction();
            QFile file;
            file.setFileName(mimeData->urls().at(0).toLocalFile());
            file.copy("../" + QFileInfo(file).fileName());
        }
    }
}

void MainWindow::clearFileInfo()
{
    ui->label_FileName->setText("Name:");
    ui->label_FileSize->setText("Size:");
    ui->label_FileExtension->setText("Extension:");
    ui->label_FilePath->setText("Path:");
    ui->label_FileLastModified->setText("Last modified:");
}

void MainWindow::setFileInfo(const QFileInfo &fileInfo)
{
    ui->label_FileName->setText("Name: " + fileInfo.fileName());
    ui->label_FileSize->setText("Size: " + addSizeUnit(fileInfo.size()));
    ui->label_FileExtension->setText("Extension: " + fileInfo.suffix());
    ui->label_FilePath->setText("Path: " + QDir::toNativeSeparators(fileInfo.absolutePath()));
    ui->label_FileLastModified->setText("Last modified: " + fileInfo.lastModified().toString());
}

QString MainWindow::addSizeUnit(const quint64 &fileSize)
{
    QString result;
    if(fileSize >= 1e9)
        result = QString::number(qRound(fileSize / 1e6)/1e3, 'f', 3) + " GB";
    else if(fileSize >= 1e6)
        result = QString::number(qRound(fileSize / 1e4)/1e2, 'f', 1) + " MB";
    else if(fileSize >= 1e3)
        result = QString::number(qRound(fileSize / 1e2)/1e1, 'f', 1) + " KB";
    else
        result = QString::number(fileSize, 'f', 1) + " Bytes";

    return result;
}
