#include "Heroes.h"

Sidon::Sidon(const QString& name,int maxHp,int atk,Owner owner)
    :Unit(name,maxHp,atk,1,60,owner,Profession::Guardian)
{}

void Sidon::castSkill()
{
    qDebug()<<getName()<<"释放技能";
}

Luna::Luna(const QString &name, int maxHp, int atk, Owner owner)
    :Unit(name,maxHp,atk,3,80,owner,Profession::Mage)
{}

void Luna::castSkill()
{
    qDebug()<<getName()<<"释放技能";
}

Noah::Noah(const QString &name, int maxHp, int atk, Owner owner)
    :Unit(name,maxHp,atk,1,60,owner,Profession::Warrior)
{}

void Noah::castSkill()
{
    qDebug()<<getName()<<"释放技能";
}
