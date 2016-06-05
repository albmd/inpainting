// Clanu 2015 - 2016
// T. Grenier : thomas.grenier@insa-lyon.fr

#ifndef GRAPHICANDDRAW_H
#define GRAPHICANDDRAW_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>
#include <QColor>
#include <QImage>
#include <QPoint>
#include <QList>

class GraphicAndDraw : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicAndDraw(QWidget *parent = 0);

    virtual ~GraphicAndDraw();
    void SetImageItem(const QGraphicsPixmapItem *imageItem);

    void SetPenColor(const QColor &newColor);
    void SetPenWidth(int newWidth);

    void SetScrollHSync(QScrollBar *SyncToViewScrollBar);
    void SetScrollVSync(QScrollBar *SyncToViewScrollBar);

    QColor penColor() const { return myPenColor; }
    int penWidth()    const { return myPenWidth; }

    QImage GetMask();
    bool isMaskModified() { return MaskModified;}

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    int myPenWidth;
    QColor myPenColor;

    QPointF OrigPoint;

    bool MaskModified;

    QGraphicsPixmapItem* ImageItem;
    QGraphicsLineItem* ItemToDraw;
    QList<QLineF> ListOfItems;

};

#endif // GRAPHICANDDRAW_H
