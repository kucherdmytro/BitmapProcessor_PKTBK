#include "bitmapproc.h"
// -----------------------------------------------------------------------------
BarchData BitmapProc::encodeImage(const Bitmap &bitmap)
{
    std::vector<unsigned char> bitmapValues(bitmap.data, bitmap.data + (bitmap.width * bitmap.height));
    BarchData barchData;

    barchData.height = bitmap.height;
    barchData.width  = bitmap.width;

    // encode image
    for(int row = 0; row < bitmap.height; ++row)
    {
        // 1. Get empty lines of image (all pixels in row is 0xff)
        int  startIndex = row * bitmap.width;
        auto rowBegin   = bitmapValues.cbegin() + startIndex;
        auto rowEnd     = bitmapValues.cbegin() + startIndex + bitmap.width;
        bool isEmpty    = std::all_of(rowBegin, rowEnd, [](unsigned char ch){ return ch == 0xff;});
        barchData.emptyRows.push_back(isEmpty);

        // 2. For not empty lines - encode row.
        if(!isEmpty)
        {
            // 2.1 Get row subvector from initial pixel data vector
            std::vector<unsigned char> initialRow{rowBegin, rowEnd};
            int16_t rowCount = 0;

            for(size_t index = 0; index < initialRow.size(); index += 4)
            {
                // 2.2 Iterate chunk of pixels by 4 at the time
                auto chunkBegin = initialRow.begin() + index;
                auto chunkEnd   = chunkBegin + 4;
                if(std::all_of(chunkBegin, chunkEnd, [](unsigned char ch){ return ch == 0xff;}))
                {
                    // 2.2.1 If all pixels in chunk is 0xff (white) - encode it's chunk by 0 (in binary)
                    barchData.encodedData.push_back(0x00); // 0
                    rowCount++;
                }
                else if(std::all_of(chunkBegin, chunkEnd, [](unsigned char ch){ return ch == 0x00;}))
                {
                    // 2.2.2 If all pixels in chunk is 0x00 (black) - encode it's chunk by 10 (in binary)
                    barchData.encodedData.push_back(0x02); // 10
                    rowCount++;
                }
                else
                {
                    // 2.2.3 If pixels in chunk is different - encode it's chunk by 11 (in binary)
                    barchData.encodedData.push_back(0x03); // 11
                    rowCount++;
                    if(chunkEnd > initialRow.end())
                    {
                        chunkEnd = initialRow.end(); // processing of last 1-3 pix if bitmap width is not odd
                    }
                    for(auto it = chunkBegin; it != chunkEnd; ++it)
                    {
                        // 2.2.4 Also after chunk encoding by 11 (in binary) - add each pixel color data
                        barchData.encodedData.push_back(*it); // pix color/
                        rowCount++;
                    }
                }
            }
            // 3 Save each row count value to barch struct
            barchData.encodedRowSizes.push_back(rowCount);
        }
    }
    return barchData;
}
// -----------------------------------------------------------------------------
Bitmap BitmapProc::decodeImage(const BarchData &barch)
{
    Bitmap bitmap;
    bitmap.height = barch.height;
    bitmap.width  = barch.width;
    bitmap.data   = new unsigned char[bitmap.height * bitmap.width];

    // decode image
    unsigned char *pData = bitmap.data;
    int lastEncodedRowIndex = 0;
    for(size_t row = 0; row < barch.emptyRows.size(); row++)
    {
        // 1. For empty rows - fill data array with 0xff pixels
        if(barch.emptyRows.at(row))
        {
            int count = 0;
            while(count < barch.width)
            {
                *pData = static_cast<unsigned char>(0xff);
                pData++;
                count++;
            }
        }
        else
        {
            // 2. For non empty rows - encode row data row by row
            auto offsetBegin = barch.encodedRowSizes.begin();
            auto offsetEnd = offsetBegin + lastEncodedRowIndex;
            u_int32_t offset = std::accumulate(offsetBegin, offsetEnd, 0); // get start position in encoded data array

            auto rowBegin = barch.encodedData.begin() + offset; // start pos
            auto rowEnd = rowBegin + barch.encodedRowSizes.at(lastEncodedRowIndex); // start pos + encoded row length
            std::vector<unsigned char> encodedRow(rowBegin, rowEnd);

            for(size_t index = 0; index < encodedRow.size();)
            {
                // 2.1 For encoded 0 value - append 4 white pixels (0xff) to bitmap data array
                int pixChunkSize = 4;
                if(encodedRow.at(index) == 0x00)
                {
                    int pixCount = 0;
                    while(pixCount < pixChunkSize)
                    {
                        *pData = static_cast<unsigned char>(0xff);
                        pData++;
                        pixCount++;
                    }
                    index++;
                }
                // 2.2 For encoded 10 value - append 4 black pixels (0x00) to bitmap data array
                else if(encodedRow.at(index) == 0x02)
                {
                    int pixCount = 0;
                    while(pixCount < pixChunkSize)
                    {
                        *pData = static_cast<unsigned char>(0x00);
                        pData++;
                        pixCount++;
                    }
                    index++;
                }
                // 2.3 For encoded 11 value - append next 4 pixels color value to bitmap data array
                else if(encodedRow.at(index) == 0x03)
                {
                    index++; // move right to next char
                    int pixCount = 0;
                    while(pixCount < pixChunkSize)
                    {
                        *pData = encodedRow.at(index);
                        pData++;
                        pixCount++;
                        index++;
                        if(index >= encodedRow.size())
                        {
                            break; // for image width % 4 != 0
                        }
                    }
                }
            }
            lastEncodedRowIndex++;
        }
    }
    return bitmap;
}
