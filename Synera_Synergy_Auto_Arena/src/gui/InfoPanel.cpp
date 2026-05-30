#include "InfoPanel.h"
#include"Unit.h"
#include"Board.h"

InfoPanel::InfoPanel(QWidget *parent):QWidget(parent)
{
    QVBoxLayout* layout=new QVBoxLayout(this);
    m_nameLabel=new QLabel("请选择单位",this);
    m_hpLabel=new QLabel("",this);
    m_atkLabel=new QLabel("",this);
    m_manaLabel=new QLabel("",this);
    m_rangeLabel=new QLabel("",this);
    m_starLabel=new QLabel("",this);
    m_buyAndSellBtn=new QPushButton("",this);
    m_unit=nullptr;
    m_isGameCombat=false;

    layout->addWidget(m_nameLabel);
    layout->addWidget(m_hpLabel);
    layout->addWidget(m_atkLabel);
    layout->addWidget(m_manaLabel);
    layout->addWidget(m_rangeLabel);
    layout->addWidget(m_starLabel);
    layout->addSpacing(30);
    layout->addWidget(m_buyAndSellBtn);
    layout->addStretch();

    setStyleSheet(R"(
        QWidget {
            background-color: rgba(30, 30, 30, 240);
            color: #E0E0E0;
            border: 2px solid #555;
            border-radius: 8px;
            font-size: 14px;
        }
    )");

    setFixedWidth(200);

    QSizePolicy sp = sizePolicy();
    sp.setRetainSizeWhenHidden(true);
    setSizePolicy(sp);

    this->hide();

    connect(m_buyAndSellBtn,&QPushButton::clicked,this,[=](){
        emit onBuyAndSellBtn(m_unit->getIsShopHero());
    });
}

void InfoPanel::updateUnitInfo(Unit *unit)
{
    m_unit=unit;
    if(!unit){
        this->hide();
        return;
    }

    m_nameLabel->setText("英雄："+unit->getName());
    m_hpLabel->setText(QString("血量：%1/%2").arg(unit->getHp()).arg(unit->getMaxHp()));
    m_atkLabel->setText(QString("攻击力：%1").arg(unit->getAtk()));
    m_manaLabel->setText(QString("法力值：%1/%2").arg(unit->getMana()).arg(unit->getMaxMana()));
    m_rangeLabel->setText(QString("攻击范围：%1").arg(unit->getRange()));
    m_starLabel->setText(QString("星级：%1").arg(unit->getStar()));
    m_buyAndSellBtn->setText(unit->getIsShopHero()?"购买(3金币)":"出售(2金币)");
    if(m_isGameCombat && m_unit->getPos().y()<Board::ROWS && m_unit->getPos().y()>=0){
        m_buyAndSellBtn->setEnabled(false);
        m_buyAndSellBtn->setStyleSheet("background-color: #2f2f2f; color: #f2f2f2;; font-weight: bold; border-radius: 4px;");
    } else {
        m_buyAndSellBtn->setEnabled(true);
        m_buyAndSellBtn->setStyleSheet(unit->getIsShopHero()?"background-color: #2e7d32; color: white; font-weight: bold; padding: 6px; border-radius: 4px;"
                                                             :"background-color: #d32f2f; color: white; font-weight: bold; border-radius: 4px;");
    }

    this->show();
}

Unit *InfoPanel::getUnit() const
{
    return m_unit;
}

void InfoPanel::onIsGameCombat(bool isCombat)
{
    m_isGameCombat=isCombat;
}
