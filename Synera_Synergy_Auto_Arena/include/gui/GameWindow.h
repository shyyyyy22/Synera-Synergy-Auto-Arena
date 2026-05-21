#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include"InfoPanel.h"
#include<QLabel.h>
#include<QPushButton>
class QGraphicsView;
class Game;

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow() override;

private:
    void setUI();
    void mousePressEvent(QMouseEvent *event)override;

    void updatePlayerInfo();

    QWidget* m_centralWidget;
    QGraphicsView* m_view;
    Game* m_game;
    QVBoxLayout* m_mainLayout;
    InfoPanel* m_infoPanel;
    QPushButton* m_startBtn;

    QLabel *m_pHpLabel;
    QLabel *m_pGoldLabel;
    QLabel *m_pLevelLabel;
    QLabel *m_pUnitNumsLabel;
};
#endif // GAMEWINDOW_H
