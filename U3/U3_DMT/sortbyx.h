#ifndef SORTBYX_H
#define SORTBYX_H

#include <QtGui>
#include "qpoint3d.h"

class SortByX
{
public:
    SortByX();
    bool operator ()(QPoint3D &p1, QPoint3D &p2)
    {
        //Use logical AND
        return p1.x() < p2.x() || (p1.x() == p2.x()) && (p1.y() < p2.y());

    }
};

#endif // SORTBYX_H
