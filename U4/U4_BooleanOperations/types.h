#ifndef TYPES_H
#define TYPES_H

//Position of the point and line
typedef enum
{
  LeftHp = 0,
  RightHp = 1,
  Colinear = 2

}TPointLinePosition;

//Position of the point and polygon
typedef enum
{
    Inner,
    Outer,
    On

}TPointPolygonPosition;

//Define Boolean operation
typedef enum
{
    Union,
    Intersect,
    DifferenceAB,
    DifferenceBA

}TBooleanOperation;

//2 lines position
typedef enum
{
  Paralel,
  Identical,
  NonIntersected,
  Intersected

}T2LinesPosition;

#endif // TYPES_H
