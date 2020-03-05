#ifndef MYSCENE_H
#define MYSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <myitem.h>
#include <QTimer>


class myscene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit myscene(QObject *parent = nullptr);

signals:

public slots:
    void showMouse();
    void startGame();
    void pauseGame();
    void stopGame();
    void InitGame();
    void resetscore();

private:
    myItem * item[16];
    QTimer * ptimer;

signals:
    void rescore();
};

#endif // MYSCENE_H
