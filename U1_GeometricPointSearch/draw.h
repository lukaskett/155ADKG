#ifndef DRAW_H
#define DRAW_H

#include <vector>
#include <QWidget>
#include <QtGui>
#include <fstream>
#include <string>
#include <iostream>

class Draw : public QWidget
{
    Q_OBJECT

private:
    bool draw_mode;
    std::vector<std::vector<QPoint>> polygons;
    QPoint q;

public:
    explicit Draw(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);
    void clearCanvas();
    void setDrawMode(){draw_mode = !draw_mode;}
    void importPolygon(std::string path);
    QPoint getPoint(){return q;}
    std::vector<QPoint> getPolygon(int index); //return polygon from vector of polygons based on the index
    int getNumberOfPolygons() {return polygons.size();} //how many polygons there are in canvas

signals:

public slots:
};

#endif // DRAW_H
