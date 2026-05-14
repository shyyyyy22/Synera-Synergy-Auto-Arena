#ifndef HERO_H
#define HERO_H
#include<QObject>

// --- Hero.h ---
class Hero : public QObject {
    Q_OBJECT // 必须加这个宏，才能使用对讲机系统！

public:
    int hp = 100;
    void takeDamage(int dmg) {
        hp -= dmg;
        // 核心：用 emit 关键字发射信号！
        emit hpChanged(hp);
    }

signals:
    // 信号只需要声明，不需要写大括号去实现它！它就像个大喇叭。
    void hpChanged(int currentHp);
};
#endif