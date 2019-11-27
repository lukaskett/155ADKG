#include "draw.h"
#include <QMessageBox>
#include <Qt>
#include <QBrush>
#include <QtGui>

Draw::Draw(QWidget *parent) : QWidget(parent)
{

}

void Draw::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.begin(this);

    //Draw points
    //Set point size - radius
    int r = 4;

    for (unsigned int i = 0; i < points.size(); i++)
    {
        painter.drawEllipse(points[i].x() - r/2 , points[i].y() - r/2, r, r);
    }

    //Draw edges
    QPen p(Qt::green, 1);
    painter.setPen(p);
    for (unsigned int i = 0; i < dt.size(); i++)
    {
        painter.drawLine(dt[i].getStart(), dt[i].getEnd());
    }

    //Draw contour lines
    QPen q;
    q.setWidth(1);
    q.setColor("brown");
    painter.setPen(q);

    for (unsigned int i = 0; i < contours.size(); i++)
    {
        painter.drawLine(contours[i].getStart(), contours[i].getEnd());
    }

    //Draw main contour lines
    QPen q2;
    q2.setWidth(2);
    q2.setColor("brown");
    painter.setPen(q2);
    for (unsigned int i = 0; i < mainContours.size(); i++)
    {
        painter.drawLine(mainContours[i].getStart(), mainContours[i].getEnd());

    }
    //Draw slope

    if(slope == TRUE)
    {
    //Rescale color scale
    double k = 255.0 / 180;
    for(Triangle t : dmt)
    {
        //Get triangle vertices
        QPoint3D p1 = t.getP1();
        QPoint3D p2 = t.getP2();
        QPoint3D p3 = t.getP3();

        //Get slope, it is equal to the color
        int slope = 150 - t.getSlope() * k;

        //Set color brush
        QColor c(slope, slope, slope);
        painter.setBrush(c);

        //Create polygon
        QPolygonF triangle;
        triangle.append(QPointF(p1.x(), p1.y()));
        triangle.append(QPointF(p2.x(), p2.y()));
        triangle.append(QPointF(p3.x(), p3.y()));

        painter.drawPolygon(triangle);

        }
    }

    //Draw aspect
    if(aspect == TRUE)
    {
        for(unsigned int i = 0; i< dmt.size();i++)
        {
        //Get triangle vertices
        Triangle t = dmt[i];
        QPoint3D p1 = t.getP1();
        QPoint3D p2 = t.getP2();
        QPoint3D p3 = t.getP3();

        //Get aspect, it is equal to the color - musime najit model kde na sebe barvy navazuji
        int aspect_type = t.getAspect();

        //Interval of aspect values for giving color - coloras are like in the Argis Pro
        //Used RGB calculator https://www.colorspire.com/rgb-color-wheel/
        if((aspect_type > 0) && (aspect_type < 22.5)){
            painter.setBrush(Qt::red);
        }
        else if((aspect_type >= 22.5) && (aspect_type < 67.5)){
            painter.setBrush(QColor(255,170,0));
        }
        else if((aspect_type >= 67.5) && (aspect_type < 112.5)){
            painter.setBrush(Qt::yellow);
        }
        else if((aspect_type >= 112.5) && (aspect_type < 157.5)){
            painter.setBrush(Qt::green);
        }
        else if((aspect_type >= 157.5) && (aspect_type < 202.5)){
            painter.setBrush(QColor(0,255,250));
        }
        else if((aspect_type >= 202.5) && (aspect_type < 247.5)){
            painter.setBrush(QColor(95,220,255));
        }
        else if((aspect_type >= 247.5) && (aspect_type < 292.5)){
            painter.setBrush(Qt::blue);
        }
        else if((aspect_type >= 292.5) && (aspect_type < 337.5)){
            painter.setBrush(QColor(255,95,255));
        }
        else if((aspect_type >= 337.5) && (aspect_type < 360)){
            painter.setBrush(Qt::red);
        }

        //QColor c(aspect_type, aspect_type, aspect_type);
        //painter.setBrush(c);

        //Create polygon
        QPolygon triangle;
        triangle.append(QPoint(p1.x(), p1.y()));
        triangle.append(QPoint(p2.x(), p2.y()));
        triangle.append(QPoint(p3.x(), p3.y()));

        painter.drawPolygon(triangle);
        }
    }

    painter.end();

}

void Draw::mousePressEvent(QMouseEvent *event)
{
    QPoint3D p;
    p.setX(event->x());
    p.setY(event->y());
    double random = std::rand() * 200.0 / RAND_MAX; //RAND_MAX is known constant
    p.setZ(random);

    points.push_back(p);

    repaint();
}
