#ifndef SETTLEMENTPANEL_H
#define SETTLEMENTPANEL_H

#include <QWidget>
#include<QLabel>
#include<QPushButton>
#include<QVBoxLayout>

class SettlementPanel : public QWidget
{
    Q_OBJECT
public:
    explicit SettlementPanel(QWidget *parent = nullptr);

public slots:
    void updateInfo(bool win,int gold,int hp);
    void onGameOver(bool win);
signals:
    void nxtRoundClicked();

private:
    QLabel* m_titleLabel;
    QLabel* m_detailLabel;
    QPushButton* m_nxtRountBtn;
    QPushButton* m_exitBtn;
};

#endif // SETTLEMENTPANEL_H
