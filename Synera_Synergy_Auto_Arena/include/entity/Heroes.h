#ifndef HEROES_H
#define HEROES_H
#include"Professions.h"
#include<QDebug>

class Link:public Warrior{
public:
    Link(Owner owner);
    void castSkill()override{
        qDebug()<<getName()<<"释放技能";
        return;
    };
};
class Sun:public Archer{
public:
    Sun(Owner owner);
    void castSkill()override{
        qDebug()<<getName()<<"释放技能";
        return;
    };
};
class Power:public Mage{
public:
    Power(Owner owner);
    void castSkill()override{
        qDebug()<<getName()<<"释放技能";
        return;
    };
};

#endif // HEROES_H
