#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QObject>
#include <QThread>
#include <QFileInfo>
#include <QImage>
#include <QMetaType>
#include <QDebug>

#include "bitmapproc.h"

class ImageProcessor : public QThread
{
    Q_OBJECT
public:
    explicit   ImageProcessor(const QFileInfo &info, QObject *parent = nullptr);
    void       run                                                           () override;

signals:
    void       encodingReady                           (const BarchData &barch);
    void       decodingReady                             (const Bitmap &bitmap);

private:
    BarchData  loadBarchFile                            (const QFileInfo &info);

private:
    BitmapProc m_processor;
    QFileInfo  m_info;
};

#endif // IMAGEPROCESSOR_H
