#include "algorithms.h"
#include <cmath>
#include <QtMath>

Algorithms::Algorithms()
{
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


int Algorithms::positionPointPolygonWinding(QPoint &q, std::vector<QPoint> &pol)
{
    // Analyze Position of the Point and the Polygon
    double wn = 0.0;

    // Tolerance
    double eps = 1.0e-6;

    double xir = pol[0].x();
    double yir = pol[0].y();


    // The size of polygon
    int n = pol.size();

    //Browse all points of polygon
    for (int i = 0; i < n; i++){

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
    int n = pol.size();

    //Reduce first point
    double xir = pol[0].x() - q.x();
    double yir = pol[0].y() - q.y();

    for (int i = 1; i < n+1 ; i++)
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
        if ((yir > 0) && (yiir <= 0) || (yiir > 0) && (yir <= 0) )
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







