#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"
#include <QMessageBox>
#include <chrono>

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

    //Measure time - start timer
    //https://stackoverflow.com/questions/2808398/easily-measure-elapsed-time
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    if(points.size() >0 )
    {
        if(ui -> comboBox_chMethod->currentIndex() == 0)
            ch = Algorithms::jarvisScan(points);

        else if(ui->comboBox_chMethod->currentIndex() == 1)
            ch = Algorithms::qHull(points);

        else if(ui->comboBox_chMethod->currentIndex() == 2)
            ch = Algorithms::sweepLine(points);
        else
            ch = Algorithms::grahamScan(points);
    }

    //Stop Timer
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    //Calculate elapsed time - microseconds precision -> miliseconds
    double time_clock = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000;
   QString time_elapsed = QString::number(time_clock);
   ui -> label_timeElapsed -> setText(time_elapsed);

    //Draw
    ui -> Canvas -> setCH(ch);
    repaint();

}

void Widget::on_pushButton_clearPoints_clicked()
{
    ui -> Canvas ->clearPoints();
    ui -> label_timeElapsed -> clear();
}

void Widget::on_pushButton_clearCH_clicked()
{
    ui -> Canvas -> clearCH();
    ui -> label_timeElapsed -> clear();
}

void Widget::on_pushButton_generatePoints_clicked()
{
    //Clear time elapsed label
    ui -> label_timeElapsed -> clear();

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
