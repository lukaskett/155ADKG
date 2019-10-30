#include "draw.h"
#include <QtGui>
#include <QMessageBox>

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
    int r = 4;
    for(unsigned int i = 0; i < points.size(); i++)
    {
        qp.drawEllipse(points[i].x() - r/2,points[i].y() - r/2, r, r);

    }

    //Draw CH
    qp.drawPolygon(ch);

    qp.end();
}

void Draw::generatePoints(int method, int count_points, int width, int height)
{
    //Generate points according to selected values - type and number of points
    //method: 1-raster, 2-random, 3-circle

    //Distance from the boundary of the window - experimental value
    int boundary_gap = 4;

    //Add four corner points QPoint(x,y)
    //left-upper
    //points.push_back(QPoint(boundary_gap, boundary_gap));
    //left-lower
    //points.push_back(QPoint(boundary_gap, height - boundary_gap));
    //right-lower
    //points.push_back(QPoint(width - boundary_gap,height - boundary_gap));
    //right-upper
    //points.push_back(QPoint(width - boundary_gap, boundary_gap));


    //Raster distribution
    if(method == 0)
    {
        int density = 2;
        int step_w = density * 10;
        int step_h = density * 15;

        for(int i = boundary_gap; i < (height - boundary_gap); i = i + step_h)
        {
            for(int j = boundary_gap; j < (width - boundary_gap); j = j + step_w)
            {
                points.push_back(QPoint(j,i));
            }
        }
    }

    //Random distribution
    //https://stackoverflow.com/questions/7560114/random-number-c-in-some-range
    else if(method == 1)
    {
        std::random_device rd; // obtain a random number from hardware
        std::mt19937 eng(rd()); // seed the generator
        std::uniform_int_distribution<> distr_x(boundary_gap, width - boundary_gap); // define the x range
        std::uniform_int_distribution<> distr_y(boundary_gap, height - boundary_gap); // define the y range

        for(int n = 0; n < count_points; n++)
        {
            //Generate values
            int x = distr_x(eng);
            int y = distr_y(eng);

            //Add generated point to vector of points
            points.push_back(QPoint(x,y));
        }

    }

    //Circle distribution
    //https://codereview.stackexchange.com/questions/165338/distributing-points-uniformly-in-a-unit-circle
    else
    {
        int stripeWidth = 100;

        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 circ(seed); // seed the generator
        std::uniform_int_distribution<> angleDistribution(0,360);
        std::uniform_int_distribution<> radiusDistribution(height / 2 - stripeWidth, height /2 );

        for(int i = 0; i < count_points; i++) {
            double angle  = angleDistribution(circ);
            int radius = radiusDistribution(circ);
            int x = width / 2 + radius * sin(angle);
            int y = height / 2 + radius * cos(angle);
            points.push_back(QPoint(x,y));
        }
    }
}


