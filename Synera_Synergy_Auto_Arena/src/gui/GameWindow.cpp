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
    ,m_shopBtn(new QPushButton("商店",this))
{
    m_shopPools=m_game->rollShop();
    setUI();
    m_game->initialize();
}

GameWindow::~GameWindow() = default;

void GameWindow::toggleShop()
{
    m_infoPanel->updateUnitInfo(nullptr);
    updateShopInfo();
    if(m_shopWidget->isHidden()){
        m_shopWidget->show();
        m_shopBtn->setText("收回商店");
        m_shopBtn->setStyleSheet("background-color: #d32f2f; color: white; font-weight: bold; border-radius: 4px;");
    }
    else {
        m_shopWidget->hide();
        m_shopBtn->setText("商店");
        m_shopBtn->setStyleSheet("background-color: #0288d1; color: white; font-weight: bold; border-radius: 4px;");
    }
}

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
            background-color: #388e3c;
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
    m_pStageLabel=new QLabel("关卡：-",this);

    topLayout->addWidget(m_pHpLabel);
    topLayout->addSpacing(20);
    topLayout->addWidget(m_pGoldLabel);
    topLayout->addStretch();
    topLayout->addWidget(m_pStageLabel);
    topLayout->addStretch();
    topLayout->addSpacing(20);
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

    //商店层
    m_shopWidget=new QWidget(this);
    m_shopWidget->setFixedHeight(80);
    m_shopWidget->setStyleSheet("background-color: rgba(30, 30, 30, 220); border-top: 2px solid #D4AF37;");

    QHBoxLayout* shopLayout=new QHBoxLayout(m_shopWidget);
    shopLayout->setContentsMargins(10, 5, 10, 5);

    m_buyXpBtn=new QPushButton("增加经验(4金币)",this);
    m_rollShopBtn=new QPushButton("刷新商店(2金币)",this);

    shopLayout->addWidget(m_buyXpBtn);
    shopLayout->addSpacing(10);

    for(int i=0;i<5;i++){
        m_shopSlots.push_back(new QPushButton(m_shopPools[i],this));
        m_shopSlots[i]->setStyleSheet("background-color: #3e2723; color: white; border: 1px solid #555; border-radius: 4px; font-weight: bold;");
        shopLayout->addWidget(m_shopSlots[i], 1);

        connect(m_shopSlots[i],&QPushButton::clicked,this,[this,i](){
            m_game->clearAllSelected();
            std::unique_ptr<Unit> unit=m_game->createHeroforPreview(m_shopPools[i]);
            m_infoPanel->updateUnitInfo(unit.release());
            m_shopIndex=i;
        });
    }
    updateShopInfo();

    shopLayout->addSpacing(10);
    shopLayout->addWidget(m_rollShopBtn);

    m_shopWidget->hide();
    m_mainLayout->addWidget(m_shopWidget);

    //下层：控制层
    QWidget* controlBar=new QWidget(this);
    controlBar->setFixedHeight(50);
    controlBar->setStyleSheet("background-color: #202020; border-top: 1px solid #444;");
    m_startBtn->setEnabled(false);
    m_shopBtn->setStyleSheet("background-color: #0288d1; color: white; font-weight: bold; border-radius: 4px;");

    QHBoxLayout* controlLayout=new QHBoxLayout(controlBar);
    controlLayout->setContentsMargins(20,0,20,0);
    controlLayout->addStretch();
    controlLayout->addWidget(m_shopBtn);
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
    connect(m_shopBtn,&QPushButton::clicked,this,&GameWindow::toggleShop);
    connect(m_game,&Game::gameIsCombat,m_infoPanel,&InfoPanel::onIsGameCombat);
    connect(m_infoPanel,&InfoPanel::onBuyAndSellBtn,this,[=](bool isShopHero){
        if(isShopHero){
            if(m_game->buyHero(-3,m_shopPools[m_shopIndex])){
                m_shopPools[m_shopIndex]="";
            }
        }
        else {
            m_game->sellHero(2,m_infoPanel->getUnit());
        }
        m_infoPanel->updateUnitInfo(nullptr);
        updatePlayerInfo();
        updateShopInfo();
    });
    connect(m_buyXpBtn,&QPushButton::clicked,this,[=](){
        m_game->buyXp();
        updatePlayerInfo();
        updateShopInfo();
    });
    connect(m_rollShopBtn,&QPushButton::clicked,this,[=](){
        m_shopPools=m_game->rollShop();
        m_game->getPlayer()->changeGold(-2);
        updateShopInfo();
        updatePlayerInfo();
    });
    connect(m_settlementPanel,&SettlementPanel::nxtRoundClicked,this,[this](){
        updatePlayerInfo();
        m_settlementPanel->hide();
        m_shopPools=m_game->rollShop();
        updateShopInfo();
        m_game->startNxtRound();
    });
    connect(m_game,&Game::roundFinishend,this,[this](bool win,int gold,int hp){
        m_settlementPanel->updateInfo(win,gold,hp);
        m_settlementPanel->show();
        m_settlementPanel->raise();
    });
    connect(m_game,&Game::gameOver,m_settlementPanel,&SettlementPanel::onGameOver);
    connect(m_game,&Game::boardUpdate,this,[this](int count){
        m_pUnitNumsLabel->setText(QString("人口：%1/%2").arg(m_game->getPlayerUnitInBoard()).arg(m_game->getPlayer()->getMaxUnit()));
        if(count>0){
            m_startBtn->setStyleSheet(R"(
            QPushButton {
                background-color: #2e7d32;
                color: white;
                font-weight: bold;
                font-size: 14px;
                padding: 6px 15px;
                border-radius: 4px;
                border: none;
            }
            QPushButton:hover {
                background-color: #388e3c;
            }
            QPushButton:pressed {
                background-color: #1b5e20;
            }
        )");
        }
        else {
            m_startBtn->setStyleSheet(R"(
            QPushButton {
                background-color: #2f2f2f;
                color: #f2f2f2;
                border: 1px solid #565656;
                border-radius: 4px;
                padding: 6px 14px;
                font-size: 13px;
            }
            QPushButton:hover {
                background-color: #388e3c;
            }
            QPushButton:pressed {
                background-color: #1b5e20;
            }
        )");
        }

        m_startBtn->setEnabled(count>0);
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
        m_pUnitNumsLabel->setText(QString("人口：%1/%2").arg(m_game->getPlayerUnitInBoard()).arg(p->getMaxUnit()));
        m_pStageLabel->setText(QString("关卡：%1-%2").arg(p->getMajorStage()).arg(p->getMinorStage()));
    }

}

void GameWindow::updateShopInfo()
{
    int playerGold=m_game->getPlayer()->getGold();
    if(playerGold<2){
        m_buyXpBtn->setStyleSheet("background-color: #2f2f2f; color: #f2f2f2;; font-weight: bold; border-radius: 4px;");
        m_rollShopBtn->setStyleSheet("background-color: #2f2f2f; color: #f2f2f2;; font-weight: bold; border-radius: 4px;");
        m_buyXpBtn->setEnabled(false);
        m_rollShopBtn->setEnabled(false);
    }
    else if(playerGold<4) {
        m_rollShopBtn->setStyleSheet("background-color: #0288d1; color: white; font-weight: bold; border-radius: 4px;");
        m_buyXpBtn->setStyleSheet("background-color: #2f2f2f; color: #f2f2f2;; font-weight: bold; border-radius: 4px;");
        m_buyXpBtn->setEnabled(false);
        m_rollShopBtn->setEnabled(true);
    }
    else {
        m_buyXpBtn->setStyleSheet("background-color: #0288d1; color: white; font-weight: bold; border-radius: 4px;");
        m_rollShopBtn->setStyleSheet("background-color: #0288d1; color: white; font-weight: bold; border-radius: 4px;");
        m_rollShopBtn->setEnabled(true);
        m_buyXpBtn->setEnabled(true);
    }
    if(m_game->getPlayer()->getLevel()==6){
        m_buyXpBtn->setStyleSheet("background-color: #2f2f2f; color: #f2f2f2;; font-weight: bold; border-radius: 4px;");
        m_buyXpBtn->setText("已达最大经验");
    }
    for(int i=0;i<5;i++){
        m_shopSlots[i]->setText(m_shopPools[i]);
    }
}