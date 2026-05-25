#include "GameWindow.h"
#include<QGraphicsView>
#include"Game.h"
#include<QMouseEvent>
GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent)
    ,m_centralWidget(new QWidget(this))
    ,m_mainLayout(new QVBoxLayout())
    ,m_view(new QGraphicsView(this))
    ,m_game(new Game(Board::ROWS,Board::COLS,this))
    ,m_infoPanel(new InfoPanel(this))
    ,m_startBtn(new QPushButton("开始对战",this))
{
    setUI();
    m_game->initialize();
}

GameWindow::~GameWindow() = default;

void GameWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    if (m_centralWidget && m_settlementPanel) {
        int x = (m_centralWidget->width() - m_settlementPanel->width()) / 2;
        int y = (m_centralWidget->height() - m_settlementPanel->height()) / 2;
        m_settlementPanel->move(x, y);
    }
}

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
        QPushButton {
            background-color: #2f2f2f;
            color: #f2f2f2;
            border: 1px solid #565656;
            border-radius: 4px;
            padding: 6px 14px;
            font-size: 13px;
        }
        QPushButton:hover {
            background-color: #3a3a3a;
        }
        QPushButton:pressed {
            background-color: #242424;
        }
    )");

    //设置view
    m_view->setRenderHint(QPainter::Antialiasing, true);
    m_view->setDragMode(QGraphicsView::NoDrag);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    m_view->setResizeAnchor(QGraphicsView::AnchorViewCenter);
    m_view->setMouseTracking(true);
    m_view->viewport()->setMouseTracking(true);
    m_view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    //设置主布局：垂直
    m_mainLayout->setContentsMargins(0,0,0,0);
    m_mainLayout->setSpacing(0);

    //上层：玩家状态栏
    QWidget* topBar=new QWidget(this);
    topBar->setFixedHeight(40);
    topBar->setStyleSheet("background-color: rgba(20, 20, 20, 200); border-bottom: 2px solid #444; color: white; font-weight: bold; padding: 5px;");

    QHBoxLayout* topLayout = new QHBoxLayout(topBar);
    topLayout->setContentsMargins(20, 0, 20, 0);

    m_pHpLabel=new QLabel("血量：-",this);
    m_pGoldLabel=new QLabel("金币：-",this);
    m_pLevelLabel=new QLabel("等级：-",this);
    m_pUnitNumsLabel=new QLabel("人口：-",this);

    topLayout->addWidget(m_pHpLabel);
    topLayout->addStretch();
    topLayout->addWidget(m_pGoldLabel);
    topLayout->addStretch();
    topLayout->addWidget(m_pLevelLabel);
    topLayout->addSpacing(20);
    topLayout->addWidget(m_pUnitNumsLabel);
    updatePlayerInfo();

    m_mainLayout->addWidget(topBar);

    //中层：游戏区：主界面-单位信息栏
    QWidget* gameContainer = new QWidget(this);
    QHBoxLayout* gameLayout = new QHBoxLayout(gameContainer);
    gameLayout->setContentsMargins(0, 0, 0, 0);
    gameLayout->addWidget(m_view, 1);
    gameLayout->addWidget(m_infoPanel);

    m_mainLayout->addWidget(gameContainer,1);

    //下层：控制层
    QWidget* controlBar=new QWidget(this);
    controlBar->setFixedHeight(50);
    controlBar->setStyleSheet("background-color: #202020; border-top: 1px solid #444;");
    m_startBtn->setStyleSheet("background-color: #2e7d32; color: white; font-weight: bold; font-size: 14px; padding: 6px 15px; border-radius: 4px;");
    QHBoxLayout* controlLayout=new QHBoxLayout(controlBar);
    controlLayout->setContentsMargins(20,0,20,0);
    controlLayout->addStretch();
    controlLayout->addWidget(m_startBtn);

    m_mainLayout->addWidget(controlBar,1);

    //结算画面
    m_settlementPanel=new SettlementPanel(m_centralWidget);
    m_settlementPanel->hide();

    //连接主界面
    m_view->setScene(m_game->getScene());

    //信号区域
    connect(m_game,&Game::unitSelected,m_infoPanel,&InfoPanel::updateUnitInfo);
    connect(m_game,&Game::unitInfoChanged,m_infoPanel,&InfoPanel::updateUnitInfo);
    connect(m_startBtn,&QPushButton::clicked,m_game,&Game::onClickStartBtn);
    connect(m_settlementPanel,&SettlementPanel::nxtRoundClicked,this,[this](){
        m_settlementPanel->hide();
        //m_game->startNxtRound();
    });
    connect(m_game,&Game::roundFinishend,this,[this](bool win,int gold,int hp){
        m_settlementPanel->updateInfo(win,gold,hp);
        m_settlementPanel->show();
        m_settlementPanel->raise();
    });
}

void GameWindow::mousePressEvent(QMouseEvent *event) {
    bool clickOnHero=false;
    QPointF scenePos = m_view->mapToScene(event->pos());
    QList<QGraphicsItem*> clickItems=m_game->getScene()->items(scenePos);
    for(QGraphicsItem* item:clickItems){
        UnitItem* uItem=dynamic_cast<UnitItem*>(item);
        if(uItem){
            clickOnHero=true;
            break;
        }
    }
    if(!clickOnHero){
        m_game->clearAllSelected();
        m_infoPanel->updateUnitInfo(nullptr);
        update();
    }
    QMainWindow::mousePressEvent(event);
}

void GameWindow::updatePlayerInfo()
{
    Player* p=m_game->getPlayer();

    if(p){
        m_pHpLabel->setText(QString("血量：%1").arg(p->getHp()));
        m_pGoldLabel->setText(QString("金币：%1").arg(p->getGold()));
        m_pLevelLabel->setText(QString("等级：Lv.%1").arg(p->getLevel()));
        m_pUnitNumsLabel->setText(QString("人口：%1/%2").arg(0).arg(p->getMaxUnit()));
    }

}