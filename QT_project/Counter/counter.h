#ifndef COUNTER_H
#define COUNTER_H

#include <QDialog>
#include "model.h"
#include <QString>

namespace Ui {
class Counter;
}

class Counter : public QDialog
{
    Q_OBJECT

public:
    explicit Counter(QWidget *parent = 0);
    ~Counter();
private slots:
    void getBtn0();
    void getBtn1();
    void getBtn2();

    void on_btn_3_clicked();

    void on_btn_4_clicked();

    void on_btn_5_clicked();

    void on_btn_6_clicked();

    void on_btn_7_clicked();

    void on_btn_8_clicked();

    void on_btn_9_clicked();

    void on_btn_plus_clicked();

    void on_btn_sub_2_clicked();

    void on_btn_mul_clicked();

    void on_btn_div_clicked();

    void on_btn_eq_clicked();

    void on_btn_del_clicked();

private:
    QString tmp;
    Ui::Counter *ui;
    model * mode;
};

#endif // COUNTER_H
