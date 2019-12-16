#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"
#include "types.h"

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


void Widget::on_pushButton_switchPolygon_clicked()
{
    //Change polygon mode - draw A or draw B
    ui -> Canvas -> changePolygon();

    //true - A, false B
    bool ab = ui -> Canvas -> getPolygonStatus();

    if(!ab)
        ui -> label_drawPolygonAB -> setText(QString("Polygon B"));

    else
        ui -> label_drawPolygonAB -> setText(QString("Polygon A"));
}

void Widget::on_pushButton_changeInOut_clicked()
{
    ui -> Canvas -> changeInOut();

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
    std::vector<Edge> res_holeA;
    std::vector<Edge> res_holeB;
    if(ui -> comboBox -> currentIndex() == 0) //Union
    {
        res_holeA = Algorithms::booleanOperationsHoles(polB, holeA,  Outer, Inner);
        res_holeB = Algorithms::booleanOperationsHoles(polA, holeB, Outer, Inner);
    }
/*
    else if(ui -> comboBox -> currentIndex() == 1) //Intersect
    {
        res_holeA = Algorithms::booleanOperationsHoles(polB, holeA, Outer, Inner);
        res_holeB = Algorithms::booleanOperationsHoles(polA, holeB, Outer, Inner);
    }
*/
    //Merge selected edges
    std::vector<Edge> res_com;
    res_com.reserve( res.size() + res_holeA.size() + res_holeB.size());
    res_com.insert( res_com.end(), res.begin(), res.end());
    res_com.insert( res_com.end(), res_holeA.begin(), res_holeA.end());
    res_com.insert( res_com.end(), res_holeB.begin(), res_holeB.end());

    //Set results and update
    ui -> Canvas -> setRes(res_com);
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
