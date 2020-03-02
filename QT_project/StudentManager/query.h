#ifndef QUERY_H
#define QUERY_H

#include <QDialog>
#include <QFile>
#include <QList>
#include <QString>
#include <QStandardItem>
#include <QStandardItemModel>


namespace Ui {
class query;
}

class query : public QDialog
{
    Q_OBJECT

public:
    explicit query(QWidget *parent = 0);
    ~query();
    int readFromFile();
    void doQuert(int index,QString cnt);
    void display(int row,QStringList subs);

private slots:
    void on_btn_serch_clicked();

private:
    Ui::query *ui;
    QList<QString>stu_lines;
    QStandardItemModel * model;
};

#endif // QUERY_H
