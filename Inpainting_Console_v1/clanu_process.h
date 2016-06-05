// Clanu 2015 - 2016
// T. Grenier : thomas.grenier@insa-lyon.fr

#ifndef CLANU_PROCESS_H
#define CLANU_PROCESS_H

// THESE 3 DECLARATIONS MUST NOT BE MODIFIED
void Question1(float **Rout, float **Gout, float **Bout, float **Rin, float **Gin, float **Bin, float **Mask, int width, int height, double param);
void InpaintingBW(float **Iout, float **Iin, float **Mask, int width, int height, double param);
void InpaintingColor(float **Rout, float **Gout, float **Bout, float **Rin, float **Gin, float **Bin, float **Mask, int width, int height, double param);

//Add your own functions' declaration below





// nothing after this line
#endif 

