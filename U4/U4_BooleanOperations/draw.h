#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include "qpointfb.h"
#include "edge.h"

class Draw : public QWidget
{
    Q_OBJECT
private:
    std::vector<QPointFB> a,b, inA, inB;
    std::vector<Edge> res;
    bool ab,inout;

public:
    explicit Draw(QWidget *parent = nullptr);
    void changePolygon(){ab = !ab;}
    void changeInOut(){inout = !inout;}

    void setA (std::vector<QPointFB> &a_){a = a_;}
    void setB (std::vector<QPointFB> &b_){b = b_;}
    void setRes (std::vector<Edge> res_){res = res_;}

    std::vector<QPointFB> getA(){return a;}
    std::vector<QPointFB> getB(){return b;}
    std::vector<QPointFB> getAHole(){return inA;}
    std::vector<QPointFB> getBHole(){return inB;}
    std::vector<Edge> getRes(){return res;}
    bool getPolygonStatus(){return ab;}
    bool getDrawStatus(){return inout;}

    void clearResults() {res.clear();}
    void clearAll() {res.clear(); a.clear(); b.clear(); inA.clear(); inB.clear();}
    void removeLast();

    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void drawPolygon(QPainter &painter, std::vector<QPointFB> &polygon);

signals:

public slots:
};

#endif // DRAW_H