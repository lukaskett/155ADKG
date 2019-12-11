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
    ui -> Canvas -> changePolygon();
}

void Widget::on_pushButton_createOverlay_clicked()
{
    //Get polygons
    std::vector<QPointFB> polA = ui -> Canvas -> getA();
    std::vector<QPointFB> polB = ui -> Canvas -> getB();

    //Perform Boolean operation
    TBooleanOperation oper = TBooleanOperation(ui -> comboBox -> currentIndex());
    std::vector<Edge> res = Algorithms::booleanOperations(polA, polB, oper);

    //Set results and update
    ui->Canvas->setRes(res);
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
