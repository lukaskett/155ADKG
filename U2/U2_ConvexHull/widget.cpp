#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"
#include <QMessageBox>
#include <chrono>
#include <fstream>
#include <sstream>

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

        else if(ui -> comboBox_chMethod -> currentIndex() == 2)
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

    //int count = ch.size();
    //QMessageBox::information(this, "Test generate", QString("Convex hull has: %1 points.").arg(count));

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
    //Inform that it may last long time to solve U2
    QString cb = ui -> comboBox_chMethod -> currentText();
    QMessageBox::information(this,
                             "Important! It may last to solve",
                             QString("Selected algorithm: %1, normally it takes around 10 minutes for Jarvis Scan and 5 minutes for other algorithms.").arg(cb)
                             );

    //Convex hull
    QPolygon ch;

    //Point counts
    std::vector<int> count_points{1000, 5000, 10000, 25000, 50000, 75000, 100000, 250000, 500000, 750000, 1000000};

    //Generating methods
    std::vector<std::string> methods{"raster", "random", "circle"};

    //Generating window size
    int width = 1000;
    int height = 1000;

    //Concatenate text file name
    //https://www.quora.com/How-do-I-split-a-string-by-space-into-an-array-in-c++

    //Selected algorithm
    std::string algorithm = ui -> comboBox_chMethod -> currentText().toStdString();

    //File extension
    std::string text_file = ".txt";

    //Remove space
    std::vector<std::string> splitted_algorithm;
    std::istringstream iss(algorithm);
    for(std::string s; iss >> algorithm; )
        splitted_algorithm.push_back(algorithm);

    //Process - concatenate strings
    std::stringstream ss_algorithm;
    ss_algorithm << splitted_algorithm[0] << splitted_algorithm[1] << text_file;

    //Final file name - without spaces
    std::string file_name = ss_algorithm.str();

    //Save to file
    std::ofstream myfile;
    myfile.open (file_name, std::ios_base::app);
    myfile << "Count points, generating method, algorithm, repetition, elapsed time[miliseconds]"<< "\n";
    std::string algorithm_selected = ui -> comboBox_chMethod -> currentText().toStdString();

    for(unsigned int c = 0; c < count_points.size(); c++)
    {
        //For generating methods - raster - 0, random - 1, circle - 2
        for( int m = 0; m < methods.size(); m++)
        {
            std::vector<QPoint> gen_points = ui -> Canvas -> Draw::generatePointsU2(m, count_points[c], width, height);

            //Repeat rep times
            unsigned int rep = 10;

            for (unsigned int r = 1; r < rep + 1; r++)
            {
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

                myfile << count_points[c] << "," << methods[m] << "," << algorithm_selected << "," << r << ","<< time_clock << "\n";

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

