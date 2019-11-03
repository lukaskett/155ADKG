#include "algorithms.h"
#include <cmath>
#include <sortbyy.h>
#include <sortbyx.h>

Algorithms::Algorithms()
{

}

double Algorithms::getPointLineDistance(QPoint &q,QPoint &p1,QPoint &p2){
    //Calculate point and line distance
    double numerator = q.x() * (p1.y() - p2.y()) + p1.x() * (p2.y() - q.y()) + p2.x() * (q.y() - p1.y());
    double dx = p2.x() - p1.x();
    double dy = p2.y() - p1.y();
    double denumerator = sqrt(dx * dx + dy * dy);

    double dist = fabs(numerator)/denumerator;

    return dist;
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

QPolygon Algorithms::jarvisScan(std::vector<QPoint> &points)
{
    //Convex hull
    QPolygon ch;

    //Sort points by y
    std::sort(points.begin(),points.end(), SortByY());

    //Pivot with the lowest y-coordinate
    QPoint q = points[0];

    //Point on the left from point q
    QPoint r(q.x()-1, q.y());

    //Initialize points pj, pjj
    QPoint pj = q;
    QPoint pjj = r;

    //Add point q to convex hull
    ch.push_back(q);

    //Find CH points
    do
    {
        //Find max angle and point index
        double om_max = 0;
        int ind_max = -1;

        //Find point od CH
        for(int h = 0; h < points.size(); h++)
        {
            double omega = getAngle2Vectors(pj, pjj, pj, points[h]);

            //Actualize maximum
            if(omega > om_max)
            {
                om_max = omega;
                ind_max = h;
            }
        }

        //Add point to the convex hull
        ch.push_back(points[ind_max]);

        //Change index
        pjj = pj;
        pj = points[ind_max];

    }while(!(pj == q));

    return ch;

}

QPolygon Algorithms::qHull(std::vector<QPoint> &points)
{
    //Convex hull by Quick Hull method

    //Convex hull
    QPolygon ch;

    //Separated upper and lower parts of points
    std::vector<QPoint> upoints, lpoints;

    //Sort points by x coordinate
    std::sort(points.begin(), points.end(), SortByX());

    //Find points q1 and q3 with lower and max x coordinate
    QPoint q1 = points[0];
    QPoint q3 = points.back();

    //Add the first line (q1 q3) to upoints and lpoints
    upoints.push_back(q1);
    upoints.push_back(q3);

    lpoints.push_back(q1);
    lpoints.push_back(q3);

    //Split the points to upper and lower
    for(unsigned int i = 0; i < points.size(); i++)
    {
        //Add to the upper part
        if(getPointLinePosition(points[i],q1, q3) == 1)
            upoints.push_back(points[i]);

        //Add to the lower part
        else if (getPointLinePosition(points[i], q1, q3) == 0)
            lpoints.push_back(points[i]);
    }

    // Call recursive

    //1)Add q3 to the convex hull
    ch.push_back(q3);

    //2)Call resursive function
    qh(1, 0, upoints, ch);

    //3)Add q3 to the convex hull
    ch.push_back(q1);

    //4)Call resursive function
    qh(0, 1, lpoints, ch);

    return ch;


}

void Algorithms::qh(unsigned int start, unsigned int end, std::vector<QPoint> &points,QPolygon &ch)
{
    //Recursive prodcedure
    int i_max = -1;
    double d_max = 0;

    //Browse all points
    for(unsigned int i = 2; i < points.size(); i++)
    {
        //Is the point in the right half plane?
        if(getPointLinePosition(points[i], points[start], points[end]) == 0)
        {
            double d = getPointLineDistance(points[i], points[start], points[end]);

            //Actualization of max distance
            if(d > d_max)
            {
                d_max = d;
                i_max = i;
            }

        }

    }

    //Suitable point has been found
    if(i_max != -1)
    {
        qh(start, i_max, points, ch);
        ch.push_back(points[i_max]);
        qh(i_max, end, points, ch);
    }

}
QPolygon Algorithms::sweepLine(std::vector<QPoint> &points)
{
    //Incremental method, Sweep Line
    QPolygon ch;

    //Number of points
    int m = points.size();

    //List of predecessors(predchudci)
    std::vector<int> p(m);

    //List of successors(naslednici)
    std::vector<int> n(m);

    //Sort points by X
    std::sort(points.begin(), points.end(), SortByX());

    //Create initial aproximation(dvojuhelnik)
    n[0] = 1;
    n[1] = 0;
    p[0] = 1;
    p[1] = 0;

    //Process all points according to X coordinates
    for(int i = 2; i < m; i++)
    {
        //Point i lies in the upper half plane
        if(points[i].y() > points[i-1].y())
        {
            //Link i and its predecessor and successor
            p[i] = i-1;
            n[i] = n[i-1];
        }

        //Point i lies in the lower half plane
        else
        {
            p[i] = p[i-1];
            n[i] = i - 1;
        }

        //Link predecessor and successor and i
        p[n[i]] = i;
        n[p[i]] = i;

        //Fix upper tangent
        while(getPointLinePosition(points[n[n[i]]], points[i], points[n[i]]) == 0)
        {
            //Change predecessor and successor
            p[n[n[i]]] = i;
            n[i] = n[n[i]];
        }

        //Fix lower tangent
        while(getPointLinePosition(points[p[p[i]]], points[i], points[p[i]]) == 1)
        {
            //Change predecessor and successor
            n[p[p[i]]] = i;
            p[i] = p[p[i]];
        }
    }

    //Convert successors to ch
    ch.push_back(points[0]);

    //Second point of CH
    int index = n[0];

    //Repeat until the first point is found
    while(index != 0)
    {
        //Add to the convex hull
        ch.push_back(points[index]);

        //Find its successor
        index = n[index];
    }

    return ch;
}

bool Algorithms::Angle(QPoint &p1, QPoint &p2){
    //Function which find bigger angle
    double epsilon = 1e-6;
    double angle1 = getAngle2Vectors(pivot,kolinear_X_point,pivot,p1);
    double angle2 = getAngle2Vectors(pivot,kolinear_X_point,pivot,p2);

    if(fabs(angle1-angle2) > epsilon){
        return angle1>angle2;
    }
    else {
     //Simillar angle
        bool a = Distance(p1,p2);
        return a;
    }
}
bool Algorithms::Distance(QPoint &p1, QPoint &p2){
    //Function which find bigger distance
    double dx1 = p1.x()-pivot.x();
    double dy1 = p1.y()-pivot.y();
    double dx2 = p2.x()-pivot.x();
    double dy2 = p2.y()-pivot.y();
    double distance1 = sqrt((dx1*dx1)+(dy1*dy1));
    double distance2 = sqrt((dx2*dx2)+(dy2*dy2));
    return distance1<distance2;
}

QPoint Algorithms::pivot;
QPoint Algorithms::kolinear_X_point;
QPolygon Algorithms::grahamScan(std::vector<QPoint> &points) //inspirated on https://www.geeksforgeeks.org
{
    //Convex hull by Graham Scan method
    QPolygon ch;
    QPolygon ch_GS;

    //Find pivot q
    std::sort(points.begin(),points.end(),SortByY());

    //Create CH
    ch.push_back(points[0]);
    ch_GS.push_back(points[0]);

    pivot = points[0];
    //Kolinear point with X creating
    kolinear_X_point.setX(pivot.x()-100);
    kolinear_X_point.setY(pivot.y());

    //Sorting points by angle
    std::sort(points.begin(), points.end(), Angle);

    //If we have points on the same line
    for(unsigned int i=0; i<=points.size()-1; i++){
        while(getPointLinePosition(points[i],points[0],points[i+1])==-1){
            i++;
        }

        ch.push_back(points[i]);
    }

    ch_GS.push_back(ch[1]);

    //Points of CH

    for(unsigned int i = 2; i <= ch.size()-1; i++){ //ch.size()-1
        QPoint top1 = ch_GS[ch_GS.size()-1];
        ch_GS.pop_back();

        QPoint top2 = ch_GS[ch_GS.size()-1];
        ch_GS.push_back(top1);

        while(getPointLinePosition(ch[i],top2, ch_GS[ch_GS.size()-1])!=1){
            ch_GS.pop_back();
            top1 = ch_GS[ch_GS.size()-1];

            ch_GS.pop_back();
            top2 = ch_GS[ch_GS.size()-1];
            ch_GS.push_back(top1);
        }

        ch_GS.push_back(ch[i]);
    }

    for(unsigned int i=0; i< ch_GS.size()-2; i++){
        if(getPointLineDistance(ch_GS[i], ch_GS[i+1],ch_GS[i+2])==-1){
            ch_GS.erase(ch_GS.begin()+(i+1));
            i = i-1;
        }
    }
    return ch_GS;
}





