// Clanu 2015 - 2016
// T. Grenier : thomas.grenier@insa-lyon.fr

#include "clanu_process.h"

// to complete for Q1
void Question1(float **Rout, float **Gout, float **Bout, float **Rin, float **Gin, float **Bin, float **Mask, int width, int height, double param) //A quoi sert double param ?
{ //Attention : ajout de {} autour du premier for de cette question. Si bugs...
    for(int x=0; x<width; x++){ //X : N2 : width
        for(int y=0;y<height; y++) // Y : N1 : height
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
}


// to do ... IF Q2
void InpaintingBW(float **Iout, float **Iin, float **Mask, int width, int height, double param)
{
//N1=int width, N2=int height
//float **Iin = Ic, c'est l'image qu'on prend
//float **Iout = y, l'image qu'on rend.
    /* Le programme commence ici */
    //b = Ic;
        float **b=0;
        b = CreationTableau2D(width,height);//Création du tableau dynamique
        RecopieMatrice(Iin, b, width, height); //Iin=>b ; b=In

    //xk = b;
        float **xk=0;
        xk = CreationTableau2D(width,height);//Création du tableau dynamique
        RecopieMatrice(b, xk, width, height);//b=>xk ; xk=b
    //res = 1;
        double res=1.0;
    //rk = b - matrice_A(xk,Masque); %%rk est une matrice
        float **rk=0;
        rk = CreationTableau2D(width,height);//Création du tableau dynamique
        float **temp=0;
        temp = CreationTableau2D(width,height);//Création du tableau dynamique
        matrice_A(temp, xk, Mask, width, height); //temp=matrice_A(xk,Masque)
        Sum2MatTerm2Term(b, temp, rk, width, height, -1.0); //rk=b-matrice_A(xk,Masque)

    //pk=rk;  %%pk est une matrice
        float **pk=0;
        pk = CreationTableau2D(width,height);//Création du tableau dynamique
        RecopieMatrice(rk, pk, width, height); //pk=rk


    //while res > 10^(-2),
        while (res>0.01)
        {
        //Apk=matrice_A(pk,Masque); %%OK méthode implémentée
        float **Apk=0;
        Apk = CreationTableau2D(width,height);//Création du tableau dynamique
        matrice_A(Apk, pk, Mask, width, height);

        //alphak = sum(sum(rk.*rk))/sum(sum(Apk.*pk)); %%Somme (OK) et produit terme à terme (OK) donne un SCALAIRE
        Prod2MatTerm2Term(rk, rk, temp, width, height);//temp=rk.*rk
        double alphak;
        double r;//On instancie r en avance pour ne pas créer de variable supplémentaire inutilement
        ProduitScalaire2D(temp, alphak, width, height);//alphak=sum(sum(rk.*rk))
        Prod2MatTerm2Term(Apk, pk, temp, width, height);//temp=Apk.*pk
        ProduitScalaire2D(temp, r, width, height);//r=sum(sum(Apk.*pk))
        alphak=alphak/r;

        //r=sum(sum(rk.*rk)); %% Somme (OK) et produit terme à terme (OK)
        Prod2MatTerm2Term(rk, rk, temp, width, height);//temp=rk.*rk
        ProduitScalaire2D(temp, r, width, height); //r=sum(sum(rk.*rk))

        //xk = xk + alphak*pk; %%Somme terme à terme (A FAIRE) et produit matriciel (PAS OK)
        Sum2MatTerm2Term(xk, pk, temp, width, height, alphak); //temp=xk + alphak*pk
        RecopieMatrice(temp, xk, width, height);//xk = xk + alphak*pk;

        //rk=rk-alphak*Apk;
        Sum2MatTerm2Term(rk, Apk, temp, width, height, -1.0*alphak);//Si problème, il vient du "-1.0*alphak" | temp=rk-alphak*Apk
        float **rk=0;
        rk = CreationTableau2D(width,height);//Création du tableau dynamique
        RecopieMatrice(temp, rk, width, height);//rk=rk-alphak*Apk

        //res=rk(:)'*rk(:)/(N1*N2);
        /*Une fois que ça sera compris, ça ira beaucoup mieux ! :p*/
        Prod2MatTerm2Term(rk, rk, temp, width, height); //temp=rk.*rk
        ProduitScalaire2D(temp, res, width, height);//res=sum(sum(temp))=sum(sum(rk.*rk))
        res=res/(width*height);

        //betak=sum(sum(rk.*rk))/r;
        Prod2MatTerm2Term(rk, rk, temp, width, height);//temp=rk.*rk
        double betak;
        ProduitScalaire2D(temp, betak, width, height);//betak=sum(sum(temp))
        betak=betak/r;//betak=sum(sum(rk.*rk))/r

        //pk=rk+betak*pk;
        Sum2MatTerm2Term(rk, pk, temp, width, height, betak); //temp=rk + betak*pk
        RecopieMatrice(temp, pk, width, height);
        }

        //y=matrice_A(xk,ones(size(Ic)));
        //Remplissage de temp par des 1
        for (int j=0; j<width; j++){
            for(int i=0; i<height; i++){
                temp[j][i]=1;
            }
        }
        matrice_A(Iout, Iin, Mask, width, height);

}
void matrice_A(float **JOut, float **ImageIn, float **Mask, int width, int height)
{
    JOut = CreationTableau2D(width, height);
    ImageIn = CreationTableau2D(width, height);
    Mask = CreationTableau2D(width, height);
//REMPLISSAGE DE J par des zéros, équivalent J=zeros(size(I)) de MATLAB
for (int j=0; j<width; j++){
    for(int i=0; i<height; i++){
        JOut[j][i]=0.0;
    }
}

for(int y=0; y<height; y++){
    //BOOLEENS DE LA BOUCLE i 1->N1
    //bool_iplus = i<N1;
    int bool_yplus=(y+1<height-1) ? 1 : 0;  //bool_iplus=1 si i<N1, =0 si i>=N1. Traité comme dans MATLAB, utile pour la suite
    //bool_imoins = i>1;
    int bool_ymoins=(y>0) ? 1 : 0;  //Même chose
    //MAX et MIN DE LA BOUCLE i 1->N1
    //iplus = min(i+1,N1);
    int yplus=(y+2<height) ? y+2 : height-1;
    //imoins = max(i-1,1);
    int ymoins=(y>1) ? y : 0; // 0 au second facteur et pas 1 pour prendre en compte le décalage MATLAB/C. Prem pixel en MATLAB = 1, 0 en C
    for(int x=0; x<width; x++){
        //BOOLEENS DE LA BOUCLE j 1->N2
        //bool_jplus = j<N2;
        int bool_xplus=(x+1<width-1) ? 1 : 0;  //bool_iplus=1 si i<N1, =0 si i>=N1. Traité comme dans MATLAB, utile pour la suite
        //bool_jmoins = j>1;
        int bool_xmoins=(x+1>1) ? 1 : 0;  //Même chose
        //MAX et MIN DE LA BOUCLE j 1->N1
        //jplus = min(j+1,N2);
        int xplus=(x+2<width-1) ? x+2 : width-1;
        //jmoins = max(j-1,1);
        int xmoins=(x>0) ? x : 0;
        if(Mask[y][x]>0){ //Fait avec des transpositions matlab => C++
            JOut[y][x]=1/36*(16*ImageIn[y][x] + 4*(bool_yplus*ImageIn[yplus][x] + bool_ymoins*ImageIn[ymoins][x] +
                            bool_xplus*ImageIn[y][xplus] + bool_xmoins*ImageIn[y][xmoins]) +
                            bool_yplus*bool_xplus*ImageIn[yplus][xplus] + bool_ymoins*bool_xplus*ImageIn[ymoins][xplus] +
                            bool_ymoins*bool_xmoins*ImageIn[ymoins][xmoins] + bool_yplus*bool_xmoins*ImageIn[yplus][xmoins]);
        }
        else{
            JOut[y][x]=-height*width*(-8*ImageIn[y][x] + 1*(ImageIn[yplus][x] + ImageIn[ymoins][x] + ImageIn[y][xplus] + ImageIn[y][xmoins]) +
                            (ImageIn[yplus][xplus] + ImageIn[ymoins][xplus] + ImageIn[ymoins][xmoins] + ImageIn[yplus][xmoins]));
        }
        }
    }
}


void Prod2MatTerm2Term(float **Mat1, float **Mat2, float **MatRes, int width, int height)
{
    for (int j=0; j<width; j++){
        for(int i=0; i<height; i++){
            MatRes[j][i]=Mat1[j][i]*Mat2[j][i];
        }
    }

}

void Sum2MatTerm2Term(float **Mat1, float **Mat2, float **MatRes, int width, int height, double param)
{
    for (int j=0; j<width; j++){ //MatRes=Mat1+k*Mat2
        for(int i=0; i<height; i++){
            MatRes[j][i]=Mat1[j][i]+param*Mat2[j][i];
        }
    }

}

void ProduitScalaire2D(float **Matrice, double SommeScal, int width, int height)
{
    SommeScal=0;
    for (int j=0; j<width; j++){
        for(int i=0; i<height; i++){
            SommeScal=SommeScal+Matrice[j][i];
        }
    }
}

void RecopieMatrice(float **Matrice1, float **Matrice2, int width, int height)
{
    for (int j=0; j<width; j++){
        for(int i=0; i<height; i++){
            Matrice2[j][i]=Matrice1[j][i];
        }
    }
}
float ** CreationTableau2D(int width, int height)
{
float ** tableau = new float*[width];
  for(int i=0; i< width; i++)
    tableau[i] = new float[height];
  return tableau;
}

/*
// to do ... IF Q3
void InpaintingColor(float **Rout, float **Gout, float **Bout, float **Rin, float **Gin, float **Bin, float **Mask, int width, int height, double param)
{

}
*/
