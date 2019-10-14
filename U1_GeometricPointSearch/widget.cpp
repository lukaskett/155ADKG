#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"
#include <QFileDialog>

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


void Widget::on_clearButton_clicked()
{
    ui->Canvas->clearCanvas();
    ui->label->clear();
}

void Widget::on_drawModeButton_clicked()
{
    ui->Canvas->setDrawMode();
}

void Widget::on_analyzeButton_clicked()
{
    //Analyze point and polygon position

    //Get point Q
    QPoint q = ui->Canvas->getPoint();

    //Get number of polygons in Canvas
    int count_pol_Canvas = ui->Canvas->getNumberOfPolygons();

    //OLD:Get one polygon for analysis
    std::vector<QPoint> polygon = ui->Canvas->getPolygon(0); //Add loop through all polygons, forced the first one

    //Store analysis results for each polygon
    std::vector<int> res;

    //flag for setting output (if point is inside/outside/on boundary), on false, output will not be changed ADDED BY ME
    //bool write_result = true;

    //Winding Number algorithm
    if (ui->comboBox->currentIndex() == 0)
        for(int h = 0; h < count_pol_Canvas; h++)
        {
            //Get polygon from polygons for analysis
            std::vector<QPoint> pol_analy = ui -> Canvas -> getPolygon(h);

            //Analyze and store the result
            res.push_back(Algorithms::positionPointPolygonWinding(q, pol_analy));

            //Stop when point found inside one polygon
            if(res[h] == 1)
            {
            break;
            }

            //Show lightpolygons which have q inside/on the boundary ADDED BY ME
            ui->Canvas->fillPolygon(res);
        }

    // Ray Crossing algorithm
    else
        for(int h = 0 ; h < count_pol_Canvas; h++)
        {
            //Get polygon from polygons for analysis
            std::vector<QPoint> pol_analy = ui -> Canvas -> getPolygon(h);

            //Analyze and store the result
            res.push_back(Algorithms::positionPointPolygonRayCrossing(q, pol_analy));

            //Stop when point found inside one polygon
            if(res[h] == 1)
               { break;
            }
        }

    //Show lightpolygons which have q inside/on the boundary ADDED BY ME
    ui->Canvas->fillPolygon(res);

    //Print results
    //Task:Create special function

}

void Widget::on_importPolygonButton_clicked()
{
    //Select text file with polygons coordinates
    QString source_file = QFileDialog::getOpenFileName(this, "Select text file with the polygons", "C://", "Text file (*.txt)");

    //Convert path from QString to string
    std::string source_file_std = source_file.toStdString();

    //Load polygons
    ui->Canvas->importPolygon(source_file_std);
}

// ADDED BY ME
/*
  void Widget::ResultofAnalyze(int res, bool &write_result)
{
    //send to output wheather point is in/out/on the boundari(es)

    //check for write_result - if true, we want to write, if false, we do not (because we know already)
    if(res == 1 && write_result)
    {
        ui->label->setText("Point is in polygon.");
        write_result = false;
    }
    else if(res == 0 && write_result)
    {
        ui->label->setText("Point is out polygon.");
    }
    else if(res == -1 && write_result)
    {
        ui->label->setText("Point is on boundary.");
        write_result = false;
    }
}
*/
