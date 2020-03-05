#include "myscene.h"
#include <stdlib.h>

myscene::myscene(QObject *parent) : QGraphicsScene(parent)
{
   int i = 0;
   for(i=0;i<16;i++)
   {
       this->item[i] = new myItem;
       this->item[i]->setPos(i/4 * this->item[i]->boundingRect().width(),i % 4 * this->item[i]->boundingRect().height());
       this->addItem(this->item[i]);
   }
   this->ptimer = new QTimer;
   connect(this->ptimer,SIGNAL(timeout()),this,SLOT(showMouse()));
}

void myscene::showMouse()
{
   //生成随机数
   int count = rand()%3 + 1;
   int i = 0;
   for(i=0;i<16;i++)
   {
       this->item[i]->setPic(":/background/picture/bg1.png");
       this->item[i]->setMouse(false);
   }
   for(i=0;i<count;i++)
   {
       int index = rand()%16;
       this->item[index]->setPic(":/mouse/picture/beatmouse.png");
       this->item[index]->setMouse(true);
   }
}

void myscene::startGame()
{
    int i = 0;
    for(i=0;i<16;i++)
    {
        this->item[i]->setstart(true);
    }
    this->ptimer->start(800);

}

void myscene::pauseGame()
{
    int i = 0;
    for(i=0;i<16;i++)
    {
        this->item[i]->setstart(false);
    }
    this->ptimer->stop();
}

void myscene::stopGame()
{
    int i = 0;
    for(i=0;i<16;i++)
    {
        this->item[i]->setstart(false);
    }
    this->ptimer->stop();
    InitGame();
    resetscore();
}

void myscene::InitGame()
{
    int i = 0;
    for(i=0;i<16;i++)
    {
        this->item[i]->setPic(":/background/picture/bg1.png");
        this->item[i]->setMouse(false);
    }   
}

void myscene::resetscore()
{
    emit rescore();
}
