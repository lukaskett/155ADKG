#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"
#include <QMessageBox>

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

void Widget::on_pushButton_generatePoints_clicked()
{
    //Get window size
    int width = ui -> Canvas -> size().width();
    int height = ui -> Canvas -> size().height();

    //Get method and point count for point generation
    int method = ui -> comboBox_generateMethod -> currentIndex();
    int count_points = ui -> comboBox_countPoints -> currentText().toInt();

    //Generate points
    ui -> Canvas -> generatePoints(method, count_points, width, height);
    repaint();

    std::vector<QPoint> points;
    points = ui->Canvas -> getPoints();
    int count = points.size();
    QMessageBox::information(this, "Test generate", QString("Point count: %1, height: %2").arg(count).arg(height));
}
