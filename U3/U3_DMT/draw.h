#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <vector>
#include <fstream>
#include <iostream>

#include "qpoint3d.h"
#include "edge.h"
#include "triangle.h"
#include "algorithms.h"

class Draw : public QWidget
{
    Q_OBJECT
private: //co chci vykreslit musim mit zde
    std::vector<QPoint3D> points;
    std::vector<Edge> dt;
    std::vector<Edge> contours;
    std::vector<double> metadata;
    std::vector<Triangle> dmt;
    std::vector<QPoint3D> polygon;
    bool draw_mode;
    bool slope, aspect;
    int colorScale;
    int dz;

public:
    explicit Draw(QWidget *parent = nullptr);

    void setPoints(std::vector<QPoint3D> &points_){points = points_;}
    void setDt(std::vector<Edge> &dt_) {dt = dt_;}
    void setDMT(std::vector<Triangle> &dmt_){dmt = dmt_;}
    void setContours(std::vector<Edge> &contours_, std::vector<double> &metadata_, int dz_);
    void setSlope(bool slope_){slope = slope_;}
    void setAspect(bool aspect_){aspect = aspect_;}
    void setColorScale(int colorScale_){colorScale = colorScale_;}
    void setDrawMode(){draw_mode = !draw_mode;}

    int getDtSize(){return dt.size();}
    std::vector<Edge>& getDt(){return dt;} //nekopirujeme, vracime jen adresu, kdyby mela DT hodne bodu
    std::vector<QPoint3D> getPoints(){return points;}
    std::vector<QPoint3D> getPolygon(){return polygon;}
    int getPolygonSize(){return polygon.size();}

    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

    void clearPoints(){points.clear();}
    void clearDT(){dt.clear();}
    void clearContours(){contours.clear();}
    void clearSlope(){slope = FALSE;}
    void clearAspect(){aspect = FALSE;}
    void clearPolygon(){polygon.clear();}
    void clearAll(){points.clear(); dt.clear(); contours.clear(); slope = FALSE; aspect = FALSE; polygon.clear();}

signals:

public slots:
};

#endif // DRAW_H
