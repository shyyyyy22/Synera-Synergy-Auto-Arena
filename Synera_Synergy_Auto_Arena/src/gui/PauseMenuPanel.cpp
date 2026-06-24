#include "PauseMenuPanel.h"
#include<QVBoxLayout>

PauseMenuPanel::PauseMenuPanel(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(15);

    m_titleLabel = new QLabel("游戏暂停", this);
    m_titleLabel->setAlignment(Qt::AlignCenter);

    m_continueBtn = new QPushButton("继续游戏", this);
    m_saveBtn = new QPushButton("保存并退出", this);

    layout->addWidget(m_titleLabel);
    layout->addWidget(m_continueBtn);
    layout->addWidget(m_saveBtn);
    layout->addStretch();

    setStyleSheet(R"(
        QWidget {
            background-color: rgba(25, 25, 25, 230);
            border: 2px solid #D4AF37;
            border-radius: 8px;
            color: white;
        }
        QLabel {
            font-size: 20px;
            font-weight: bold;
            color: #D4AF37;
            border: none;
            padding-bottom: 5px;
        }
        QPushButton {
            background-color: #37474f;
            color: white;
            border: 1px solid #555;
            border-radius: 4px;
            font-weight: bold;
            font-size: 14px;
            padding: 8px 15px;
            min-width: 140px;
        }
        QPushButton:hover { background-color: #455a64; border-color: #D4AF37; }
    )");

    setFixedSize(220, 180);

    connect(m_continueBtn, &QPushButton::clicked, this, &PauseMenuPanel::clickContinueBtn);
    connect(m_saveBtn, &QPushButton::clicked, this, &PauseMenuPanel::clickSaveBtn);
}
