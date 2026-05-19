#ifndef INFOPANEL_H
#define INFOPANEL_H

#include <QWidget>
#include<QVBoxLayout>
#include<QLabel>
class Unit;

class InfoPanel : public QWidget
{
    Q_OBJECT
public:
    explicit InfoPanel(QWidget *parent = nullptr);
    void updateUnitInfo(Unit* unit);

private:
    QLabel *m_nameLabel;
    QLabel *m_hpLabel;
    QLabel *m_atkLabel;
    QLabel *m_manaLabel;
    QLabel *m_rangeLabel;
    QLabel *m_starLabel;
};

#endif // INFOPANEL_H
