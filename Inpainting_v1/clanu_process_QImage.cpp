#include "clanu_process.h"

/*
void MyProcess(const QImage &input, QImage &output, double param)
{
    output = QImage(input.size(), input.format()); //copie des informations image et allocation
    QRgb value;
    QRgb new_value;

    for(int x=0; x<input.width(); x++)
        for(int y=0;y<input.height(); y++)
            {
            value = input.pixel( x, y); // lecture pixel (x,y)
            new_value = qRgb( param - qRed(value), param - qGreen(value), param - qBlue(value) );
            output.setPixel(input.width() - x - 1, input.height() - y - 1, new_value);
            }
}

void MyProcessBW(const QImage &input, QImage &output, double param)
{
    output = QImage(input.size(), input.format()); //copie des informations image et allocation
    QRgb value;
    QRgb new_value;

    for(int x=0; x<input.width(); x++)
        for(int y=0;y<input.height(); y++)
            {
            value = input.pixel( x, y); // lecture pixel (x,y)

            new_value = qRgb( param - qRed(value), param - qGreen(value), param - qBlue(value) );
            output.setPixel(input.width() - x - 1, input.height() - y - 1, new_value);
            }
}
*/


void MyProcess(float **Rout, float **Gout, float **Bout, float **Rin, float **Gin, float **Bin, int width, int height, double param)
{
    for(int x=0; x<width; x++)
        for(int y=0;y<height; y++)
            {
            Rout[x][y] = 255 -  Rin[x][y];
            if(Gin != 0) Gout[x][y] = 255 -  Gin[x][y];
            if(Bin != 0) Bout[x][y] = 255 - Bin[x][y];
            }
}
