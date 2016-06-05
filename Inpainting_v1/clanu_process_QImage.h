#ifndef CLANU_PROCESS_H
#define CLANU_PROCESS_H

#include <QImage>
#include <QRgb>
#include "imageconvert.h"

/*
void MyProcess(const QImage &input, QImage &output, double param);
void MyProcessBW(const QImage &input, QImage &output, double param);
*/

void MyProcess(float **Rout, float **Gout, float **Bout, float **Rin, float **Gin, float **Bin, int width, int height, double param);

#endif 

