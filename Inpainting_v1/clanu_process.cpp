#include "clanu_process.h"




void InpaintingBW(float **Iout, float **Iin, float **Mask, int width, int height, double param)
{

}

void InpaintingColor(float **Rout, float **Gout, float **Bout, float **Rin, float **Gin, float **Bin, float **Mask, int width, int height, double param)
{
    for(int x=0; x<width; x++)
        for(int y=0;y<height; y++)
            {
            if(Mask[x][y] > param )
                {
                Rout[x][y] = 255 -  Rin[x][y];
                if(Gin != 0) Gout[x][y] = 255 -  Gin[x][y];
                if(Bin != 0) Bout[x][y] = 255 - Bin[x][y];
                }
            else
                {
                Rout[x][y] = Rin[x][y];
                if(Gin != 0) Gout[x][y] = Gin[x][y];
                if(Bin != 0) Bout[x][y] = Bin[x][y];
                }

            }
}






/*{
    for(int x=0; x<width; x++)
        for(int y=0;y<height; y++)
            {
            Rout[x][y] = 255 - Rin[x][y];               // cas noir et blanc
            if(Gin != 0) Gout[x][y] = 255 - Gin[x][y];  // cas des images couleurs
            if(Bin != 0) Bout[x][y] = 255 - Bin[x][y];  // cas des images couleurs
            }
}
*/
