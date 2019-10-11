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

    //Winding Number algorithm
    if (ui->comboBox->currentIndex() == 0)
        for(int h; h < count_pol_Canvas; h++)
        {
            //Get polygon from polygons for analysis
            std::vector<QPoint> pol_analy = ui -> Canvas -> getPolygon(h);

            //Analyze and store the result
            res.push_back(Algorithms::positionPointPolygonWinding(q, pol_analy));

            //Stop when point found inside one polygon
            if(res[h] == 1)
                break;

        }

    // Ray Crossing algorithm
    else
        for(int h; h < count_pol_Canvas; h++)
        {
            //Get polygon from polygons for analysis
            std::vector<QPoint> pol_analy = ui -> Canvas -> getPolygon(h);

            //Analyze and store the result
            res.push_back(Algorithms::positionPointPolygonRayCrossing(q, pol_analy));

            //Stop when point found inside one polygon
            if(res[h] == 1)
                break;
        }

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
