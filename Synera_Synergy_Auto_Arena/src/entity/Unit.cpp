#include "Unit.h"
#include"Board.h"
#include<QDebug>
#include<QHash>
#include<queue>
int Unit::m_nxtUnitId=0;
Unit::Unit(const QString &name,int maxHp,int atk,int range,int maxMana,Owner owner,Profession profession,bool isShopHero,QObject *parent)
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
    ,m_moveCoolDown(0)
    ,m_atkCoolDown(0)
    ,m_startPos(-1,-1)
    ,m_profession(profession)
    ,m_isShopHero(isShopHero)
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

QPoint Unit::getStartPos() const
{
    return m_startPos;
}

Unit *Unit::getTarget() const
{
    return m_target;
}

int Unit::getMoveCoolDown() const
{
    return m_moveCoolDown;
}

int Unit::getAtkCoolDown() const
{
    return m_atkCoolDown;
}

bool Unit::getIsShopHero() const
{
    return m_isShopHero;
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

void Unit::setStartPos(QPoint pos)
{
    m_startPos=pos;
}

void Unit::setState(State state)
{
    m_state=state;
}

void Unit::setMoveCoolDown(int newCoolDown)
{
    m_moveCoolDown=newCoolDown;
}

void Unit::setAtkCoolDown(int newCoolDown)
{
    m_atkCoolDown=newCoolDown;
}

void Unit::setMaxMana(int newMaxMana)
{
    m_maxMana=newMaxMana;
}

void Unit::setIsShop(bool isShop)
{
    m_isShopHero=isShop;
}

//状态机
void Unit::updateUnit(Board &board, const std::vector<Unit *> allUnits)
{
    if(m_pos.y()<Board::ROWS){
        if(m_hp<=0 || m_state==State::Dead){
            m_state=State::Dead;
            return;
        }
        if(m_mana==m_maxMana && m_state!=State::Dead){
            m_state=State::Casting;
        }
        switch(m_state){
            case State::Idle:
                handleIdle(board,allUnits);
                break;
            case State::Moving:
                handleMoving(board);
                break;
            case State::Attacking:
                handleAttking();
                break;
            case State::Casting:
                handleCasting(board,allUnits);
                break;
        }
    }
}
void Unit::handleIdle(Board &board,const std::vector<Unit*> allUnits)
{
    Unit* closeEnemy=nullptr;
    qreal dist=1e18;
    for(Unit* enemyUnit:allUnits){
        if(enemyUnit && enemyUnit->getOwner()!=m_owner &&enemyUnit->getPos().y()<Board::ROWS && enemyUnit->getState()!=State::Dead){
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
                if(enemyUnit->getHp()<closeEnemy->getHp()){
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
        //qDebug()<<m_name<<"目标是"<<closeEnemy->getName();
    }
}

void Unit::handleMoving(Board &board)
{

    if(m_target==nullptr || m_target->getState()==State::Dead){
        m_state=State::Idle;
        m_target=nullptr;
        return;
    }

    QSet<QPoint> oldRangeGrids=board.getRangeGrid(m_pos,m_range);
    for(const QPoint& rangeGrid:oldRangeGrids){
        if(m_target->getPos()==rangeGrid){
            m_state=State::Attacking;
            return;
        }
    }

    if(m_moveCoolDown>0){
        m_moveCoolDown--;
        return;
    }
    if(m_moveCoolDown==0){
        std::vector<QPoint> path=breadFirstSearch(board);
        if(path.size()==0){
            m_state=State::Idle;
            return;
        }
        board.removeUnit(this);
        board.addUnit(this,path[0]);
        m_moveCoolDown=20;
    }

    QSet<QPoint> rangeGrids=board.getRangeGrid(m_pos,m_range);
    for(const QPoint& rangeGrid:rangeGrids){
        if(m_target->getPos()==rangeGrid){
            m_state=State::Attacking;
        }
    }

}

void Unit::handleAttking()
{
    if(m_target==nullptr || m_target->getState()==State::Dead){
        m_state=State::Idle;
        m_target=nullptr;
        return;
    }
    if(m_atkCoolDown>0){
        m_atkCoolDown--;
        return;
    }
    if(m_atkCoolDown==0){
        m_target->takeDamage(m_atk);
        m_mana=qMin(m_mana+10,m_maxMana);
        //qDebug()<<m_name<<"对"<<m_target->getName()<<"发起攻击："<<m_atk;
        m_atkCoolDown=60;
    }
}

void Unit::handleCasting(Board& board,const std::vector<Unit*> allUnits)
{
    castSkill(board,allUnits);
    m_mana=0;
    m_state=State::Idle;
}

std::vector<QPoint> Unit::breadFirstSearch(Board &board)
{
    QPoint state(-1,-1);
    QPoint step=m_target->getPos();
    QHash<QPoint,QPoint> parentMap;
    std::queue<QPoint> frontier;
    std::vector<QPoint> path;
    std::vector<QPoint> visited;
    frontier.push(m_pos);
    visited.push_back(m_pos);

    while(frontier.empty()==false){
        state=frontier.front();
        frontier.pop();
        if(state==m_target->getPos()){
            while(step!=m_pos){
                path.push_back(step);
                step=parentMap[step];
            }
            std::reverse(path.begin(),path.end());
            return path;
        }
        for(QPoint nxtState:board.getNeighborGrid(state)){
            if(board.hasUnitAt(nxtState) && nxtState!=m_target->getPos()){
                continue;
            }
            if(std::find(visited.begin(),visited.end(),nxtState)==visited.end()){
                frontier.push(nxtState);
                parentMap[nxtState]=state;
                visited.push_back(nxtState);
            }
        }
    }

    return path;
}

void Unit::takeDamage(int atk)
{
    if(m_hp<0)return;
    m_hp=qMax(m_hp-atk,0);

    emit infoChanged(this);
    if(m_hp<=0){
        m_state=State::Dead;
        m_mana=0;

        emit isDead(this);

        m_pos=QPoint(-1,-1);
    }

}