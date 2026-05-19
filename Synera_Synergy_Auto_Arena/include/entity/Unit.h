#ifndef UNIT_H
#define UNIT_H
#include<QObject>
#include<QString>
#include<QPoint>

enum class Owner{PlayerCtrl,EnemyCtrl};
enum class Traits{};
class Unit:public QObject
{
    Q_OBJECT
public:
    Unit(const QString &name,int maxHp,int atk,int range,int maxMana,Owner owner,QObject *parent=nullptr);
    virtual ~Unit()=default;

    //属性相关
    int getId()const;
    int getHp()const;
    int getMaxHp()const;
    int getAtk()const;
    int getRange()const;
    int getMana()const;
    int getMaxMana()const;
    QString getName()const;
    QPoint getPos()const;
    Owner getOwner()const;
    int getStar()const;

    void setHp(int newHp);
    void setAtk(int newAtk);
    void setRange(int newRange);
    void setMana(int newMana);
    void setPos(QPoint newPos);

    //技能虚函数
    //virtual void skill()=0;

private:
    static int m_nxtUnitId;

    int m_id;
    int m_maxHp;
    int m_hp;
    int m_atk;
    int m_range;
    int m_maxMana;
    int m_mana;
    QString m_name;
    QPoint m_pos;
    Owner m_owner;
    int m_star;
};

#endif // UNIT_H
