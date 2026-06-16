#include "Game.h"
#include"Heroes.h"
#include"EquipmentSlotItem.h"
#include<QRandomGenerator>
#include<iostream>
#include<fstream>

namespace {
constexpr qreal kZGrid = 0.0;
constexpr qreal kZUnit = 1.0;
constexpr qreal kZDraggingUnit = 2.0;
}
const int FPS=1000/60;

Game::Game(int rows,int cols,QObject *parent)
    :QObject{parent}
    ,m_rows(rows)
    ,m_cols(cols)
    ,m_radius(46.0)
    ,m_board(rows,cols,true)
    ,m_bench(1,cols,false)
    ,m_scene(new QGraphicsScene(this))
    ,m_dragActive(false)
    ,m_activeUnitId(-1)
    ,m_sourcePos(QPoint(-1,-1))
    ,m_player(new Player)
    ,m_timer(new QTimer(this))
    ,m_phase(GamePhase::Prep)
    ,m_playerUnitInBoard(0)
    ,m_dragEquipActive(false)
    ,m_activeIndex(-1)
    ,m_dragEquipPos(QPointF(-1,-1))
{
    m_heroPools={"卓拉守卫-辛顿","卓拉战士-诺亚","卓拉祭司-露娜"
                ,"利特射手-力巴","利特舞者-卡西","利特风语者-艾文"
                ,"格鲁德士兵-乌尔","格鲁德咒师-娜吉","格鲁德刺客-希卡"
                ,"鼓隆铁卫-达鲁克","鼓隆狂战士-戈玛","鼓隆火焰祭司-玛格"
                ,"骑士团长-雷欧","皇家剑士-艾伦","宫廷法师-辛德拉"};
    m_EquipmentPools={Equipment::Sword,Equipment::Mail,Equipment::Gloves,Equipment::Crystal};

    //设置定时器
    connect(m_timer,&QTimer::timeout,this,&Game::gameTick);
}
Game::~Game(){
    qDeleteAll(m_units);
    m_units.clear();
    m_scene->clear();
    delete m_player;
}

//初始化相关
void Game::initialize(){
    startNewGame();
}
void Game::startNewGame() {

    m_scene->clear();
    qDeleteAll(m_units);
    m_units.clear();
    m_unitItems.clear();
    m_unitItemById.clear();

    m_player->setHp(100);
    m_player->setGold(10);
    m_player->setLevel(1);
    m_player->setMaxXP(2);
    m_player->setXp(0);
    m_player->setMaxUnit(3);
    m_player->initialStage();
    m_player->clearBless();
    m_board.clear();
    m_bench.clear();

    std::vector<QString> iniList=rollShop();
    for(int i=0;i<3;i++){
       Unit* unit=createHeroforPreview(iniList[i]).release();
        unit->setIsShop(false);
       m_units.push_back(unit);
    }
    const QPoint initialPositions[] = { QPoint(0,Board::ROWS), QPoint(1,Board::ROWS), QPoint(2,Board::ROWS) };
    for (int i = 0; i < 3; ++i) {
        m_bench.addUnit(m_units[i], initialPositions[i]);
        m_units[i]->setStartPos(initialPositions[i]);
    }

    generateEnemy();

    buildScene();

    m_phase = GamePhase::Prep;
    syncFromBoardAndBench();
}

//属性获取
QGraphicsScene* Game::getScene()const{
    return m_scene;
}

Unit *Game::getUnitById(int unitId) const
{
    for(Unit* unit:m_units){
        if(unit && unit->getId()==unitId){
            return unit;
        }
    }
    return nullptr;
}

UnitItem *Game::getUnitItem(int unitId) const
{
    auto it=m_unitItemById.find(unitId);
    if(it==m_unitItemById.end()){
        return nullptr;
    }
    return it->second;
}

Player *Game::getPlayer() const
{
    return m_player;
}

int Game::getPlayerUnitInBoard() const
{
    return m_playerUnitInBoard;
}

std::vector<QString> Game::getActivateSynergyList() const
{
    return m_activateSynergyList;
}

GridItem *Game::getGridItem(const QPoint &gridPos)const
{
    for(GridItem* item:m_gridItems){
        if(item && item->getPos()==gridPos){
            return item;
        }
    }
    for(GridItem* item:m_benchItems){
        if(item && item->getPos()==gridPos){
            return item;
        }
    }
    return nullptr;
}

QString Game::getRaceName(Race race) const
{
    switch(race){
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

QString Game::getProName(Profession pro) const
{
    switch(pro){
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

GamePhase Game::getPhase() const
{
    return m_phase;
}

EquipmentItem *Game::getEquipmentItem(int index) const
{
    auto it=m_equipmentByIndex.find(index);
    if(it==m_equipmentByIndex.end()){
        return nullptr;
    }
    return it->second;
}

bool Game::getEquipmentSlotFull()
{
    int count=0;
    for(int j=0;j<4;j++){
        if(m_equipmentByIndex[j] && m_equipmentByIndex[j]->getType()!=Equipment::None){
            count++;
            break;
        }
    }
    return count==4;
}

QString Game::getSynergyDes(Race race, Profession pro)
{
    switch(race){
    case Race::Hyrulean:
        return "全体生命+150/300";
        break;
    case Race::Zora:
        return "战斗前全体获得20/40法力";
        break;
    case Race::Goron:
        return "全体最大生命值+10%/20%";
        break;
    case Race::Gerudo:
        return "全体攻击力+10/20";
        break;
    case Race::Rito:
        return "全体攻击冷却减少10%/20%";
        break;
    default:
        switch(pro){
        case Profession::Warrior:
            return "全体攻击力+5/战士生命值低于50%时，造成的伤害提高 30%";
            break;
        case Profession::Archer:
            return "全体攻击距离+1/射手每攻击3次，下一次攻击造成150%伤害";
            break;
        case Profession::Mage:
            return "全体最大法力值减少20（至少保留20）/法师每次施放技能后，使目标眩晕0.5秒";
            break;
        case Profession::Assassin:
            return "全体攻击速度+15%/刺客的首次攻击必定造成200%伤害";
            break;
        case Profession::Guardian:
            return "全体生命+100/守卫在战斗开始时获得一次伤害免疫";
            break;
        default:
            return "";
        }
    }
}

//画棋盘
void Game::buildScene(){
    m_scene->clear();
    m_gridItems.clear();
    m_benchItems.clear();
    m_unitItems.clear();

    QRectF totalBounds;
    bool first=true;
    int curStage=m_player->getMajorStage();

    //棋盘
    for(int i=0;i<m_rows;++i){
        for(int j=0;j<m_cols;++j){
            GridItem* gridItem=new GridItem(i,j,m_radius,GridShape::Hexagon,curStage);
            gridItem->setZValue(kZGrid);
            gridItem->setBaseColor(i < m_rows / 2 ? QColor(80, 60, 60) : QColor(60, 60, 80));
            gridItem->setPos(gridToWorld(i,j,true));

            m_scene->addItem(gridItem);
            m_gridItems.push_back(gridItem);

            const QRectF bounds=gridItem->mapRectToScene(gridItem->boundingRect());
            totalBounds=first?bounds:totalBounds.united(bounds);
            first=false;
        }
    }

    //备战区
    for(int j=0;j<m_cols;++j){
        GridItem *benchItem=new GridItem(m_rows,j,m_radius,GridShape::Square,curStage);
        benchItem->setZValue(kZGrid);
        benchItem->setBaseColor(QColor(50, 50, 50));
        QPointF pos=gridToWorld(m_rows,j,false);
        benchItem->setPos(pos);

        m_scene->addItem(benchItem);
        m_benchItems.push_back(benchItem);

        const QRectF bounds=benchItem->mapRectToScene(benchItem->boundingRect());
        totalBounds=first?bounds:totalBounds.united(bounds);
        first=false;
    }

    //单位
    for(Unit *unit:m_units){
        UnitItem* item=nullptr;
        if(unit->getOwner()==Owner::PlayerCtrl){
            bool isBoard=unit->getPos().y()<Board::ROWS;
            item=new UnitItem(unit,isBoard);
        }
        else {
            item=new UnitItem(unit,true);
        }
        item->setZValue(kZUnit);
        m_scene->addItem(item);
        m_unitItems.push_back(item);
        m_unitItemById[unit->getId()]=item;

        if(unit->getOwner()==Owner::PlayerCtrl){
            connect(item,&UnitItem::dragStarted,this,&Game::onDragStarted);
            connect(item,&UnitItem::dragMoved,this,&Game::onDragMoved);
            connect(item,&UnitItem::dragDropped,this,&Game::onDragDropped);
        }
        connect(item,&UnitItem::clicked,this,&Game::onClicked);

        connect(unit,&Unit::infoChanged,item,&UnitItem::unitInfoChanged);
        connect(item,&UnitItem::unitInfoReflash,this,&Game::unitInfoChanged);
        connect(unit,&Unit::isDead,this,&Game::onUnitDead);
        connect(unit, &Unit::damaged, item, &UnitItem::onDamaged);
    }

    //装备栏
    for(int i=0;i<4;++i){
        EquipmentSlotItem *equipmentSlotItem = new EquipmentSlotItem(i);
        equipmentSlotItem->setZValue(kZGrid);
        m_scene->addItem(equipmentSlotItem);

        EquipmentItem *item=new EquipmentItem(Equipment::None,i);
        m_equipmentByIndex[i]=item;
        item->setZValue(kZUnit);
        m_scene->addItem(item);
        connect(item,&EquipmentItem::dragStarted,this,&Game::onEquipStarted);
        connect(item,&EquipmentItem::dragMoved,this,&Game::onEquipMoved);
        connect(item,&EquipmentItem::dragDropped,this,&Game::onEquipDropped);

        QPointF pos=gridToWorld(m_rows,m_cols,false);
        pos=QPointF(pos.x()+40,pos.y()-50*(3-i)+25);
        equipmentSlotItem->setPos(pos);
        item->setPos(pos);
        m_equipmentSlotPos.push_back(pos);

        const QRectF bounds=equipmentSlotItem->mapRectToScene(equipmentSlotItem->boundingRect());
        totalBounds=first?bounds:totalBounds.united(bounds);
        first=false;
    }

    m_scene->setSceneRect(totalBounds.adjusted(-40, -40, 40, 40));
}
void Game::syncFromBoardAndBench(){

    m_playerUnitInBoard=0; //用于判断能否开始
    for(UnitItem* item:m_unitItems){
        if(!item || !item->getUnit()){
            continue;
        }

        if(item->getUnit()->getState()==State::Dead){
            item->setVisible(false);
            continue;
        }

        //处理拖拽闪烁
        if(m_dragActive && item->getUnit()->getId()==m_activeUnitId){
            item->setZValue(kZDraggingUnit);
            continue;
        }

        QPoint pos=item->getUnit()->getPos();
        if(item->getIsBoard()){
            if(!m_board.isValidPosition(pos) || m_board.getUnitAt(pos)!=item->getUnit()){
                item->setVisible(false);
                continue;
            }
            else {
                if(item->getUnit()->getOwner()==Owner::PlayerCtrl){
                    m_playerUnitInBoard++;
                }
            }
        }
        else {
            if(!m_bench.isValidPosition(pos) || m_bench.getUnitAt(pos)!=item->getUnit()){
                item->setVisible(false);
                continue;
            }
        }

        item->setVisible(true);
        item->setZValue(kZUnit);
        QPointF worldPos=item->getIsBoard()?gridToWorld(pos.y(),pos.x(),true):gridToWorld(pos.y(),pos.x(),false);

        if (item->pos() != worldPos) {
            item->slidePosTo(worldPos);
            if(!item->getIsBoard()){
                item->setGridPos(QPoint(pos.x(),pos.y()));
            }
            else{
                item->setGridPos(pos);
            }
        }
    }
    if(m_phase==GamePhase::Prep)calculateSynergies();
    emit boardUpdate(m_playerUnitInBoard);
    m_scene->update();
}

void Game::clearGridHighLights()
{
    for(GridItem* item:m_gridItems){
        if(!item){
            continue;
        }
        item->setHoverActive(false);
        item->setDropActive(false);
        item->setRangeActive(false);
    }
    for(GridItem* item:m_benchItems){
        if(!item){
            continue;
        }
        item->setHoverActive(false);
        item->setDropActive(false);
    }
}

bool Game::canApplyDrop(int unitId, const QPoint &sourcePos, const QPoint &target)
{
    Unit* unit=getUnitById(unitId);
    if(!unit){
        return false;
    }

    if(sourcePos.y()==m_rows){
        if(target.y()==m_rows){
            if(sourcePos.x()<0 || sourcePos.x()>=m_cols || target.x()<0 || target.x()>=m_cols){
                return false;
            }
            if(sourcePos==target){
                return false;
            }
        }
        else {
            if(m_playerUnitInBoard>=m_player->getMaxUnit() && !m_board.hasUnitAt(target)){
                return false;
            }
            if(sourcePos.x()<0 || sourcePos.x()>=m_cols || !m_board.isValidPosition(target)){
                return false;
            }
            if(!m_board.isPlayerHalf(target)){
                return false;
            }
        }
    }
    else {
        if(target.y()==m_rows){
            if(!m_board.isValidPosition(sourcePos) || target.x()<0 || target.x()>=m_cols){
                return false;
            }
            if(!m_board.isPlayerHalf(sourcePos)){
                return false;
            }
        }
        else {
            if(!m_board.isValidPosition(sourcePos) || !m_board.isValidPosition(target)){
                return false;
            }
            if(!m_board.isPlayerHalf(sourcePos) || !m_board.isPlayerHalf(target)){
                return false;
            }
            if(sourcePos==target){
                return false;
            }
        }
    }

    return true;

}

void Game::applyDrop(int unitId, const QPoint &sourcePos, const QPoint &target)
{
    Unit* unit=getUnitById(unitId);
    UnitItem* item=getUnitItem(unitId);
    if(!unit || !item){
        return;
    }
    if(sourcePos.y()==m_rows){
        if(target.y()==m_rows){
            Unit* targetUnit=m_bench.getUnitAt(target);
            if(targetUnit){
                m_bench.removeUnit(targetUnit);
                m_bench.removeUnit(unit);
                m_bench.addUnit(targetUnit,sourcePos);
                m_bench.addUnit(unit,target);
                unit->setStartPos(unit->getPos());
                targetUnit->setStartPos(targetUnit->getPos());
            }
            else {
                m_bench.moveUnit(unit,target);
                unit->setStartPos(unit->getPos());
            }
        }
        else {
            Unit* targetUnit=m_board.getUnitAt(target);
            UnitItem* targetItem=nullptr;
            if(targetUnit){
                targetItem=getUnitItem(targetUnit->getId());
                m_board.removeUnit(targetUnit);
                m_bench.removeUnit(unit);
                m_board.addUnit(unit,target);
                item->setIsBoard(true);
                m_bench.addUnit(targetUnit,sourcePos);
                targetItem->setIsBoard(false);
                unit->setStartPos(unit->getPos());
                targetUnit->setStartPos(targetUnit->getPos());
            }
            else{
                m_bench.removeUnit(unit);
                m_board.addUnit(unit,target);
                item->setIsBoard(true);
                unit->setStartPos(unit->getPos());
            }
        }
    }
    else {
        if(target.y()==m_rows){
            Unit* targetUnit=m_bench.getUnitAt(target);
            UnitItem* targetItem=nullptr;
            if(targetUnit){
                targetItem=getUnitItem(targetUnit->getId());
                m_bench.removeUnit(targetUnit);
                m_board.removeUnit(unit);
                m_bench.addUnit(unit,target);
                item->setIsBoard(false);
                m_board.addUnit(targetUnit,sourcePos);
                targetItem->setIsBoard(true);
                unit->setStartPos(unit->getPos());
                targetUnit->setStartPos(targetUnit->getPos());
            }
            else{
                m_board.removeUnit(unit);
                m_bench.addUnit(unit,target);
                item->setIsBoard(false);
                unit->setStartPos(unit->getPos());
            }
            unit->restoreOriAtt();
        }
        else {
            Unit* targetUnit=m_board.getUnitAt(target);
            if(targetUnit){
                m_board.removeUnit(targetUnit);
                m_board.removeUnit(unit);
                m_board.addUnit(targetUnit,sourcePos);
                m_board.addUnit(unit,target);
                unit->setStartPos(unit->getPos());
                targetUnit->setStartPos(targetUnit->getPos());
            }
            else{
                m_board.moveUnit(unit,target);
                unit->setStartPos(unit->getPos());
            }
        }
    }
}
QPointF Game::gridToWorld(int row, int col,bool isBoard) const{
    qreal w = m_radius * qSqrt(3.0);

    qreal x = col * w;
    if (row % 2 != 0) {
        x += w / 2.0;
    }

    qreal y = row * (m_radius * 1.5);

    return isBoard?QPointF(x, y):QPointF(x-0.4*m_radius+40,y+80);
}

QPoint Game::worldToGrid(QPointF worldPos) const
{
    QPoint best(-1,-1);
    qreal distance=1e18;

    for(int row=0;row<=m_rows;++row){
        for(int col=0;col<m_cols;++col){
            QPointF center=gridToWorld(row,col,row==m_rows?false:true);
            qreal dx=worldPos.x()-center.x();
            qreal dy=worldPos.y()-center.y();
            qreal d=dx*dx+dy*dy;
            if(d<distance){
                distance=d;
                best=QPoint(col,row);
            }
        }
    }

    return best;
}

void Game::applySynergyBuffs(std::map<Race, int> raceCount, std::map<Profession, int> proCount, Owner owner)
{
    int hyruleanCount=raceCount[Race::Hyrulean];
    int hyrulean=hyruleanCount<2?0:(hyruleanCount>=4?300:150);

    int gerudoCount=raceCount[Race::Gerudo];
    int gerudo=gerudoCount<2?0:(gerudoCount>=4?20:10);

    int goronCount=raceCount[Race::Goron];
    int goron=goronCount<2?10:(goronCount>=4?12:11);

    int ritoCount=raceCount[Race::Rito];
    int rito=ritoCount<2?10:(ritoCount>=4?8:9);

    int zoraCount=raceCount[Race::Zora];
    int zora=zoraCount<2?0:(zoraCount>=4?40:20);

    int warriorCount=proCount[Profession::Warrior];
    int warrior=warriorCount>=2?5:0;
    bool warriorSyn=warriorCount>=3?true:false;

    int archerCount=proCount[Profession::Archer];
    int archer=archerCount>=2?4:3;
    bool archerSyn=archerCount>=3?true:false;

    int mageCount=proCount[Profession::Mage];
    int mage=mageCount>=2?20:0;
    bool mageSyn=mageCount>=3?true:false;

    int assassinCount=proCount[Profession::Assassin];
    int assassin=assassinCount>=2?115:100;
    bool assassinSyn=assassinCount>=3?true:false;

    int guardianCount=proCount[Profession::Guardian];
    int guardian=guardianCount>=2?100:0;
    bool guardianSyn=guardianCount>=3?true:false;

    for(Unit* unit:m_units){
        if(!unit || unit->getState()==State::Dead || unit->getPos().y()==Board::ROWS || unit->getOwner()!=owner || unit->getRace()==Race::Boss){
            continue;
        }
        unit->restoreOriAtt();
        Race race=unit->getRace();
        Profession pro=unit->getProfession();

        if(m_player->hasWaterBless()){
            unit->setMaxHp(unit->getMaxHp()+60);
            unit->setHp(unit->getMaxHp());
        }
        if(m_player->hasWindBless()){
            unit->setOriAtkCoolDown(unit->getOriATkCoolDown()*9/10);
        }
        if(m_player->hasThunderBless()){
            unit->setMaxMana(qMax(unit->getMaxMana()-10,30));
        }
        if(m_player->hasFireBless()){
            unit->setAtk(unit->getAtk()+10);
        }

        switch(race){
        case Race::Hyrulean:
            unit->setMaxHp(unit->getMaxHp()+hyrulean);
            unit->setHp(unit->getMaxHp());
            break;
        case Race::Gerudo:
            unit->setAtk(unit->getAtk()+gerudo);
            break;
        case Race::Goron:
            unit->setMaxHp(unit->getMaxHp()*goron/10);
            unit->setHp(unit->getMaxHp());
            break;
        case Race::Rito:
            unit->setOriAtkCoolDown(unit->getOriATkCoolDown()*rito/10);
            break;
        case Race::Zora:
            unit->setMana(zora);
            break;
        default:
            break;
        }
        switch(pro){
        case Profession::Warrior:
            unit->setAtk(unit->getAtk()+warrior);
            unit->m_warriorSyn=warriorSyn;
            break;
        case Profession::Archer:
            unit->setRange(archer);
            unit->m_archerSyn=archerSyn;
            break;
        case Profession::Mage:
            unit->setMaxMana(qMax(unit->getMaxMana()-mage,30));
            unit->m_mageSyn=mageSyn;
            break;
        case Profession::Assassin:
            unit->setOriAtkCoolDown(unit->getOriATkCoolDown()*100/assassin);
            unit->m_assassinSyn=assassinSyn;
            break;
        case Profession::Guardian:
            unit->setMaxHp(unit->getMaxHp()+guardian);
            unit->setHp(unit->getMaxHp());
            unit->m_guardianSyn=guardianSyn;
            break;
        default:
            break;
        }
    }
}

bool Game::canApplyEquipDrop(int index, const QPoint &target)
{
    EquipmentItem* item=getEquipmentItem(index);
    if(!item){
        return false;
    }
    Unit* unit=nullptr;
    if(target.y()==Board::ROWS){
        unit=m_bench.getUnitAt(target);
    }
    else unit=m_board.getUnitAt(target);
    if(!unit || unit->getOwner()==Owner::EnemyCtrl || unit->getEquipment()!=Equipment::None){
        return false;
    }
    return true;
}

void Game::applyEquipmentDrop(int index, const QPoint &target)
{
    EquipmentItem* item=getEquipmentItem(index);
    if(!item)return;
    Unit* unit=nullptr;
    if(target.y()==Board::ROWS){
        unit=m_bench.getUnitAt(target);
    }
    else unit=m_board.getUnitAt(target);
    if(!unit)return;

    unit->addEquipment(item->getType());

    item->setType(Equipment::None);
    item->setPos(m_equipmentSlotPos[index]);
    UnitItem* UItem=getUnitItem(unit->getId());
    if(UItem){
        UItem->update();
    }
    syncFromBoardAndBench();
}

void Game::generateRandomEquip()
{
    bool generateEquip=QRandomGenerator::global()->bounded(100)<50?true:false;
    int randomIndex=QRandomGenerator::global()->bounded(m_EquipmentPools.size());
    if(generateEquip){
        int find=-1;
        for(int i=0;i<4;i++){
            if(m_equipmentByIndex[i] && m_equipmentByIndex[i]->getType()==Equipment::None){
                find=i;
                break;
            }
        }
        if(find!=-1){
            m_equipmentByIndex[find]->setType(m_EquipmentPools[randomIndex]);
        }
    }

}

//敌人生成
void Game::generateEnemy()
{
    int round=(m_player->getMajorStage()-1)*4+m_player->getMinorStage();
    qreal scale=1.0+(m_player->getMajorStage()-1)*0.15+(m_player->getMinorStage()-1)*0.03;

    switch(round){
    case 1:{
        Unit* enemy1=new Noah("腐化-卓拉战士",Owner::EnemyCtrl,scale);
        m_units.push_back(enemy1);
        m_board.addUnit(enemy1,QPoint(3,2));
        break;
    }
    case 2:{
        Unit* enemy1=new Noah("腐化-卓拉战士",Owner::EnemyCtrl,scale);
        Unit* enemy2=new Luna("腐化-卓拉祭司",Owner::EnemyCtrl,scale);

        m_units.push_back(enemy1);
        m_units.push_back(enemy2);

        m_board.addUnit(enemy1,QPoint(2,2));
        m_board.addUnit(enemy2,QPoint(4,0));
        break;
    }
    case 3:{
        Unit* enemy1=new Noah("腐化-卓拉战士",Owner::EnemyCtrl,scale);
        Unit* enemy2=new Sidon("腐化-卓拉守卫",Owner::EnemyCtrl,scale);
        Unit* enemy3=new Luna("腐化-卓拉祭司",Owner::EnemyCtrl,scale);

        m_units.push_back(enemy1);
        m_units.push_back(enemy2);
        m_units.push_back(enemy3);

        m_board.addUnit(enemy1,QPoint(2,2));
        m_board.addUnit(enemy2,QPoint(3,3));
        m_board.addUnit(enemy3,QPoint(4,0));
        break;
    }
    case 4:{
        Unit* boss=new WaterblightGanon("水咒盖侬");
        Unit* enemy1=new Luna("腐化-卓拉祭司",Owner::EnemyCtrl,scale);
        Unit* enemy2=new Luna("腐化-卓拉祭司",Owner::EnemyCtrl,scale);

        m_units.push_back(enemy1);
        m_units.push_back(enemy2);
        m_units.push_back(boss);

        m_board.addUnit(enemy1,QPoint(3,1));
        m_board.addUnit(enemy2,QPoint(4,1));
        m_board.addUnit(boss,QPoint(4,0));
        break;
    }
    case 5:{
        Unit* enemy1=new Revali("腐化-利特射手",Owner::EnemyCtrl,scale);
        Unit* enemy2=new Evan("腐化-利特风语者",Owner::EnemyCtrl,scale);

        m_units.push_back(enemy1);
        m_units.push_back(enemy2);

        m_board.addUnit(enemy1,QPoint(2,0));
        m_board.addUnit(enemy2,QPoint(5,0));
        break;
    }
    case 6:{
        Unit* enemy1=new Revali("腐化-利特射手",Owner::EnemyCtrl,scale);
        Unit* enemy2=new Evan("腐化-利特风语者",Owner::EnemyCtrl,scale);
        Unit* enemy3=new Kashi("腐化-利特舞者",Owner::EnemyCtrl,scale);

        m_units.push_back(enemy1);
        m_units.push_back(enemy2);
        m_units.push_back(enemy3);

        m_board.addUnit(enemy1,QPoint(3,0));
        m_board.addUnit(enemy2,QPoint(5,0));
        m_board.addUnit(enemy3,QPoint(2,3));
        break;
    }
    case 7:{
        Unit* enemy1=new Revali("腐化-利特射手",Owner::EnemyCtrl,scale);
        Unit* enemy2=new Revali("腐化-利特射手",Owner::EnemyCtrl,scale);
        Unit* enemy3=new Evan("腐化-利特风语者",Owner::EnemyCtrl,scale);
        Unit* enemy4=new Kashi("腐化-利特舞者",Owner::EnemyCtrl,scale);

        m_units.push_back(enemy1);
        m_units.push_back(enemy2);
        m_units.push_back(enemy3);
        m_units.push_back(enemy4);

        m_board.addUnit(enemy1,QPoint(2,0));
        m_board.addUnit(enemy2,QPoint(4,0));
        m_board.addUnit(enemy3,QPoint(3,1));
        m_board.addUnit(enemy4,QPoint(5,3));
        break;
    }
    case 8:{
        Unit* boss=new WindblightGanon("风咒盖侬");
        Unit* enemy1=new Kashi("腐化-利特舞者",Owner::EnemyCtrl,scale);

        m_units.push_back(enemy1);
        m_units.push_back(boss);

        m_board.addUnit(enemy1,QPoint(3,3));
        m_board.addUnit(boss,QPoint(3,0));
        break;
    }
    case 9:{
        Unit* enemy1=new Ur("腐化-格鲁德队长",Owner::EnemyCtrl,scale,2);
        Unit* enemy2=new Naji("腐化-格鲁德咒师",Owner::EnemyCtrl,scale,2);

        m_units.push_back(enemy1);
        m_units.push_back(enemy2);

        m_board.addUnit(enemy1,QPoint(3,2));
        m_board.addUnit(enemy2,QPoint(4,0));
        break;
    }
    case 10:{
        Unit* enemy1=new Ur("腐化-格鲁德队长",Owner::EnemyCtrl,scale,2);
        Unit* enemy2=new Shika("腐化-格鲁德刺客",Owner::EnemyCtrl,scale,2);
        Unit* enemy3=new Naji("腐化-格鲁德咒师",Owner::EnemyCtrl,scale,2);

        m_units.push_back(enemy1);
        m_units.push_back(enemy2);
        m_units.push_back(enemy3);

        m_board.addUnit(enemy1,QPoint(2,2));
        m_board.addUnit(enemy2,QPoint(3,3));
        m_board.addUnit(enemy3,QPoint(4,0));
        break;
    }
    case 11:{
        Unit* enemy1=new Ur("腐化-格鲁德队长",Owner::EnemyCtrl,scale,2);
        Unit* enemy2=new Shika("腐化-格鲁德刺客",Owner::EnemyCtrl,scale,2);
        Unit* enemy3=new Shika("腐化-格鲁德刺客",Owner::EnemyCtrl,scale,2);
        Unit* enemy4=new Shika("腐化-格鲁德刺客",Owner::EnemyCtrl,scale,2);

        m_units.push_back(enemy1);
        m_units.push_back(enemy2);
        m_units.push_back(enemy3);
        m_units.push_back(enemy4);

        m_board.addUnit(enemy1,QPoint(3,2));
        m_board.addUnit(enemy2,QPoint(1,3));
        m_board.addUnit(enemy3,QPoint(5,3));
        m_board.addUnit(enemy4,QPoint(4,0));
        break;
    }
    case 12:{
        Unit* boss=new ThunderblightGanon("雷咒盖侬");
        Unit* enemy1=new Ur("腐化-格鲁德队长",Owner::EnemyCtrl,scale,2);
        Unit* enemy2=new Naji("腐化-格鲁德咒师",Owner::EnemyCtrl,scale,2);

        m_units.push_back(enemy1);
        m_units.push_back(enemy2);
        m_units.push_back(boss);

        m_board.addUnit(enemy1,QPoint(3,2));
        m_board.addUnit(enemy2,QPoint(4,0));
        m_board.addUnit(boss,QPoint(3,0));
        break;
    }
    case 13:{
        Unit* enemy1=new Daruk("腐化-鼓隆铁卫",Owner::EnemyCtrl,scale,2);
        Unit* enemy2=new Goma("腐化-鼓隆狂战士",Owner::EnemyCtrl,scale,2);

        m_units.push_back(enemy1);
        m_units.push_back(enemy2);

        m_board.addUnit(enemy1,QPoint(3,3));
        m_board.addUnit(enemy2,QPoint(4,2));
        break;
    }
    case 14:{
        Unit* enemy1=new Daruk("腐化-鼓隆铁卫",Owner::EnemyCtrl,scale,2);
        Unit* enemy2=new Goma("腐化-鼓隆狂战士",Owner::EnemyCtrl,scale,2);
        Unit* enemy3=new Mag("腐化-鼓隆火焰祭司",Owner::EnemyCtrl,scale,2);

        m_units.push_back(enemy1);
        m_units.push_back(enemy2);
        m_units.push_back(enemy3);

        m_board.addUnit(enemy1,QPoint(3,3));
        m_board.addUnit(enemy2,QPoint(2,2));
        m_board.addUnit(enemy3,QPoint(4,0));
        break;
    }
    case 15:{
        Unit* enemy1=new Daruk("腐化-鼓隆铁卫",Owner::EnemyCtrl,scale,2);
        Unit* enemy2=new Daruk("腐化-鼓隆铁卫",Owner::EnemyCtrl,scale,2);
        Unit* enemy3=new Goma("腐化-鼓隆狂战士",Owner::EnemyCtrl,scale,2);
        Unit* enemy4=new Mag("腐化-鼓隆火焰祭司",Owner::EnemyCtrl,scale,2);

        m_units.push_back(enemy1);
        m_units.push_back(enemy2);
        m_units.push_back(enemy3);
        m_units.push_back(enemy4);

        m_board.addUnit(enemy1,QPoint(2,3));
        m_board.addUnit(enemy2,QPoint(4,3));
        m_board.addUnit(enemy3,QPoint(3,2));
        m_board.addUnit(enemy4,QPoint(3,0));
        break;
    }
    case 16:{
        Unit* boss=new FireblightGanon("火咒盖侬");
        Unit* enemy1=new Daruk("腐化-鼓隆铁卫",Owner::EnemyCtrl,scale,2);
        Unit* enemy2=new Mag("腐化-鼓隆火焰祭司",Owner::EnemyCtrl,scale,2);

        m_units.push_back(enemy1);
        m_units.push_back(enemy2);
        m_units.push_back(boss);

        m_board.addUnit(enemy1,QPoint(3,2));
        m_board.addUnit(enemy2,QPoint(4,0));
        m_board.addUnit(boss,QPoint(3,0));
        break;
    }
    case 17:{
        Unit* boss=new Ganondorf("灾厄盖侬");
        m_units.push_back(boss);
        m_board.addUnit(boss,QPoint(3,0));
        break;
    }

    }

    if(round!=1){
        for(Unit* unit:m_units){
            UnitItem* item=nullptr;
            if(unit->getOwner()==Owner::EnemyCtrl){
                item=new UnitItem(unit,true);
            }
            else {
                continue;
            }
            if(item){
                item->setZValue(kZUnit);
                m_scene->addItem(item);
                m_unitItems.push_back(item);
                m_unitItemById[unit->getId()]=item;
                connect(item,&UnitItem::clicked,this,&Game::onClicked);

                connect(unit,&Unit::infoChanged,item,&UnitItem::unitInfoChanged);
                connect(item,&UnitItem::unitInfoReflash,this,&Game::unitInfoChanged);
                connect(unit,&Unit::isDead,this,&Game::onUnitDead);
                connect(unit, &Unit::damaged, item, &UnitItem::onDamaged);
            }
        }
    }

    syncFromBoardAndBench();
    m_scene->update();

}

//属性面板
void Game::onClicked(Unit *unit)
{

    UnitItem* item=getUnitItem(unit->getId());
    bool flag=item->getIsSelected();
    clearAllSelected();
    item->setIsSelected(flag);
    if(item->getIsSelected()){
        emit unitSelected(unit);
    }
    else {
        emit unitSelected(nullptr);
    }
}

void Game::clearAllSelected()
{
    for(UnitItem* item:m_unitItems){
        item->setIsSelected(false);
    }
    m_scene->update();
}

//拖拽
void Game::onDragStarted(int unitId, const QPoint &sourcePos, const QPointF &worldPos)
{
    if(m_phase!=GamePhase::Prep){
        return;
    }
    m_dragActive=true;
    m_activeUnitId=unitId;
    m_sourcePos=sourcePos;

    UnitItem * item=getUnitItem(unitId);
    if(item){
        item->setZValue(kZDraggingUnit);
    }
}

void Game::onDragMoved(int unitId, const QPoint &sourcePos, const QPointF &worldPos)
{
    if(!m_dragActive){
        return;
    }
    if(m_phase!=GamePhase::Prep){
        return;
    }
    UnitItem* item=getUnitItem(unitId);
    if(item){
        item->setPos(worldPos);
    }
    clearGridHighLights();

    const QPoint target=worldToGrid(worldPos);
    GridItem* targetItem=getGridItem(target);
    if(!targetItem){
        return;
    }
    targetItem->setHoverActive(true);
    if(canApplyDrop(unitId,m_sourcePos,target)){
        targetItem->setDropActive(true);
        Unit* putUnit=getUnitById(unitId);
        QSet rangeGrids=m_board.getRangeGrid(QPoint(target.x(),target.y()),putUnit->getRange());
        for(QPoint rangeGrid:rangeGrids){
            GridItem* rangeItem=getGridItem(rangeGrid);
            rangeItem->setRangeActive(true);
        }
    }
}

void Game::onDragDropped(int unitId, const QPoint &sourcePos, const QPointF &worldPos)
{
    if(!m_dragActive){
        return;
    }
    QPoint target=worldToGrid(worldPos);
    clearGridHighLights();
    if(canApplyDrop(unitId,m_sourcePos,target)){
        applyDrop(unitId,m_sourcePos,target);
    }

    UnitItem* item=getUnitItem(unitId);
    if(item){
        item->setZValue(kZUnit);
    }

    m_dragActive=false;
    m_activeUnitId=-1;
    m_sourcePos=QPoint(-1,-1);


    syncFromBoardAndBench();
}

void Game::onEquipStarted(int index, Equipment type, const QPointF &worldPos)
{
    if(m_phase!=GamePhase::Prep){
        return;
    }
    m_dragEquipActive=true;
    m_activeIndex=index;
    m_dragEquipPos=m_equipmentSlotPos[index];

    EquipmentItem* item=getEquipmentItem(index);
    if(item){
        item->setZValue(kZDraggingUnit);
    }
}

void Game::onEquipMoved(int index, Equipment type, const QPointF &worldPos)
{
    if(!m_dragEquipActive){
        return;
    }
    if(m_phase!=GamePhase::Prep){
        return;
    }
    EquipmentItem* item=getEquipmentItem(index);
    if(item){
        item->setPos(worldPos);
    }

}

void Game::onEquipDropped(int index, Equipment type, const QPointF &worldPos)
{
    if(!m_dragEquipActive){
        return;
    }
    QPoint target=worldToGrid(worldPos);
    EquipmentItem* item=getEquipmentItem(index);
    if(item){
        if(canApplyEquipDrop(index,target)){
            applyEquipmentDrop(index,target);
        }else{
            item->setPos(m_dragEquipPos);
        }
        item->setZValue(kZUnit);
    }

    m_dragEquipActive=false;
    m_activeIndex=-1;
    m_dragEquipPos=QPointF(-1,-1);
}

//游戏逻辑
void Game::gameTick()
{
    if(m_phase!=GamePhase::Combat)return;
    int playerLive=0,enemyLive=0;
    for (Unit* unit : m_units) {
        if (!unit) continue;
        unit->updateUnit(m_board,m_units);
        if(unit->getState()!=State::Dead && unit->getPos().y()<m_rows){
            if(unit->getOwner()==Owner::PlayerCtrl){
                playerLive++;
            }
            else {
                enemyLive++;
            }
        }
    }

    if(playerLive==0 || enemyLive==0){
        m_phase=GamePhase::Resolve;
        m_timer->stop();
        emit gameIsCombat(false);
        handleStageResolve(playerLive>0);
    }

    syncFromBoardAndBench();
}

void Game::onClickStartBtn()
{
    if(m_phase==GamePhase::Prep){
        for(Unit* unit:m_units){
            if(unit->getOwner()==Owner::PlayerCtrl){
                unit->setStartPos(unit->getPos());
            }
        }
        m_phase=GamePhase::Combat;
        emit gameIsCombat(true);
        m_timer->start(FPS);
    }
}

void Game::onUnitDead(Unit *unit)
{
    if(unit && unit->getState()==State::Dead){
        m_board.removeUnit(unit);

        syncFromBoardAndBench();
        m_scene->update();
    }
}

void Game::handleStageResolve(bool win)
{
    if(win){
        m_player->changeGold(4);
        m_player->addXp(3);
        if(m_player->getMinorStage()==4){
            m_player->activateBless(m_player->getMajorStage());
        }
    }
    else {
        int total=0;
        for(Unit* unit:m_units){
            if(unit->getOwner()==Owner::EnemyCtrl && unit->getState()!=State::Dead){
                total+=3;
            }
        }
        total=qBound(5,total,20);
        m_player->setHp(m_player->getHp()-total);
        m_player->changeGold(2);
        m_player->addXp(1);
    }
    if(m_player->getHp()<=0 || m_player->getMajorStage()==5){
        emit gameOver(win);
    }
    else{
        generateRandomEquip();
        m_player->nxtStage();
        emit roundFinishend(win,m_player->getGold(),m_player->getHp());
    }

}

void Game::clearEnemyBeforeRound()
{
    for(auto it=m_units.begin();it!=m_units.end();){
        Unit* enemy=*it;
        if(enemy && enemy->getOwner()==Owner::EnemyCtrl){
            m_board.removeUnit(enemy);

            UnitItem* item=getUnitItem(enemy->getId());
            if(item){
                m_scene->removeItem(item);

                auto itemIt=std::find(m_unitItems.begin(),m_unitItems.end(),item);
                if(itemIt!=m_unitItems.end()){
                    m_unitItems.erase(itemIt);
                }
                delete item;
            }

            m_unitItemById.erase(enemy->getId());

            delete enemy;

            it=m_units.erase(it);
        }
        else {
            it++;
        }
    }
}

bool Game::buyHero(int gold,QString name)
{
    if(m_player->getGold()+gold<0){
        return false;
    }
    m_player->changeGold(gold);
    QPoint pos(-1,-1);
    for(int i=0;i<m_cols;i++){
        if(!m_bench.hasUnitAt(QPoint(i,Board::ROWS))){
            pos=QPoint(i,Board::ROWS);
            break;
        }
    }

    if(m_bench.isValidPosition(pos)){
        Unit * unit=createHeroforPreview(name).release();
        unit->setIsShop(false);
        m_units.push_back(unit);
        m_bench.addUnit(unit,pos);
        unit->setStartPos(pos);
        UnitItem* item=new UnitItem(unit,false);
        item->setZValue(kZUnit);
        m_scene->addItem(item);
        m_unitItems.push_back(item);
        m_unitItemById[unit->getId()]=item;

        connect(item,&UnitItem::dragStarted,this,&Game::onDragStarted);
        connect(item,&UnitItem::dragMoved,this,&Game::onDragMoved);
        connect(item,&UnitItem::dragDropped,this,&Game::onDragDropped);
        connect(item,&UnitItem::clicked,this,&Game::onClicked);

        connect(unit,&Unit::infoChanged,item,&UnitItem::unitInfoChanged);
        connect(item,&UnitItem::unitInfoReflash,this,&Game::unitInfoChanged);
        connect(unit,&Unit::isDead,this,&Game::onUnitDead);
        connect(unit, &Unit::damaged, item, &UnitItem::onDamaged);

        if(m_phase==GamePhase::Prep){
            upUnitStar(name,1);
            upUnitStar(name,2);
        }
    }


    syncFromBoardAndBench();
    return true;
}

void Game::sellHero(Unit *unit)
{
    if(!unit){
        return;
    }
    int star=unit->getStar();
    int gold=2*star*star-3*star+3;
    Equipment equip=unit->getEquipment();
    if(equip!=Equipment::None){
        int find=-1;
        for(int j=0;j<4;j++){
            if(m_equipmentByIndex[j] && m_equipmentByIndex[j]->getType()==Equipment::None){
                find=j;
                break;
            }
        }
        if(find!=-1){
            m_equipmentByIndex[find]->setType(equip);
        }
        else {
            gold+=2;
        }
    }

    m_player->changeGold(gold);

    UnitItem* item=getUnitItem(unit->getId());
    if(item){
        if(item->getIsBoard()){
            m_board.removeUnit(unit);
        }
        else {
            m_bench.removeUnit(unit);
        }
        m_scene->removeItem(item);

        auto itemIt=std::find(m_unitItems.begin(),m_unitItems.end(),item);
        if(itemIt!=m_unitItems.end()){
            m_unitItems.erase(itemIt);
        }
        delete item;
    }

    m_unitItemById.erase(unit->getId());

    auto it=std::find(m_units.begin(),m_units.end(),unit);
    if(it!=m_units.end()){
        m_units.erase(it);
    }
    delete unit;

    emit unitSelled(nullptr);
    syncFromBoardAndBench();
}

std::unique_ptr<Unit> Game::createHeroforPreview(QString name,int star)
{
    if(name.isEmpty()){
        return nullptr;
    }
    if(name=="卓拉战士-诺亚"){
        return std::make_unique<Noah>(name,Owner::PlayerCtrl,1,star,true);
    } else if(name=="卓拉守卫-辛顿"){
        return std::make_unique<Sidon>(name,Owner::PlayerCtrl,1,star,true);
    } else if(name=="卓拉祭司-露娜"){
        return std::make_unique<Luna>(name,Owner::PlayerCtrl,1,star,true);
    } else if(name=="利特射手-力巴"){
        return std::make_unique<Revali>(name,Owner::PlayerCtrl,1,star,true);
    } else if(name=="利特舞者-卡西"){
        return std::make_unique<Kashi>(name,Owner::PlayerCtrl,1,star,true);
    } else if(name=="利特风语者-艾文"){
        return std::make_unique<Evan>(name,Owner::PlayerCtrl,1,star,true);
    } else if(name=="格鲁德士兵-乌尔"){
        return std::make_unique<Ur>(name,Owner::PlayerCtrl,1,star,true);
    } else if(name=="格鲁德咒师-娜吉"){
        return std::make_unique<Naji>(name,Owner::PlayerCtrl,1,star,true);
    } else if(name=="格鲁德刺客-希卡"){
        return std::make_unique<Shika>(name,Owner::PlayerCtrl,1,star,true);
    } else if(name=="鼓隆铁卫-达鲁克"){
        return std::make_unique<Daruk>(name,Owner::PlayerCtrl,1,star,true);
    } else if(name=="鼓隆狂战士-戈玛"){
        return std::make_unique<Goma>(name,Owner::PlayerCtrl,1,star,true);
    } else if(name=="鼓隆火焰祭司-玛格"){
        return std::make_unique<Mag>(name,Owner::PlayerCtrl,1,star,true);
    } else if(name=="骑士团长-雷欧"){
        return std::make_unique<Leo>(name,Owner::PlayerCtrl,1,star,true);
    } else if(name=="皇家剑士-艾伦"){
        return std::make_unique<Alan>(name,Owner::PlayerCtrl,1,star,true);
    } else if(name=="宫廷法师-辛德拉"){
        return std::make_unique<Syndra>(name,Owner::PlayerCtrl,1,star,true);
    }
}

void Game::calculateSynergies()
{
    m_raceCount.clear();
    m_professionCount.clear();

    std::map<Race,int> enemyRaceCount;
    std::map<Profession,int> enemyProfessionCount;

    for(Unit* unit:m_units){
        if( !unit || unit->getPos().y()==Board::ROWS ||unit->getState()==State::Dead || unit->getPos().y()<0){
            continue;
        }
        else if(unit->getOwner()==Owner::PlayerCtrl){
            m_raceCount[unit->getRace()]++;
            m_professionCount[unit->getProfession()]++;
        }
        else {
            enemyRaceCount[unit->getRace()]++;
            enemyProfessionCount[unit->getProfession()]++;
        }
    }

    m_activateSynergyList.clear();
    for(const auto& racePair:m_raceCount){
        if(racePair.second>0){
            QString name=getRaceName(racePair.first);
            int target=racePair.second>=2?4:2;
            QString text=racePair.second>=2?QString("(已激活第%1级)").arg(racePair.second>=4?2:1):"";
            QString append=getSynergyDes(racePair.first,Profession::None);
            m_activateSynergyList.push_back(QString("%1 : %2/%3 %4\n%5").arg(name).arg(racePair.second).arg(target).arg(text).arg(append));
        }
    }
    for(const auto& ProPair:m_professionCount){
        if(ProPair.second>0){
            QString name=getProName(ProPair.first);
            int target=ProPair.second>=2?3:2;
            QString text=ProPair.second>=2?QString("(已激活第%1级)").arg(ProPair.second>=3?2:1):"";
            QString append=getSynergyDes(Race::Boss,ProPair.first);
            m_activateSynergyList.push_back(QString("%1 : %2/%3 %4\n%5").arg(name).arg(ProPair.second).arg(target).arg(text).arg(append));
        }
    }
    applySynergyBuffs(m_raceCount,m_professionCount,Owner::PlayerCtrl);
    applySynergyBuffs(enemyRaceCount,enemyProfessionCount,Owner::EnemyCtrl);
}

std::vector<QString> Game::rollShop()
{
    std::vector<QString> shopPools;
    for(int i=0;i<5;i++){
        int randomIndex=QRandomGenerator::global()->bounded(m_heroPools.size());
        shopPools.push_back(m_heroPools[randomIndex]);
    }
    return shopPools;
}

void Game::upUnitStar(QString name, int star)
{
    bool first=true;
    int count=0;
    QPoint pos(-1,-1);
    Unit* units[3];
    for(Unit* unit:m_units){
        if(unit->getOwner()==Owner::EnemyCtrl){
            continue;
        }
        else {
            if(unit->getName()==name && unit->getStar()==star){
                if(first){
                    pos=unit->getPos();
                    first=false;
                }
                units[count++]=unit;
            }
        }
        if(count==3)break;
    }
    if(count==3){
        std::vector<Equipment> equipments;
        for(int i=0;i<count;i++){
            Equipment equip=units[i]->getEquipment();
            if(equip!=Equipment::None){
                equipments.push_back(equip);
            }
            UnitItem* item=getUnitItem(units[i]->getId());
            if(item){
                if(item->getIsBoard()){
                    m_board.removeUnit(units[i]);
                }
                else {
                    m_bench.removeUnit(units[i]);
                }
                m_scene->removeItem(item);

                auto itemIt=std::find(m_unitItems.begin(),m_unitItems.end(),item);
                if(itemIt!=m_unitItems.end()){
                    m_unitItems.erase(itemIt);
                }
                delete item;
            }

            m_unitItemById.erase(units[i]->getId());

            auto it=std::find(m_units.begin(),m_units.end(),units[i]);
            if(it!=m_units.end()){
                m_units.erase(it);
            }
            delete units[i];
        }
        Unit * unit=createHeroforPreview(name,star+1).release();
        if(!equipments.empty()){
            unit->addEquipment(equipments[0]);
            for(int i=1;i<equipments.size();++i){
                int find=-1;
                for(int j=0;j<4;j++){
                    if(m_equipmentByIndex[j] && m_equipmentByIndex[j]->getType()==Equipment::None){
                        find=j;
                        break;
                    }
                }
                if(find!=-1){
                    m_equipmentByIndex[find]->setType(equipments[i]);
                }
                else {
                    m_player->changeGold(2);
                }
            }

        }
        unit->setIsShop(false);
        m_units.push_back(unit);
        UnitItem* item=nullptr;
        if(pos.y()==Board::ROWS){
            m_bench.addUnit(unit,pos);
            item=new UnitItem(unit,false);
        }
        else {
            m_board.addUnit(unit,pos);
            item=new UnitItem(unit,true);
        }

        item->setZValue(kZUnit);
        m_scene->addItem(item);
        m_unitItems.push_back(item);
        m_unitItemById[unit->getId()]=item;

        connect(item,&UnitItem::dragStarted,this,&Game::onDragStarted);
        connect(item,&UnitItem::dragMoved,this,&Game::onDragMoved);
        connect(item,&UnitItem::dragDropped,this,&Game::onDragDropped);
        connect(item,&UnitItem::clicked,this,&Game::onClicked);

        connect(unit,&Unit::infoChanged,item,&UnitItem::unitInfoChanged);
        connect(item,&UnitItem::unitInfoReflash,this,&Game::unitInfoChanged);
        connect(unit,&Unit::isDead,this,&Game::onUnitDead);
        connect(unit, &Unit::damaged, item, &UnitItem::onDamaged);
    }

    syncFromBoardAndBench();
}

void Game::pauseGame()
{
    m_timer->stop();
}

void Game::resumeGame()
{
    if(m_phase==GamePhase::Combat){
        m_timer->start(FPS);
    }
}

void Game::saveGame()
{
    std::ofstream out("save.txt", std::ios::out);
    if(!out.is_open())exit(1);

    out<<m_player->getHp()<<"\n";
    out<<m_player->getGold()<<"\n";
    out<<m_player->getLevel()<<"\n";
    out<<m_player->getMaxUnit()<<"\n";
    out<<m_player->getXp()<<"\n";
    out<<m_player->getMaxXP()<<"\n";
    out<<m_player->getMajorStage()<<"\n";
    out<<m_player->getMinorStage()<<"\n";
    out<<static_cast<int>(m_player->hasWaterBless())<<"\n";
    out<<static_cast<int>(m_player->hasFireBless())<<"\n";
    out<<static_cast<int>(m_player->hasThunderBless())<<"\n";
    out<<static_cast<int>(m_player->hasFireBless())<<"\n";
    int unitCount=0;
    for(Unit* unit:m_units){
        if(!unit || unit->getOwner()==Owner::EnemyCtrl)continue;
        unitCount++;
    }

    out<<unitCount<<"\n";

    for(Unit* unit:m_units){
        if(!unit || unit->getOwner()==Owner::EnemyCtrl)continue;

        UnitItem* item=getUnitItem(unit->getId());
        bool isBoard=item?item->getIsBoard():false;

        out<<unit->getName().toStdString()<<" "
            <<unit->getStar()<<" "
            <<unit->getStartPos().x()<<" "
            <<unit->getStartPos().y()<<" "
            <<isBoard<<" "
            <<static_cast<int>(unit->getEquipment())<<"\n";
    }

    for(int i=0;i<4;i++){
        out<<static_cast<int>(m_equipmentByIndex[i]->getType())<<" ";
    }
    out<<"\n";
    out.close();
}

bool Game::LoadGame()
{
    m_scene->clear();
    qDeleteAll(m_units);
    m_units.clear();
    m_unitItems.clear();
    m_unitItemById.clear();
    m_board.clear();
    m_bench.clear();

    std::ifstream in("save.txt",std::ios::in);
    if(!in)exit(1);

    int hp,gold,level,maxUnit,xp,maxXp,majorStage,minorStage,waterBless,windBless,thunderBless,fireBless;
    in>>hp>>gold>>level>>maxUnit>>xp>>maxXp>>majorStage>>minorStage>>waterBless>>windBless>>thunderBless>>fireBless;

    m_player->setHp(hp);
    m_player->setGold(gold);
    m_player->setLevel(level);
    m_player->setMaxUnit(maxUnit);
    m_player->setXp(xp);
    m_player->setMaxXP(maxXp);
    m_player->setStage(majorStage,minorStage);
    if(waterBless)m_player->activateBless(1);
    if(windBless)m_player->activateBless(2);
    if(thunderBless)m_player->activateBless(3);
    if(fireBless)m_player->activateBless(4);

    int unitCount;
    in>>unitCount;
    for(int i=0;i<unitCount;++i){
        std::string name;
        int star,x,y,isBoardInt,equipInt;

        in>>name>>star>>x>>y>>isBoardInt>>equipInt;
        bool isBoard=isBoardInt>0;
        Equipment equip=static_cast<Equipment>(equipInt);

        Unit* unit=createHeroforPreview(QString::fromStdString(name),star).release();
        unit->setIsShop(false);
        if(equip!=Equipment::None){
            unit->addEquipment(equip);
        }
        m_units.push_back(unit);
        if(isBoard){
            m_board.addUnit(unit,QPoint(x,y));
        }
        else {
            m_bench.addUnit(unit,QPoint(x,y));
        }
        unit->setStartPos(unit->getPos());
    }

    generateEnemy();

    buildScene();
    for(int i=0;i<4;i++){
        int typeInt;
        in>>typeInt;
        Equipment type = static_cast<Equipment>(typeInt);
        m_equipmentByIndex[i]->setType(type);
    }

    in.close();

    m_phase = GamePhase::Prep;
    syncFromBoardAndBench();

    return true;
}

void Game::buyXp()
{
    if(m_player->getGold()>=4){
        m_player->changeGold(-4);
        m_player->addXp(4);
    }
}

void Game::startNxtRound()
{
    clearEnemyBeforeRound();

    for(Unit* unit:m_units){
        if(unit->getOwner()==Owner::PlayerCtrl){
            unit->setHp(unit->getMaxHp());
            unit->setState(State::Idle);
            unit->setMana(0);

            m_board.removeUnit(unit);

        }
    }
    for(Unit* unit:m_units){
        if(unit->getOwner()==Owner::PlayerCtrl){
            QPoint pos=unit->getStartPos();
            m_board.addUnit(unit,pos);
        }
    }


    generateEnemy();
    for(QString& name:m_heroPools){
        upUnitStar(name,1);
        upUnitStar(name,2);
    }

    m_phase=GamePhase::Prep;

    syncFromBoardAndBench();
    m_scene->update();
}

