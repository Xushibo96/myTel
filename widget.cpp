#include "widget.h"
#include "ui_widget.h"
#include <QDataStream>
#include <QMessageBox>
#include <QDateTime>
#include <QColorDialog>
#include <QFileDialog>
#include <QFile>
Widget::Widget(QWidget *parent, QString name)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    // 初始化操作
    udpSocket = new QUdpSocket(this);
    //用户名获取;
    uName = name;
    // 端口号
    this->port = 8888;
    // 绑定端口号 绑定模式： 共享地址|断线重连
    udpSocket->bind(this->port,QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    // 发送新用户进入：
    sndMsg(UsrEnter);

    // 绑定按钮
    connect(ui->sendBtn, &QPushButton::clicked,[=](){
        sndMsg(Msg);
    });

    // 监听别人发的数据
    connect(udpSocket,&QUdpSocket::readyRead,this,&Widget::ReceiveMessage);

    // 点击按钮关闭窗口
    connect(ui->exitBtn,&QPushButton::clicked,[=](){
       this->close();
    });

    ///////////// 辅助功能
    //字体
    connect(ui->fontCBX,&QFontComboBox::currentFontChanged,[=](const QFont &font){
        ui->msgEdit->setCurrentFont(font);
        ui->msgEdit->setFocus();
    });

    //字号

    // 这个信号有重载，所以用一个函数指针指向他
    void(QComboBox::*cbxsingal)(const QString &text) = &QComboBox::currentTextChanged;

    connect(ui->sizecomboBox,cbxsingal,[=](const QString &text){
        ui->msgEdit->setFontPointSize(text.toDouble());
        ui->msgBroweser->setFocus();
    });

    // 加粗
    connect(ui->boldBtn,&QToolButton::clicked,[=](bool isclicked){
        if(isclicked)
        {
            ui->msgEdit->setFontWeight(QFont::Bold);

        }else
        {
            ui->msgEdit->setFontWeight(QFont::Normal);
        }
    });

    // 倾斜

    connect(ui->italiscBtn,&QToolButton::clicked,[=](bool isclicked){

        ui->msgEdit->setFontItalic(isclicked);
    });

    // 下划线
    connect(ui->underlineBtn,&QToolButton::clicked,[=](bool isclicked){

        ui->msgEdit->setFontUnderline(isclicked);
    });

    // 字体颜色
    connect(ui->colorBtn,&QToolButton::clicked,[=](){
        QColor color = QColorDialog::getColor(Qt::red);
        ui->msgEdit->setTextColor(color);
    });

    // 清空聊天记录
    connect(ui->clearBtn,&QToolButton::clicked,[=](){
        ui->msgBroweser->clear();
    });

    // 保存聊天记录
    connect(ui->SaveBtn,&QToolButton::clicked,[=]{
        QString path = QFileDialog::getSaveFileName(this,"保存记录","聊天记录","(*.txt)");
        if(path.isEmpty() || ui->msgBroweser->document()->isEmpty())
        {
            QMessageBox::warning(this,"路径为空","路径不能为空");
            if(ui->msgBroweser->document()->isEmpty())
            {
                QMessageBox::warning(this,"内容为空","内容不能为空");
            }
            return;
        }else
        {
            QFile file(path);
            // 打开模式加换行操作
            file.open(QIODevice::WriteOnly | QIODevice::Text);
            QTextStream stream(&file);
            stream  << ui->msgBroweser->toPlainText();
            file.close();
        }

    });
}



Widget::~Widget()
{
    delete ui;
}

// 关闭事件
void Widget::closeEvent(QCloseEvent *e)
{
    emit this->closeWidget();
    sndMsg(UsrLeft);
    // 断开套接字
    udpSocket->close();
    udpSocket->destroyed();

    QWidget::closeEvent(e);

}

void Widget::usrLeft(QString usrname, QString time)// 处理用户离开
{
    //更新右侧tableWidget
    bool isEmpty = ui->userTableWidget->findItems(usrname,Qt::MatchExactly).isEmpty();
    if(!isEmpty)
    {
        int row = ui->userTableWidget->findItems(usrname,Qt::MatchExactly).first()->row();
        ui->userTableWidget->removeRow(row);

        // 追加聊天记录
        ui->msgBroweser->setTextColor(Qt::gray);
        ui->msgBroweser->append(QString("%1于%2离开").arg(usrname).arg(time));
        // 更新在线人数
        ui->numlabel->setText(QString("在线用户：%1人").arg(ui->userTableWidget->rowCount()));
    }
}

void Widget::usrEnter(QString username) // 处理新用户进入
{
    bool isempty = ui->userTableWidget->findItems(username,Qt::MatchExactly).isEmpty();
    if(isempty)
    {
        QTableWidgetItem *usr = new QTableWidgetItem(username);
        //插入一个行
        ui->userTableWidget->insertRow(0);
        ui->userTableWidget->setItem(0,0,usr);
        // 追加聊天记录
        ui->msgBroweser->setTextColor(Qt::gray);
        ui->msgBroweser->append(QString("%1在线").arg(username));
        //在线人数更新

        ui->numlabel->setText(QString("在线人数： %1人").arg(ui->userTableWidget->rowCount()));
//            把自身信息广播出去
        sndMsg(UsrEnter);
    }
}

// 接收端：
void Widget::ReceiveMessage()
{
//    拿到数据报文
    qint64 size = udpSocket->pendingDatagramSize();
    QByteArray array = QByteArray(size, 0);
    udpSocket->readDatagram(array.data(),size);
//    解析数据
    // 第一段是类型 第二段是用户名，第三端是内容
    QDataStream stream(&array, QIODevice::ReadOnly);
    int msgType;
    QString usrName;
    QString msg;

    // 获取当前时间：
    QString time =  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    stream >> msgType;
    switch (msgType) {
    case Msg:
        stream >> usrName >> msg;
        // 追加聊天记录

        ui->msgBroweser->setTextColor(Qt::blue);
        ui->msgBroweser->append("{"+ usrName+"}" + time);
        ui->msgBroweser->append(msg);
        break;
    case UsrEnter:
        // 更新TableWidget:
        stream >> usrName;
        usrEnter(usrName);
        break;
    case UsrLeft:
        stream >> usrName;

        usrLeft(usrName,time);
        break;

    }
}


QString Widget::getUsr()// 获取用户名
{
    return this->uName;
}

QString Widget::getMsg()// 获取聊天信息
{
    QString str = ui->msgEdit->toHtml();
    ui->msgEdit->clear();
    ui->msgEdit->setFocus();
    return str;
}

void Widget::sndMsg(MsgType type)
{
    //发送的消息分三类
    //发送的数据做分段处理
    // 第一段类型 第二段用户名 第三段具体内容
    QByteArray array;
    QDataStream stream(&array,QIODevice::ReadWrite);
    stream << type << getUsr(); //第一 二段内容添加到流中
    int msgType;
    QString usrName;
    QString msg;
    QString time =  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    switch (type) {

    case Msg:// 普通消息发送

        if(ui->msgEdit->toPlainText() == "")// 用户没有输入就不发送消息
        {
            QMessageBox::warning(this,"警告","无法发送空消息");
            return ;
        }
        // 第三段，具体说的话
        msg = getMsg();
        stream << msg;

        /*能够看到自己说的话*/

        // 追加聊天记录

        ui->msgBroweser->setTextColor(Qt::blue);
        ui->msgBroweser->append("{"+this->uName +"}" + time);
        ui->msgBroweser->append(msg);
        break;

    case UsrEnter:// 新用户进入

        break;
    case UsrLeft:// 用户离开

        break;
    default:
        break;

    }

    // 书写报文 //广播发送 地址为255.255.255.255
    udpSocket->writeDatagram(array,QHostAddress("10.30.3.165"),port);
//    udpSocket->writeDatagram(array,QHostAddress("10.30.12.248"),port);

}
