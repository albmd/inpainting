#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QInputDialog>
#include <QColorDialog>

#include <QMessageBox>

#include <QStandardPaths>

#include <QGraphicsScene>
#include <QImageReader>
#include <QGraphicsPixmapItem>
#include <QColor>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    bool loadImage(const QString &fileName);
    bool loadMask(const QString &fileName);

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_horizontalSlider_valueChanged(int value);

    void on_actionSave_triggered();

    void on_actionProcess_triggered();

    void on_actionSize_triggered();

    void on_actionSave_Mask_triggered();

    void on_actionOpen_Mask_Image_triggered();

private:
    Ui::MainWindow *ui;

    QImage InputImage;
    QImage MaskImage;
    QImage OutputImage;

   QGraphicsScene* InputScene;
   QGraphicsScene* OutputScene;

   QGraphicsPixmapItem* InputItem;
   QGraphicsPixmapItem* OutputItem;
   QGraphicsPixmapItem* MaskItem;

   int myPenWidth;

};

#endif // MAINWINDOW_H
