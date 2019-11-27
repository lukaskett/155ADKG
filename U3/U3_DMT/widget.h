#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "qpoint3d.h"
#include "algorithms.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
private:
    double z_min, z_max, dz;

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_createDT_clicked();

    void on_pushButton_createContours_clicked();

    void on_lineEdit_zmin_editingFinished();

    void on_lineEdit_zmax_editingFinished();

    void on_lineEdit_dz_editingFinished();

    void on_pushButton_importMeasurement_clicked();

    void on_pushButton_generateShape_clicked();

    void on_pushButton_clearSelected_clicked();

    void on_pushButton_clearAll_clicked();

    void on_pushButton_analyze_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
