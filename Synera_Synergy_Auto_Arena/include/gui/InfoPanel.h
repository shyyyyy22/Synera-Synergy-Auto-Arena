#ifndef INFOPANEL_H
#define INFOPANEL_H

#include <QWidget>
#include<QVBoxLayout>
#include<QLabel>
#include<QPushButton>
class Unit;

class InfoPanel : public QWidget
{
    Q_OBJECT
public:
    explicit InfoPanel(QWidget *parent = nullptr);
    void updateUnitInfo(Unit* unit);

    Unit* getUnit()const;

public slots:
    void onIsGameCombat(bool isCombat);

signals:
    void onBuyAndSellBtn(bool isShopHero);

private:
    Unit* m_unit;
    bool m_isGameCombat;

    QLabel *m_nameLabel;
    QLabel *m_hpLabel;
    QLabel *m_atkLabel;
    QLabel *m_manaLabel;
    QLabel *m_rangeLabel;
    QLabel *m_starLabel;

    QPushButton* m_buyAndSellBtn;
};

#endif // INFOPANEL_H
