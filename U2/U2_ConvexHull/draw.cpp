#include "draw.h"
#include <QtGui>
#include <QMessageBox>
#include <fstream>

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
    int r = 3;
    unsigned int size = points.size();

    for(unsigned int i = 0; i < size; i++)
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
    int b_gap = 4;

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
        //Calculate counts on row and column
        int count_on_row = sqrt(count_points / (width / height));
        int count_on_column = count_points / count_on_row;

        //Generate points row by row
        for(int i = 0; i < count_on_row; i++)
        {
            for(int j = 0; j < count_on_column; j++)
            {
                double x = width / 2 - (width - 2 * b_gap) / 2 + j * (width - 2 * b_gap) / count_on_row;
                double y = height / 2 - (height - 2 * b_gap) / 2 + i * (height - 2 * b_gap) / count_on_column;
                points.push_back(QPoint(x, y));
            }
        }
    }

    //Random distribution
    //https://stackoverflow.com/questions/7560114/random-number-c-in-some-range
    else if(method == 1)
    {
        std::random_device rd; // obtain a random number from hardware
        std::mt19937 eng(rd()); // seed the generator
        std::uniform_int_distribution<> distr_x(b_gap, width - b_gap); // define the x range
        std::uniform_int_distribution<> distr_y(b_gap, height - b_gap); // define the y range

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

std::vector<QPoint> Draw::generatePointsU2(int method, int count_points, int width, int height)
{
    //Generate points according to selected values - type and number of points
    //method: 1-raster, 2-random, 3-circle

    //Return vector
    std::vector<QPoint> gen_points;

    //Distance from the boundary of the window - experimental value
    int b_gap = 4;

    //Raster distribution
    if(method == 0)
    {
        //Calculate counts on row and column
        int count_on_row = sqrt(count_points / (width / height));
        int count_on_column = count_points / count_on_row;

        //Generate points row by row
        for(int i = 0; i < count_on_row; i++)
        {
            for(int j = 0; j < count_on_column; j++)
            {
                double x = width / 2 - (width - 2 * b_gap) / 2 + j * (width - 2 * b_gap) / count_on_row;
                double y = height / 2 - (height - 2 * b_gap) / 2 + i * (height - 2 * b_gap) / count_on_column;
                points.push_back(QPoint(x, y));
            }
        }

    }

    //Random distribution
    //https://stackoverflow.com/questions/7560114/random-number-c-in-some-range
    else if(method == 1)
    {
        std::random_device rd; // obtain a random number from hardware
        std::mt19937 eng(rd()); // seed the generator
        std::uniform_int_distribution<> distr_x(b_gap, width - b_gap); // define the x range
        std::uniform_int_distribution<> distr_y(b_gap, height - b_gap); // define the y range

        for(int n = 0; n < count_points; n++)
        {
            //Generate values
            int x = distr_x(eng);
            int y = distr_y(eng);

            //Add generated point to vector of points
            gen_points.push_back(QPoint(x,y));
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
            gen_points.push_back(QPoint(x,y));
        }
    }
    return gen_points;
}


