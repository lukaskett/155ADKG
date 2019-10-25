#ifndef SORTBYOMEGA_H
#define SORTBYOMEGA_H
#include "algorithms.h"
#include <QtGui>

class SortByOmega
{
public:
    struct OmegaStruct
    {
        QPoint point;
        double omega;
    };

    SortByOmega();
    bool operator ()(OmegaStruct &p1, OmegaStruct &p2)
    {
        //Is omega2 bigger than omega1
        return (p1.omega < p2.omega);

    }
private:

};

#endif // SORTBYOMEGA_H
