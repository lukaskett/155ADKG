#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include <vector>
#include "types.h"
#include "qpointfb.h"
#include "edge.h"

class Algorithms
{
public:
    Algorithms();
    static TPointLinePosition getPointLinePosition(QPointFB &q, QPointFB &p1, QPointFB &p2);
    static double getAngle2Vectors(QPointFB &p1, QPointFB &p2, QPointFB &p3, QPointFB &p4);
    static TPointPolygonPosition positionPointPolygonWinding(QPointFB &q, std::vector<QPointFB> &pol);
    static T2LinesPosition get2LinesPosition(QPointFB &p1, QPointFB &p2, QPointFB &p3, QPointFB &p4, QPointFB &pi);
    static std::vector<Edge> booleanOperations(std::vector<QPointFB> &polygonA,std::vector<QPointFB> &polygonB, TBooleanOperation operation);
    static void processIntersection(QPointFB &pi, double &t, std::vector<QPointFB> &polygon, int &i);
    static void computePolygonIntersection(std::vector<QPointFB> &pa, std::vector<QPointFB> &pb);
    static void setPositionsAB(std::vector<QPointFB> &pa, std::vector<QPointFB> &pb);
    static void setPositions(std::vector<QPointFB> &pa, std::vector<QPointFB> &pb);
    static void selectEdges(std::vector<QPointFB> &pol, TPointPolygonPosition position, std::vector<Edge> &edges);
};

#endif // ALGORITHMS_H
