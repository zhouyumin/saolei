#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QMenuBar>
#include <QTimer>
#include <QMouseEvent>
#include <QSound>

class Widget : public QWidget
{
    Q_OBJECT
private:
    int COL;
    int ROW;
    int NUM;
    int SIZE;
    int cnt;
    int *map;
    int Border;
    QLabel *labels;
    QPixmap img[13];
    QMenuBar *bar;
    QMenu *hard;
    QTimer *timer;
    QLabel *timeLabel;
    QLabel *leftLabel;
    int leftNum;
    int duration;
    QSound *startSound;
    QSound *lClickSound;
    QSound *rClickSound;
    QSound *searchSound;
public:
    void gameInit();   
    void openZero(int r,int c);
    void loadRes();
    Widget(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *event);
    void gameOver();
    void gameVictory();
    ~Widget();
public slots:
    void trigerMenu(QAction* act);
};

#endif // WIDGET_H
