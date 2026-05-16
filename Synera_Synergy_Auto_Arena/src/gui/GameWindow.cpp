#include "GameWindow.h"
#include<QGraphicsView>
#include"Game.h"
GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent)
    ,m_centralWidget(new QWidget(this))
    ,m_mainLayout(new QVBoxLayout())
    ,m_view(new QGraphicsView(this))
    ,m_game(new Game(8,8,this))
{
    setUI();
    m_game->initialize();
}

GameWindow::~GameWindow() = default;

void GameWindow::setUI(){
    setCentralWidget(m_centralWidget);
    m_centralWidget->setLayout(m_mainLayout);

    setStyleSheet(R"(
        QMainWindow {
            background-color: #2b2b2b;
        }
        QWidget {
            background-color: #2b2b2b;
            color: #f2f2f2;
        }
    )");

    m_view->setRenderHint(QPainter::Antialiasing, true);
    m_view->setDragMode(QGraphicsView::NoDrag);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    m_view->setResizeAnchor(QGraphicsView::AnchorViewCenter);
    m_view->setMouseTracking(true);
    m_view->viewport()->setMouseTracking(true);

    m_mainLayout->addWidget(m_view, 1);
    m_view->setScene(m_game->getScene());
}