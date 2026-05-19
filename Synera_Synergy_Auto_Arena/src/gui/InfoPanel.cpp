#include "InfoPanel.h"
#include"Unit.h"


InfoPanel::InfoPanel(QWidget *parent):QWidget(parent)
{
    QVBoxLayout* layout=new QVBoxLayout(this);
    m_nameLabel=new QLabel("请选择单位",this);
    m_hpLabel=new QLabel("",this);
    m_atkLabel=new QLabel("",this);
    m_manaLabel=new QLabel("",this);
    m_rangeLabel=new QLabel("",this);
    m_starLabel=new QLabel("",this);

    layout->addWidget(m_nameLabel);
    layout->addWidget(m_hpLabel);
    layout->addWidget(m_atkLabel);
    layout->addWidget(m_manaLabel);
    layout->addWidget(m_rangeLabel);
    layout->addWidget(m_starLabel);
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
}

void InfoPanel::updateUnitInfo(Unit *unit)
{
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
    this->show();
}
