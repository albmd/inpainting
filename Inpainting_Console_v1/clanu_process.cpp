// Clanu 2015 - 2016
// T. Grenier : thomas.grenier@insa-lyon.fr

#include "clanu_process.h"
#include <iostream>

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
    std::cout << "paramQ1"<<param;
}


// to do ... IF Q2
void InpaintingBW(float **Iout, float **Iin, float **Mask, int width, int height, double param)
{
    //Déclaration des variables
     float **b = CreationTableau2D(width,height);//Création du tableau dynamique
     float **xk= CreationTableau2D(width,height);//Création du tableau dynamique
     float **rk = CreationTableau2D(width,height);//Création du tableau dynamique
     float **temp= CreationTableau2D(width,height);//Création du tableau dynamique
     //std::cout << "Temp[fin][random]AU DEBUT "<<temp[45][22]<<"   ";
     float **pk=CreationTableau2D(width,height);//Création du tableau dynamique
     double res=1.0;
     float **Apk = CreationTableau2D(width,height);//Création du tableau dynamique
     double alphak;
     double r;//On instancie r en avance pour ne pas créer de variable supplémentaire inutilement
     //float **rk= CreationTableau2D(width,height);//Création du tableau dynamique
     float betak;
     //float **y=CreationTableau2D(width, height);
     //std::cout << "paramQ2   "<<param;

    /*std::cout << "Iout[20][20]"<<Iout[20][20];
    std::cout << "Iin[20][20]"<<Iin[20][20];
    std::cout << "Mask[20][20]"<<Mask[20][20];*/
//N1=int width, N2=int height
//float **Iin = Ic, c'est l'image qu'on prend
//float **Iout = y, l'image qu'on rend.
    /* Le programme commence ici */
    //b = Ic;
        //float **b = CreationTableau2D(width,height);//Création du tableau dynamique
        RecopieMatrice(Iin, b, width, height); //Iin=>b ; b=In
         //std::cout << "Iin[20][20]"<<Iin[20][20]<<endl;
         //std::cout << "b[20][20]"<<b[20][20];
    //xk = b;
        //float **xk= CreationTableau2D(width,height);//Création du tableau dynamique
        RecopieMatrice(b, xk, width, height);//b=>xk ; xk=b
    //res = 1;
        //double res=1.0;
    //rk = b - matrice_A(xk,Masque); %%rk est une matrice
        //float **rk = CreationTableau2D(width,height);//Création du tableau dynamique
        //float **temp= CreationTableau2D(width,height);//Création du tableau dynamique
//        std::cout << "temp[20][20] AVANT MATRICE A"<<temp[20][20]<<std::endl;
//        std::cout << "xk[20][20] AVANT MATRICE A"<<xk[20][20]<<std::endl;
//        std::cout << "Mask[20][20] AVANT MATRICE A"<<Mask[20][20]<<std::endl;

        matrice_A(temp, xk, Mask, width, height); //temp=matrice_A(xk,Masque)

//        std::cout << "temp[20][20] APRES MATRICE A"<<temp[20][20]<<std::endl;
//        std::cout << "xk[20][20] APRES MATRICE A"<<xk[20][20]<<std::endl;
//        std::cout << "Mask[20][20] APRES MATRICE A"<<Mask[20][20]<<std::endl;
        Sum2MatTerm2Term(b, temp, rk, width, height, -1.0); //rk=b-matrice_A(xk,Masque)

    //pk=rk;  %%pk est une matrice
        //float **pk = CreationTableau2D(width,height);//Création du tableau dynamique
        RecopieMatrice(rk, pk, width, height); //pk=rk


    //while res > 10^(-2),
        while (res>0.01)
        {
            r=0.0;
        //Apk=matrice_A(pk,Masque); %%OK méthode implémentée
        //float **Apk = CreationTableau2D(width,height);//Création du tableau dynamique
        matrice_A(Apk, pk, Mask, width, height);
        //std::cout << "Temp[40][22]APRES matrice_a "<<temp[45][22]<<"   ";//=0

        //alphak = sum(sum(rk.*rk))/sum(sum(Apk.*pk)); %%Somme (OK) et produit terme à terme (OK) donne un SCALAIRE
        Prod2MatTerm2Term(rk, rk, temp, width, height);//temp=rk.*rk
        //std::cout << "Temp[40][22]APRES PROD2MATTERM2TERM "<<temp[45][22]<<"   ";//1904
        //std::cout << "temp[20][20] dans while apres Prod2MatTerm2Term"<<temp[20][20]; //=0
        //double alphak;
        //double r=0.0;//On instancie r en avance pour ne pas créer de variable supplémentaire inutilement
        alphak = ProduitScalaire2D(temp, width, height);//alphak=sum(sum(rk.*rk))
        //std::cout << "Temp[49][22]APRES PRODSCALAIRE2D "<<temp[220][220]<<"   ";
        //std::cout << "alphak"<<alphak<<"   ";
        Prod2MatTerm2Term(Apk, pk, temp, width, height);//temp=Apk.*pk
        r = ProduitScalaire2D(temp, width, height);//r=sum(sum(Apk.*pk))
//        std::cout << "r"<<r;
        alphak=alphak/r;
        r=0.0;
        //r=sum(sum(rk.*rk)); %% Somme (OK) et produit terme à terme (OK)
        Prod2MatTerm2Term(rk, rk, temp, width, height);//temp=rk.*rk
        r = ProduitScalaire2D(temp, width, height); //r=sum(sum(rk.*rk))

        //xk = xk + alphak*pk; %%Somme terme à terme (A FAIRE) et produit matriciel (PAS OK)
        Sum2MatTerm2Term(xk, pk, temp, width, height, alphak); //temp=xk + alphak*pk
        RecopieMatrice(temp, xk, width, height);//xk = xk + alphak*pk;

        //rk=rk-alphak*Apk;
        Sum2MatTerm2Term(rk, Apk, temp, width, height, -1.0*alphak);//Si problème, il vient du "-1.0*alphak" | temp=rk-alphak*Apk
        //float **rk= CreationTableau2D(width,height);//Création du tableau dynamique
        RecopieMatrice(temp, rk, width, height);//rk=rk-alphak*Apk

        //res=rk(:)'*rk(:)/(N1*N2);
        /*Une fois que ça sera compris, ça ira beaucoup mieux ! :p*/
        //std::cout << "Temp[40][22]AVANT PROD2MATTERM2TERM "<<temp[45][22]<<"   "; //Not a number
        Prod2MatTerm2Term(rk, rk, temp, width, height); //temp=rk.*rk
        //std::cout << "Temp[40][22]APRES PROD2MATTERM2TERM "<<temp[45][22]<<"   ";
        res = ProduitScalaire2D(temp, width, height);//res=sum(sum(temp))=sum(sum(rk.*rk))
        res=res/(width*height);
//        std::cout << "Res = "<<res<<"   ";

        //betak=sum(sum(rk.*rk))/r;
        Prod2MatTerm2Term(rk, rk, temp, width, height);//temp=rk.*rk
        //double betak;
//        std::cout << "Temp[fin][random]AVANT ProduitScalaire2D "<<temp[45][22]<<"   ";
        betak = ProduitScalaire2D(temp, width, height);//betak=sum(sum(temp))
        betak=betak/r;//betak=sum(sum(rk.*rk))/r

        //pk=rk+betak*pk;

        Sum2MatTerm2Term(rk, pk, temp, width, height, betak); //temp=rk + betak*pk
//        std::cout << "Temp[fin][random] "<<temp[45][22]<<"   ";
//        std::cout << "pk[fin][random] "<<pk[45][22]<<"   ";
        RecopieMatrice(temp, pk, width, height);
        std::cout << "res"<<res<<"   ";
        }
        //std::cout << "Temp[fin][random] "<<temp[45][22]<<"   ";
        //std::cout << "pk[fin][random] "<<pk[45][22]<<"   ";

        //y=matrice_A(xk,ones(size(Ic)));
        //Remplissage de temp par des 1
        for (int j=0; j<width; j++){
            for(int i=0; i<height; i++){
                temp[j][i]=1.0;
            }
        }
        //matrice_A(Iout, Iin, Mask, width, height);
        //float **y=CreationTableau2D(width, height);

        std::cout << "res"<<res<<"   ";
        matrice_A(Iout, xk, temp, width, height);
        //RecopieMatrice(y,Iout,width,height);

}
void matrice_A(float **JOut, float **ImageIn, float **Mask, int width, int height)
{
    //float **ImageIn=0;
    //ImageIn = CreationTableau2D(width, height); Lorsqu'on avait encore ImageEntree en paramètre de la fonction et qu'on travaillait sur une copie
    //RecopieMatrice(ImageEntree,ImageIn,width,height);
    //JOut = CreationTableau2D(width, height);
    //ImageIn = CreationTableau2D(width, height);
    //Mask = CreationTableau2D(width, height);
//REMPLISSAGE DE J par des zéros, équivalent J=zeros(size(I)) de MATLAB
for (int j=0; j<width; j++){
    for(int i=0; i<height; i++){
        JOut[j][i]=0.0;
    }
}

for(int y=0; y<height; y++){
    //BOOLEENS DE LA BOUCLE i 1->N1
    //bool_iplus = i<N1;
    int bool_yplus=(y<height-1) ? 1 : 0;  //bool_iplus=1 si i<N1, =0 si i>=N1. Traité comme dans MATLAB, utile pour la suite
    //bool_imoins = i>1;
    int bool_ymoins=(y>0) ? 1 : 0;  //Même chose
    //MAX et MIN DE LA BOUCLE i 1->N1
    //iplus = min(i+1,N1);
    int yplus=(y+1<height-1) ? y+1 : height-1;
    //imoins = max(i-1,1);
    int ymoins=(y>0) ? y : 0; // 0 au second facteur et pas 1 pour prendre en compte le décalage MATLAB/C. Prem pixel en MATLAB = 1, 0 en C
    for(int x=0; x<width; x++){
        //BOOLEENS DE LA BOUCLE j 1->N2
        //bool_jplus = j<N2;
        int bool_xplus=(x<width-1) ? 1 : 0;  //bool_iplus=1 si i<N1, =0 si i>=N1. Traité comme dans MATLAB, utile pour la suite
        //bool_jmoins = j>1;
        int bool_xmoins=(x>0) ? 1 : 0;  //Même chose
        //MAX et MIN DE LA BOUCLE j 1->N1
        //jplus = min(j+1,N2);
        int xplus=(x+1<width-1) ? x+1 : width-1;
        //jmoins = max(j-1,1);
        int xmoins=(x>0) ? x : 0;
        if(Mask[y][x]>0){ //Fait avec des transpositions matlab => C++
            JOut[y][x]=(1/36)*(16*ImageIn[y][x] + 4*(bool_yplus*ImageIn[yplus][x] + bool_ymoins*ImageIn[ymoins][x] +
                            bool_xplus*ImageIn[y][xplus] + bool_xmoins*ImageIn[y][xmoins]) +
                            bool_yplus*bool_xplus*ImageIn[yplus][xplus] + bool_ymoins*bool_xplus*ImageIn[ymoins][xplus] +
                            bool_ymoins*bool_xmoins*ImageIn[ymoins][xmoins] + bool_yplus*bool_xmoins*ImageIn[yplus][xmoins]);
        }
        else{
            JOut[y][x]=-1.0*height*width*(-8*ImageIn[y][x] + 1*(ImageIn[yplus][x] + ImageIn[ymoins][x] + ImageIn[y][xplus] + ImageIn[y][xmoins]) +
                            (ImageIn[yplus][xplus] + ImageIn[ymoins][xplus] + ImageIn[ymoins][xmoins] + ImageIn[yplus][xmoins]));
        }

        }
    //std::cout << JOut[20][25]; // pour vérifier les valeurs de l'image.. RESULTAT PAS BIEN : il faut creuser du côté de JOut.
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

double ProduitScalaire2D(float **Matrice, int width, int height)
{
    double SommeScal=0.0;
    for (int j=0; j<width; j++){
        for(int i=0; i<height; i++){
            SommeScal=SommeScal+Matrice[j][i];
            //std::cout << "SommeScal"<<SommeScal;
        }
    }
    return SommeScal;
}

void RecopieMatrice(float **Matrice1, float **Matrice2, int width, int height) //Matrice 1 => Matrice 2
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
  //std::cout << "Tableau nouvellement instancié "<<tableau[45][22]<<"   ";
  return tableau;
}

/*
// to do ... IF Q3
void InpaintingColor(float **Rout, float **Gout, float **Bout, float **Rin, float **Gin, float **Bin, float **Mask, int width, int height, double param)
{

}
*/
