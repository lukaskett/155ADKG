#ifndef SORTBYY_H
#define SORTBYY_H

#include <QtGui>


class SortByY
{
public:
    SortByY();
    bool operator ()(QPoint &p1, QPoint &p2)
    {
        return p1.y() < p2.y();
    }
};

#endif // SORTBYY_H
