// New file
#include "backend.h"
#include <QFileDialog>
#include <QDebug>
#include <QFileInfo>

Backend::Backend(QObject *parent)
    : QObject(parent)
{
}

void Backend::importClip()
{
    // Opens native file dialog and allows selection of an mp4 file.
    QString file = QFileDialog::getOpenFileName(nullptr,
                                                QObject::tr("Import clip"),
                                                QString(),
                                                QObject::tr("Video Files (*.mp4);;All Files (*)"));
    if (!file.isEmpty())
    {
        // print just the file name to stdout / qDebug for now
        QFileInfo fi(file);
        qDebug() << "Imported clip:" << fi.fileName();
    }
    else
    {
        qDebug() << "Import cancelled";
    }
}