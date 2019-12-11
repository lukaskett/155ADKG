#ifndef EDGE_H
#define EDGE_H
#include "qpointfb.h"

class Edge
{
private:
    QPointFB s,e;

public:
    Edge():s(0,0), e(0,0){}
    Edge(QPointFB &start, QPointFB &end):s(start), e(end){}

    QPointFB & getStart(){return s;}
    void setStart(QPointFB &s){this->s = s;}
    QPointFB & getEnd(){return e;}
    void setEnd(QPointFB &e){this->e = e;}
};

#endif // EDGE_H
