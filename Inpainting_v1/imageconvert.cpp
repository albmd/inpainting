// Clanu 2015 - 2016
// T. Grenier : thomas.grenier@insa-lyon.fr

#include "imageconvert.h"


bool QImageToFloat(const QImage &in, int *width, int *height, float ***R, float ***G, float ***B)
{
    *width  = in.width();       // copie des informations de taille de l'image
    *height = in.height();

    bool r = in.isGrayscale();  // savoir si l'image peut être considérée en niveaux de gris

    *R = AllocateFloatArray(*width, *height); // dans tout les cas on alloue R
    if(r)
        {
        *G = 0; // pas d'allocation
        *B = 0; // pas d'allocation
        }
    else
        {
        *G = AllocateFloatArray( *width, *height);  // allocation pour la composante Verte
        *B = AllocateFloatArray( *width, *height);  // ... puis pour la bleue
        }

    QRgb value;
    for(int x=0; x< *width; x++)
        for(int y=0;y< *height; y++)
            {
            value = in.pixel( x, y); // lecture pixel (x,y)
            R[0][x][y] = qRed(value);
            if(!r)
                {
                G[0][x][y] = qGreen(value);
                B[0][x][y] = qBlue(value);
                }
            }

    return r;
}


void FloatToQImage(QImage &out, int width, int height, const float * const *R, const float * const *G, const float * const *B, QImage::Format format)
{
     out = QImage( width, height, format); //creation image out

     QRgb value;
     if (R!=0)
        if( (G!=0) && (B!=0) )
        {
             for(int x=0; x< width; x++)
                for(int y=0;y<height; y++)
                    {
                    value = qRgb( (int) R[x][y], (int) G[x][y], (int) B[x][y]  );
                    out.setPixel( x, y, value);
                    }
        }
        else
            for(int x=0; x< width; x++)
                for(int y=0;y<height; y++)
                    {
                    value = qRgb( (int) R[x][y], (int) R[x][y], (int) R[x][y]  );
                    out.setPixel( x, y, value);
                    }

}


float ** AllocateFloatArray(int width, int height)
{
    float ** r = new float*[width];
    for(int i=0; i<width; i++)
        r[i] = new float[height];
    return r;
}

void DestroyFloatArray(float ***a, int width)
{
    if( *a == 0 ) return;
    for(int i=0; i<width; i++)
        delete[] a[0][i];
    delete[] *a;
    *a = 0;
}
