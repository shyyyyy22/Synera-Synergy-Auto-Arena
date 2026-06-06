#include "Unit.h"
#include"Board.h"
#include<QDebug>
#include<QHash>
#include<queue>
int Unit::m_nxtUnitId=0;
Unit::Unit(const QString &name,int maxHp,int atk,int range,int maxMana,Owner owner,Profession profession,int star,bool isShopHero,QObject *parent)
    :QObject(parent)
    ,m_id(m_nxtUnitId++)
    ,m_maxHp(maxHp)
    ,m_oriMaxHp(maxHp)
    ,m_hp(maxHp)
    ,m_atk(atk)
    ,m_oriAtk(atk)
    ,m_range(range)
    ,m_oriRange(range)
    ,m_maxMana(maxMana)
    ,m_oriMaxMana(maxMana)
    ,m_mana(0)
    ,m_name(name)
    ,m_pos(-1,-1)
    ,m_owner(owner)
    ,m_star(star)
    ,m_state(State::Idle)
    ,m_target(nullptr)
    ,m_moveCoolDown(0)
    ,m_oriMoveCoolDown(20)
    ,m_atkCoolDown(0)
    ,m_oriAtkCoolDown(60)
    ,m_startPos(-1,-1)
    ,m_profession(profession)
    ,m_isShopHero(isShopHero)
    ,m_race(Race::Boss)
    ,m_equipment(Equipment::None)
{
    m_warriorSyn=m_archerSyn=m_mageSyn=m_assassinSyn=m_guardianSyn=false;
    m_atkCount=0;
    resetWithStar();
}

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

Profession Unit::getProfession() const
{
    return m_profession;
}

Race Unit::getRace() const
{
    return m_race;
}
QString Unit::getRaceName() const
{
    switch(m_race){
    case Race::Gerudo:
        return "格鲁德";
        break;
    case Race::Goron:
        return "鼓隆";
        break;
    case Race::Hyrulean:
        return "海拉鲁";
        break;
    case Race::Rito:
        return "利特";
        break;
    case Race::Zora:
        return "卓拉";
        break;
    default:
        return "";
    }
}

QString Unit::getProName() const
{
    switch(m_profession){
    case Profession::Warrior:
        return "战士";
        break;
    case Profession::Archer:
        return "射手";
        break;
    case Profession::Mage:
        return "法师";
        break;
    case Profession::Assassin:
        return "刺客";
        break;
    case Profession::Guardian:
        return "守卫";
        break;
    default:
        return "";
    }
}

Equipment Unit::getEquipment() const
{
    return m_equipment;
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

void Unit::setOriAtkCoolDown(int newCooldown)
{
    m_oriAtkCoolDown=newCooldown;
}

void Unit::setMaxMana(int newMaxMana)
{
    m_maxMana=newMaxMana;
}

void Unit::setMaxHp(int newMaxHp)
{
    m_maxHp=newMaxHp;
}

void Unit::setIsShop(bool isShop)
{
    m_isShopHero=isShop;
}

void Unit::setRace(Race race)
{
    m_race=race;
}

void Unit::setTarget(Unit *newTarget)
{
    m_target=newTarget;
}

void Unit::setEquipment(Equipment type)
{
    m_equipment=type;
}

void Unit::resetWithStar()
{
    switch(m_star){
    case 1:
        break;
    case 2:
        setMaxHp(getMaxHp()*1.8);
        setHp(getMaxHp());
        setAtk(getAtk()*1.8);
        break;
    case 3:
        setMaxHp(getMaxHp()*3.2);
        setHp(getMaxHp());
        setAtk(getAtk()*3.2);
        break;
    }
}

void Unit::restoreOriAtt()
{
    m_maxHp=m_oriMaxHp;
    m_hp=m_maxHp;
    m_atk=m_oriAtk;
    m_range=m_oriRange;
    m_maxMana=m_oriMaxMana;
    m_oriMoveCoolDown=20;
    m_oriAtkCoolDown=60;
    m_mana=0;
    m_warriorSyn=m_archerSyn=m_mageSyn=m_assassinSyn=m_guardianSyn=false;
    switch(m_equipment){
    case Equipment::Sword:
        m_atk+=15;
        break;
    case Equipment::Mail:
        m_maxHp+=150;
        m_hp=m_maxHp;
        break;
    case Equipment::Gloves:
        m_oriAtkCoolDown=50;
        break;
    case Equipment::Crystal:
        m_maxMana=qMax(m_maxMana-30,20);
        break;
    default:
        break;
    }
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
        m_moveCoolDown=m_oriMoveCoolDown;
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
        if(m_warriorSyn && getHp()<=getMaxHp()/2){
            m_target->takeDamage(m_atk*13/10);
            m_atkCount++;
        }
        else if(m_archerSyn && m_atkCount>=3){
            m_target->takeDamage(m_atk*3/2);
            m_atkCount=0;
        }
        else if(m_assassinSyn){
            m_target->takeDamage(m_atk*2);
            m_assassinSyn=false;
            m_atkCount++;
        }
        else {
            m_target->takeDamage(m_atk);
            m_atkCount++;
        }
        m_mana=qMin(m_mana+10,m_maxMana);
        //qDebug()<<m_name<<"对"<<m_target->getName()<<"发起攻击："<<m_atk;
        m_atkCoolDown=m_oriAtkCoolDown;
    }
}

void Unit::handleCasting(Board& board,const std::vector<Unit*> allUnits)
{
    castSkill(board,allUnits);
    if(m_mageSyn && m_target && m_target->getState()!=State::Dead){
        m_target->setAtkCoolDown(30);
        m_target->setMoveCoolDown(30);
    }
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
    if(m_hp<=0)return;
    if(m_guardianSyn){
        m_guardianSyn=false;
    }
    else {
        m_hp=qMax(m_hp-atk,0);
    }

    emit infoChanged(this);
    if(m_hp<=0){
        m_state=State::Dead;
        m_mana=0;

        emit isDead(this);

        m_pos=QPoint(-1,-1);
    }

}

void Unit::addEquipment(Equipment type)
{
    setEquipment(type);
    switch(type){
    case Equipment::Sword:
        m_atk+=15;
        break;
    case Equipment::Mail:
        m_maxHp+=150;
        m_hp=m_maxHp;
        break;
    case Equipment::Gloves:
        m_oriAtkCoolDown=50;
        break;
    case Equipment::Crystal:
        m_maxMana=qMax(m_maxMana-30,20);
        break;
    default:
        break;
    }
}