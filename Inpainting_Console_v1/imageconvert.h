// Clanu 2015 - 2016
// T. Grenier : thomas.grenier@insa-lyon.fr
#ifndef IMAGECONVERT
#define IMAGECONVERT

#include <QImage>
#include <QRgb>

//! Convert QImage to three float **. The arrays are allocated within this function.
//! \param width is the input image width
//! \param height is the input image height
//! \return true if the input image can be considered as a grey scale image
bool QImageToFloat(const QImage &in, int *width, int *height, float ***R, float ***G, float ***B);


//! Build a QImage from three float **. Without argument for format, QImage will be set to QImage::Format_RGB32
//! \param width is the input image width
//! \param height is the input image height
void FloatToQImage(QImage &out, int width, int height, const float *const *R, const float *const*G, const float *const*B, QImage::Format format=QImage::Format_RGB32);


//! Allocate an array of float of size width x height.
//! Example of use : float ** t = AllocateFloatArray(20, 100);
//!                  t[19][99] = 0; // set 0 for the 'last' pixel
//! \return The new allocated array is returned by this function
float ** AllocateFloatArray(int width, int height);

//! Free memory of the array a
void DestroyFloatArray(float ***a, int width);

#endif // IMAGECONVERT
