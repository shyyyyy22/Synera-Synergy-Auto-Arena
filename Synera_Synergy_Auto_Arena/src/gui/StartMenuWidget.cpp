#include "StartMenuWidget.h"
#include<QVBoxLayout>
#include<QApplication>
StartMenuWidget::StartMenuWidget(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(50, 80, 50, 50);
    mainLayout->setSpacing(20);

    m_logoLabel = new QLabel("Legend of Hyrule Auto Arena", this);
    m_logoLabel->setAlignment(Qt::AlignCenter);

    m_startBtn = new QPushButton("新建游戏", this);
    m_loadBtn = new QPushButton("加载存档", this);
    m_exitBtn = new QPushButton("离开海拉鲁", this);

    mainLayout->addStretch();
    mainLayout->addWidget(m_logoLabel);
    mainLayout->addSpacing(40);

    mainLayout->addWidget(m_startBtn,0,Qt::AlignCenter);
    mainLayout->addWidget(m_loadBtn,0,Qt::AlignCenter);
    mainLayout->addWidget(m_exitBtn,0,Qt::AlignCenter);

    mainLayout->addStretch();

    QLabel* footer = new QLabel("© 2026 NJU高级程序设计PA,developed by 251880536.", this);
    footer->setAlignment(Qt::AlignCenter);
    footer->setStyleSheet("color: #555555; font-size: 11px;");
    mainLayout->addWidget(footer);

    setStyleSheet(R"(
        QWidget {
            background-color: #1a1a1a;
        }
        QLabel {
            color: #D4AF37;
            font-size: 36px;
            font-weight: bold;
            font-family: "Microsoft YaHei", "华文细黑";
            letter-spacing: 4px;
        }
        QPushButton {
            background-color: rgba(45, 45, 45, 200);
            color: #E0E0E0;
            border: 2px solid #555555;
            border-radius: 6px;
            padding: 10px 40px;
            font-size: 16px;
            font-weight: bold;
            min-width: 240px;
            max-width: 320px;
            margin: 0 auto;
        }
        QPushButton:hover {
            background-color: rgba(212, 175, 55, 30);
            border-color: #D4AF37;
            color: #D4AF37;
        }
        QPushButton:pressed {
            background-color: rgba(212, 175, 55, 10);
            border-color: #b89028;
        }
    )");

    connect(m_startBtn, &QPushButton::clicked, this, &StartMenuWidget::clickStartBtn);
    connect(m_loadBtn, &QPushButton::clicked, this, &StartMenuWidget::clickLoadBtn);
    connect(m_exitBtn, &QPushButton::clicked, qApp, &QApplication::quit);

}
