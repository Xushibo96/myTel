#ifndef DIALOGLIST_H
#define DIALOGLIST_H

#include <QWidget>

namespace Ui {
class DialogList;
}

class DialogList : public QWidget
{
    Q_OBJECT

public:
    explicit DialogList(QWidget *parent = nullptr);
    ~DialogList();
    // 容器保存这九个人是不是打开了
    QVector<bool> vIsShow;

private:
    Ui::DialogList *ui;
};

#endif // DIALOGLIST_H
