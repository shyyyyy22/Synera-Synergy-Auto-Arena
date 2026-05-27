#ifndef HEROES_H
#define HEROES_H

#include"Unit.h"
#include<QDebug>

class Sidon:public Unit{
public:
    Sidon(const QString& name,int maxHp,int atk,Owner owner);
    void castSkill()override;
};
class Luna:public Unit{
public:
    Luna(const QString& name,int maxHp,int atk,Owner owner);
    void castSkill()override;
};
class Noah:public Unit{
public:
    Noah(const QString& name,int maxHp,int atk,Owner owner);
    void castSkill()override;
};

#endif // HEROES_H
