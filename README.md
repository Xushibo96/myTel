# myTel
A very rudimentary peer-to-peer communication software
# QT练习 实现自己的QQ
正好linux上没有聊天软件，微信要装wine，telegram还需要翻墙，这个可以用来作为两个人之间的通信软件。（强行找到用途）
# 需要修改:
`widget.cpp`中的273行（末尾）的ip地址做为要发送的地址。  
```c++
udpSocket->writeDatagram(array,QHostAddress("10.30.3.165"),port);
```
