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
    QString getSkillDes()const override;
    QString getClassName()const override;
private:
    int m_skillTime;
};
class Luna:public Unit{
public:
    Luna(const QString& name,Owner owner,qreal scale,int star=1,bool isShopHero=false);
    void castSkill(Board& board,const std::vector<Unit*> allUnits)override;
    QString getSkillDes()const override;
    QString getClassName()const override;
};
class Noah:public Unit{
public:
    Noah(const QString& name,Owner owner,qreal scale,int star=1,bool isShopHero=false);
    void castSkill(Board& board,const std::vector<Unit*> allUnits)override;
    void updateUnit(Board& board,const std::vector<Unit*> allUnits)override;
    QString getSkillDes()const override;
    QString getClassName()const override;
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
    void updateUnit(Board& board,const std::vector<Unit*> allUnits)override;
    QString getSkillDes()const override;
    QString getClassName()const override;

private:
    bool m_hasPhase2;
    int m_skillTime;
    bool m_inSkill;
};

//风
class Revali:public Unit{
public:
    Revali(const QString& name,Owner owner,qreal scale,int star=1,bool isShopHero=false);
    void castSkill(Board& board,const std::vector<Unit*> allUnits)override;
    QString getSkillDes()const override;
    QString getClassName()const override;
};
class Kashi:public Unit{
public:
    Kashi(const QString& name,Owner owner,qreal scale,int star=1,bool isShopHero=false);
    void castSkill(Board& board,const std::vector<Unit*> allUnits)override;
    QString getSkillDes()const override;
    QString getClassName()const override;
};
class Evan:public Unit{
public:
    Evan(const QString& name,Owner owner,qreal scale,int star=1,bool isShopHero=false);
    void castSkill(Board& board,const std::vector<Unit*> allUnits)override;
    QString getSkillDes()const override;
    QString getClassName()const override;
};
class WindblightGanon:public Unit{
public:
    WindblightGanon(const QString& name);
    void castSkill(Board& board,const std::vector<Unit*> allUnits)override;
    void takeDamage(int atk)override;
    QString getSkillDes()const override;
    QString getClassName()const override;

private:
    bool m_hasPhase2;
};

//雷
class Ur:public Unit{
public:
    Ur(const QString& name,Owner owner,qreal scale,int star=1,bool isShopHero=false);
    void castSkill(Board& board,const std::vector<Unit*> allUnits)override;
    QString getSkillDes()const override;
    QString getClassName()const override;
};
class Naji:public Unit{
public:
    Naji(const QString& name,Owner owner,qreal scale,int star=1,bool isShopHero=false);
    void castSkill(Board& board,const std::vector<Unit*> allUnits)override;
    void updateUnit(Board& board,const std::vector<Unit*> allUnits)override;
    QString getSkillDes()const override;
    QString getClassName()const override;

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
    QString getSkillDes()const override;
    QString getClassName()const override;
};
class ThunderblightGanon:public Unit{
public:
    ThunderblightGanon(const QString& name);
    void castSkill(Board& board,const std::vector<Unit*> allUnits)override;
    void takeDamage(int atk)override;
    QString getSkillDes()const override;
    QString getClassName()const override;

private:
    bool m_hasPhase2;
};

//火
class Daruk:public Unit{
public:
    Daruk(const QString& name,Owner owner,qreal scale,int star=1,bool isShopHero=false);
    void castSkill(Board& board,const std::vector<Unit*> allUnits)override;
    void updateUnit(Board& board,const std::vector<Unit*> allUnits)override;
    QString getSkillDes()const override;
    QString getClassName()const override;
private:
    int m_skillTime;
    bool m_inSkill;
};
class Goma:public Unit{
public:
    Goma(const QString& name,Owner owner,qreal scale,int star=1,bool isShopHero=false);
    void castSkill(Board& board,const std::vector<Unit*> allUnits)override;
    QString getSkillDes()const override;
    QString getClassName()const override;
};
class Mag:public Unit{
public:
    Mag(const QString& name,Owner owner,qreal scale,int star=1,bool isShopHero=false);
    void castSkill(Board& board,const std::vector<Unit*> allUnits)override;
    void updateUnit(Board& board,const std::vector<Unit*> allUnits)override;
    QString getSkillDes()const override;
    QString getClassName()const override;
private:
    int m_skillTime;
    bool m_inSkill;
    QPoint m_burnCenter;
};
class FireblightGanon:public Unit{
public:
    FireblightGanon(const QString& name);
    void castSkill(Board& board,const std::vector<Unit*> allUnits)override;
    void takeDamage(int atk)override;
    QString getSkillDes()const override;
    QString getClassName()const override;

private:
    bool m_hasPhase2;
};

//海拉鲁
class Leo:public Unit{
public:
    Leo(const QString& name,Owner owner,qreal scale,int star=1,bool isShopHero=false);
    void castSkill(Board& board,const std::vector<Unit*> allUnits)override;
    QString getSkillDes()const override;
    QString getClassName()const override;
};
class Alan:public Unit{
public:
    Alan(const QString& name,Owner owner,qreal scale,int star=1,bool isShopHero=false);
    void castSkill(Board& board,const std::vector<Unit*> allUnits)override;
    QString getSkillDes()const override;
    QString getClassName()const override;
};
class Syndra:public Unit{
public:
    Syndra(const QString& name,Owner owner,qreal scale,int star=1,bool isShopHero=false);
    void castSkill(Board& board,const std::vector<Unit*> allUnits)override;
    QString getSkillDes()const override;
    QString getClassName()const override;
};
class Ganondorf:public Unit{
public:
    Ganondorf(const QString& name);
    void castSkill(Board& board,const std::vector<Unit*> allUnits)override;
    void takeDamage(int atk)override;
    void setMoveCoolDown(int newCoolDown)override;
    void setAtkCoolDown(int newCoolDown)override;
    void updateUnit(Board& board,const std::vector<Unit*> allUnits)override;
    QString getSkillDes()const override;
    QString getClassName()const override;

private:
    bool m_hasPhase2;
    bool m_hasPhase3;
    int m_skillTime;
};

#endif // HEROES_H
