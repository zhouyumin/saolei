#include "widget.h"
#include <QMenuBar>
#include <QPushButton>
#include <QMessageBox>
#include <QMouseEvent>
#include <QDebug>
#include <QFile>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowTitle("扫雷Qt版");
    this->COL=18;
    this->ROW=14;
    this->SIZE=50;
    this->cnt=0;
    this->duration=0;
    this->NUM=40;
    this->leftNum=NUM;
    this->Border=25;
    this->map=nullptr;
    this->labels=nullptr;
    bar=new QMenuBar(this);
    hard=new QMenu("难度二",bar);
    bar->addMenu(hard);
    hard->addAction("难度一");
    hard->addAction("难度二");
    hard->addAction("难度三");
    connect(hard,SIGNAL(triggered(QAction *)),SLOT(trigerMenu(QAction*)));
    this->timer=new QTimer;
    this->timeLabel=new QLabel(this);
    timeLabel->resize(100,30);
    connect(timer,&QTimer::timeout,[=](){
        timeLabel->setNum(++duration);
    });
    this->leftLabel=new QLabel(this);
    this->leftLabel->resize(100,30);
    gameInit();
}
void Widget::trigerMenu(QAction *act)
{
    if(act->text()==hard->title())
        return;
    timer->stop();
    if (act->text()=="难度一")
    {
        this->ROW=8;
        this->COL=10;
        this->NUM=10;
        this->SIZE=65;
    }
    else if(act->text()=="难度二")
    {
        this->ROW=14;
        this->COL=18;
        this->NUM=40;
        this->SIZE=50;
    }
    else if(act->text()=="难度三")
    {
        this->ROW=20;
        this->COL=24;
        this->NUM=99;
        this->SIZE=40;
        //this->bar.
    }
    hard->setTitle(act->text());
    gameInit();
}
void Widget::loadRes()
{
    for(int i=0;i<13;++i)
    {
        this->img[i]=QPixmap(":/res/"+QString::number(i)+".jpg");
        this->img[i]=this->img[i].scaled(this->SIZE,this->SIZE);
    }
    startSound=new QSound(":/res/start.wav",this);
    lClickSound=new QSound(":/res/click.wav",this);
    rClickSound=new QSound(":/res/rightClick.wav",this);
    searchSound=new QSound(":/res/search.wav",this);
}

void Widget::gameInit()
{
    loadRes();
    leftNum=NUM;
    startSound->play();
    timeLabel->setNum(duration=0);
    this->setFixedSize(this->COL*this->SIZE,this->ROW*this->SIZE+Border);
    timeLabel->move(COL*SIZE-100,0);
    leftLabel->move(COL*SIZE/2,0);
    leftLabel->setNum(leftNum);
    this->cnt=0;
    if(this->map!=nullptr)
    {
        delete[] this->map;
        this->map=nullptr;        
    }
    this->map=new int[ROW*COL];
    if(this->labels!=nullptr)
    {
        delete[] this->labels;
        labels=nullptr;
    }
    this->labels=new QLabel[ROW*COL];
    //初始化label
    for (int i = 0; i < ROW; ++i)
        for (int j = 0; j < COL; ++j)
        {
            labels[i*COL+j].resize(SIZE,SIZE);
            labels[i*COL+j].move(j*this->SIZE,i*this->SIZE+Border);
            labels[i*COL+j].setParent(this);

            if((i+j)%2==0)
            {
                labels[i*COL+j].setStyleSheet("QLabel{background-color:rgb(0,200,200);}"
                                                            "QLabel:hover{background-color:rgb(0,200,250)}");
            }
            else
            {
                labels[i*COL+j].setStyleSheet("QLabel{background-color:rgb(0,225,220)}"
                                               "QLabel:hover{background-color:rgb(0,200,250)}");
            }

            labels[i*COL+j].show();
        }
    //数组初始化为零 为了再玩一局
   for (int i = 0; i < ROW; ++i)
       for (int j = 0; j < COL; ++j)
       {
           map[i*COL+j] = 0;
       }

   //-1 表示雷
   //布雷
   int r, c;
   srand((unsigned int)time(NULL));
   for (int i = 0; i < NUM;)
   {
       r = rand() % ROW;
       c = rand() % COL;
       if (map[r*COL+c] == 0)
       {
           map[r*COL+c] = -1;
           ++i;
       }
   }
   //根据雷的分布填充其他元素
   for (int i = 0; i < ROW; ++i)
       for (int j = 0; j < COL; ++j)
       {
           if (map[i*COL+j] != -1) //不是雷的元素
           {
               for (int m = i - 1; m <= i + 1; ++m)
               {
                   for (int n = j - 1; n <= j + 1; ++n)
                   {
                       if (m >= 0 && n >= 0&&m<ROW&&n<COL && map[m*COL+n] == -1)//先进行越界判断
                           ++map[i*COL+j];
                   }
               }
           }
       }

    for (int i = 0; i < ROW; ++i)
       for (int j = 0; j < COL; ++j)
       {
           map[i*COL+j] += 20;
       }//玩游戏时鼠标点一下-20
}

void Widget::openZero(int r,int c)
{
    map[r*COL+c] -= 20;
    ++cnt;
    labels[r*COL+c].setPixmap(img[map[r*COL+c]]);
//    if((r+c)%2)
//    {
//        labels[r*COL+c].setPixmap(img[0]);
//    }
//    else
//    {
//        labels[r*COL+c].setPixmap(img[map[12]]);
//    }

    //labels[r*COL+c].setFrameShape(QFrame::Box);
    for (int i = r - 1; i <= r + 1; ++i)
    {
        for (int j = c - 1; j <= c + 1; ++j)
        {
            if (i >= 0 && j >= 0 && i < ROW && j < COL)//越界判断
            {
                if (map[i*COL+j] > 19 && map[i*COL+j] <= 28)
                {
                    if (map[i*COL+j] != 20)
                    {
                        map[i*COL+j] -= 20;
                        ++cnt;
                        if (map[i*COL+j] >= 0 && map[i*COL+j] <= 8)//数字
                        {
                             labels[i*COL+j].setPixmap(img[map[i*COL+j]]);
                        }
                    }
                    else
                        this->openZero(i, j);
                }
            }
        }
    }
}
void Widget::mousePressEvent(QMouseEvent *event)
{
    int r,c;
    if(event->y()<=Border)return;
    if(duration==0)timer->start(1000);
    if(event->button()==Qt::LeftButton)
    {
        r=(event->y()-Border)/this->SIZE;
        c=event->x()/this->SIZE;
        if (map[r*COL+c] >= 19 && map[r*COL+c] <= 28)//是加密的格子
        {
            if (map[r*COL+c] == 20)
            {
                openZero(r, c);
                searchSound->play();
            }
            else
            {
                if (-1 == (map[r*COL+c] -= 20))
                {
                    if(map[r*COL+c]==-1)//雷
                    {
                        labels[r*COL+c].setPixmap(img[9]);
                    }
                    gameOver();
                    return;
                }
                else if (map[r*COL+c] >= 0 && map[r*COL+c] <= 8)//数字
                {
                    labels[r*COL+c].setPixmap(img[map[r*COL+c]]);
                }
                ++cnt;
                lClickSound->play();
            }
        }
    }
    else if(event->button()==Qt::RightButton)
    {
        r=(event->y()-Border)/this->SIZE;
        c=event->x()/this->SIZE;
        if (map[r*COL+c] >= 19 && map[r*COL+c] <= 28)//是加密的格子
        {
            map[r*COL+c] += 100;//添加标记
            labels[r*COL+c].setPixmap(img[11]);
            leftNum--;
            leftLabel->setNum(leftNum);
            rClickSound->play();
        }
        else if (map[r*COL+c] > 30)
        {
            map[r*COL+c] -= 100;//取消标记
            labels[r*COL+c].clear();
            leftNum++;
            leftLabel->setNum(leftNum);
            rClickSound->play();
        }
    }
    if ((cnt + NUM) == ROW * COL)
    {
       gameVictory();
    }
}

void Widget::gameOver()
{
    timer->stop();
    QMessageBox *msg=new QMessageBox(this);
    msg->setWindowTitle("失败");
    msg->setText("游戏失败，是否重新开始");
    QAbstractButton *Yes=msg->addButton("Yes",QMessageBox::YesRole);
    msg->addButton("No",QMessageBox::NoRole);
    msg->exec();
    if(msg->clickedButton()==Yes)
    {
        gameInit();
    }
    else
    {
        this->close();
    }
}

void Widget::gameVictory()
{
    timer->stop();
    QMessageBox *msg=new QMessageBox(this);
    msg->setWindowTitle("胜利");
    msg->setText("游戏胜利，用时"+QString::number(duration)+"秒");
    QAbstractButton *Yes=msg->addButton("重新开始",QMessageBox::YesRole);
    msg->addButton("退出",QMessageBox::NoRole);
    msg->exec();
    if(msg->clickedButton()==Yes)
    {
        gameInit();
    }
    else
    {
        this->close();
    }
}
Widget::~Widget()
{

}
