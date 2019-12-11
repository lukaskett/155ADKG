#include "draw.h"
#include "qpointfb.h"
#include <QtGui>

Draw::Draw(QWidget *parent) : QWidget(parent)
{
    ab = true;
}

void Draw::mousePressEvent(QMouseEvent *event)
{
     QPointFB q(event->x(), event->y());

     //Add to A
     if(ab)
         a.push_back(q);

     //Add to B
     else
         b.push_back(q);

     repaint();
}

void Draw::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.begin(this);

    //Draw A
    QPen pA(Qt::green);
    painter.setPen(pA);
    drawPolygon(painter, a);

    //Draw B
    QPen pB(Qt::blue);
    painter.setPen(pB);
    drawPolygon(painter, b);

    //Draw edges
    QPen pE(Qt::red);
    pE.setWidth(2);
    painter.setPen(pE);
    for(int i = 0; i < res.size(); i++)
    {
        painter.drawLine(res[i].getStart(), res[i].getEnd());
    }

    painter.end();
}

void Draw::drawPolygon(QPainter &painter, std::vector<QPointFB> &polygon)
{
    //Draw polygon
    QPolygon polyg;
    for(int i = 0; i < polygon.size(); i++)
    {
        polyg.append(QPoint (polygon[i].x(), polygon[i].y()));
    }

    painter.drawPolygon(polyg);
}
