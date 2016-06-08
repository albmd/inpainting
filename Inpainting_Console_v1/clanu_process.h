// Clanu 2015 - 2016
// T. Grenier : thomas.grenier@insa-lyon.fr

#ifndef CLANU_PROCESS_H
#define CLANU_PROCESS_H

// THESE 3 DECLARATIONS MUST NOT BE MODIFIED
void Question1(float **Rout, float **Gout, float **Bout, float **Rin, float **Gin, float **Bin, float **Mask, int width, int height, double param);
void InpaintingBW(float **Iout, float **Iin, float **Mask, int width, int height, double param);
void InpaintingColor(float **Rout, float **Gout, float **Bout, float **Rin, float **Gin, float **Bin, float **Mask, int width, int height, double param);

//Add your own functions' declaration below
void matrice_A(float **JOut, float **ImageIn, float **Mask, int width, int height);
void Prod2MatTerm2Term(float **Mat1, float **Mat2, float **MatRes, int width, int height);
void Sum2MatTerm2Term(float **Mat1, float **Mat2, float **MatRes, int width, int height, double param);
//void Soustr2MatTerm2Term(float **Mat1, float **Mat2, float **MatRes, int width, int height);
void ProduitScalaire2D(float **Matrice, double SommeScal, int width, int height); //sumsum en matlab
void RecopieMatrice(float **Matrice1, float **Matrice2, int width, int height); //Matrice 1 => Matrice 2
float ** CreationTableau2D(int width, int height);
// nothing after this line
#endif 

