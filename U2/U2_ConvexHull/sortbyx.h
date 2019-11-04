#ifndef SORTBYX_H
#define SORTBYX_H

#include <QtGui>

class SortByX
{
public:
    SortByX();
    bool operator ()(QPoint &p1, QPoint &p2)
    {
        //Use logical AND
        return (p1.x() < p2.x()) || ((p1.x() == p2.x()) && (p1.y() < p2.y()));

    }
};

#endif // SORTBYX_H
