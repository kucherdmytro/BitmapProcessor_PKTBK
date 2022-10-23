#include "appengine.h"
#include "imageprocessor.h"

AppEngine::AppEngine(const QString &dirPath, QObject *parent) : QObject{parent},
    m_model(FileListModel::instance(this))
{
    m_dir.setPath(dirPath);
    processDir();
}
// -----------------------------------------------------------------------------
void AppEngine::processImage(const QFileInfo &info)
{
    m_fileInfo = info;
    if(m_fileInfo.suffix() != "bmp" && m_fileInfo.suffix() != "barch")
    {
        emit wrongFormat("Accepted file format is *.bmp and *.barch only");
        return;
    }
    else
    {
        if(m_fileInfo.suffix() == "bmp")
        {
            QImage image(m_fileInfo.filePath());
            if(image.format() != QImage::Format_Grayscale8)
            {
                emit wrongFormat("Warning! Image not in grayscale 8-bit format. \n"
                                 "It will be converted to grayscale 8-bit format...");
            }
        }
        ImageProcessor *processor = new ImageProcessor(info, this);
        connect(processor, &ImageProcessor::encodingReady, this, &AppEngine::slot_handleBarchData);
        connect(processor, &ImageProcessor::decodingReady, this, &AppEngine::slot_handleBitmapData);

        processor->start();
    }
}
// -----------------------------------------------------------------------------
void AppEngine::slot_handleBarchData(const BarchData &barch)
{
    QMutex mutex;
    QString barchFilePath(m_fileInfo.path() + "/" + m_fileInfo.baseName() + "_packed.barch");
    QFile file(barchFilePath);
    mutex.lock();
    if(file.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        QDataStream stream(&file);
        stream.setByteOrder(QDataStream::BigEndian);
        stream.setVersion(QDataStream::Qt_5_15);
        stream.setFloatingPointPrecision(QDataStream::DoublePrecision);

        stream << barch.width;
        stream << barch.height;
        stream << QVector<bool>(barch.emptyRows.begin(), barch.emptyRows.end());
        stream << QVector<int16_t>(barch.encodedRowSizes.begin(), barch.encodedRowSizes.end());
        stream << QVector<unsigned char>(barch.encodedData.begin(), barch.encodedData.end());

        file.close();
    }
    mutex.unlock();

    processDir();
}
// -----------------------------------------------------------------------------
void AppEngine::slot_handleBitmapData(const Bitmap &bitmap)
{
    QMutex mutex;

    mutex.lock();
    QString bmpImagePath(m_fileInfo.path() + "/" + m_fileInfo.baseName().remove("_packed") + "_unpacked.bmp");
    QImage image(bitmap.data, bitmap.width, bitmap.height, bitmap.width, QImage::Format_Grayscale8);
    image.save(bmpImagePath, "bmp");
    mutex.unlock();

    processDir();
}
// -----------------------------------------------------------------------------
void AppEngine::processDir()
{
    // set file format filter
    QStringList filterList{"*.png", "*.bmp", "*.barch"};
    m_dir.setNameFilters(filterList);

    // read file info from dir
    QFileInfoList list = m_dir.entryInfoList();
    if(!list.isEmpty())
    {
        foreach(const QFileInfo &info, list)
        {
            if(info.suffix() == "bmp" || info.suffix() == "png" || info.suffix() == "barch")
            {
                m_model->addFileInfo(info);
            }
        }
    }
}
