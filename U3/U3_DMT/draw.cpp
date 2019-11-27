#include "draw.h"
#include <QMessageBox>

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

    //Draw slope
    //Rescale color scale
    double k = 255.0 / 180;
    for(Triangle t : dmt)
    {
        //Get triangle vertices
        QPoint3D p1 = t.getP1();
        QPoint3D p2 = t.getP2();
        QPoint3D p3 = t.getP3();

        //Get slope, it is equal to the color
        int slope = 255 - t.getSlope() * k;

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

    //Draw aspect
    for(Triangle t : dmt)
    {
        //Get triangle vertices
        QPoint3D p1 = t.getP1();
        QPoint3D p2 = t.getP2();
        QPoint3D p3 = t.getP3();

        //Get slope, it is equal to the color - musime najit model kde na sebe barvy navazuji
        int aspect = t.getAspect() * k;

        QColor c(aspect, aspect, aspect);
        painter.setBrush(c);

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
