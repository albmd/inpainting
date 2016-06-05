#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "clanu_process.h"
#include "imageconvert.h"

// Constructeur : Initialiser les champs de la classe
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    InputScene = new QGraphicsScene();
    OutputScene = new QGraphicsScene();

     ui->Input_graphicsView->setScene( InputScene );
     ui->Output_graphicsView->setScene( OutputScene );

    InputItem = NULL;
    OutputItem = NULL;
    MaskItem = NULL;

    ui->Input_graphicsView->SetScrollHSync( ui->Output_graphicsView->horizontalScrollBar() );
    ui->Input_graphicsView->SetScrollVSync( ui->Output_graphicsView->verticalScrollBar()   );

}

// destructeur: libérer les espaces mémoires réservés
MainWindow::~MainWindow()
{

    if(InputItem != NULL) delete InputItem;
    if(OutputItem != NULL) delete OutputItem;

    delete InputScene;
    delete OutputScene;

    delete ui;
}

// Action du menu 'open'
void MainWindow::on_actionOpen_triggered()
{
    // Récuperer les types d'images supportés par le système
    QStringList mimeTypeFilters;
    foreach (const QByteArray &mimeTypeName, QImageReader::supportedMimeTypes())
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();

    // Récuperer la liste des répertoires privilégiés du
    // stockage des 'Images' du système
    const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    QFileDialog dialog(this, tr("Open File"),
                       picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.first());
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");

    // essayer de lire et convertir au format image le fichier souhaité
    while (dialog.exec() == QDialog::Accepted && !loadImage(dialog.selectedFiles().first())) {}
}


// Fonction de lecture d'un fichier image (+ conversion)
bool MainWindow::loadImage(const QString &fileName)
{
    // tout est dans cette premiere ligne (merci Qt !)
    InputImage = QImage(fileName);

    //mais si ca marche pas, prévenir qu'il y a eu un souci
    if (InputImage.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1.").arg(QDir::toNativeSeparators(fileName)));
        setWindowFilePath(QString());

        //mettre à jour les affichages
        if(InputItem != NULL)  // affichage de l'image d'entrée
            {
            InputScene->removeItem( InputItem );
            delete InputItem;
            }
        if(MaskItem != NULL)  //efface le masque
            {
            InputScene->removeItem( MaskItem );
            delete MaskItem;
            }
        if(OutputItem != NULL) // affichage de l'image traitée
            {
            OutputScene->removeItem( OutputItem );
            delete OutputItem;
            }
        InputItem = new QGraphicsPixmapItem( QPixmap() );
        OutputItem = new QGraphicsPixmapItem( QPixmap() );

        InputScene->clear(); //nettoyage qui marche pas!
        InputScene->addItem( InputItem );
        ui->Input_graphicsView->show();

        OutputScene->addItem( OutputItem );
        ui->Output_graphicsView->show();
        return false;
    }

    //si tout va bien on affiche l'image lue ...
    InputScene->clear();
    /*if(InputItem != NULL)
        {
        InputScene->removeItem( InputItem );
        delete InputItem;
        }
    if(MaskItem != NULL)  // efface le masque
        {
        InputScene->removeItem( MaskItem );
        delete MaskItem;
        }
    // ... puis on efface le résultat présent
   */ if(OutputItem != NULL)
        {
        OutputScene->removeItem( OutputItem );
        delete OutputItem;
        }

    InputItem = new QGraphicsPixmapItem( QPixmap::fromImage(InputImage) );
    OutputItem = new QGraphicsPixmapItem( QPixmap() );
    ui->Input_graphicsView->SetImageItem(InputItem);

    InputScene->addItem( InputItem );
    ui->Input_graphicsView->show();

    OutputScene->addItem( OutputItem );
    ui->Output_graphicsView->show();


    setWindowFilePath(fileName);
    return true;
}

// Gestion du zoom : on applique une transformation (scale) aux images
void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    QTransform t = ui->Input_graphicsView->transform();
    t.reset(); // matrice identité
    if( value > 0) // si le slider a une valeur positive
        t.scale(1.0/value, 1.0/value ); // on s'éloigne (ratio < 1)
    if( value < 0) // si la valeur du slider est négative
         t.scale(-value, -value ); // on zoome et on oublie pas d'inverser le signe
    ui->Input_graphicsView->setTransform(t); // on applique la transformation
    ui->Output_graphicsView->setTransform(t);  // au deux graphicView
}


// Sauvegarde de l'image
void MainWindow::on_actionSave_triggered()
{
    if( !OutputImage.isNull() )
        {
        QStringList mimeTypeFilters; // les types images du système
        foreach (const QByteArray &mimeTypeName, QImageReader::supportedMimeTypes())
            mimeTypeFilters.append(mimeTypeName);
        mimeTypeFilters.sort();

        // dialogue pour la sauvegarde de l'image
        QFileDialog dialog(this, tr("Save File"), QDir::currentPath() );
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.setMimeTypeFilters(mimeTypeFilters);
        dialog.selectMimeTypeFilter("image/jpeg");

        while (dialog.exec() == QDialog::Accepted && !OutputImage.save(dialog.selectedFiles().first())) {}
        }
}

// Traitement de l'image
void MainWindow::on_actionProcess_triggered()
{
    if( !InputImage.isNull() )
        { // /*v 0.1*/  MyProcess( InputImage, OutputImage, 255 );
        // ------------------------------------------
        // Appel de la fonction de traitement

        int w, h;
        float **Rin = 0, **Gin  = 0 , **Bin = 0;
        float **Rout = 0, **Gout  = 0 , **Bout = 0;
        int Mask_w, Mask_h;
        float **Mask_R = 0, **Mask_G  = 0 , **Mask_B = 0; // float arrays for mask

        // convert QImage to float **
        bool isGray = QImageToFloat(InputImage, &w,  &h, &Rin, &Gin, &Bin);

        // Allocate spaces for output
        Rout = AllocateFloatArray(w,h);
        if(!isGray) Gout = AllocateFloatArray(w,h);
        if(!isGray) Bout = AllocateFloatArray(w,h);

        if( MaskImage.isNull() ) MaskImage = ui->Input_graphicsView->GetMask();

        if( ui->Input_graphicsView->isMaskModified() )  MaskImage = ui->Input_graphicsView->GetMask();

        // Convert Mask
        QImageToFloat(MaskImage, &Mask_w, &Mask_h, &Mask_R, &Mask_G, &Mask_B);
        if( (w != Mask_w) || (h != Mask_h) )
        {
            QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                     tr("Mask and image size are different (%1.%2  %3.%4)")
                                     .arg(QString::number(h), QString::number(Mask_h),
                                          QString::number(w), QString::number(Mask_w) ) );
        }
        else
        {
            // Call student processing function
            InpaintingColor( Rout, Gout, Bout, Rin, Gin, Bin, Mask_R, w, h, 0);

            // back convert to QImage
            FloatToQImage( OutputImage, w, h, Rout, Gout, Bout, InputImage.format());
        }



        // Free spaces
        DestroyFloatArray(&Mask_B, Mask_w);
        DestroyFloatArray(&Mask_G, Mask_w);
        DestroyFloatArray(&Mask_R, Mask_w);
        DestroyFloatArray(&Bout, w);
        DestroyFloatArray(&Gout, w);
        DestroyFloatArray(&Rout, w);
        DestroyFloatArray(&Bin, w);
        DestroyFloatArray(&Gin, w);
        DestroyFloatArray(&Rin, w);
        // ------------------------------------------

        // Mise à jour de l'affichage
        if(OutputItem != NULL) // s'il existe, on supprime l'affichage précédent
            {
            OutputScene->removeItem( OutputItem );
            delete OutputItem;
            }
        // on affiche le résultat contenu dans OutputImage
        OutputItem = new QGraphicsPixmapItem(QPixmap::fromImage(OutputImage));

        OutputScene->addItem( OutputItem );
        ui->Output_graphicsView->show();
        }
}


void MainWindow::on_actionSize_triggered()
{
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("Pen"),
                                        tr("Select pen width:"),
                                        myPenWidth, 1, 200, 1, &ok);
    if (ok)
        {
        myPenWidth = newWidth;
        ui->Input_graphicsView->SetPenWidth(myPenWidth);
        }
}

void MainWindow::on_actionSave_Mask_triggered()
{
   MaskImage = ui->Input_graphicsView->GetMask();
   QStringList mimeTypeFilters; // les types images du système
   foreach (const QByteArray &mimeTypeName, QImageReader::supportedMimeTypes())
       mimeTypeFilters.append(mimeTypeName);
   mimeTypeFilters.sort();

   // dialogue pour la sauvegarde de l'image
   QFileDialog dialog(this, tr("Save File"), QDir::currentPath() );
   dialog.setAcceptMode(QFileDialog::AcceptSave);
   dialog.setMimeTypeFilters(mimeTypeFilters);
   dialog.selectMimeTypeFilter("image/png");

   while (dialog.exec() == QDialog::Accepted && !MaskImage.save(dialog.selectedFiles().first())) {}
}

void MainWindow::on_actionOpen_Mask_Image_triggered()
{

    // Récuperer les types d'images supportés par le système
    QStringList mimeTypeFilters;
    foreach (const QByteArray &mimeTypeName, QImageReader::supportedMimeTypes())
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();

    // Récuperer la liste des répertoires privilégiés du
    // stockage des 'Images' du système
    const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    QFileDialog dialog(this, tr("Open File"),
                       picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.first());
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/png");

    // essayer de lire et convertir au format image le fichier souhaité
    while (dialog.exec() == QDialog::Accepted && !loadMask(dialog.selectedFiles().first())) {}
}

bool MainWindow::loadMask(const QString &fileName)
{
   if( InputItem != NULL)
   {
    // tout est dans cette premiere ligne (merci Qt !)
    MaskImage = QImage(fileName);

    //mais si ca marche pas, prévenir qu'il y a eu un souci
    if (MaskImage.isNull())
        {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1.").arg(QDir::toNativeSeparators(fileName)));
        return false;
        }
    if( MaskItem != NULL)
    {
        InputScene->removeItem( MaskItem );
        delete MaskItem;
    }
    MaskItem = new QGraphicsPixmapItem(  QPixmap::fromImage(MaskImage) );
    MaskItem->setOpacity(0.2);  // <- Set the opacity here
    InputScene->addItem( MaskItem );
    ui->Input_graphicsView->show();
    return true;
   }

}
