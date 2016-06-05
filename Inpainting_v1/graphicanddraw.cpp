// Clanu 2015 - 2016
// T. Grenier : thomas.grenier@insa-lyon.fr


#include <QtWidgets>
#include "graphicanddraw.h"


GraphicAndDraw::GraphicAndDraw(QWidget *parent) : QGraphicsView(parent)
{
    myPenWidth = 10;
    myPenColor = Qt::black;

    ItemToDraw = 0;
    ImageItem  = 0;
    this->setInteractive(false);

    MaskModified = false;
}


GraphicAndDraw::~GraphicAndDraw()
{


}

void GraphicAndDraw::SetScrollHSync(QScrollBar *SyncToViewScrollBar)
{
    const QScrollBar * scrollbarH = horizontalScrollBar();
    connect( scrollbarH, SIGNAL(valueChanged(int)), SyncToViewScrollBar, SLOT(setValue(int)));
}

void GraphicAndDraw::SetScrollVSync(QScrollBar *SyncToViewScrollBar)
{
    const QScrollBar * scrollbarV = verticalScrollBar();
    connect( scrollbarV, SIGNAL(valueChanged(int)), SyncToViewScrollBar, SLOT(setValue(int)));
}



// Add a new Image Item, and so remove existing drawings
void GraphicAndDraw::SetImageItem(const QGraphicsPixmapItem *imageItem)
{
    ImageItem = const_cast<QGraphicsPixmapItem*>( imageItem );
    ListOfItems.clear();
    this->setInteractive(true);
}


// Color change
void GraphicAndDraw::SetPenColor(const QColor &newColor)
{
    myPenColor = newColor;
}


// Pen size change
void GraphicAndDraw::SetPenWidth(int newWidth)
{
    myPenWidth = newWidth;
}


// Start recording mouse moving
void GraphicAndDraw::mousePressEvent(QMouseEvent *event)
{
    if(ImageItem != 0) OrigPoint = mapToScene( event->pos( ));
    MaskModified = true;

    QGraphicsView::mousePressEvent(event);


}

// The mouse is moving: paint!
void GraphicAndDraw::mouseMoveEvent(QMouseEvent *event)
{
    if(ImageItem != 0)
        {
   //     if(!ItemToDraw)
            {
            ItemToDraw = new QGraphicsLineItem;
            this->scene()->addItem(ItemToDraw);
            ItemToDraw->setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap));
            //ItemToDraw->setPos(OrigPoint);  // to draw some lines and not point
            }

        QPointF tmp = mapToScene( event->pos() );
        QLineF tmp_line(OrigPoint.x(), OrigPoint.y(), tmp.x(), tmp.y() );
                              //tmp.x() - OrigPoint.x() ,
                              // tmp.y() - OrigPoint.y() ); // other approach
        ItemToDraw->setLine( tmp_line );
        ListOfItems.push_back( ItemToDraw->line() );
        OrigPoint = tmp;

        }
    QGraphicsView::mouseMoveEvent(event);
}

// Ok stop to drow as the mouse button is released
void GraphicAndDraw::mouseReleaseEvent(QMouseEvent *event)
{
    if(ImageItem != 0)
        {
        ListOfItems.push_back( ItemToDraw->line() );
        ItemToDraw = 0;
        }
    QGraphicsView::mouseReleaseEvent(event);
}

// Convert mouse drawings to an image
QImage GraphicAndDraw::GetMask()
{   // First create a scene
    QGraphicsScene *tmp_scene = new QGraphicsScene( this->sceneRect() );

    //Then we receate all small recorded drawing to this new scene
    for (QList<QLineF> ::const_iterator it = ListOfItems.begin(); it != ListOfItems.end(); ++it)
        tmp_scene->addLine( *it, QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap) );

    // At the end of this previous for loop, the content is the same as the one displayed without the Image

    // Ok so we get the original image size in order to produce a mask of exactly the same size
    QImage mask(ImageItem->pixmap().size(), QImage::Format_RGB32);  // Create the image with the exact size of the shrunk scene
    mask.fill(Qt::white);                                              // Start all pixels white

    // Now we convert the scene to an image (named mask) using the QPainter capabilities
    QPainter painter( &mask);
    tmp_scene->render(&painter, QRectF(0,0,0,0) , ImageItem->sceneBoundingRect() );

    // the scene is destroyed
    delete tmp_scene;

    MaskModified = false;
    // and we return the created image (mask)
    return mask;
}

