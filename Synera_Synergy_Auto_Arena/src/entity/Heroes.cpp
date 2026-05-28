#include "Heroes.h"
#include"Board.h"

Sidon::Sidon(const QString& name,Owner owner,qreal scale)
    :Unit(name,750*scale,30*scale,1,60,owner,Profession::Guardian)
    ,m_skillTime(0)
{}

void Sidon::castSkill(Board& board,const std::vector<Unit*> allUnits)
{
    m_skillTime=180;
}
void Sidon::takeDamage(int atk)
{
    if(m_skillTime){
        Unit::takeDamage(atk/2);
    }
    else {
        Unit::takeDamage(atk);
    }
}

void Sidon::updateUnit(Board &board, const std::vector<Unit *> allUnits)
{
    if(m_skillTime){
        m_skillTime--;
    }
    Unit::updateUnit(board,allUnits);
}


Luna::Luna(const QString& name,Owner owner,qreal scale)
    :Unit(name,450*scale,35*scale,3,70,owner,Profession::Mage)
{}

void Luna::castSkill(Board& board,const std::vector<Unit*> allUnits)
{
    Unit* unit1=nullptr;
    Unit* unit2=nullptr;
    double per1=1,per2=1;
    for(Unit* unit:allUnits){
        if(unit->getOwner()==getOwner() && unit->getState()!=State::Dead && unit->getPos().y()<Board::ROWS){
            double persent=(double)(unit->getHp())/unit->getMaxHp();
            if(persent){
                unit2=unit1;
                unit1=unit;
                per2=per1;
                per1=persent;
            }
            else {
                if(persent<per2){
                    unit2=unit;
                    per2=persent;
                }
            }
        }
    }
    if(unit1){
        unit1->setHp(qMin(unit1->getHp()+unit1->getHp()/4,unit1->getMaxHp()));
    }
    if(unit2){
        unit2->setHp(qMin(unit2->getHp()+unit2->getHp()/4,unit2->getMaxHp()));
    }
}

Noah::Noah(const QString& name,Owner owner,qreal scale)
    :Unit(name,620*scale,52*scale,1,60,owner,Profession::Warrior)
    ,m_skillTime(0)
    ,m_inSkill(false)
    ,m_debuffUnit(nullptr)
    ,m_originAtk(-1)
{}

void Noah::castSkill(Board& board,const std::vector<Unit*> allUnits)
{
    if(getTarget()){
        getTarget()->takeDamage(getAtk()*3/2);
        m_originAtk=getTarget()->getAtk();
        m_debuffUnit=getTarget();
        getTarget()->setAtk(getTarget()->getAtk()*4/5);
        m_inSkill=true;
    }
    m_skillTime=180;
}

void Noah::updateUnit(Board &board, const std::vector<Unit *> allUnits)
{
    if(m_skillTime){
        m_skillTime--;
    }
    else if(m_inSkill){
        if(m_debuffUnit && m_debuffUnit->getState()!=State::Dead){
            m_debuffUnit->setAtk(m_originAtk);
            m_inSkill=false;
        }
        m_debuffUnit=nullptr;
    }
    Unit::updateUnit(board,allUnits);
}

WaterblightGanon::WaterblightGanon(const QString &name)
    :Unit(name,1300,45,3,90,Owner::EnemyCtrl,Profession::Mage)
    ,m_hasPhase2(false)
{}

void WaterblightGanon::castSkill(Board& board,const std::vector<Unit*> allUnits)
{
    for(Unit* unit:allUnits){
        if(!unit){
            continue;
        }
        else {
            if(unit->getOwner()==Owner::PlayerCtrl && unit->getState()!=State::Dead && unit->getPos().y()<Board::ROWS){
                unit->takeDamage(50);
                unit->setMoveCoolDown(qMax(unit->getMoveCoolDown(),30));
                unit->setAtkCoolDown(qMax(unit->getAtkCoolDown(),30));
            }
        }
    }
}

void WaterblightGanon::takeDamage(int atk)
{
    Unit::takeDamage(atk);
    if(!m_hasPhase2 && getHp()<getMaxHp()/2 && getState()!=State::Dead){
        m_hasPhase2=true;

        setMaxMana(45);
        setMana(0);
    }
}
