#include "Unit.h"
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
{}

//属性相关
int Unit::getId()const{
    return m_id;
}
int Unit::getHp()const{
    return m_hp;
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
QString Unit::getName()const{
    return m_name;
}
QPoint Unit::getPos()const{
    return m_pos;
}
Owner Unit::getOwner()const{
    return m_owner;
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