#include "draw.h"
#include <QMessageBox>
#include <Qt>
#include <QBrush>
#include <QtGui>

Draw::Draw(QWidget *parent) : QWidget(parent)
{

}

void Draw::setContours(std::vector<Edge> &contours_, std::vector<double> &metadata_, int dz_)
{
    contours = contours_;
    metadata = metadata_;
    dz = dz_;
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
    for (unsigned int i = 0; i < contours.size(); i++)
    {
        int height = metadata[i]; //contour height
        int mainContour = dz * 5; // main contour
        //If contour is main contour
        if(!(height%(mainContour))){
                double x = (contours[i].getStart().x() + contours[i].getEnd().x())/2;
                double y = (contours[i].getStart().y() + contours[i].getEnd().y())/2;
                QPen q;
                q.setWidth(2);
                q.setColor("brown");
                painter.setPen(q);
                painter.drawLine(contours[i].getStart(), contours[i].getEnd());
                painter.drawText( x, y, QString::number(height)); // not ideal way to success, in some type of terain do lot of dimension in small area
        }
        else //clasic contour
        {
            QPen q_;
            q_.setWidth(1);
            q_.setColor("brown");
            painter.setPen(q_);
        }
        painter.drawLine(contours[i].getStart(), contours[i].getEnd());
    }

   /* //Draw main contour lines
    QPen q2;
    q2.setWidth(2);
    q2.setColor("brown");
    painter.setPen(q2);
    for (unsigned int i = 0; i < mainContours.size(); i++)
    {
       int h = metadata[i];
       int hl = dz * 5;

       //Určenie, že sa jedná o hlavné vrstevnice
       if(!(h%(hl))){
               double x = (mainContours[i].getStart().x() + mainContours[i].getEnd().x())/2;
               double y = (mainContours[i].getStart().y() + mainContours[i].getEnd().y())/2;
               painter.drawLine(mainContours[i].getStart(), mainContours[i].getEnd());
               painter.drawText( x, y, QString::number(h));
       }
       else

    }
    */
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
        std::vector<QColor> colorSlope = {QColor(slope, slope, slope)};
        painter.setBrush(colorSlope[0]);

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

        //Color posibilitis
        std::vector<QColor> cAspect = {Qt::red, QColor(255,170,0), Qt::yellow, Qt::green, QColor(0,255,250), QColor(95,220,255), Qt::blue, QColor(255,95,255)};

        //Interval of aspect values for giving color - coloras are like in the Argis Pro
        //Used RGB calculator https://www.colorspire.com/rgb-color-wheel/
        if((aspect_type > 0) && (aspect_type < 22.5)){
            painter.setBrush(cAspect[0]);
        }
        else if((aspect_type >= 22.5) && (aspect_type < 67.5)){
            painter.setBrush(cAspect[1]);
        }
        else if((aspect_type >= 67.5) && (aspect_type < 112.5)){
            painter.setBrush(cAspect[2]);
        }
        else if((aspect_type >= 112.5) && (aspect_type < 157.5)){
            painter.setBrush(cAspect[3]);
        }
        else if((aspect_type >= 157.5) && (aspect_type < 202.5)){
            painter.setBrush(cAspect[4]);
        }
        else if((aspect_type >= 202.5) && (aspect_type < 247.5)){
            painter.setBrush(cAspect[5]);
        }
        else if((aspect_type >= 247.5) && (aspect_type < 292.5)){
            painter.setBrush(cAspect[6]);
        }
        else if((aspect_type >= 292.5) && (aspect_type < 337.5)){
            painter.setBrush(cAspect[7]);
        }
        else if((aspect_type >= 337.5) && (aspect_type < 360)){
            painter.setBrush(cAspect[0]);
        }

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
