#ifndef BITMAPPROC_H
#define BITMAPPROC_H

#include "bmpproc_global.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
// -----------------------------------------------------------------------------
struct Bitmap
{
    int width;           // bitmap width in pixels
    int height;          // bitmap height in pixels
    unsigned char *data; // Pointer to bitmap data. data[j * width + i] is color of pixel in row j and column i.
};
// -----------------------------------------------------------------------------
struct BarchData
{
    int                        width;
    int                        height;
    std::vector<bool>          emptyRows;
    std::vector<int16_t>       encodedRowSizes;
    std::vector<unsigned char> encodedData;
};
// -----------------------------------------------------------------------------
class BMPPROC_EXPORT BitmapProc
{
public:
    BarchData encodeImage  (const Bitmap &bitmap);
    Bitmap    decodeImage(const BarchData &barch);
};



#endif // BITMAPPROC_H
