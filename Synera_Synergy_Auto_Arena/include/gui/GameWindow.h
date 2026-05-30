#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include"InfoPanel.h"
#include<QLabel.h>
#include<QPushButton>
#include"SettlementPanel.h"
class QGraphicsView;
class Game;

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow() override;

public slots:
    void toggleShop();

protected:
    void resizeEvent(QResizeEvent* event)override;

private:
    void setUI();
    void mousePressEvent(QMouseEvent *event)override;

    void updatePlayerInfo();
    void updateShopInfo();

    QWidget* m_centralWidget;
    QGraphicsView* m_view;
    Game* m_game;
    QVBoxLayout* m_mainLayout;
    InfoPanel* m_infoPanel;
    QPushButton* m_startBtn;
    QWidget* m_shopWidget;
    QPushButton* m_shopBtn;
    QPushButton* m_buyXpBtn;
    QPushButton* m_rollShopBtn;
    std::vector<QPushButton*> m_shopSlots;

    QLabel *m_pHpLabel;
    QLabel *m_pGoldLabel;
    QLabel *m_pLevelLabel;
    QLabel *m_pUnitNumsLabel;
    QLabel *m_pStageLabel;

    SettlementPanel* m_settlementPanel;

    std::vector<QString> m_shopPools;
    int m_shopIndex;
};
#endif // GAMEWINDOW_H
