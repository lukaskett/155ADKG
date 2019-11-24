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
        double distance;
    };

    SortByOmega();
    bool operator ()(OmegaStruct &p1, OmegaStruct &p2)
    {
        //Is omega2 bigger than omega1
        return (p1.omega < p2.omega) || ((p1.omega == p2.omega) && (p1.distance < p2.distance));

    }
private:

};

#endif // SORTBYOMEGA_H
