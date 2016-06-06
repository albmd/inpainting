// Clanu 2015 - 2016
// T. Grenier : thomas.grenier@insa-lyon.fr

#include "clanu_process.h"

// to complete for Q1
void Question1(float **Rout, float **Gout, float **Bout, float **Rin, float **Gin, float **Bin, float **Mask, int width, int height, double param)
{
    for(int x=0; x<width; x++)
        for(int y=0;y<height; y++)
            {
            if(Mask[x][y]>0){
            Rout[x][y] = 255 -  Rin[x][y];
            if(Gin != 0) Gout[x][y] = 255 -  Gin[x][y];
            if(Bin != 0) Bout[x][y] = 255 - Bin[x][y];
            }
            else{
            Rout[x][y] = Rin[x][y];
            if(Gin != 0) Gout[x][y] = Gin[x][y];
            if(Bin != 0) Bout[x][y] = Bin[x][y];
            }
            }
}


// to do ... IF Q2
void InpaintingBW(float **Iout, float **Iin, float **Mask, int width, int height, double param)
{

}


// to do ... IF Q3
void InpaintingColor(float **Rout, float **Gout, float **Bout, float **Rin, float **Gin, float **Bin, float **Mask, int width, int height, double param)
{

}
