#ifndef GAME_H
#define GAME_H

#include <QObject>
#include "board.h"
#include<QGraphicsScene>
#include"GridItem.h"
#include"UnitItem.h"
#include"Player.h"

class Game : public QObject
{
    Q_OBJECT
public:
    explicit Game(int rows,int cols,QObject *parent = nullptr);
    ~Game();

    //初始化相关
    void initialize();
    void reset();

    //属性获取
    QGraphicsScene* getScene()const;
    Unit* getUnitById(int unitId)const;
    UnitItem* getUnitItem(int unitId)const;
    GridItem* getGridItem(const QPoint &gridPos)const;
    Player* getPlayer()const;

    //测试使用
    void initialUnitForTest();


public slots:
    //属性面板
    void onClicked(Unit* unit);
    void clearAllSelected();
    //拖拽
    void onDragStarted(int unitId,const QPoint& sourcePos,const QPointF &worldPos);
    void onDragMoved(int unitId,const QPoint& sourcePos,const QPointF &worldPos);
    void onDragDropped(int unitId,const QPoint& sourcePos,const QPointF &worldPos);

private:
    void buildScene();
    void syncFromBoardAndBench();
    void clearGridHighLights();
    bool canApplyDrop(int unitId,const QPoint& sourcePos,const QPoint& target);
    void applyDrop(int unitId,const QPoint& sourcePos,const QPoint& target);
    QPointF gridToWorld(int row, int col,bool isBoard) const;
    QPoint worldToGrid(QPointF worldPos)const;

    //敌人生成
    void generateEnemy();

    int m_rows;
    int m_cols;
    qreal m_radius;

    Board m_board;
    Board m_bench;

    Player* m_player;

    QGraphicsScene *m_scene;

    std::vector<Unit*> m_units;
    std::vector<GridItem*> m_gridItems;
    std::vector<GridItem*> m_benchItems;
    std::vector<UnitItem*> m_unitItems;
    std::unordered_map<int ,UnitItem*>m_unitItemById;

    bool m_dragActive;
    int m_activeUnitId;
    QPoint m_sourcePos;

    friend bool Board::isValidPosition(const QPoint &pos)const;
signals:
    void unitSelected(Unit* unit);
};

#endif // GAME_H
