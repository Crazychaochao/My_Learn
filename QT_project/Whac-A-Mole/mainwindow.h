#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <myscene.h>
#include <QCloseEvent>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent * event);
    int score;

private:
    Ui::MainWindow *ui;
    myscene * sc;

private slots:
    void updateScore();
    void resetScore();
};

#endif // MAINWINDOW_H
