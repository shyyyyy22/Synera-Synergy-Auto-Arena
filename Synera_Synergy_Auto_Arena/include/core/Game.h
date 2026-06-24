#ifndef GAME_H
#define GAME_H

#include <QObject>
#include "Board.h"
#include<QGraphicsScene>
#include"GridItem.h"
#include"UnitItem.h"
#include"Player.h"
#include<QTimer>
#include<QPushButton>
#include<EquipmentItem.h>
#include<map>
#include<memory>
#include<unordered_map>
#include<vector>
enum class GamePhase{Prep,Combat,Resolve};

class Game : public QObject
{
    Q_OBJECT
public:
    explicit Game(int rows,int cols,QObject *parent = nullptr);
    ~Game();

    //初始化相关
    void initialize();
    void startNewGame();

    //属性获取
    QGraphicsScene* getScene()const;
    Unit* getUnitById(int unitId)const;
    UnitItem* getUnitItem(int unitId)const;
    GridItem* getGridItem(const QPoint &gridPos)const;
    Player* getPlayer()const;
    int getPlayerUnitInBoard()const;
    std::vector<QString> getActivateSynergyList()const;
    QString getRaceName(Race race)const;
    QString getProName(Profession pro)const;
    GamePhase getPhase()const;
    EquipmentItem* getEquipmentItem(int index)const;
    bool getEquipmentSlotFull();
    QString getSynergyDes(Race race,Profession pro);

    //回合与经济逻辑
    void startNxtRound();
    void buyXp();
    std::vector<QString> rollShop();
    void upUnitStar(QString name,int star);
    bool buyHero(int gold,QString name);
    void sellHero(Unit* unit);

    //暂停与存档
    void pauseGame();
    void resumeGame();
    void saveGame();
    bool LoadGame();

public slots:
    //选择与拖拽相关
    void onClicked(Unit* unit);
    void clearAllSelected();

    void onDragStarted(int unitId,const QPoint& sourcePos,const QPointF &worldPos);
    void onDragMoved(int unitId,const QPoint& sourcePos,const QPointF &worldPos);
    void onDragDropped(int unitId,const QPoint& sourcePos,const QPointF &worldPos);
    void onEquipStarted(int index,Equipment type,const QPointF &worldPos);
    void onEquipMoved(int index,Equipment type,const QPointF &worldPos);
    void onEquipDropped(int index,Equipment type,const QPointF &worldPos);

    //战斗流程相关
    void gameTick();
    void onClickStartBtn();
    void onUnitDead(Unit* unit);
    void handleStageResolve(bool win);
    void clearEnemyBeforeRound();
    std::unique_ptr<Unit> createHeroforPreview(QString name,int star=1);
    void calculateSynergies();

private:
    //GUI相关
    void buildScene();
    void syncFromBoardAndBench();
    void clearGridHighLights();
    QPointF gridToWorld(int row, int col,bool isBoard) const;
    QPoint worldToGrid(QPointF worldPos)const;

    //拖拽逻辑
    bool canApplyDrop(int unitId,const QPoint& sourcePos,const QPoint& target);
    void applyDrop(int unitId,const QPoint& sourcePos,const QPoint& target);
    bool canApplyEquipDrop(int index,const QPoint& target);
    void applyEquipmentDrop(int index,const QPoint& target);

    //游戏逻辑
    void generateRandomEquip();
    void generateEnemy();
    void applySynergyBuffs(std::map<Race,int> raceCount,std::map<Profession,int> proCount,Owner owner);

    //游戏数据
    int m_rows;
    int m_cols;

    Board m_board;
    Board m_bench;

    GamePhase m_phase;

    //各类实体
    Player* m_player;

    std::vector<Unit*> m_units;
    std::vector<UnitItem*> m_unitItems;
    std::unordered_map<int ,UnitItem*>m_unitItemById;
    std::unordered_map<int,EquipmentItem*>m_equipmentByIndex;
    std::vector<Equipment>m_EquipmentPools;
    std::vector<GridItem*> m_gridItems;
    std::vector<GridItem*> m_benchItems;
    std::vector<QString> m_heroPools;

    QTimer* m_timer;

    //统计数据
    int m_playerUnitInBoard;
    bool equipmentSlotFull;
    std::map<Race,int> m_raceCount;
    std::map<Profession,int> m_professionCount;
    std::vector<QString> m_activateSynergyList;

    //gui层面
    qreal m_radius;

    QGraphicsScene *m_scene;

    bool m_dragActive;
    int m_activeUnitId;
    bool m_dragEquipActive;
    int m_activeIndex;

    QPoint m_sourcePos;
    QPointF m_dragEquipPos;
    std::vector<QPointF> m_equipmentSlotPos;

signals:
    void unitSelected(Unit* unit);
    void unitInfoChanged(Unit* unit);
    void roundFinishend(bool win,int gold,int hp);
    void gameOver(bool win);
    void boardUpdate(int count);
    void gameIsCombat(bool isCombat);
    void unitSelled(Unit* unit);
};

#endif // GAME_H
