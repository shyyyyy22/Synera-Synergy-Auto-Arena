#ifndef HEROES_H
#define HEROES_H

#include"Unit.h"
#include<QDebug>

class Sidon:public Unit{
public:
    Sidon(const QString& name,Owner owner,qreal scale,bool isShopHero=false);
    void castSkill(Board& board,const std::vector<Unit*> allUnits)override;
    void takeDamage(int atk)override;
    void updateUnit(Board& board,const std::vector<Unit*> allUnits)override;
private:
    int m_skillTime;
};
class Luna:public Unit{
public:
    Luna(const QString& name,Owner owner,qreal scale,bool isShopHero=false);
    void castSkill(Board& board,const std::vector<Unit*> allUnits)override;
};
class Noah:public Unit{
public:
    Noah(const QString& name,Owner owner,qreal scale,bool isShopHero=false);
    void castSkill(Board& board,const std::vector<Unit*> allUnits)override;
    void updateUnit(Board& board,const std::vector<Unit*> allUnits)override;
private:
    int m_skillTime;
    bool m_inSkill;
    Unit* m_debuffUnit;
    int m_originAtk;
};
class WaterblightGanon:public Unit{
public:
    WaterblightGanon(const QString& name);
    void castSkill(Board& board,const std::vector<Unit*> allUnits)override;
    void takeDamage(int atk)override;

private:
    bool m_hasPhase2;
};

#endif // HEROES_H
