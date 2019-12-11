#ifndef QPOINTFB_H
#define QPOINTFB_H
#include <QPointF>
#include "types.h"

class QPointFB : public QPointF
{
private:
    double alpha, beta;
    TPointPolygonPosition position;

public:
    QPointFB():QPointF(0,0), alpha(0), beta(0), position(On){}
    QPointFB(double x, double y):QPointF(x,y), alpha(0), beta(0), position(On){}

    double getAlpha (){return alpha;}
    double getBeta (){return beta;}
    TPointPolygonPosition getPosition (){return position;}

    void setAlpha (double alpha_){alpha = alpha_;}
    void setBeta (double beta_){beta = beta_;}
    void setPosition (TPointPolygonPosition position_){position = position_;}
};

#endif // QPOINTFB_H
