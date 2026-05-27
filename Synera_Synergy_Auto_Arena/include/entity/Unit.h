#ifndef UNIT_H
#define UNIT_H
#include<QObject>
#include<QString>
#include<QPoint>
class Board;

enum class Owner{PlayerCtrl,EnemyCtrl};
enum class Race{Hyrulean,Zora,Rito,Gerudo,Goron};
enum class Profession{Warrior,Archer,Mage,Guardian,Assassin};
enum class State{Idle,Moving,Attacking,Casting,Dead};

class Unit:public QObject
{
    Q_OBJECT
public:
    Unit(const QString &name,int maxHp,int atk,int range,int maxMana,Owner owner,Profession profession,QObject *parent=nullptr);
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
    State getState()const;
    QPointF getWorldPos(const QPoint& gridPos)const;
    QPoint getStartPos()const;


    void setHp(int newHp);
    void setAtk(int newAtk);
    void setRange(int newRange);
    void setMana(int newMana);
    void setPos(QPoint newPos);
    void setStartPos(QPoint pos);
    void setState(State state);

    //状态机
    void updateUnit(Board& board,const std::vector<Unit*> allUnits);
    void handleIdle(Board& board,const std::vector<Unit*> allUnits);
    void handleMoving(Board& board);
    void handleAttking();
    void handleCasting();

    //寻路
    std::vector<QPoint> breadFirstSearch(Board& board);

    //技能虚函数
    virtual void castSkill()=0;

signals:
    void isDead(Unit* unit);
    void infoChanged(Unit* unit);

private:
    void takeDamage(int atk);

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
    State m_state;
    Unit* m_target;
    QPoint m_startPos;

    int m_moveCoolDown;
    int m_atkCoolDown;

    Profession m_profession;
    Race m_race;
};

#endif // UNIT_H
