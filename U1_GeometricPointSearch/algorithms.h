#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include <QPoint>
#include <vector>

class Algorithms
{
public:

    Algorithms();
    static int getPointLinePosition(QPoint &q,QPoint &p1,QPoint &p2);
    static double getAngle2Vectors(QPoint &p1,QPoint &p2,QPoint &p3,QPoint &p4);
    static int positionPointPolygonWinding(QPoint &q, std::vector<QPoint> &pol);
    static int positionPointPolygonRayCrossing(QPoint &q, std::vector<QPoint> &pol);
    static std::vector<QPoint> polGen(int pol_count);
    static std::vector<QPoint> GrahamScan(std::vector<QPoint> gen_points);
    static bool omegaSort(QPoint &p1, QPoint &p2, QPoint &pivot);
};

#endif // ALGORITHMS_H
