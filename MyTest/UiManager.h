#ifndef UIMANAGER_H
#define UIMANAGER_H
#include<QObject>
#include<QDebug>
// --- UiManager.h ---
class UiManager : public QObject {
    Q_OBJECT
public slots: // 槽函数，就是普通的成员函数，用来接收信号
    void updateHpBar(int currentHp) {
        qDebug() << "收到信号！UI血条立刻变短，当前血量：" << currentHp;
    }
};

#endif // UIMANAGER_H
