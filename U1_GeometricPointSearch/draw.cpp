#include "draw.h"
#include <QMessageBox>
#include <algorithms.h>

Draw::Draw(QWidget *parent) : QWidget(parent)
{
    //Initialization
    draw_mode = true; // true = polygon, false = point

    q.setX(-100);
    q.setY(-100);

}

void Draw::mousePressEvent(QMouseEvent *e)
{
    //Get coordinates od the mouse
    int x = e->x();
    int y = e->y();

    //Set the coordinates to point q
    q.setX(x);
    q.setY(y);

    repaint();
}

void Draw::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);

    //Set pen style for polygon boundary drawing
    QPen pen_boun(Qt::black, 1,Qt::SolidLine,Qt::RoundCap, Qt::RoundJoin); // added by me Qt::RoundCap, Qt::RoundJoin
    painter.setPen(pen_boun);

    //Draw generated polygon
    polygons.push_back(generated_points);
    /*QPolygon gen_draw;
    for(unsigned int i = 0; i < generated_points.size();i++)
    {
        gen_draw.append(generated_points[i]);
    }

    painter.drawPolygon(gen_draw);
    gen_draw.clear();*/


    //Draw polygons from file
    for(unsigned int i = 0; i < polygons.size();i++)
    {
        //Create polygon
        QPolygon p_draw;

        //Get polygons one by one from file
        std::vector<QPoint> actual_pol = polygons[i];

        //Add points to the polygon
        for(unsigned int j = 0; j < actual_pol.size(); j++)
        {
            p_draw.append(actual_pol[j]);
        }

        //Draw actual polygon
        painter.drawPolygon(p_draw);
    }

    //Fill polygon by color

    //set pen
    QPen pen_fill(Qt::green, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen_fill);

    //set fill color and style
    QBrush brush;
    brush.setColor(Qt::blue);
    brush.setStyle(Qt::Dense4Pattern);
    QPainterPath path;

    QPolygon polyg;

    for(unsigned i = 0; i < analyze_results_by_polygons.size(); i++)
    {
        if(analyze_results_by_polygons[i]) //if point is inside or on the boundary
        {
            std::vector<QPoint> pol_to_fill = polygons[i];
            for(unsigned l = 0; l < pol_to_fill.size(); l++)
            {
                polyg << pol_to_fill[l];
            }
            path.addPolygon(polyg);
            painter.drawPolygon(polyg);
            painter.fillPath(path, brush);
            polyg.clear();
        }
    }


    //Set point size (radius)    
    int r_q = 10;//Point Q
    int r_p = 6; //Generated points

    //Set painter for point drawing
    painter.setPen(pen_boun);

    //Draw generated points
    for(unsigned int i = 0; i < generated_points.size(); i++)
    {
        painter.drawEllipse(generated_points[i].x() - r_p/2,generated_points[i].y() - r_p/2, r_p, r_p);
    }

    //Draw point Q
    painter.drawEllipse(q.x() - r_q/2,q.y() - r_q/2, r_q, r_q);

}

void Draw::clearCanvas()
{
      polygons.clear();
      generated_points.clear();

      //Hide point Q
      q.setX(-100);
      q.setY(-100);

      //Remove polygon fill
      analyze_results_by_polygons.clear();

      repaint();
}

void Draw::importPolygon(std::string path)
{
    //Open the file
    std::ifstream source_file;
    source_file.open(path);

    //Check if the file is opened
    if(!source_file.is_open())
    {
        source_file.close();
        QMessageBox::warning(this, "Error", "File can NOT be opened");
    }

    //Get the number of polygons in text file
    int polygons_count;
    source_file >> polygons_count;

    QMessageBox::information(this, "Test information", QString("Show the number of polygons %1").arg(polygons_count));

    //Number of points in the polygon
    int points_count;

    //Vector for single polygon points
    std::vector<QPoint> single_polygon; //QPointF for floating point accuracy?

    //Go through whole file and store all polygons to variable polygon
    while(source_file.good() && polygons_count--)
    {
        //Get number of points in one polygon
        source_file >> points_count;

        //Go through all points in single polygon and store them in variable single_polygon
        while(points_count--)
        {
            QPoint single_point; //QPointF for floating point accuracy?
            double point_x, point_y;
            source_file >> point_x;
            source_file >> point_y;
            single_point.setX(point_x);
            single_point.setY(point_y);
            single_polygon.push_back(single_point);
        }

        polygons.push_back(single_polygon);
        single_polygon.clear();

    }

    source_file.close();

}

std::vector<QPoint> Draw::getPolygon(unsigned int index)
{
    return polygons[index];
}

// Fill polygon - analyze results
void Draw::fillPolygon(std::vector<int> analyze_results_by_polygons)
{
    //fill polygons containing point q with colour
    this->analyze_results_by_polygons = analyze_results_by_polygons;
    repaint();
}

void Draw::generatePoints(int pol_count)
{
    //generate points
    std::vector<QPoint> gen_points = Algorithms::polGen(pol_count);

    //Graham Scan sort points
    std::vector<QPoint> GraSca_points = Algorithms::GrahamScan(gen_points);

    //Add generated points to generated_points
    for (unsigned int i = 0;i < GraSca_points.size(); i++) {
        generated_points.push_back(GraSca_points[i]);
    }

    repaint();
}
