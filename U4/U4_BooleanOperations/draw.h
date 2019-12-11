#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include "qpointfb.h"
#include "edge.h"

class Draw : public QWidget
{
    Q_OBJECT
private:
    std::vector<QPointFB> a,b;
    std::vector<Edge> res;
    bool ab;

public:
    explicit Draw(QWidget *parent = nullptr);
    void changePolygon(){ab = !ab;}
    void setA (std::vector<QPointFB> &a_){a = a_;}
    void setB (std::vector<QPointFB> &b_){b = b_;}
    void setRes (std::vector<Edge> res_){res = res_;}
    std::vector<QPointFB> getA(){return a;}
    std::vector<QPointFB> getB(){return b;}
    std::vector<Edge> getRes(){return res;}
    void clearResults() {res.clear();}
    void clearAll() {res.clear(); a.clear(); b.clear();}
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void drawPolygon(QPainter &painter, std::vector<QPointFB> &polygon);

signals:

public slots:
};

#endif // DRAW_H
