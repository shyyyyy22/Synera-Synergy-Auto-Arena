#ifndef GAME_H
#define GAME_H

#include <QObject>
#include "board.h"
#include<QGraphicsScene>
#include"GridItem.h"
#include"UnitItem.h"
class Game : public QObject
{
    Q_OBJECT
public:
    explicit Game(int rows,int cols,QObject *parent = nullptr);
    ~Game()=default;

    //初始化相关
    void initialize();
    void reset();

    //属性获取
    QGraphicsScene* getScene()const;

    //测试使用
    void initialUnitForTest();

private:
    void buildScene();
    void syncFromBoardAndBench();
    QPointF gridToWorld(int row, int col) const;

    int m_rows;
    int m_cols;
    qreal m_radius;
    Board m_board;
    Board m_bench;
    std::vector<Unit*> m_units;
    QGraphicsScene *m_scene;
    std::vector<GridItem*> m_gridItems;
    std::vector<GridItem*> m_benchItems;
    std::vector<UnitItem*> m_unitItems;
};

#endif // GAME_H
