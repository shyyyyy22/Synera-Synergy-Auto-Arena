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
QString Sidon::getSkillDes() const
{
    return QString("技能：【守卫之誓】\n自身获得一个 50% 的伤害减免 Buff，持续 3 秒。");
}
QString Sidon::getClassName() const
{
    return QString("Sidon");
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
QString Luna::getSkillDes() const
{
    return QString("技能：【卓拉的祈福】\n治愈之歌，为生命值百分比最低的 2 个队友回复 25%最大生命值。。");
}
QString Luna::getClassName() const
{
    return QString("Luna");
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
QString Noah::getSkillDes() const
{
    return QString("技能：【潮汐重击】\n重击目标造成 ATK * 1.5 伤害，并在 3 秒内使其攻击力降低 20%。");
}
QString Noah::getClassName() const
{
    return QString("Noah");
}

WaterblightGanon::WaterblightGanon(const QString &name)
    :Unit(name,1500,45,3,90,Owner::EnemyCtrl,Profession::None)
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
QString WaterblightGanon::getSkillDes() const
{
    return QString("大招【大洪水】：\n"
                   "在 5 秒内，每秒对全场所有玩家存活单位造成 25 点真实伤害。\n"
                   "半血被动【狂暴激流】：\n"
                   "当血量跌破 50%时，它进入狂暴状态:最大蓝量永久减半，攻击力永久提升 30%");
}
QString WaterblightGanon::getClassName() const
{
    return QString("WaterblightGanon");
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
QString Revali::getSkillDes() const
{
    return QString("技能：【狙击】\n"
                   "对当前攻击范围内最远的敌人射出一支强力箭矢，造成 200% 攻击力 的物理伤害，并眩晕1.5 秒。\n"
                   "如果该敌人距离 ≥ 3 格，则伤害提升至 250%。");
}
QString Revali::getClassName() const
{
    return QString("Revali");
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
QString Kashi::getSkillDes() const
{
    return QString("技能：【风之探戈】\n瞬移到最远敌人身后，造成 ATK * 2.2物理爆发伤害。");
}
QString Kashi::getClassName() const
{
    return QString("Kashi");
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
QString Evan::getSkillDes() const
{
    return QString("技能：【风之牵引】\n将敌方最远的敌人牵引至我方战士相邻的格子内，并使其眩晕1秒");
}
QString Evan::getClassName() const
{
    return QString("Evan");
}

WindblightGanon::WindblightGanon(const QString &name)
    :Unit(name,1800,60,3,80,Owner::EnemyCtrl,Profession::None)
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
QString WindblightGanon::getSkillDes() const
{
    return QString("大招【飓风之风】：\n"
                   "效果：对当前目标造成 1.5 倍 ATK 伤害，并“吹飞”目标 3 秒。\n"
                   "半血被动【飞空高度】：\n"
                   "当血量跌破 50%时，它永久提升自己的攻速并且射程 +1");
}
QString WindblightGanon::getClassName() const
{
    return QString("WindblightGanon");
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
QString Ur::getSkillDes() const
{
    return QString("技能：【双重打击】\n连续攻击 2 次（各 100% 伤害），并让目标麻痹 1 秒。");
}
QString Ur::getClassName() const
{
    return QString("Ur");
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
QString Naji::getSkillDes() const
{
    return QString("技能：【黄沙遮天】\n召唤沙尘暴使当前目标致盲 4 秒，致盲期间其普攻伤害变为 0。");
}
QString Naji::getClassName() const
{
    return QString("Naji");
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
QString Shika::getSkillDes() const
{
    return QString("技能：【暗影突袭】\n瞬移到血量最低敌人身后，造成 ATK * 1.8伤害。");
}
QString Shika::getClassName() const
{
    return QString("Shika");
}

ThunderblightGanon::ThunderblightGanon(const QString &name)
    :Unit(name,2200,75,2,80,Owner::EnemyCtrl,Profession::None)
    ,m_hasPhase2(false)
{
    setRace(Race::Boss);
}
void ThunderblightGanon::castSkill(Board &board, const std::vector<Unit *> allUnits)
{
    if(getTarget()){
        getTarget()->takeDamage(getAtk()*3/2);
        getTarget()->setState(State::Idle);
        getTarget()->setMoveCoolDown(90);
        getTarget()->setAtkCoolDown(90);
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
QString ThunderblightGanon::getSkillDes() const
{
    return QString("大招：【雷鸣锁链】\n"
                   "对当前锁定目标造成 1.5 倍 ATK的伤害，强力眩晕目标 1.5 秒。\n"
                   "同时强行扣除目标 30 点法力值。\n"
                   "半血被动【神速雷震】：\n"
                   "基础攻击力永久提升 30%\n"
                   "基础普攻冷却降低。");
}
QString ThunderblightGanon::getClassName() const
{
    return QString("ThunderblightGanon");
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
QString Daruk::getSkillDes() const
{
    return QString("技能：【达鲁克的守护】\n强制嘲讽周围一圈敌人攻击自己，同时自身4秒内每秒恢复30血量。");
}
QString Daruk::getClassName() const
{
    return QString("Daruk");
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
QString Goma::getSkillDes() const
{
    return QString("技能：【碎岩猛击】\n大锤猛砸造成2倍伤害，并让目标眩晕 1 秒。");
}
QString Goma::getClassName() const
{
    return QString("Goma");
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
QString Mag::getSkillDes() const
{
    return QString("技能【地热熔岩】:\n轰击目标所在的格子，对其造成1.5倍伤害。\n"
                   "同时使其相邻格子内的敌方单位每秒收到20点灼烧伤害，持续4秒。");
}
QString Mag::getClassName() const
{
    return QString("Mag");
}

FireblightGanon::FireblightGanon(const QString &name)
    :Unit(name,2700,90,1,90,Owner::EnemyCtrl,Profession::None)
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
QString FireblightGanon::getSkillDes() const
{
    return QString("大招：【熔岩爆裂斩】\n"
                   "大槌猛砸，对主目标造成 1.5 倍的高额物理伤害。\n"
                   "同时对目标周围一圈邻居格子的所有玩家英雄造成 40 点固定真实伤害。\n"
                   "半血被动【烈焰斗篷】：\n"
                   "立刻回复自己 300 滴血！并且攻击力提升 20%\n");
}
QString FireblightGanon::getClassName() const
{
    return QString("FireblightGanon");
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
QString Leo::getSkillDes() const
{
    return QString("技能【皇家庇护】:\n传送到血量百分比最少的我方单位身边，并为其回复250血量。");
}
QString Leo::getClassName() const
{
    return QString("Leo");
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
QString Alan::getSkillDes() const
{
    return QString("技能【横扫千军】:\n对周围格子内敌人群攻造成1.5倍伤害并使其眩晕0.5秒。");
}
QString Alan::getClassName() const
{
    return QString("Alan");
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
QString Syndra::getSkillDes() const
{
    return QString("技能【能量倾泻】:\n对目标造成2.5倍的单体魔法爆发伤害。");
}
QString Syndra::getClassName() const
{
    return QString("Syndra");
}

Ganondorf::Ganondorf(const QString &name)
    :Unit(name,5000,100,3,120,Owner::EnemyCtrl,Profession::None)
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
QString Ganondorf::getSkillDes() const
{
    return QString(" 第一阶段：黑暗化身\n"
                   "战斗表现：正常的远程暗黑波动攻击，充能较慢。\n"
                   "大招【暗黑冲击波】：对当前目标发射毁灭射线，造成 2.5 倍 伤害。\n"
                   "第二阶段：魔王狂暴(当血量跌破 70%时自动触发)\n"
                   "被动效果 A：【数值暴走】攻击力永久提升 30%。攻速加快。\n"
                   "被动效果B:【虚空霸体】加农道夫彻底免疫任何眩晕和迟缓控制。\n"
                   "第三阶段：灾厄核心(当血量跌破 30%时触发)。\n"
                   "被动效果 A：【灾厄壁垒】永久获得 50% 的伤害减免。\n"
                   "被动效果 B：【深渊自愈】他不再进行普通攻击。每隔 5 秒，他会自动疯狂回复自己最大生命值的 10%。\n"
                   "大招【大灾厄】：对全场玩家单位造成 100 点 的毁灭性真实伤害。");
}
QString Ganondorf::getClassName() const
{
    return QString("Ganondorf");
}
