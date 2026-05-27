#include "Game.h"
#include"Heroes.h"
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
{

}
Game::~Game(){
    qDeleteAll(m_units);
    m_units.clear();
    m_scene->clear();
    delete m_player;
}

//初始化相关
void Game::initialize(){
    initialUnits();
    generateEnemy();
    buildScene();
    reset();

    //设置定时器
    connect(m_timer,&QTimer::timeout,this,&Game::gameTick);
}
void Game::reset(){
    m_bench.clear();
    int playerPos=0;
    const QPoint initialPositions[]{
        QPoint (0,m_rows),
        QPoint (1,m_rows),
        QPoint (2,m_rows),
        QPoint (3,m_rows)
    };

    for(int i=0;i<m_units.size();++i){
        if(m_units[i]->getOwner()==Owner::PlayerCtrl){
            m_bench.addUnit(m_units[i],initialPositions[playerPos++]);
        }
    }
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


//画棋盘
void Game::buildScene(){
    m_scene->clear();
    m_gridItems.clear();
    m_benchItems.clear();
    m_unitItems.clear();

    QRectF totalBounds;
    bool first=true;

    //棋盘
    for(int i=0;i<m_rows;++i){
        for(int j=0;j<m_cols;++j){
            GridItem* gridItem=new GridItem(i,j,m_radius,GridShape::Hexagon);
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
        GridItem *benchItem=new GridItem(m_rows,j,m_radius,GridShape::Square);
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
            item=new UnitItem(unit,false);
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
        if(!item->getIsBoard()){
            item->setPos(worldPos);
            item->setGridPos(QPoint(pos.x(),pos.y()));
        }
        else{
            item->setPos(worldPos);
            item->setGridPos(pos);
        }
    }
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
            }
            else {m_bench.moveUnit(unit,target);}
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
            }
            else{
                m_bench.removeUnit(unit);
                m_board.addUnit(unit,target);
                item->setIsBoard(true);
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
            }
            else{
                m_board.removeUnit(unit);
                m_bench.addUnit(unit,target);
                item->setIsBoard(false);
            }
        }
        else {
            Unit* targetUnit=m_board.getUnitAt(target);
            if(targetUnit){
                m_board.removeUnit(targetUnit);
                m_board.removeUnit(unit);
                m_board.addUnit(targetUnit,sourcePos);
                m_board.addUnit(unit,target);
            }
            else{
                m_board.moveUnit(unit,target);
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

//敌人生成
void Game::generateEnemy()
{
    int round=(m_player->getMajorStage()-1)*4+m_player->getMinorStage();
    qreal scale=qMin(2.0,(1+0.1*(round-1)));

    switch(round){
    case 1:{
        Unit* enemy1=new Noah("腐化-卓拉战士",620*scale,55*scale,Owner::EnemyCtrl);
        m_units.push_back(enemy1);
        m_board.addUnit(enemy1,QPoint(3,1));
        break;
    }
    case 2:{
        Unit* enemy1=new Noah("腐化-卓拉战士",620*scale,55*scale,Owner::EnemyCtrl);
        Unit* enemy2=new Luna("腐化-卓拉法师",450*scale,38*scale,Owner::EnemyCtrl);

        m_units.push_back(enemy1);
        m_units.push_back(enemy2);

        m_board.addUnit(enemy1,QPoint(2,1));
        m_board.addUnit(enemy2,QPoint(5,0));
        break;
    }
    case 3:{
        Unit* enemy1=new Noah("腐化-卓拉战士",620*scale,55*scale,Owner::EnemyCtrl);
        Unit* enemy2=new Sidon("腐化-卓拉守卫",700*scale,30*scale,Owner::EnemyCtrl);
        Unit* enemy3=new Luna("腐化-卓拉法师",450*scale,38*scale,Owner::EnemyCtrl);

        m_units.push_back(enemy1);
        m_units.push_back(enemy2);
        m_units.push_back(enemy3);

        m_board.addUnit(enemy1,QPoint(2,1));
        m_board.addUnit(enemy2,QPoint(3,1));
        m_board.addUnit(enemy3,QPoint(4,0));
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
            }
        }
    }

    syncFromBoardAndBench();
    m_scene->update();

}

void Game::initialUnits(){
    if(!m_units.empty()){
        return;
    }
    m_units.push_back(new Sidon("卓拉守卫-辛顿",700,30,Owner::PlayerCtrl));
    m_units.push_back(new Luna("卓拉祭司-露娜",450,38,Owner::PlayerCtrl));
    m_units.push_back(new Noah("卓拉战士-诺亚",620,55,Owner::PlayerCtrl));
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
    //qDebug() << "Target item found:" << targetItem<<111;
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
        m_player->addGold(5);
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
        m_player->addGold(3);
    }
    if(m_player->getHp()<=0){
        emit gameOver();
    }
    else{
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

    m_phase=GamePhase::Prep;

    syncFromBoardAndBench();
    m_scene->update();
}

