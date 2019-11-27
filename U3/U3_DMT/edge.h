#ifndef EDGE_H
#define EDGE_H
#include "qpoint3d.h"


class Edge
{

private:
    QPoint3D s, e;

public:
    Edge():s(0, 0, 0),e(0, 0, 0){}
    //Edge(QPoint3D &start, QPoint3D &end){  dva mozne zpusoby moznosti zapisu konstruktoru
        //s = start;
        //e = end;
    //}

    Edge(QPoint3D &start, QPoint3D &end):s(start), e(end){} //druha moznost

    QPoint3D& getStart(){return s;} //navratovy typ reference
    void setStart(QPoint3D &s){this -> s = s;}
    QPoint3D& getEnd(){return e;}
    void setEnd(QPoint3D &e){this -> e = e;} //Bud s this nebo jine jmeno, treba s podtrzitkem
    void changeOrientation(){
        QPoint3D pom = s;
        s = e;
        e = pom;
    }

    bool operator == (const Edge &h) const {
        return (h.s == s) && (h.e == e);
    }
};

#endif // EDGE_H
