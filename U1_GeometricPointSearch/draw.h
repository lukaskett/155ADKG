#ifndef DRAW_H
#define DRAW_H

#include <vector>
#include <QWidget>
#include <QtGui>
#include <fstream>
#include <string>
#include <iostream>
#include <QPainterPath>
#include <QBrush>
#include <QPen>

class Draw : public QWidget
{
    Q_OBJECT

private:
    bool draw_mode;
    std::vector<std::vector<QPoint>> polygons;
    QPoint q;
    std::vector<int> analyze_results_by_polygons; //store point-polygon analyze result

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
    void fillPolygon(std::vector<int> analyze_results_by_polygons); //highlight polygon containing point q ADDED BY ME
signals:

public slots:
};

#endif // DRAW_H
