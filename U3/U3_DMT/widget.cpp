#include "widget.h"
#include "ui_widget.h"

#include "algorithms.h"
#include "triangle.h"
#include "edge.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QtGui>
#include <vector>
#include <fstream>

Widget::Widget(QWidget *parent):
     QWidget(parent),
     ui(new Ui::Widget)
{
    ui->setupUi(this);
    z_min = 0;
    z_max = 1000;
    dz = 10;

    //Set initial values
    ui -> lineEdit_zmin -> setText(QString::number(z_min));
    ui -> lineEdit_zmax -> setText(QString::number(z_max));
    ui -> lineEdit_dz -> setText(QString::number(dz));
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_createDT_clicked()
{
    //Clear triangulation
    ui -> Canvas -> clearDT();
    //Delete contours
    ui -> Canvas -> clearContours();

    //Construct DT
    std::vector<QPoint3D> points = ui -> Canvas -> getPoints();
    std::vector<Edge> dt = Algorithms::DT(points);
    ui -> Canvas -> setDt(dt);

    repaint();
}

void Widget::on_pushButton_createContours_clicked()
{
    std::vector<Edge> dt;

    if (ui->Canvas->getDtSize() == 0)
    {
        //Construct DT
        std::vector<QPoint3D> points=ui->Canvas->getPoints();
        dt = Algorithms::DT(points);
        ui->Canvas->setDt(dt);
    }
    //Triangulation has been created
    else
        //Get triangulation
        dt = ui->Canvas->getDt();

    //Create contour lines

     std::vector<Edge> contours = Algorithms::createContourLines(dt, z_min, z_max, dz);
     std::vector<Edge> mainContours = Algorithms::createContourLines(dt, z_min, z_max, 5*dz);
     ui -> Canvas -> setContours(contours);
     ui -> Canvas -> setMainContours(mainContours);

    repaint();
}

void Widget::on_lineEdit_zmin_editingFinished()
{
    //Set z_min
    z_min = ui -> lineEdit_zmin -> text().toDouble();
}

void Widget::on_lineEdit_zmax_editingFinished()
{
    //Set z_max
    z_max = ui -> lineEdit_zmax -> text().toDouble();
}

void Widget::on_lineEdit_dz_editingFinished()
{
    //Set dz
    dz = ui -> lineEdit_dz -> text().toDouble();
}

void Widget::on_pushButton_importMeasurement_clicked()
{
    //Get window size
    int width = ui -> Canvas -> size().width();
    int height = ui -> Canvas -> size().height();

    //Get current directory
    QDir current_path = QDir::currentPath();
    current_path.cdUp();
    QString path = current_path.path();

    //Select text file with polygons coordinates
    QString source_file = QFileDialog::getOpenFileName(this, "Select text file with coordinates", path, "Text file (*.txt)");

    //Convert path from QString to string
    std::string source_file_std = source_file.toStdString();

    //Load measured points
    std::vector<QPoint3D> measured_points = Algorithms::importMeasurement(source_file_std, width, height, z_min, z_max);

    //Test if file was opened correctly size > 0
    if (measured_points.empty())
        QMessageBox::warning(this, "Import error", "File can NOT be opened or read correctly");

    else
    {
        //Set measured point
        ui -> Canvas ->setPoints(measured_points);

        //Set text for the nearest and highest contour
        ui -> lineEdit_zmin -> setText(QString::number(z_min));
        ui -> lineEdit_zmax -> setText(QString::number(z_max));
    }

    //repaint();
}

void Widget::on_pushButton_generateShape_clicked()
{
    //Get window size
    int width = ui -> Canvas -> size().width();
    int height = ui -> Canvas -> size().height();

    //Get shape
    int shape = ui -> comboBox_shape -> currentIndex();

    //Generate shape
    std::vector<QPoint3D> shape_points = Algorithms::generateShapes(shape,width, height);
    ui -> Canvas ->setPoints(shape_points);

    repaint();

}

void Widget::on_pushButton_clearSelected_clicked()
{
    //What clear
    int select = ui -> comboBox_clear -> currentIndex();
    if (select == 0)
        ui -> Canvas -> clearDT();
    else if (select == 1)
        ui -> Canvas -> clearContours();
    else if (select == 2)
        ui -> Canvas -> clearPoints();

    repaint();

}

void Widget::on_pushButton_clearAll_clicked()
{
    ui -> Canvas -> clearDT();
    ui -> Canvas -> clearContours();
    ui -> Canvas -> clearPoints();

    repaint();

}

void Widget::on_pushButton_analyze_clicked()
{
    bool slope = FALSE;
    bool aspect = FALSE;

    //Analyze slope and aspect
    std::vector<Edge> dt= ui -> Canvas -> getDt();
    std::vector<Triangle> dmt = Algorithms::analyzeDMT(dt);
    ui -> Canvas -> setDMT(dmt);

    if(ui -> comboBox_analyze -> currentIndex()==0){
        slope = TRUE;
        aspect = FALSE;
    }
    else if(ui -> comboBox_analyze -> currentIndex()==1){
        slope = FALSE;
        aspect = TRUE;
    }
    ui -> Canvas -> setSlope(slope);
    ui -> Canvas -> setAspect(aspect);
    repaint();
}
