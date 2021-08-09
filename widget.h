#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QUdpSocket>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
    enum MsgType {Msg,UsrEnter,UsrLeft};

public:
    Widget(QWidget *parent, QString name);
    ~Widget();
    // 关闭事件
    void closeEvent(QCloseEvent *);

    void sndMsg(MsgType type);// 广播udp消息
    void usrEnter(QString username); // 处理新用户进入
    void usrLeft(QString usrname, QString time);// 处理用户离开
    QString getUsr();// 获取用户名
    QString getMsg();// 获取聊天信息p
private:
    QUdpSocket *udpSocket; // udp 套接字
    qint16 port;//端口
    QString uName;//用户名

    void ReceiveMessage(); // 接收消息

private:
    Ui::Widget *ui;

signals:
    //关闭窗口发送一个关闭信号
    void closeWidget();


};
#endif // WIDGET_H
