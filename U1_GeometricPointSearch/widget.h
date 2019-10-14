#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <string>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    //void ResultofAnalyze(int res, bool &write_result); // ADDED BY ME

private slots:
    void on_clearButton_clicked();

    void on_drawModeButton_clicked();

    void on_analyzeButton_clicked();

    void on_importPolygonButton_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
