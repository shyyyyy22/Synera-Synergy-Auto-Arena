#include "Unit.h"
#include"Board.h"
#include<QDebug>
int Unit::m_nxtUnitId=0;
Unit::Unit(const QString &name,int maxHp,int atk,int range,int maxMana,Owner owner,QObject *parent)
    :QObject(parent)
    ,m_id(m_nxtUnitId++)
    ,m_maxHp(maxHp)
    ,m_hp(maxHp)
    ,m_atk(atk)
    ,m_range(range)
    ,m_maxMana(maxMana)
    ,m_mana(0)
    ,m_name(name)
    ,m_pos(-1,-1)
    ,m_owner(owner)
    ,m_star(1)
    ,m_state(State::Idle)
    ,m_target(nullptr)
{}

//属性相关
int Unit::getId()const{
    return m_id;
}
int Unit::getHp()const{
    return m_hp;
}

int Unit::getMaxHp() const
{
    return m_maxHp;
}
int Unit::getAtk()const{
    return m_atk;
}
int Unit::getRange()const{
    return m_range;
}
int Unit::getMana()const{
    return m_mana;
}

int Unit::getMaxMana() const
{
    return m_maxMana;
}
QString Unit::getName()const{
    return m_name;
}
QPoint Unit::getPos()const{
    return m_pos;
}
Owner Unit::getOwner()const{
    return m_owner;
}
int Unit::getStar() const
{
    return m_star;
}
State Unit::getState() const
{
    return m_state;
}

QPointF Unit::getWorldPos(const QPoint& gridPos) const
{
    qreal radius=46.0;
    qreal w = radius * qSqrt(3.0);

    qreal x = gridPos.x() * w;
    if (gridPos.y() % 2 != 0) {
        x += w / 2.0;
    }

    qreal y = gridPos.y() * (radius * 1.5);

    return QPointF(x, y);
}

void Unit::setHp(int newHp){
    m_hp=newHp;
}
void Unit::setAtk(int newAtk){
    m_atk=newAtk;
}
void Unit::setRange(int newRange){
    m_range=newRange;
}
void Unit::setMana(int newMana){
    m_mana=newMana;
}
void Unit::setPos(QPoint newPos){
    m_pos=newPos;
}

//状态机
void Unit::updateUnit(Board &board, const std::vector<Unit *> allUnits)
{
    if(m_pos.y()<Board::ROWS){
        if(m_hp<=0){
            m_state=State::Dead;
        }

        switch(m_state){
            case State::Idle:
            handleIdle(board,allUnits);
                break;
            case State::Moving:
                handleMoving(board,allUnits);
                break;
            case State::Attacking:
                break;
            case State::Casting:
                break;
            case State::Dead:
                break;
        }
    }
}
void Unit::handleIdle(Board &board,const std::vector<Unit*> allUnits)
{
    Unit* closeEnemy=nullptr;
    qreal dist=1e18;
    for(Unit* enemyUnit:allUnits){
        if(enemyUnit && enemyUnit->getOwner()!=m_owner &&enemyUnit->getPos().y()<Board::ROWS){
            QPointF myWorldPos=getWorldPos(m_pos);
            QPointF enemyWorldPos=getWorldPos(enemyUnit->getPos());
            qreal dx=myWorldPos.x()-enemyWorldPos.x();
            qreal dy=myWorldPos.y()-enemyWorldPos.y();

            qreal distance=dx * dx + dy * dy;
            if(distance<dist-0.1){
                dist=distance;
                closeEnemy=enemyUnit;
            }
            else if(qAbs(distance-dist)<=0.1){
                if(enemyUnit->getHp()>closeEnemy->getHp()){
                    closeEnemy=enemyUnit;
                }
                else if(enemyUnit->getHp()==closeEnemy->getHp()){
                    if(enemyUnit->getPos().x()<closeEnemy->getPos().x()){
                        closeEnemy=enemyUnit;
                    }
                    else if(enemyUnit->getPos().x()==closeEnemy->getPos().x()){
                        if(enemyUnit->getPos().y()>closeEnemy->getPos().y()){
                            closeEnemy=enemyUnit;
                        }
                    }
                }
            }
        }
    }

    if(closeEnemy){
        m_target=closeEnemy;
        m_state=State::Moving;
        qDebug()<<m_name<<"目标是"<<closeEnemy->getName();
    }
}

void Unit::handleMoving(Board &board, const std::vector<Unit *> allUnits)
{

}