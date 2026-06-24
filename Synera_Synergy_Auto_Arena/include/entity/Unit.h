#ifndef UNIT_H
#define UNIT_H
#include<QObject>
#include<QString>
#include<QPoint>
#include"EquipmentItem.h"
#include<vector>
class Board;

enum class Owner{PlayerCtrl,EnemyCtrl};
enum class Race{Hyrulean,Zora,Rito,Gerudo,Goron,Boss};
enum class Profession{Warrior,Archer,Mage,Guardian,Assassin,None};
enum class State{Idle,Moving,Attacking,Casting,Dead};

class Unit:public QObject
{
    Q_OBJECT
public:
    Unit(const QString &name,int maxHp,int atk,int range,int maxMana,Owner owner,Profession profession,int star=1,bool isShopHero=false,QObject *parent=nullptr);
    virtual ~Unit()=default;

    //属性获取
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
    Unit* getTarget()const;
    int getMoveCoolDown()const;
    int getAtkCoolDown()const;
    bool getIsShopHero()const;
    Profession getProfession()const;
    Race getRace()const;
    QString getRaceName()const;
    QString getProName()const;
    Equipment getEquipment()const;
    int getOriATkCoolDown()const;
    virtual QString getSkillDes()const=0;
    virtual QString getClassName()const=0;

    //属性修改
    void setHp(int newHp);
    void setAtk(int newAtk);
    void setRange(int newRange);
    void setMana(int newMana);
    void setPos(QPoint newPos);
    void setStartPos(QPoint pos);
    void setState(State state);
    virtual void setMoveCoolDown(int newCoolDown);
    virtual void setAtkCoolDown(int newCoolDown);
    void setOriAtkCoolDown(int newCooldown);
    void setMaxMana(int newMaxMana);
    void setMaxHp(int newMaxHp);
    void setIsShop(bool isShop);
    void setRace(Race race);
    void setTarget(Unit* newTarget);
    void setEquipment(Equipment type);
    void resetWithStar();
    void restoreOriAtt();

    //状态与战斗
    virtual void updateUnit(Board& board,const std::vector<Unit*> allUnits);
    virtual void castSkill(Board& board,const std::vector<Unit*> allUnits)=0;
    virtual void takeDamage(int atk);

    //羁绊
    bool m_warriorSyn,m_archerSyn,m_mageSyn,m_assassinSyn,m_guardianSyn;

    //装备
    void addEquipment(Equipment type);
signals:
    void isDead(Unit* unit);
    void infoChanged(Unit* unit);
    void damaged(int dmg);

private:
    //状态处理
    void handleIdle(Board& board,const std::vector<Unit*> allUnits);
    void handleMoving(Board& board);
    void handleAttking();
    void handleCasting(Board& board,const std::vector<Unit*> allUnits);

    //寻路
    std::vector<QPoint> breadFirstSearch(Board& board);

    static int m_nxtUnitId;

    //基本属性
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
    bool m_isShopHero;
    Profession m_profession;
    Race m_race;
    int m_moveCoolDown;
    int m_atkCoolDown;
    Equipment m_equipment;

    //用于逻辑控制
    int m_oriMaxHp;
    int m_oriAtk;
    int m_oriRange;
    int m_oriMaxMana;
    int m_oriMoveCoolDown;
    int m_oriAtkCoolDown;
    int m_atkCount;

    //技能描述
    QString m_skillDes;
};

#endif // UNIT_H
