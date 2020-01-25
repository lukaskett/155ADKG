#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_switchPolygon_clicked();

    void on_pushButton_createOverlay_clicked();

    void on_pushButton_clearResult_clicked();

    void on_pushButton_clearAll_clicked();

    void on_pushButton_changeInOut_clicked();

    void on_pushButton_removeLast_clicked();

    void on_pushButton_import_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
