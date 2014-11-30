#include "downloadmanager.h"

DownloadManager::DownloadManager(QObject *parent, QWebdav *webdav) :
    QObject(parent)
{
    this->webdav = webdav;
}

void DownloadManager::enqueueDownload(QString remotePath, QString mimeType, qint64 size)
{
    QString name = remotePath.mid(remotePath.lastIndexOf("/") + 1);
    QString destination = destinationFromMIME(mimeType) + "/" + name;

    DownloadEntry *newDownload = new DownloadEntry(0, name, remotePath, destination, size);
    connect(newDownload, SIGNAL(downloadCompleted()), this, SLOT(handleDownloadCompleted()));
    if(downloadQueue.size() == 0) {
        newDownload->startDownload();
    }
    downloadQueue.enqueue(newDownload);
}

void DownloadManager::handleDownloadCompleted()
{
    downloadQueue.dequeue();

    if(downloadQueue.size() > 0)
        downloadQueue.head()->startDownload();
}

QString DownloadManager::destinationFromMIME(QString mime)
{
    QStandardPaths::StandardLocation location;

    if (mime.startsWith("image")) {
        location = QStandardPaths::PicturesLocation;
    } else if (mime.startsWith("video")) {
        location = QStandardPaths::MoviesLocation;
    } else if (mime.startsWith("audio")) {
        location = QStandardPaths::MusicLocation;
    } else {
        location = QStandardPaths::DownloadLocation;
    }

    return QStandardPaths::writableLocation(location);
}
