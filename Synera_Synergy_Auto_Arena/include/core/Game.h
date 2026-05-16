#ifndef GAME_H
#define GAME_H

#include <QObject>
#include "board.h"
#include<QGraphicsScene>
#include"GridItem.h"
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

private:
    void buildScene();
    QPointF gridToWorld(int row, int col) const;

    int m_rows;
    int m_cols;
    qreal m_radius;
    Board m_board;
    Board m_bench;
    QGraphicsScene *m_scene;
    std::vector<GridItem*> m_gridItems;
    std::vector<GridItem*> m_benchItems;
};

#endif // GAME_H
