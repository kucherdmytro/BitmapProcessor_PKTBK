#ifndef APPENGINE_H
#define APPENGINE_H

#include <QObject>
#include <QDir>
#include <QThread>
#include <QVector>
#include <QMutex>
#include <vector>
#include <qqml.h>

#include "filelistmodel.h"
#include "bitmapproc.h"

class AppEngine : public QObject
{
    Q_OBJECT
public:
    explicit         AppEngine        (const QString &dirPath, QObject *parent = nullptr);
    Q_INVOKABLE void processImage                                 (const QFileInfo &info);

signals:
    void             wrongFormat                                  (const QString &reason);

private slots:
    void             slot_handleBarchData                        (const BarchData &barch);
    void             slot_handleBitmapData                         (const Bitmap &bitmap);

private:
    void             processDir                                                        ();

private:
    FileListModel   *m_model = nullptr;
    QDir             m_dir;
    QFileInfo        m_fileInfo;
};

#endif // APPENGINE_H
