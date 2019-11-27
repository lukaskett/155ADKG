#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <vector>
#include <fstream>
#include <iostream>

#include "qpoint3d.h"
#include "edge.h"
#include "triangle.h"

class Draw : public QWidget
{
    Q_OBJECT
private: //co chci vykreslit musim mit zd
    std::vector<QPoint3D> points;
    std::vector<Edge> dt;
    std::vector<Edge> contours;
    std::vector<Triangle> dmt;

public:
    explicit Draw(QWidget *parent = nullptr);

    void setPoints(std::vector<QPoint3D> &points_){points = points_;}
    void setDt(std::vector<Edge> &dt_) {dt = dt_;}
    void setDMT(std::vector<Triangle> &dmt_){dmt = dmt_;}
    void setContours(std::vector<Edge> &contours_) {contours = contours_;}

    int getDtSize(){return dt.size();}
    std::vector<Edge>& getDt(){return dt;} //nekopirujeme, vracime jen adresu, kdyby mela DT hodne bodu
    std::vector<QPoint3D> getPoints(){return points;}

    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

    void clearPoints(){points.clear();}
    void clearDT(){dt.clear();}
    void clearContours(){contours.clear();}

signals:

public slots:
};

#endif // DRAW_H
