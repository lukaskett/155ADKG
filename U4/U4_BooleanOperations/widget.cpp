#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"
#include "types.h"
#include <cmath>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui -> label_drawPolygonAB -> setStyleSheet("QLabel { color: green }");
    ui -> label_drawHoleBorder -> setStyleSheet("QLabel { color: green }");
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_switchPolygon_clicked()
{
    //Change polygon mode - draw A or draw B
    ui -> Canvas -> changePolygon();

    //true - A, false B
    bool ab = ui -> Canvas -> getPolygonStatus();

    if(!ab)
    {
        ui -> label_drawPolygonAB -> setText(QString("Polygon B"));
        ui -> label_drawPolygonAB -> setStyleSheet("QLabel { color: blue }");
        ui -> label_drawHoleBorder -> setStyleSheet("QLabel { color: blue }");
    }

    else
    {
        ui -> label_drawPolygonAB -> setText(QString("Polygon A"));
        ui -> label_drawPolygonAB -> setStyleSheet("QLabel { color: green }");
        ui -> label_drawHoleBorder -> setStyleSheet("QLabel { color: green }");
    }

}

void Widget::on_pushButton_changeInOut_clicked()
{
    ui -> Canvas -> changeInOut();

    //Hole or border status
    bool inout = ui -> Canvas -> getDrawStatus();
    if(inout)
        ui -> label_drawHoleBorder -> setText(QString("Hole"));
    else
        ui -> label_drawHoleBorder -> setText(QString("Border"));
}

void Widget::on_pushButton_createOverlay_clicked()
{
    //Get polygons
    std::vector<QPointFB> polA = ui -> Canvas -> getA();
    std::vector<QPointFB> polB = ui -> Canvas -> getB();

    //Get holes
    std::vector<QPointFB> holeA = ui -> Canvas -> getAHole();
    std::vector<QPointFB> holeB = ui -> Canvas -> getBHole();

    //Perform Boolean operation
    TBooleanOperation oper = TBooleanOperation(ui -> comboBox -> currentIndex());
    std::vector<Edge> res = Algorithms::booleanOperations(polA, polB, oper);

    //Process holes
    std::vector<Edge> res_hole, remove;

    //Union
    if(ui -> comboBox -> currentIndex() == 0)
    {
        //Polygon B and hole A
        std::vector<Edge>res_holeA1 = Algorithms::booleanOperationsHoles(polB, holeA, Outer);
        std::vector<Edge>res_holeA2 = Algorithms::booleanOperationsHoles(holeA, polB, Inner);
        Algorithms::mergeVectors(res_holeA1, res_holeA2, res_hole);

        //Polygon A and hole B
        std::vector<Edge>res_holeB1 = Algorithms::booleanOperationsHoles(polA, holeB, Outer);
        std::vector<Edge>res_holeB2 = Algorithms::booleanOperationsHoles(holeB, polA, Inner);
        Algorithms::mergeVectors(res_holeB1, res_holeB2, res_hole);
    }

    //Intersect
    else if(ui -> comboBox -> currentIndex() == 1)
    {
        //Hole A and hole B
        std::vector<Edge>res_holeA = Algorithms::booleanOperationsHoles(holeA, holeB, Inner);
        std::vector<Edge>res_holeB = Algorithms::booleanOperationsHoles(holeB, holeA, Inner);
        Algorithms::mergeVectors(res_holeA, res_holeB, res_hole);
    }

    //Difference A - B
    else if(ui -> comboBox -> currentIndex() == 2)
    {
        //Polygon A and hole B
        std::vector<Edge>res_holeB1 = Algorithms::booleanOperationsHoles(polA, holeB, Inner);
        std::vector<Edge>res_holeB2 = Algorithms::booleanOperationsHoles(holeB, polA, Inner);
        Algorithms::mergeVectors(res_holeB1, res_holeB2, res_hole);

        //Polygon B and hole A
        std::vector<Edge>res_holeA1 = Algorithms::booleanOperationsHoles(polB, holeA, Outer);

        //Hole A and hole B        
        std::vector<Edge>res_holeB3 = Algorithms::booleanOperationsHoles(holeB, holeA, Inner);

        //Merge single cases
        Algorithms::mergeVectors(res_holeA1, res_holeB3, res_hole);

        //Remove
        std::vector<Edge>remove_holA_polB = Algorithms::booleanOperationsHoles(holeA, polB, Inner);
        std::vector<Edge>remove_holA_holB = Algorithms::booleanOperationsHoles(holeA, holeB, Inner);
        Algorithms::mergeVectors(remove_holA_polB, remove_holA_holB, remove);

    }

    //Difference B - A
    else if(ui -> comboBox -> currentIndex() == 3)
    {
        //Polygon A and hole B
        std::vector<Edge>res_holeB1 = Algorithms::booleanOperationsHoles(polB, holeA, Inner);
        std::vector<Edge>res_holeB2 = Algorithms::booleanOperationsHoles(holeA, polB, Inner);
        Algorithms::mergeVectors(res_holeB1, res_holeB2, res_hole);

        //Polygon B and hole A
        std::vector<Edge>res_holeA1 = Algorithms::booleanOperationsHoles(polA, holeB, Outer);

        //Hole A and hole B
        std::vector<Edge>res_holeB3 = Algorithms::booleanOperationsHoles(holeA, holeB, Inner);

        //Merge single cases
        Algorithms::mergeVectors(res_holeA1, res_holeB3, res_hole);

        //Remove
        std::vector<Edge>remove_holB_polA = Algorithms::booleanOperationsHoles(holeB, polA, Inner);
        std::vector<Edge>remove_holB_holA = Algorithms::booleanOperationsHoles(holeB, holeA, Inner);
        Algorithms::mergeVectors(remove_holB_polA, remove_holB_holA, remove);

    }

    //Merge basic select(res) and updated for holes(res_holes)
    std::vector<Edge> res_com;
    Algorithms::mergeVectors(res_hole, res, res_com);

    //Set results and update
    ui -> Canvas -> setRes(res_com);
    ui -> Canvas -> setRemoveEdges(remove);
    repaint();
}

void Widget::on_pushButton_clearResult_clicked()
{
    ui -> Canvas -> clearResults();
    repaint();
}

void Widget::on_pushButton_clearAll_clicked()
{
    ui -> Canvas -> clearAll();
    repaint();
}

void Widget::on_pushButton_removeLast_clicked()
{
    ui -> Canvas -> removeLast();
}

void Widget::on_pushButton_import_clicked()
{
    //Clear window
    ui -> Canvas -> clearResults();

    //Get current directory
    QDir current_path = QDir::currentPath();
    current_path.cdUp();
    QString path = current_path.path();

    //Select text file with polygons coordinates
    QString source_file = QFileDialog::getOpenFileName(this, "Select text file with polygon/hole coordinates", path, "Text file (*.txt)");

    //Convert path from QString to string
    std::string source_file_std = source_file.toStdString();

    //Load measured points
    std::vector<QPointFB> points;
    Algorithms::importMeasurement(source_file_std, points);

    //Test if file was opened correctly size > 0
    if (points.empty())
        QMessageBox::warning(this, "Import error", "File can NOT be opened or read correctly");

    else
    {
        //Set imported points
        //Get which polygon/hole insert points to
        int into = ui -> comboBox_import -> currentIndex();

        //Decide which polygon
        if(into == 0)
        {
            ui -> Canvas -> clearA();
            ui -> Canvas -> setA(points);
        }

        else if(into == 1)
        {
            ui -> Canvas -> clearB();
            ui -> Canvas -> setB(points);
        }

        else if(into == 2)
        {
            ui -> Canvas -> clearInA();
            ui -> Canvas -> setInA(points);
        }

        else if(into == 3)
        {
            ui -> Canvas -> clearInB();
            ui -> Canvas -> setInB(points);
        }
    }

}
