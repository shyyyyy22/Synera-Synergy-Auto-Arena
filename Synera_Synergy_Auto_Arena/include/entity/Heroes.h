#ifndef HEROES_H
#define HEROES_H

#include"Unit.h"
#include<QDebug>

class Sidon:public Unit{
public:
    Sidon(const QString& name,Owner owner,qreal scale,int star=1,bool isShopHero=false);
    void castSkill(Board& board,const std::vector<Unit*> allUnits)override;
    void takeDamage(int atk)override;
    void updateUnit(Board& board,const std::vector<Unit*> allUnits)override;
private:
    int m_skillTime;
};
class Luna:public Unit{
public:
    Luna(const QString& name,Owner owner,qreal scale,int star=1,bool isShopHero=false);
    void castSkill(Board& board,const std::vector<Unit*> allUnits)override;
};
class Noah:public Unit{
public:
    Noah(const QString& name,Owner owner,qreal scale,int star=1,bool isShopHero=false);
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
class Revali:public Unit{
public:
    Revali(const QString& name,Owner owner,qreal scale,int star=1,bool isShopHero=false);
    void castSkill(Board& board,const std::vector<Unit*> allUnits)override;
};
class Kashi:public Unit{
public:
    Kashi(const QString& name,Owner owner,qreal scale,int star=1,bool isShopHero=false);
    void castSkill(Board& board,const std::vector<Unit*> allUnits)override;
};
class Evan:public Unit{
public:
    Evan(const QString& name,Owner owner,qreal scale,int star=1,bool isShopHero=false);
    void castSkill(Board& board,const std::vector<Unit*> allUnits)override;
};

//雷
class Ur:public Unit{
public:
    Ur(const QString& name,Owner owner,qreal scale,int star=1,bool isShopHero=false);
    void castSkill(Board& board,const std::vector<Unit*> allUnits)override;
};
class Naji:public Unit{
public:
    Naji(const QString& name,Owner owner,qreal scale,int star=1,bool isShopHero=false);
    void castSkill(Board& board,const std::vector<Unit*> allUnits)override;
    void updateUnit(Board& board,const std::vector<Unit*> allUnits)override;

private:
    bool m_inSkill;
    int m_originAtk;
    Unit* m_debuffUnit;
    int m_skillTime;
};
class Shika:public Unit{
public:
    Shika(const QString& name,Owner owner,qreal scale,int star=1,bool isShopHero=false);
    void castSkill(Board& board,const std::vector<Unit*> allUnits)override;
};

#endif // HEROES_H
