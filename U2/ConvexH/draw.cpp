#include "draw.h"
#include <QtGui>

Draw::Draw(QWidget *parent) : QWidget(parent)
{

}

void Draw::mousePressEvent(QMouseEvent *event)
{
    //Get coordinates
    int x = event -> x();
    int y = event -> y();

    //Add to the list
    QPoint q(x,y);
    points.push_back(q);

    repaint();
}

void Draw::paintEvent(QPaintEvent *event)
{
    QPainter qp(this);

    //Start drawing
    qp.begin(this);

    //Draw points
    for(unsigned int i = 0; i < points.size(); i++)
    {
        qp.drawEllipse(points[i].x() - 3,points[i].y() - 3, 6, 6);

    }

    //Draw CH
    qp.drawPolygon(ch);

    qp.end();
}
