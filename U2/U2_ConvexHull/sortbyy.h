#ifndef SORTBYY_H
#define SORTBYY_H

#include <QtGui>

class SortByY
{
public:
    SortByY();
    bool operator ()(QPoint &p1, QPoint &p2)
    {
        //Use logical AND
        return (p1.y() < p2.y()) || ((p1.y() == p2.y()) && (p1.x() < p2.x()));
    }
};

#endif // SORTBYY_H
