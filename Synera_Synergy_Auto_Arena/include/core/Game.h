#ifndef GAME_H
#define GAME_H

#include <QObject>
#include "board.h"
#include<QGraphicsScene>
#include"GridItem.h"
#include"UnitItem.h"
#include"Player.h"
#include<QTimer>
#include<QPushButton>
enum class GamePhase{Prep,Combat,Over};

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

    //游戏逻辑
    void gameTick();
    void onClickStartBtn();

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

    //游戏数据
    int m_rows;
    int m_cols;
    qreal m_radius;

    Board m_board;
    Board m_bench;

    Player* m_player;

    std::vector<Unit*> m_units;
    std::vector<GridItem*> m_gridItems;
    std::vector<GridItem*> m_benchItems;
    std::vector<UnitItem*> m_unitItems;
    std::unordered_map<int ,UnitItem*>m_unitItemById;

    //gui层面
    QGraphicsScene *m_scene;
    bool m_dragActive;
    int m_activeUnitId;
    QPoint m_sourcePos;

    //逻辑控制层
    QTimer* m_timer;
    GamePhase m_phase;

    friend bool Board::isValidPosition(const QPoint &pos)const;
signals:
    void unitSelected(Unit* unit);
    void unitInfoChanged(Unit* unit);
};

#endif // GAME_H
