#include "draw.h"
#include "qpointfb.h"
#include <QtGui>

Draw::Draw(QWidget *parent) : QWidget(parent)
{
    ab = true;
    inout = false;
}

void Draw::mousePressEvent(QMouseEvent *event)
{
     QPointFB q(event->x(), event->y());

     //Add to A
     if(ab)
         if(inout)
             inA.push_back(q);
         else
             a.push_back(q);

     //Add to B
     else
         if(!inout)
             b.push_back(q);
         else
             inB.push_back(q);

     repaint();
}

void Draw::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.begin(this);

    //Draw A
    QPen pA(Qt::green, 2, Qt::SolidLine);
    painter.setPen(pA);
    drawPolygon(painter, a);

    //Draw B
    QPen pB(Qt::blue, 2, Qt::SolidLine);
    painter.setPen(pB);
    drawPolygon(painter, b);

    //Draw inner ring A
    QPen pinA(Qt::green, 2, Qt::DashDotLine);
    painter.setPen(pinA);
    drawPolygon(painter, inA);

    //Draw inner ring B
    QPen pinB(Qt::blue, 2, Qt::DashDotLine);
    painter.setPen(pinB);
    drawPolygon(painter, inB);

    //Draw edges    
    //1)Result
    QPen pE(Qt::red, 2, Qt::SolidLine);
    painter.setPen(pE);
    for(int i = 0; i < res.size(); i++)
    {
        painter.drawLine(res[i].getStart(), res[i].getEnd());
    }

    //2)Remove few edges
    QPen pR(Qt::white, 2, Qt::SolidLine);
    painter.setPen(pR);
    for(int i = 0; i < removeEdges.size(); i++)
    {
        painter.drawLine(removeEdges[i].getStart(), removeEdges[i].getEnd());
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

void Draw::removeLast()
{
    //Remove from A
    if(ab)
        if(inout)
            inA.pop_back();
        else
            a.pop_back();

    //Remove from B
    else
        if(!inout)
            b.pop_back();
        else
            inB.pop_back();

    repaint();
}
