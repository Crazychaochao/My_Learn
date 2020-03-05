#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "handler.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->sc = new myscene;
    this->ui->graphicsView->setScene(sc);
    connect(this->ui->btn_start,SIGNAL(clicked(bool)),this->sc,SLOT(startGame()));
    connect(this->ui->btn_pause,SIGNAL(clicked(bool)),this->sc,SLOT(pauseGame()));
    connect(this->ui->btn_stop,SIGNAL(clicked(bool)),this->sc,SLOT(stopGame()));
    this->score = 0;

    handler * hand = handler::getInstance();
    connect(hand,SIGNAL(beatMouse()),this,SLOT(updateScore()));
    connect(sc,SIGNAL(rescore()),this,SLOT(resetScore()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
   int ret = QMessageBox::question(this,"确定","你真的要关闭吗？","是","否");
   if(ret == 1)
   {
       event->ignore();
   }
}

void MainWindow::updateScore()
{
    this->score +=10;
    this->ui->lcdNumber->display(this->score);
}

void MainWindow::resetScore()
{
    this->ui->lcdNumber->display(0);
}
