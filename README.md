# myTel
A very rudimentary peer-to-peer communication software
# QT练习 实现自己的QQ
正好linux上没有聊天软件，微信要装wine，telegram还需要翻墙，这个可以用来作为两个人之间的通信软件。（强行找到用途）
# 需要修改:
`widget.cpp`中的273行（末尾）的ip地址做为要发送的地址。  
修改为255.255.255.255 可以在本地进行测试，也可以做为局域网群聊
```c++
udpSocket->writeDatagram(array,QHostAddress("10.30.3.165"),port);
```
# 截图：
主界面
![main](https://github.com/Xushibo96/myTel/blob/main/pic/mainmenu.png)
聊天界面
![chat](https://github.com/Xushibo96/myTel/blob/main/pic/chat.png)
