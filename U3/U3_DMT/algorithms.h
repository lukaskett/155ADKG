#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include <QtGui>
#include <fstream>
#include <iostream>

#include "qpoint3d.h"
#include "edge.h"
#include "triangle.h"


class Algorithms
{
public:
    Algorithms();
    static int getPointLinePosition(QPoint3D &q,QPoint3D &p1,QPoint3D &p2);
    static double getCircleRadius(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3, QPoint3D &c);
    static int getNerestPoint(QPoint3D &p, std::vector<QPoint3D> &points);
    static double getDistance(QPoint3D &p1, QPoint3D &p2);
    static int getDelaunayPoint(QPoint3D &s, QPoint3D &e, std::vector<QPoint3D> &points);
    static std::vector<Edge> DT(std::vector<QPoint3D> &points);
    static QPoint3D getContourPoint(QPoint3D &p1,QPoint3D &p2, double z);
    static std::vector<Edge> createContourLines(std::vector<Edge> &dt, double z_min, double z_max, double dz);
    static double calculateSlope(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3);
    static double calculateAspect(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3);
    static std::vector<Triangle> analyzeDMT(std::vector<Edge> &dt);
    static std::vector<QPoint3D> importMeasurement(std::string path, int width, int height,  double &min_z, double &max_z);
    static std::vector<QPoint3D> generateShapes(int shape, int width, int height);
};

#endif // ALGORITHMS_H
