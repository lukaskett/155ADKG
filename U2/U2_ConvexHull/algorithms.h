#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include <QtGui>


class Algorithms
{
private:
    static QPoint pivot;
    static QPoint kolinear_X_point;

public:
    Algorithms();
    //static QPoint pivot;
    //static QPoint kolinear_X_point;

    static int getPointLinePosition(QPoint &q,QPoint &p1,QPoint &p2);
    static double getAngle2Vectors(QPoint &p1,QPoint &p2,QPoint &p3,QPoint &p4);
    static QPolygon jarvisScan(std::vector<QPoint> &points);
    static QPolygon qHull(std::vector<QPoint> &points);
    static QPolygon grahamScan(std::vector<QPoint> &points);
    static QPolygon sweepLine(std::vector<QPoint> &points);
    static double getPointLineDistance(QPoint &q,QPoint &p1,QPoint &p2);
    static void qh(unsigned int start, unsigned int end, std::vector<QPoint> &points,QPolygon &convexHull);
    static bool Angle(QPoint &p1, QPoint &p2);
    static bool Distance(QPoint &p1, QPoint &p2);

};

#endif // ALGORITHMS_H
