#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include"InfoPanel.h"
#include<QLabel.h>
#include<QPushButton>
#include<QStackedWidget>
#include"StartMenuWidget.h"
#include"SettlementPanel.h"
#include"PauseMenuPanel.h"
#include<vector>
class QGraphicsView;
class Game;

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow() override;

public slots:
    //商店显示与折叠
    void toggleShop();

protected:
    //定位窗口中心
    void resizeEvent(QResizeEvent* event)override;

private:
    void setUI();
    void mousePressEvent(QMouseEvent *event)override;//重写用于点击空白取消选中

    void updatePlayerInfo();
    void updateShopInfo();
    void updateSynergyUI();

    //基本组件
    QWidget* m_centralWidget;
    QGraphicsView* m_view;
    Game* m_game;
    QVBoxLayout* m_mainLayout;

    InfoPanel* m_infoPanel;

    QPushButton* m_startBtn;
    StartMenuWidget* m_startMenu;
    QStackedWidget* m_stackedWidget;

    SettlementPanel* m_settlementPanel;

    QWidget* m_shopWidget;
    QPushButton* m_shopBtn;
    QPushButton* m_buyXpBtn;
    QPushButton* m_rollShopBtn;
    std::vector<QPushButton*> m_shopSlots;
    std::vector<QString> m_shopPools;
    int m_shopIndex;

    QWidget* m_synergySidebar;
    QVBoxLayout* m_synergyLayout;

    PauseMenuPanel* m_pauseMenu;
    QPushButton* m_pauseBtn;

    //玩家信息
    QLabel *m_pHpLabel;
    QLabel *m_pGoldLabel;
    QLabel *m_pLevelLabel;
    QLabel *m_pUnitNumsLabel;
    QLabel *m_pStageLabel;
    QLabel *m_pBlessLabel;
};
#endif // GAMEWINDOW_H
