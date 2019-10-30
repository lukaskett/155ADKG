#include "algorithms.h"
#include <cmath>
#include <QtMath>
#include <QDebug>
#include <random>
#include <sortbyy.h>
#include <sortbyomega.h>
#include <QMessageBox>

Algorithms::Algorithms()
{

}

int Algorithms::positionPointPolygonWinding(QPoint &q, std::vector<QPoint> &pol)
{
    // Analyze Position of the Point and the Polygon
    double wn = 0.0;

    // Tolerance
    double eps = 1.0e-6;

    //double xir = pol[0].x();
    //double yir = pol[0].y();


    // The size of polygon
    unsigned int n = pol.size();

    //Browse all points of polygon
    for (unsigned int i = 0; i < n; i++){

        //Measure angle
        double omega = getAngle2Vectors(pol[i], q, pol[(i+1)%n], q);

        //Get orientation of the point and the polygon edge
        int orient = getPointLinePosition(q, pol[i], pol[(i+1)%n]);

        //Point in the left half plane
        if (orient == 1)
            wn += omega;

        //Point in the right half plane
        else
            wn -= omega;

       }

    //Point inside polygon
    if (fabs(fabs(wn) - 2 * M_PI) <= eps){
        return 1;
    }

    //Point outside polygon

    else if (fabs(fabs(wn)) <= eps){
        return 0;
    }


    //Point on the boundary
    else
    {
        return -1;
    }
    }



int Algorithms::positionPointPolygonRayCrossing(QPoint &q, std::vector<QPoint> &pol){
    // Tolerance
    double eps = 1.0e-6;

    // Analyze Position of the Point and the Polygon
    int k = 0;

    // Size of polygon
    unsigned int n = pol.size();

    //Reduce first point
    double xir = pol[0].x() - q.x();
    double yir = pol[0].y() - q.y();

    for (unsigned int i = 1; i < n+1 ; i++)
    {
        //Reduce coordinates
        double xiir = pol[i%n].x() - q.x();
        double yiir = pol[i%n].y() - q.y();

        // Calculate distance between [xi,yi] and [xii, yii]
        double dist_i_and_ii = sqrt((xir - xiir)*(xir - xiir) + (yir - yiir)*(yir - yiir));

        // Sum of distance between [xi, yi] and q, and [xiir, yiir] and q
        double dist_q = sqrt((xir*xir + yir*yir)) + sqrt((xiir*xiir + yiir*yiir));

        // Control if point is on the boundary
        if (fabs(dist_i_and_ii-dist_q)< eps){
            return -1;

        }
        //Point in the upper half plane
        if (((yir > 0) && (yiir <= 0)) || ((yiir > 0) && (yir <= 0)))
        {

            // Compute intersection
            double xm = (xir * yiir - xiir * yir)/(yir - yiir);

            // Point in the right plane
            if (xm > 0)
                k +=1;
        }

        //Assign coordinates
        yir = yiir;
        xir = xiir;
    }

    //Odd/even amount of intersections
    //0 - point is outside, 1 - point is inside
        return k%2;
}


int Algorithms::getPointLinePosition(QPoint &q,QPoint &p1,QPoint &p2)
{
//Analyze point and line position
//1 point in the left half plane
//0 point in the right half plane
//-1 point on the line
double ux = p2.x() - p1.x();
double uy = p2.y() - p1.y();

double vx = q.x() - p1.x();
double vy = q.y() - p1.y();

double t = ux * vy - uy * vx;

//Point in the left half plane
if (t>0)
    return 1;
if (t<0)
    return 0;
return -1;
}

double Algorithms::getAngle2Vectors(QPoint &p1, QPoint &p2, QPoint &p3, QPoint &p4)
{
    // Calculate Vector betwen 2 vectors.
    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();

    double vx = p4.x() - p3.x();
    double vy = p4.y() - p3.y();

    //Norms
    double nu = sqrt(ux * ux + uy * uy);
    double nv = sqrt(vx * vx + vy * vy);

    //Dot product
    double scalar = ux * vx + uy * vy;

    double angle = fabs(acos(scalar/(nu*nv)));
    return angle;
}

std::vector<QPoint>Algorithms::polGen(int pol_count)
{
    //Vector with generated points
    std::vector<QPoint> body;

    //Can not generate polygon for 2 points and less
    if(pol_count > 2)
    {

    //Generate random points, number of points as argument
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator
    std::uniform_int_distribution<> distr_x(100, 700); // define the x range
    std::uniform_int_distribution<> distr_y(50, 650); // define the y range

    //Generate new point
    QPoint point_gen;

    for(int n = 0; n < pol_count; n++)
    {

        //Generate values
        int x = distr_x(eng);
        int y = distr_y(eng);

        //Assign values to the coordinates
        point_gen.setX(x);
        point_gen.setY(y);

        //Add generated point to vector
        body.push_back(point_gen);
    }
    return body;
    }        
}

std::vector<QPoint> Algorithms::GrahamScan(std::vector<QPoint> gen_points_part)
{
    //Store sorted points as they create non-convex polygon
    std::vector<QPoint> gen_points_sorted;

    //Sort points by y
    std::sort(gen_points_part.begin(),gen_points_part.end(), SortByY());

    //Pivot with the lowest y-coordinate
    QPoint pivot = gen_points_part[0];

    //Point on the left from point q
    QPoint r(pivot.x()-1, pivot.y());

    //Add th first point of polygon
    gen_points_sorted.push_back(pivot);

    // Feature count
    unsigned int n = gen_points_part.size();

    // Vecctor of structure objects
    std::vector<SortByOmega::OmegaStruct> points_structure;

    //Define struct
    SortByOmega::OmegaStruct PointOmega;

    //Calculate angles for all points and save them into the structure
    for (unsigned int i = 0; i < n; i++)
    {
        //Calculate omega angle
        double om_point = getAngle2Vectors(pivot, r, pivot, gen_points_part[i]);

        //Calculate distance
        double dx = gen_points_part[i].x() - pivot.x();
        double dy = gen_points_part[i].y() - pivot.y();;
        double dist_q = sqrt(dx * dx + dy * dy);

        //Save values
        PointOmega.point = gen_points_part[i];
        PointOmega.omega = om_point;
        PointOmega.distance = dist_q;
        points_structure.push_back(PointOmega);
    }

    //Number of features
    int m = points_structure.size();

    //Sort points by omega value
    std::sort(points_structure.begin(), points_structure.end(), SortByOmega());

    //Separate sorted points back to vector of points without omega angle
    for (unsigned int i = 0; i < m; i++)
    {        
            gen_points_sorted.push_back(points_structure[i].point);
    }

    //Return sorted points
    return gen_points_sorted;
}


