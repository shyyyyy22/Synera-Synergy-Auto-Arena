#include "Game.h"
namespace {
constexpr qreal kZGrid = 0.0;
constexpr qreal kZUnit = 1.0;
constexpr qreal kZDraggingUnit = 2.0;
}

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
{

}
Game::~Game(){
    qDeleteAll(m_units);
    m_units.clear();
}

//初始化相关
void Game::initialize(){
    initialUnitForTest();
    buildScene();
    reset();
}
void Game::reset(){
    m_board.clear();
    m_bench.clear();

    const QPoint initialPositions[]{
        QPoint (0,m_rows),
        QPoint (1,m_rows),
        QPoint (2,m_rows),
        QPoint (3,m_rows)
    };

    for(int i=0;i<m_units.size();++i){
        m_bench.addUnit(m_units[i],initialPositions[i]);
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

GridItem *Game::getGridItem(const QPoint &gridPos)
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
        UnitItem* item=new UnitItem(unit,false);
        item->setZValue(kZUnit);
        m_scene->addItem(item);
        m_unitItems.push_back(item);
        m_unitItemById[unit->getId()]=item;

        connect(item,&UnitItem::dragStarted,this,&Game::onDragStarted);
        connect(item,&UnitItem::dragMoved,this,&Game::onDragMoved);
        connect(item,&UnitItem::dragDropped,this,&Game::onDragDropped);
    }

    m_scene->setSceneRect(totalBounds.adjusted(-40, -40, 40, 40));
}
void Game::syncFromBoardAndBench(){

    clearGridHighLights();

    for(UnitItem* item:m_unitItems){
        if(!item || !item->getUnit()){
            continue;
        }
        QPoint pos=item->getUnit()->getPos();
        //qDebug() << "Unit:" << item->getUnit()->getName() << "at pos:" << pos;
        if(item->getIsBoard()){
            if(!m_board.isValidPosition(pos) || m_board.getUnitAt(pos)!=item->getUnit()){

                item->setVisible(false);
                continue;
            }
        }
        else {
            if(!m_bench.isValidPosition(pos) || m_bench.getUnitAt(pos)!=item->getUnit()){
                qDebug() << "Debug消失:" << item->getUnit()->getName() << "应该在备战区(" << pos << ")，但备战区没它！";
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
}

void Game::clearGridHighLights()
{
    for(GridItem* item:m_gridItems){
        if(!item){
            continue;
        }
        item->setHoverActive(false);
        item->setDropActive(false);
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
            UnitItem* targetItem=nullptr;
            if(targetUnit){
                targetItem=getUnitItem(targetUnit->getId());
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
            UnitItem* targetItem=nullptr;
            if(targetUnit){
                targetItem=getUnitItem(targetUnit->getId());
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

//测试使用
void Game::initialUnitForTest(){
    if(!m_units.empty()){
        return;
    }
    m_units.push_back(new Unit("战士",150,5,1,100,Owner::PlayerCtrl));
    m_units.push_back(new Unit("射手",100,5,3,100,Owner::PlayerCtrl));
    m_units.push_back(new Unit("法师",100,8,3,80,Owner::PlayerCtrl));
    m_units.push_back(new Unit("召唤师",80,10,5,100,Owner::PlayerCtrl));
}

//拖拽
void Game::onDragStarted(int unitId, const QPoint &sourcePos, const QPointF &worldPos)
{
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