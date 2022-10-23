#include "imageprocessor.h"

ImageProcessor::ImageProcessor(const QFileInfo &info, QObject *parent) : QThread{parent}
{
    m_info = info;
    qRegisterMetaType<BarchData>("BarchData");
    qRegisterMetaType<Bitmap>("Bitmap");
}
// -----------------------------------------------------------------------------
void ImageProcessor::run()
{
    if(m_info.suffix() == "bmp")
    {
        QImage image(m_info.filePath());
        image = image.convertToFormat(QImage::Format_Grayscale8);
        Bitmap bitmap;

        bitmap.width  = image.width();
        bitmap.height = image.height();
        bitmap.data   = new unsigned char[image.width() * image.height()];

        unsigned char *pData = bitmap.data;
        for(int row = 0; row < image.height(); ++row)
        {
            unsigned char *line = image.scanLine(row);
            int count = 0;
            while(count < image.width())
            {
                *pData = line[count];
                pData++;
                count++;
            }
        }

        BitmapProc proc;
        BarchData barch = proc.encodeImage(bitmap);;

        emit encodingReady(barch);
    }
    else
    {
        BarchData  barch = loadBarchFile(m_info);
        Bitmap     bitmap;
        BitmapProc proc;
        bitmap = proc.decodeImage(barch);

        emit decodingReady(bitmap);
    }
}
// -----------------------------------------------------------------------------
BarchData ImageProcessor::loadBarchFile(const QFileInfo &info)
{
    BarchData barch;
    QFile file(info.filePath());
    if(file.open(QIODevice::ReadOnly))
    {
        QDataStream stream(&file);
        stream.setByteOrder(QDataStream::BigEndian);
        stream.setVersion(QDataStream::Qt_5_15);
        stream.setFloatingPointPrecision(QDataStream::DoublePrecision);

        QVector<bool> emptyRows;
        QVector<int16_t> rowSizes;
        QVector<unsigned char> encodedData;

        stream >> barch.width;
        stream >> barch.height;
        stream >> emptyRows;
        stream >> rowSizes;
        stream >> encodedData;

        barch.emptyRows       = std::vector<bool>(emptyRows.begin(), emptyRows.end());
        barch.encodedRowSizes = std::vector<int16_t>(rowSizes.begin(), rowSizes.end());
        barch.encodedData     = std::vector<unsigned char>(encodedData.begin(), encodedData.end());

        file.close();
    }
    return barch;
}
