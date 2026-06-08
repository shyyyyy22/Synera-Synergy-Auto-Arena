#include "Heroes.h"
#include"Board.h"

//水
Sidon::Sidon(const QString& name,Owner owner,qreal scale,int star,bool isShopHero)
    :Unit(name,750*scale,30*scale,1,60,owner,Profession::Guardian,star,isShopHero)
    ,m_skillTime(0)
{
    setRace(Race::Zora);
}
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


Luna::Luna(const QString& name,Owner owner,qreal scale,int star,bool isShopHero)
    :Unit(name,450*scale,35*scale,3,70,owner,Profession::Mage,star,isShopHero)
{
    setRace(Race::Zora);
}
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


Noah::Noah(const QString& name,Owner owner,qreal scale,int star,bool isShopHero)
    :Unit(name,620*scale,52*scale,1,60,owner,Profession::Warrior,star,isShopHero)
    ,m_skillTime(0)
    ,m_inSkill(false)
    ,m_debuffUnit(nullptr)
    ,m_originAtk(-1)
{
    setRace(Race::Zora);
}
void Noah::castSkill(Board& board,const std::vector<Unit*> allUnits)
{
    if(getTarget()){
        getTarget()->takeDamage(getAtk()*3/2);
        m_originAtk=getTarget()->getAtk();
        m_debuffUnit=getTarget();
        getTarget()->setAtk(getTarget()->getAtk()*4/5);
        m_inSkill=true;
        m_skillTime=180;
    }

}
void Noah::updateUnit(Board &board, const std::vector<Unit *> allUnits)
{
    if(m_skillTime){
        m_skillTime--;
    }
    else if(m_inSkill){
        if(m_debuffUnit && m_debuffUnit->getState()!=State::Dead){
            m_debuffUnit->setAtk(m_originAtk);

        }
        m_inSkill=false;
        m_debuffUnit=nullptr;
    }
    Unit::updateUnit(board,allUnits);
}


WaterblightGanon::WaterblightGanon(const QString &name)
    :Unit(name,1300,45,3,90,Owner::EnemyCtrl,Profession::Mage)
    ,m_hasPhase2(false)
    ,m_skillTime(0)
{
    setRace(Race::Boss);
}
void WaterblightGanon::castSkill(Board& board,const std::vector<Unit*> allUnits)
{
    m_inSkill=true;
    m_skillTime=300;
}
void WaterblightGanon::takeDamage(int atk)
{
    Unit::takeDamage(atk);
    if(!m_hasPhase2 && getHp()<getMaxHp()/2 && getHp()>0){
        m_hasPhase2=true;

        setAtk(getAtk()*13/10);
        setMaxMana(45);
        setMana(0);
    }
}
void WaterblightGanon::updateUnit(Board &board, const std::vector<Unit *> allUnits)
{
    if(m_skillTime){
        if(m_skillTime%60==0){
            for(Unit* unit:allUnits){
                if(!unit){
                    continue;
                }
                else {
                    if(unit->getOwner()==Owner::PlayerCtrl && unit->getState()!=State::Dead && unit->getPos().y()<Board::ROWS){
                        unit->takeDamage(25);
                    }
                }
            }
        }

        m_skillTime--;
    }
    else if(m_inSkill){
        m_inSkill=false;
    }
    Unit::updateUnit(board,allUnits);
}

//风
Revali::Revali(const QString &name, Owner owner, qreal scale, int star, bool isShopHero)
    :Unit(name,450*scale,50*scale,3,50,owner,Profession::Archer,star,isShopHero)
{
    setRace(Race::Rito);
}
void Revali::castSkill(Board &board, const std::vector<Unit *> allUnits)
{
    Unit* target=nullptr;
    int col=getPos().x(),row=getPos().y();
    int q=col-(row+(row&1))/2;
    int r=row;
    int s=-q-r;
    int dist=0;
    for(Unit* enemy:allUnits){
        if(enemy->getOwner()==getOwner() || enemy->getState()==State::Dead || enemy->getPos().y()==Board::ROWS){
            continue;
        }
        else {
            int Ecol=enemy->getPos().x(),Erow=enemy->getPos().y();
            int Eq=Ecol-(Erow+(Erow&1))/2;
            int Er=Erow;
            int Es=-Eq-Er;
            int dq=qAbs(Eq-q);
            int dr=qAbs(Er-r);
            int ds=qAbs(Es-s);
            if((dq+dr+ds)/2>dist){
                dist=(dq+dr+ds)/2;
                target=enemy;
            }
        }
    }
    if(target){
        if(dist>=3){
            target->takeDamage(5*getAtk()/2);
        }
        else {
            target->takeDamage(2*getAtk());
        }
        target->setMoveCoolDown(90);
        target->setAtkCoolDown(90);
    }
}

Kashi::Kashi(const QString &name, Owner owner, qreal scale, int star, bool isShopHero)
    :Unit(name,490*scale,58*scale,1,50,owner,Profession::Assassin,star,isShopHero)
{
    setRace(Race::Rito);
}
void Kashi::castSkill(Board &board, const std::vector<Unit *> allUnits)
{
    Unit* target=nullptr;
    int col=getPos().x(),row=getPos().y();
    int q=col-(row+(row&1))/2;
    int r=row;
    int s=-q-r;
    int dist=0;
    for(Unit* enemy:allUnits){
        if(enemy->getOwner()==getOwner() || enemy->getState()==State::Dead || enemy->getPos().y()==Board::ROWS){
            continue;
        }
        else {
            int Ecol=enemy->getPos().x(),Erow=enemy->getPos().y();
            int Eq=Ecol-(Erow+(Erow&1))/2;
            int Er=Erow;
            int Es=-Eq-Er;
            int dq=qAbs(Eq-q);
            int dr=qAbs(Er-r);
            int ds=qAbs(Es-s);
            if((dq+dr+ds)/2>dist){
                dist=(dq+dr+ds)/2;
                target=enemy;
            }
        }
    }
    if(target){
        std::vector<QPoint> neighbor=board.getNeighborGrid(target->getPos());
        for(QPoint pos:neighbor){
            if(board.hasUnitAt(pos)){
                continue;
            }
            else {
                board.removeUnit(this);
                board.addUnit(this,pos);
                break;
            }
        }
        target->takeDamage(11*getAtk()/5);
        this->setState(State::Idle);
    }
}

Evan::Evan(const QString &name, Owner owner, qreal scale, int star, bool isShopHero)
    :Unit(name,420*scale,42*scale,3,65,owner,Profession::Mage,star,isShopHero)
{
    setRace(Race::Rito);
}
void Evan::castSkill(Board &board, const std::vector<Unit*> allUnits) {
    Unit* furthestEnemy = nullptr;
    int col = getPos().x(), row = getPos().y();

    int q = col - (row + (row & 1)) / 2;
    int r = row;
    int s = -q - r;
    int maxDist = 0;

    for (Unit* enemy : allUnits) {
        if (!enemy || enemy->getOwner() == getOwner() ||
            enemy->getState() == State::Dead ||
            enemy->getPos().y() >= Board::ROWS) continue;

        int Ecol = enemy->getPos().x(), Erow = enemy->getPos().y();
        int Eq = Ecol - (Erow + (Erow & 1)) / 2;
        int Er = Erow;
        int Es = -Eq - Er;
        int dist = (qAbs(Eq-q) + qAbs(Er-r) + qAbs(Es-s)) / 2;

        if (dist > maxDist) {
            maxDist = dist;
            furthestEnemy = enemy;
        }
    }

    if (furthestEnemy) {
        QPoint landingPos(-1, -1);

        for (Unit* ally : allUnits) {
            if (ally && ally->getOwner() == getOwner() &&
                ally->getProfession() == Profession::Warrior &&
                ally->getState() != State::Dead &&
                ally->getPos().y() < Board::ROWS)
            {
                std::vector<QPoint> neighbors = board.getNeighborGrid(ally->getPos());
                for (const QPoint& p : neighbors) {
                    if (!board.hasUnitAt(p)) {
                        landingPos = p;
                        break;
                    }
                }
            }
            if (landingPos != QPoint(-1, -1)) break;
        }

        if (landingPos != QPoint(-1, -1)) {

            board.removeUnit(furthestEnemy);
            board.addUnit(furthestEnemy, landingPos);

            furthestEnemy->setMoveCoolDown(60);
            furthestEnemy->setAtkCoolDown(60);
            furthestEnemy->setState(State::Idle);


            furthestEnemy->takeDamage(getAtk());
        }
    }
}

WindblightGanon::WindblightGanon(const QString &name)
    :Unit(name,1600,60,3,80,Owner::EnemyCtrl,Profession::Archer)
    ,m_hasPhase2(false)
{
    setRace(Race::Boss);
}
void WindblightGanon::castSkill(Board& board,const std::vector<Unit*> allUnits){
    if(getTarget()){
        getTarget()->takeDamage(getAtk()*3/2);
        getTarget()->setState(State::Idle);
        getTarget()->setMoveCoolDown(180);
        getTarget()->setAtkCoolDown(180);
    }
    else {
        for(Unit* unit:allUnits){
            if(!unit){
                continue;
            }
            else {
                if(unit->getOwner()==Owner::PlayerCtrl && unit->getState()!=State::Dead && unit->getPos().y()<Board::ROWS){
                    unit->takeDamage(20);
                }
            }
        }
    }
}
void WindblightGanon::takeDamage(int atk)
{
    Unit::takeDamage(atk);
    if(!m_hasPhase2 && getHp()<getMaxHp()/2 && getHp()>0){
        m_hasPhase2=true;

        setOriAtkCoolDown(40);
        setRange(4);
    }
}

//雷
Ur::Ur(const QString &name, Owner owner, qreal scale, int star, bool isShopHero)
    :Unit(name,620*scale,60*scale,1,60,owner,Profession::Warrior,star,isShopHero)
{
    setRace(Race::Gerudo);
}
void Ur::castSkill(Board &board, const std::vector<Unit *> allUnits)
{
    if(getTarget()){
        getTarget()->takeDamage(getAtk());
        getTarget()->takeDamage(getAtk());
        getTarget()->setMoveCoolDown(60);
        getTarget()->setAtkCoolDown(60);
    }
}

Naji::Naji(const QString &name, Owner owner, qreal scale, int star, bool isShopHero)
    :Unit(name,420*scale,40*scale,3,60,owner,Profession::Mage,star,isShopHero)
    ,m_skillTime(0)
    ,m_inSkill(false)
    ,m_debuffUnit(nullptr)
    ,m_originAtk(-1)
{
    setRace(Race::Gerudo);
}
void Naji::castSkill(Board &board, const std::vector<Unit *> allUnits)
{
    if(getTarget()){
        m_originAtk=getTarget()->getAtk();
        m_debuffUnit=getTarget();
        getTarget()->setAtk(0);
        m_inSkill=true;
        m_skillTime=240;
    }

}
void Naji::updateUnit(Board &board, const std::vector<Unit *> allUnits)
{
    if(m_skillTime){
        m_skillTime--;
    }
    else if(m_inSkill){
        if(m_debuffUnit && m_debuffUnit->getState()!=State::Dead){
            m_debuffUnit->setAtk(m_originAtk);
        }
        m_inSkill=false;
        m_debuffUnit=nullptr;
    }
    Unit::updateUnit(board,allUnits);
}

Shika::Shika(const QString &name, Owner owner, qreal scale, int star, bool isShopHero)
    :Unit(name,480*scale,55*scale,1,50,owner,Profession::Assassin,star,isShopHero)
{
    setRace(Race::Gerudo);
}
void Shika::castSkill(Board &board, const std::vector<Unit *> allUnits)
{
    Unit* target=nullptr;
    int hp=1e8;
    for(Unit* enemy:allUnits){
        if(enemy->getOwner()==getOwner() || enemy->getState()==State::Dead || enemy->getPos().y()==Board::ROWS){
            continue;
        }
        else {
            if(enemy->getHp()<hp){
                hp=enemy->getHp();
                target=enemy;
            }
        }
    }
    if(target){
        std::vector<QPoint> neighbor=board.getNeighborGrid(target->getPos());
        for(QPoint pos:neighbor){
            if(board.hasUnitAt(pos)){
                continue;
            }
            else {
                board.removeUnit(this);
                board.addUnit(this,pos);
                break;
            }
        }
        target->takeDamage(9*getAtk()/5);
        this->setState(State::Idle);
    }
}

ThunderblightGanon::ThunderblightGanon(const QString &name)
    :Unit(name,2000,75,2,80,Owner::EnemyCtrl,Profession::Warrior)
    ,m_hasPhase2(false)
{
    setRace(Race::Boss);
}
void ThunderblightGanon::castSkill(Board &board, const std::vector<Unit *> allUnits)
{
    if(getTarget()){
        getTarget()->takeDamage(getAtk()*3/2);
        getTarget()->setState(State::Idle);
        getTarget()->setMoveCoolDown(180);
        getTarget()->setAtkCoolDown(180);
        getTarget()->setMana(qMax(0,getTarget()->getMana()-30));
    }
    else {
        for(Unit* unit:allUnits){
            if(!unit){
                continue;
            }
            else {
                if(unit->getOwner()==Owner::PlayerCtrl && unit->getState()!=State::Dead && unit->getPos().y()<Board::ROWS){
                    unit->takeDamage(30);
                    unit->setMana(qMax(0,unit->getMana()-15));
                }
            }
        }
    }
}
void ThunderblightGanon::takeDamage(int atk)
{
    Unit::takeDamage(atk);
    if(!m_hasPhase2 && getHp()<getMaxHp()/2 && getHp()>0){
        m_hasPhase2=true;

        setOriAtkCoolDown(40);
        setAtk(getAtk()*13/10);
    }
}

//火
Daruk::Daruk(const QString &name, Owner owner, qreal scale, int star, bool isShopHero)
    :Unit(name,800*scale,32*scale,1,60,owner,Profession::Guardian,star,isShopHero)
    ,m_skillTime(0)
    ,m_inSkill(false)
{
    setRace(Race::Goron);
}
void Daruk::castSkill(Board &board, const std::vector<Unit *> allUnits)
{
    std::vector<QPoint> neighbor=board.getNeighborGrid(getPos());
    for(QPoint pos:neighbor){
        if(board.hasUnitAt(pos)){
            Unit* target=board.getUnitAt(pos);
            if(target->getOwner()!=getOwner()){
                target->setTarget(this);
                target->setState(State::Moving);
            }
        }
    }
    m_inSkill=true;
    m_skillTime=240;
}
void Daruk::updateUnit(Board &board, const std::vector<Unit *> allUnits)
{
    if(m_skillTime){
        if(m_skillTime%60==0){
            setHp(qMin(getHp()+30,getMaxHp()));
        }
        m_skillTime--;
    }
    else if(m_inSkill){
        m_inSkill=false;
    }
    Unit::updateUnit(board,allUnits);
}

Goma::Goma(const QString &name, Owner owner, qreal scale, int star, bool isShopHero)
    :Unit(name,650*scale,62*scale,1,60,owner,Profession::Warrior,star,isShopHero)
{
    setRace(Race::Goron);
}
void Goma::castSkill(Board &board, const std::vector<Unit *> allUnits)
{
    if(getTarget()){
        getTarget()->takeDamage(getAtk()*2);
        getTarget()->setMoveCoolDown(60);
        getTarget()->setAtkCoolDown(60);
    }
}

Mag::Mag(const QString &name, Owner owner, qreal scale, int star, bool isShopHero)
    :Unit(name,480*scale,48*scale,3,70,owner,Profession::Mage,star,isShopHero)
    ,m_skillTime(0)
    ,m_inSkill(false)
{
    setRace(Race::Goron);
}
void Mag::castSkill(Board &board, const std::vector<Unit *> allUnits)
{
    if(getTarget()){
        getTarget()->takeDamage(getAtk()*3/2);
        m_burnCenter=getTarget()->getPos();
    }
    m_inSkill=true;
    m_skillTime=240;
}
void Mag::updateUnit(Board &board, const std::vector<Unit *> allUnits)
{
    if(m_skillTime){
        if(m_skillTime%60==0){
            if(getTarget()){
                std::vector<QPoint> neighbor=board.getNeighborGrid(m_burnCenter);
                for(QPoint pos:neighbor){
                    if(board.hasUnitAt(pos)){
                        Unit* target=board.getUnitAt(pos);
                        if(target->getOwner()!=getOwner()){
                            target->takeDamage(20);
                        }
                    }
                }
            }
        }
        m_skillTime--;
    }
    else if(m_inSkill){
        m_inSkill=false;
    }
    Unit::updateUnit(board,allUnits);
}
FireblightGanon::FireblightGanon(const QString &name)
    :Unit(name,2500,90,1,90,Owner::EnemyCtrl,Profession::Guardian)
    ,m_hasPhase2(false)
{
    setRace(Race::Boss);
}
void FireblightGanon::castSkill(Board &board, const std::vector<Unit *> allUnits)
{
    if(getTarget()){
        getTarget()->takeDamage(getAtk()*3/2);
        std::vector<QPoint> neighbor=board.getNeighborGrid(getTarget()->getPos());
        for(QPoint pos:neighbor){
            Unit* unit=board.getUnitAt(pos);
            if(!unit || unit->getOwner()==Owner::EnemyCtrl)continue;
            unit->takeDamage(40);
        }
    }
    else {
        for(Unit* unit:allUnits){
            if(!unit){
                continue;
            }
            else {
                if(unit->getOwner()==Owner::PlayerCtrl && unit->getState()!=State::Dead && unit->getPos().y()<Board::ROWS){
                    unit->takeDamage(50);
                    unit->setMoveCoolDown(unit->getMoveCoolDown()+30);
                    unit->setAtkCoolDown(unit->getAtkCoolDown()+30);
                }
            }
        }
    }
}
void FireblightGanon::takeDamage(int atk)
{
    Unit::takeDamage(atk);
    if(!m_hasPhase2 && getHp()<getMaxHp()/2 && getHp()>0){
        m_hasPhase2=true;
        setAtk(getAtk()*6/5);
        setHp(qMin(getMaxHp(),getHp()+300));
    }
}

//海拉鲁
Leo::Leo(const QString &name, Owner owner, qreal scale, int star, bool isShopHero)
    :Unit(name,720*scale,35*scale,1,60,owner,Profession::Guardian,star,isShopHero)
{
    setRace(Race::Hyrulean);
}
void Leo::castSkill(Board &board, const std::vector<Unit *> allUnits)
{
    Unit* lowestAlly = nullptr;
    double lowestPercent = 1.1;
    for (Unit* ally : allUnits) {
        if (!ally) continue;

        if (ally != this &&
            ally->getOwner() == getOwner() &&
            ally->getState() != State::Dead &&
            ally->getPos().y() < Board::ROWS)
        {
            double percent = static_cast<double>(ally->getHp()) / ally->getMaxHp();
            if (percent < lowestPercent) {
                lowestPercent = percent;
                lowestAlly = ally;
            }
        }
    }

    if (lowestAlly) {
        QPoint landingPos(-1, -1);
        std::vector<QPoint> neighbors = board.getNeighborGrid(lowestAlly->getPos());
        for (const QPoint& p : neighbors) {
            if (!board.hasUnitAt(p)) {
                landingPos = p;
                break;
            }
        }
        if (landingPos != QPoint(-1, -1)) {
            board.removeUnit(this);
            board.addUnit(this, landingPos);
            lowestAlly->setHp(qMin(lowestAlly->getMaxHp(),lowestAlly->getHp()+250));
            setState(State::Idle);
        }
    }
}

Alan::Alan(const QString &name, Owner owner, qreal scale, int star, bool isShopHero)
    :Unit(name,600*scale,55*scale,1,60,owner,Profession::Warrior,star,isShopHero)
{
    setRace(Race::Hyrulean);
}
void Alan::castSkill(Board &board, const std::vector<Unit *> allUnits)
{
    std::vector<QPoint> neighbor=board.getNeighborGrid(getPos());
    for(QPoint pos:neighbor){
        if(board.hasUnitAt(pos)){
            Unit* target=board.getUnitAt(pos);
            if(target->getOwner()!=getOwner()){
                target->takeDamage(3*getAtk()/2);
                target->setMoveCoolDown(30);
                target->setAtkCoolDown(30);
            }
        }
    }
}

Syndra::Syndra(const QString &name, Owner owner, qreal scale, int star, bool isShopHero)
    :Unit(name,450*scale,45*scale,3,70,owner,Profession::Mage,star,isShopHero)
{
    setRace(Race::Hyrulean);
}
void Syndra::castSkill(Board &board, const std::vector<Unit *> allUnits)
{
    if(getTarget()){
        getTarget()->takeDamage(getAtk()*3);
    }
}

Ganondorf::Ganondorf(const QString &name)
    :Unit(name,4000,100,3,120,Owner::EnemyCtrl,Profession::Guardian)
    ,m_hasPhase2(false)
    ,m_hasPhase3(false)
    ,m_skillTime(0)
{
    setRace(Race::Boss);
}
void Ganondorf::castSkill(Board &board, const std::vector<Unit *> allUnits)
{
    if(!m_hasPhase3){
        if(getTarget() && getTarget()->getHp()>0){
            getTarget()->takeDamage(getAtk()*5/2);
        }
        else {
            for(Unit* unit:allUnits){
                if(!unit){
                    continue;
                }
                else {
                    if(unit->getOwner()==Owner::PlayerCtrl && unit->getState()!=State::Dead && unit->getPos().y()<Board::ROWS){
                        unit->takeDamage(60);
                    }
                }
            }
        }
    }
    else {
        for(Unit* unit:allUnits){
            if(!unit){
                continue;
            }
            else {
                if(unit->getOwner()==Owner::PlayerCtrl && unit->getState()!=State::Dead && unit->getPos().y()<Board::ROWS){
                    unit->takeDamage(100);
                }
            }
        }
    }

}
void Ganondorf::takeDamage(int atk)
{
    if(!m_hasPhase3){
        Unit::takeDamage(atk);
    }
    else {
        Unit::takeDamage(atk/2);
    }
    if(!m_hasPhase2 && getHp()<getMaxHp()*7/10 && getHp()>0){
        m_hasPhase2=true;
        setAtk(getAtk()*13/10);
        setOriAtkCoolDown(40);
    }
    if(!m_hasPhase3 && getHp()<getMaxHp()*3/10 && getHp()>0){
        m_hasPhase3=true;
        setAtk(0);
        m_skillTime=300;
    }
}
void Ganondorf::setMoveCoolDown(int newCoolDown)
{
    if(m_hasPhase2)return;
    Unit::setMoveCoolDown(newCoolDown);
}
void Ganondorf::setAtkCoolDown(int newCoolDown)
{
    if(m_hasPhase2)return;
    Unit::setAtkCoolDown(newCoolDown);
}
void Ganondorf::updateUnit(Board &board, const std::vector<Unit *> allUnits)
{
    if(m_hasPhase3 && m_skillTime==0){
        setHp(qMin(getHp()+400,getMaxHp()));
        m_skillTime=300;
    }
    if(m_hasPhase3 && m_skillTime>0){
        m_skillTime--;
    }
    Unit::updateUnit(board,allUnits);
}
