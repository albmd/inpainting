// Clanu 2015 - 2016
// T. Grenier : thomas.grenier@insa-lyon.fr

#include <iostream>

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QString>
#include <QImage>

#include "imageconvert.h"
#include "clanu_process.h"


// --input=E:/Documents/Insa-GE/Modules/IF2/Clanu/2015/Data/coming-soon.jpg --mask=E:/Documents/Insa-GE/Modules/IF2/Clanu/2015/Data/coming-soon_mask.jpg --output=E:/Documents/Insa-GE/Modules/IF2/Clanu/2015/Data/coming-soon_out_IFQ1.jpg
int main(int argc, char *argv[])
{
        // ------------------------------------------
        // Gestion des paramètres en ligne de commande
        QCoreApplication app(argc, argv);
        QCoreApplication::setApplicationName("clanu-inpainting");
        QCoreApplication::setApplicationVersion("1.0");

        QCommandLineParser parser;
        parser.setApplicationDescription("Inpainting Console");
        parser.addHelpOption();
        parser.addVersionOption();

        QCommandLineOption inputFileOption(QStringList() << "i" << "input", "Fullpath and extension of the input <file>.", "file");
        parser.addOption(inputFileOption);

        QCommandLineOption maskFileOption(QStringList() << "m" << "mask", "Fullpath and extension of the mask <file>.", "file");
        parser.addOption(maskFileOption);

        QCommandLineOption outputFileOption(QStringList() << "o" << "output", "Fullpath and extension of the output <file>.", "file");
        parser.addOption(outputFileOption);

        // Process the actual command line arguments given by the user
        parser.process(app);

        QString inputFileName  = parser.value(inputFileOption);
        QString maskFileName   = parser.value(maskFileOption);
        QString outputFileName = parser.value(outputFileOption);

        std::cout << " input  " << inputFileName.toStdString()  <<  std::endl;
        std::cout << " output " << outputFileName.toStdString() <<  std::endl;
        std::cout << " mask   " << maskFileName.toStdString()   <<  std::endl;

        if(   maskFileName.isEmpty() ) { std::cout << "!! Mask is NOT SET and must be set!"   << std::endl; return -1; }
        if(  inputFileName.isEmpty() ) { std::cout << "!! Input is NOT SET and must be set!"  << std::endl; return -1; }
        if( outputFileName.isEmpty() ) { std::cout << "!! Output is NOT SET and must be set!" << std::endl; return -1; }

        std::cout << " - Input image file read : " << inputFileName.toStdString() << std::endl;
        std::cout << " - Mask image file read  : " << maskFileName.toStdString() << std::endl;
        std::cout << " - Output image file     : " << outputFileName.toStdString() << std::endl;
        // ------------------------------------------


        // ------------------------------------------
        // Lecture des images input et mask, and
        std::cout << "> Open input image          " ;
        QImage    InputImage = QImage(inputFileName);
        std::cout << " done !" << std::endl;

        std::cout << "> Open mask image           " ;
        QImage    MaskImage  = QImage(maskFileName);
        std::cout << " done !" << std::endl;

        if(   InputImage.isNull() ) { std::cout << "!! Error reading input image file : !" << inputFileName.toStdString() << std::endl; return -1; }
        if(    MaskImage.isNull() ) { std::cout << "!! Error reading mask image file  : !" << maskFileName.toStdString()  << std::endl; return -1; }
        // ------------------------------------------


        // ------------------------------------------
        // Opening images
        int In_w, In_h;   // width and height for input image
        float **In_R = 0, **In_G  = 0 , **In_B = 0; // float arrays for input

        int Mask_w, Mask_h;
        float **Mask_R = 0, **Mask_G  = 0 , **Mask_B = 0; // float arrays for mask

        float **Out_R = 0, **Out_G  = 0 , **Out_B = 0; // float arrays for output

        // convert QImage to float **
        std::cout << "> Converting : " << std::endl;
        std::cout << ">            - input        " ;
        bool isGray = QImageToFloat(InputImage, &In_w,  &In_h, &In_R, &In_G, &In_B);
        std::cout << " done !" << std::endl;

        std::cout << ">            - mask         " ;
        QImageToFloat(MaskImage, &Mask_w, &Mask_h, &Mask_R, &Mask_G, &Mask_B);
        std::cout << " done !" << std::endl;
        if( (In_w != Mask_w) || (In_h != Mask_h) ) { std::cout << "!! Sizes mismatch between mask and input image !!" << std::endl; return -1;}

        // Allocate spaces for output
        std::cout << "> Allocate output space     " ;
        Out_R             = AllocateFloatArray( In_w, In_h);
        if(!isGray) Out_G = AllocateFloatArray( In_w, In_h);
        if(!isGray) Out_B = AllocateFloatArray( In_w, In_h);
        std::cout << " done !" << std::endl;
        // ------------------------------------------


        // ------------------------------------------
        // Appel de la fonction de traitement Question 1
        // Call student processing function
        std::cout << "> Computing...              ";
        // For question 1 :
        Question1( Out_R, Out_G, Out_B, In_R, In_G, In_B, Mask_R, In_w, In_h, 10);
        std::cout << " done !" << std::endl;

        // back convert to QImage
        std::cout << "> Convert to QImage         " ;
        QImage    OutputImage;
        FloatToQImage( OutputImage, In_w, In_h, Out_R, Out_G, Out_B, InputImage.format());
        std::cout << " done !" << std::endl;
        // ------------------------------------------



        /*  //  <- Remove for question 2
        // ------------------------------------------
        // Appel de la fonction de traitement Question 2
        // Call student processing function
        std::cout << "> Computing B/W inpainting  ";
        InpaintingBW( Out_R, In_R, Mask_R, In_w, In_h, 10);
        std::cout << " done !" << std::endl;

        // back convert to QImage
        std::cout << "> Convert to QImage         " ;
        QImage    OutputImage;
        FloatToQImage( OutputImage, In_w, In_h, Out_R, 0, 0, InputImage.format());
        std::cout << " done !" << std::endl;
        // ------------------------------------------
        */ //   <- Remove for question 2



        /*  //  <- Remove for question 3
        // ------------------------------------------
        // Appel de la fonction de traitement Question 3
        // Call student processing function
        std::cout << "> Computing color inpainting";
        InpaintingColor( Out_R, Out_G, Out_B, In_R, In_G, In_B, Mask_R, In_w, In_h, 10);
        std::cout << " done !" << std::endl;

        // back convert to QImage
        std::cout << "> Convert to QImage         " ;
        QImage    OutputImage;
        FloatToQImage( OutputImage, In_w, In_h, Out_R, Out_G, Out_B, InputImage.format());
        std::cout << " done !" << std::endl;
        // ------------------------------------------
        */ //   <- Remove for question 3


        // ------------------------------------------
        // Free spaces
        std::cout << "> Free spaces               " ;
        DestroyFloatArray(&Out_B, In_w);
        DestroyFloatArray(&Out_G, In_w);
        DestroyFloatArray(&Out_R, In_w);
        DestroyFloatArray(&Mask_B, Mask_w);
        DestroyFloatArray(&Mask_G, Mask_w);
        DestroyFloatArray(&Mask_R, Mask_w);
        DestroyFloatArray(&In_B, In_w);
        DestroyFloatArray(&In_G, In_w);
        DestroyFloatArray(&In_R, In_w);
        std::cout << " done !" << std::endl;
        // ------------------------------------------


        // ------------------------------------------
        // Write image output
        std::cout << "> Write output image        " ;
        OutputImage.save(outputFileName);
        std::cout << " done !" << std::endl;
        // ------------------------------------------


    return 0;
}
