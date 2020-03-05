#include "myitem.h"
#include <QPixmap>
#include <QDebug>
#include "handler.h"
#include <QCursor>

myItem::myItem()
{
    this->setPixmap(QPixmap(":/background/picture/bg1.png"));
    this->start = false;
    this->mouse = false;
    this->setCursor(QCursor(QPixmap(":/mouse/picture/pictureUp.png")));
}

void myItem::setPic(QString path)
{
  this->setPixmap(QPixmap(path));
}

void myItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
     this->setCursor(QCursor(QPixmap(":/mouse/picture/pictureUp.png")));
}

void myItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    this->setCursor(QCursor(QPixmap(":/mouse/picture/picturedown.png")));
    if(this->isstart()){
        handler * hand = handler::getInstance();
        if(this->isMouse()){
           this->setPixmap(QPixmap(":/mouse/picture/was_hit.png"));
           hand->addScore();
           this->mouse = false;
        }
        else{
            qDebug()<<"不是老鼠";
        }
    }
}
void myItem::setMouse(bool mouse)
{
    this->mouse = mouse;
}

bool myItem::isMouse()
{
    return this->mouse;
}

void myItem::setstart(bool start)
{
    this->start = start;
}

bool myItem::isstart()
{
    return this->start;
}


