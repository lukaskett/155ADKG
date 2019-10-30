#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <vector>

class Draw : public QWidget
{
    Q_OBJECT

private:
    std::vector<QPoint> points;
    QPolygon ch;

public:
    explicit Draw(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void clearCH(){ch.clear(); repaint();}
    void clearPoints(){points.clear(); repaint();}
    std::vector<QPoint>getPoints(){return points;}
    void setCH(QPolygon &hull){ch = hull;}

signals:

public slots:
};

#endif // DRAW_H
