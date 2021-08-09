#include "dialoglist.h"
#include "ui_dialoglist.h"
#include <widget.h>
#include <QToolButton>
#include <QMessageBox>
DialogList::DialogList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DialogList)
{
    ui->setupUi(this);

    // 设置标题
   setWindowTitle("QQ1996");
   setWindowIcon(QPixmap(":/head/demo/QQ.jpeg"));

   QList<QString>nameList;
   nameList << "小列害" << "小"<< "小列"<< "小害"<< "列害"<< "害"<< "列"<< "阿巴吧"<< "啦啦啦"<< "崩大大";

   // 容器保存住九个按钮 方便以后操作
   QVector<QToolButton *> vToolbtn;

   for (int i = 0; i < 9; i++) {
       // 设置头像
       QToolButton  * btn = new QToolButton;
       btn->setText(nameList[i]);

       btn->setIcon(QPixmap(":/head/demo/28b5e2c0b2f7ea69c26fb45fb34e86b6.jpeg"));
       btn->setIconSize(QPixmap(":/head/demo/28b5e2c0b2f7ea69c26fb45fb34e86b6.jpeg").size());
       //设置按钮风格
       btn->setAutoRaise(true);
       btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
       // 加到垂直布局中
       ui->vlayout->addWidget(btn);
       vToolbtn.push_back(btn);
       vIsShow.push_back(false);
   }

   // 对九个按钮进行添加信号槽
   for (int i = 0; i < vToolbtn.size(); i++) {

       connect(vToolbtn[i], &QToolButton::clicked,[=]() {
          // 弹出聊天对话框
           //构造聊天窗口时，告诉窗口名字
           // 参数1 顶层方式弹出 参数2 窗口名字
           // 注意！ widget 构造函数并没有这两个参数 要加上
           if(vIsShow[i] == true)
           {
               QString msg = QString("%1的窗口已经打开了").arg(vToolbtn[i]->text());
                QMessageBox::warning(this,"警告",msg);
                return ;
           }

           vIsShow[i] = true;

           Widget * widget = new Widget(0,vToolbtn[i]->text());
           widget->setWindowTitle(vToolbtn[i]->text());
           widget->setWindowIcon(vToolbtn[i]->icon());
           widget->show();

           connect(widget,&Widget::closeWidget,[=](){
               vIsShow[i] = false;
           });

       });



   }

}

DialogList::~DialogList()
{
    delete ui;
}
