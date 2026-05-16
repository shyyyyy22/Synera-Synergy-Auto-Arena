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
    ,m_board(rows,cols)
    ,m_bench(1,cols)
    ,m_scene(new QGraphicsScene(this))
{}

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
        QPoint (0,0),
        QPoint (1,0),
        QPoint (2,0),
        QPoint (3,0)
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
            gridItem->setPos(gridToWorld(i,j));

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
        QPointF pos=gridToWorld(m_rows,j);
        benchItem->setPos(pos.x()-0.4*m_radius,pos.y()+40);

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
    }

    m_scene->setSceneRect(totalBounds.adjusted(-40, -40, 40, 40));
}
void Game::syncFromBoardAndBench(){
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
                item->setVisible(false);
                continue;
            }
        }

        item->setVisible(true);
        item->setGridPos(pos);
        item->setZValue(kZUnit);
        QPointF worldPos=item->getIsBoard()?gridToWorld(pos.y(),pos.x()):gridToWorld(pos.y()+m_rows,pos.x());
        if(!item->getIsBoard()){
            item->setPos(worldPos.x()-0.4*m_radius+40,worldPos.y()+80);
        }
        else{
            item->setPos(worldPos);
        }
    }
}
QPointF Game::gridToWorld(int row, int col) const{
    qreal w = m_radius * qSqrt(3.0);

    qreal x = col * w;
    if (row % 2 != 0) {
        x += w / 2.0;
    }

    qreal y = row * (m_radius * 1.5);

    return QPointF(x, y);
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