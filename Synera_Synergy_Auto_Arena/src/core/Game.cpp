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
    buildScene();
    reset();
}
void Game::reset(){
    m_board.clear();
    m_bench.clear();
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

    QRectF totalBounds;
    bool first=true;
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

    m_scene->setSceneRect(totalBounds.adjusted(-40, -40, 40, 40));
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