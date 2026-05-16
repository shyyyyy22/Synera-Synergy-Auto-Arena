#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include<QVBoxLayout>

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

    QWidget* m_centralWidget;
    QGraphicsView* m_view;
    Game* m_game;
    QVBoxLayout* m_mainLayout;
};
#endif // GAMEWINDOW_H
