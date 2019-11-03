#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"
#include <QMessageBox>
#include <chrono>
#include <fstream>

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

    if(points.size() > 0 )
    {
        if(ui -> comboBox_chMethod -> currentIndex() == 0)
            ch = Algorithms::jarvisScan(points);

        else if(ui -> comboBox_chMethod -> currentIndex() == 1)
            ch = Algorithms::qHull(points);

        else
            ch = Algorithms::sweepLine(points);
    }

    //Stop Timer
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    //Calculate elapsed time - microseconds precision -> miliseconds
    double time_clock = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000;
   QString time_elapsed = QString::number(time_clock);
   ui -> label_timeElapsed -> setText(time_elapsed);

   //Save results to the table

   //Get values
   int v_algorithm = ui -> comboBox_chMethod -> currentIndex();
   int v_method = ui -> comboBox_generateMethod -> currentIndex();
   int v_count_points = ui -> comboBox_countPoints -> currentText().toInt();

   //Prepare table
   ui -> tableWidget -> setColumnCount(4);
   //Set column size
   ui -> tableWidget -> setColumnWidth(0, 5);
   ui -> tableWidget -> setColumnWidth(1, 10);
   ui -> tableWidget -> setColumnWidth(2, 10);
   ui -> tableWidget -> setColumnWidth(3, 10);

   //Set table header
   QStringList horzHeaders;
   horzHeaders << "Algorithm" << "Number of points" << "Generating method" << "Time";
   ui ->tableWidget ->setHorizontalHeaderLabels(horzHeaders);

   //Set row
   int row = ui -> tableWidget->rowCount();
   ui -> tableWidget -> insertRow(ui -> tableWidget->rowCount());
   ui -> tableWidget -> setRowHeight(row, 5);

   //Insert values
   ui -> tableWidget -> setItem(row, ALGORITHM, new QTableWidgetItem(QString::number(v_algorithm)));
   ui -> tableWidget -> setItem(row, N_POINTS, new QTableWidgetItem(QString::number(v_count_points)));
   ui -> tableWidget -> setItem(row, METHOD, new QTableWidgetItem(QString::number(v_method)));
   ui -> tableWidget -> setItem(row, TIME, new QTableWidgetItem(time_elapsed));

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

    //QMessageBox::information(this, "Test generate", QString("Point count: %1, height: %2").arg(count).arg(height));
}

void Widget::on_pushButton_solveU2_clicked()
{
    //Convex hull
    QPolygon ch;

    //Point counts
    std::vector<int> count_points{1000, 5000, 10000, 25000, 50000, 75000, 100000, 250000, 500000, 750000, 1000000};

    //Generating methods
    std::vector<int> methods{0, 1, 2};

    //Generating window size
    int width = 1000;
    int height = 1000;

    //Save to file
    std::ofstream myfile;
    myfile.open ("ConvexHull.txt", std::ios_base::app);
    myfile << "Generating method: 0 - raster, 1 - random, 3 - circle "<< "\n";
    myfile << "Algorithm: 0 - Javis Scan, 1 - Quick Hull, 2 - Sweep Line, 3 - Graham Scan"<< "\n";
    myfile << "Count points, generating method, algorithm, repetition, elapsed time[miliseconds]"<< "\n";

    for(unsigned int c = 0; c < count_points.size(); c++)
    {
        //For generating methods - raster - 0, random - 1, circle - 2
        for( int m = 0; m < 3; m++)
        {
            std::vector<QPoint> gen_points = ui -> Canvas -> Draw::generatePointsU2(m, count_points[c], width, height);

            //Algorithms: Jarvis Scan - 0, Quick Hull - 1, Sweep Line - 2, Graham Scan - 3

            //Repeat rep times
            unsigned int rep = 10;

            for (unsigned int r = 1; r < rep + 1; r++)
            {
                int a = ui -> comboBox_chMethod -> currentIndex();
                //Start timer
                std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

                if(ui -> comboBox_chMethod -> currentIndex() == 0)
                    ch = Algorithms::jarvisScan(gen_points);

                else if( ui -> comboBox_chMethod -> currentIndex() == 1)
                    ch = Algorithms::qHull(gen_points);

                else if (ui -> comboBox_chMethod -> currentIndex() == 2)
                    ch = Algorithms::sweepLine(gen_points);

                else
                    ch = Algorithms::grahamScan(gen_points);

                //Stop timer
                std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

                //Calculate elapsed time - microseconds precision -> miliseconds
                double time_clock = (std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) / 1000;

                myfile << count_points[c] << "," << m << "," << a << "," << r << ","<< time_clock << "\n";

            }
            //Clear convex hull for the next round
            ch.clear();

            //Clear points for another generating
            gen_points.clear();
        }

    }

    myfile.close();
    ui -> label -> setText("Finished and saved in file :)");
}

