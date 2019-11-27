#ifndef QPOINT3D_H
#define QPOINT3D_H
#include <QtGui>

class QPoint3D : public QPointF
{
private:
    double z;

public:
    //konstruktor rodicovske tridy QPointF, s carkou volam inicializuju datovou polozku nasi tridy
    QPoint3D():QPointF(0, 0),z(0){} //Inicializace konstrktoru
    QPoint3D(double x, double y, double z_):QPointF(x,y),z(z_){} //Parametricky konstruktor

    //Getter and setter
    double getZ(){return z;}
    void setZ(double z){this -> z = z;} //Operator this, řešení konfliktu jmen

    //bool operator == (QPoint3D &p){}

};

#endif // QPOINT3D_H
