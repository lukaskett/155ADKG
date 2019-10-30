#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_createCH_clicked()
{
    //Create convex hull
    std::vector<QPoint> points = ui -> Canvas -> getPoints();
    QPolygon ch;

    if(points.size() >0 )
    {
        if(ui -> comboBox_chMethod->currentIndex() == 0)
            ch = Algorithms::jarvisScan(points);

        else if(ui->comboBox_chMethod->currentIndex() == 1)
            ch = Algorithms::qHull(points);

        else
            ch = Algorithms::sweepLine(points);
    }

    //Draw
    ui -> Canvas -> setCH(ch);
    repaint();

}

void Widget::on_pushButton_clearPoints_clicked()
{
    ui -> Canvas ->clearPoints();
}

void Widget::on_pushButton_clearCH_clicked()
{
    ui -> Canvas -> clearCH();
}
