#include "SettlementPanel.h"

SettlementPanel::SettlementPanel(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout* layout=new QVBoxLayout(this);
    layout->setContentsMargins(20,20,20,20);

    m_titleLabel=new QLabel("胜利!",this);
    m_detailLabel=new QLabel("金币++\n玩家血量：100",this);
    m_nxtRountBtn=new QPushButton("下一关卡",this);

    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_detailLabel->setAlignment(Qt::AlignCenter);

    layout->addWidget(m_titleLabel);
    layout->addWidget(m_detailLabel);
    layout->addWidget(m_nxtRountBtn);

    setStyleSheet(R"(
            QWidget {
                background-color: rgba(20, 20, 20, 240);
                border: 2px solid #D4AF37;
                border-radius: 12px;
                color: #FFFFFF;
            }
            QLabel#Title {
                font-size: 24px;
                font-weight: bold;
                color: #D4AF37;
            }
            QPushButton {
                background-color: #2e7d32;
                border: 1px solid #555;
                border-radius: 4px;
                color: white;
                font-weight: bold;
                font-size: 14px;
                padding: 8px 15px;
            }
            QPushButton:hover { background-color: #388e3c; }
        )");

    m_titleLabel->setObjectName("Title");
    setFixedSize(280, 180);

    connect(m_nxtRountBtn,&QPushButton::clicked,this,&SettlementPanel::nxtRoundClicked);
}

void SettlementPanel::updateInfo(bool win, int gold, int hp)
{
    if(win){
        m_titleLabel->setText("胜利");
        m_titleLabel->setStyleSheet("color: #D4AF37; font-size: 26px; font-weight: bold;"); // 金色胜利
        m_detailLabel->setText(QString("💰 获得金币: +5\n❤️ 玩家血量: %1").arg(hp));
    }
    else {
        m_titleLabel->setText("失败");
        m_titleLabel->setStyleSheet("color: #d32f2f; font-size: 26px; font-weight: bold;"); // 红色失败
        m_detailLabel->setText(QString("💰 获得金币: +3 \n❤️ 玩家血量: %1").arg(hp));
    }
}

void SettlementPanel::onGameOver()
{
    qDebug()<<"Game Over";
}
