#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QString>

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
    ui->label_displayResult->clear();
    ui -> lineEdit_polygonCount -> clear();
}

void Widget::on_analyzeButton_clicked()
{
    //Analyze point and polygon position

    //Get point Q
    QPoint q = ui->Canvas->getPoint();

    //Get number of polygons in Canvas
    unsigned int count_pol_Canvas = ui->Canvas->getNumberOfPolygons();

    //In case of no polygon
    if(count_pol_Canvas == 0 )
    {
         QMessageBox::warning(this, "Empty polygon network", "You have to import polygons first");
    }

    //Store analysis results for each polygon
    std::vector<int> res;

    //flag for showing analysis result (inside/outside/on the boundary)
    //true - show result in label_displayResult; false - already displayed
    bool show_result = true;

    //Winding Number algorithm
    if (ui->comboBox->currentIndex() == 0)
    {
        for(unsigned int h = 0; h < count_pol_Canvas; h++)
        {
            //Get polygon from polygons for analysis
            std::vector<QPoint> pol_analy = ui -> Canvas -> getPolygon(h);

            //Analyze and store the result
            res.push_back(Algorithms::positionPointPolygonWinding(q, pol_analy));

            //Display result in label
            showResultOfAnalysis(res[h], show_result);

            //Stop when point found inside one polygon
            if(res[h] == 1)
            {
                break;
            }
        }

        //Show lightpolygons which have q inside/on the boundary
        ui->Canvas->fillPolygon(res);
    }

    // Ray Crossing algorithm
    else
    {
        for(unsigned int h = 0 ; h < count_pol_Canvas; h++)
        {
            //Get polygon from polygons for analysis
            std::vector<QPoint> pol_analy = ui -> Canvas -> getPolygon(h);

            //Analyze and store the result
            res.push_back(Algorithms::positionPointPolygonRayCrossing(q, pol_analy));

            //Display result in label
            showResultOfAnalysis(res[h], show_result);

            //Stop when point found inside one polygon
            if(res[h] == 1)
            {
                break;
            }
        }
        //Show lightpolygons which have q inside/on the boundary
        ui->Canvas->fillPolygon(res);
    }

    //Clear for next analysis
    res.clear();
}

void Widget::on_importPolygonButton_clicked()
{
    ui->Canvas->clearCanvas();

    //Select text file with polygons coordinates
    QString source_file = QFileDialog::getOpenFileName(this, "Select text file with the polygons", "C://", "Text file (*.txt)");

    //Convert path from QString to string
    std::string source_file_std = source_file.toStdString();

    //Load polygons
    ui->Canvas->importPolygon(source_file_std);
}

  void Widget::showResultOfAnalysis(int res, bool &show_result)
{
    //send to output wheather point is in/out/on the boundari(es)

    //SHOW or not - check show_result - if true, show result, if false, don´t show (already displayed)
    if(res == 1 && show_result)
    {
        ui->label_displayResult->setText("Point INSIDE polygon.");
        show_result = false;
    }
    else if(res == 0 && show_result)
    {
        ui->label_displayResult->setText("Point OUTSIDE polygon network.");
    }
    else if(res == -1 && show_result)
    {
        ui->label_displayResult->setText("Point placed ON THE BOUNDARY.");
        show_result = false;
    }
}


void Widget::on_generatePolygonsButton_clicked()
{
    ui->Canvas->clearCanvas();

    //Get number of generated polygons
    int pol_count = ui -> lineEdit_polygonCount -> text().toInt();

    //Test
    //1)Value not inserted
    if(pol_count == 0)
    {
        QMessageBox::warning(this, "How many points in the polygon do you want?", "Insert number of points in the polygon");
    }

    //2)Inserted value lower than 3, polygon not defined
    else if(pol_count < 3 && pol_count > 0)
    {
        QMessageBox::warning(this, "Polygon not defined", "Insert number bigger than 2");
    }

    //Generate points, there must be 3 or more points generated
    else
        ui -> Canvas -> generatePoints(pol_count);
    repaint();
}
