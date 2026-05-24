#ifndef PROFESSIONS_H
#define PROFESSIONS_H
#include"Unit.h"
class Warrior:public Unit{
public:
    Warrior(const QString &name,int maxHp,int atk,int maxMana,Owner owner)
        :Unit(name,maxHp,atk,1,maxMana,owner){}
};

class Archer:public Unit{
public:
    Archer(const QString &name,int maxHp,int atk,int maxMana,Owner owner)
        :Unit(name,maxHp,atk,3,maxMana,owner){}
};

class Mage:public Unit{
public:
    Mage(const QString &name,int maxHp,int atk,int maxMana,Owner owner)
        :Unit(name,maxHp,atk,3,maxMana,owner){}
};

#endif // PROFESSIONS_H
