#ifndef EQUIPMENTSLOTITEM_H
#define EQUIPMENTSLOTITEM_H

#include <QGraphicsItem>

class EquipmentSlotItem : public QGraphicsItem
{
public:
    EquipmentSlotItem(int index,QGraphicsItem* parent=nullptr);

    //基础功能
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
    int m_index;
};

#endif // EQUIPMENTSLOTITEM_H
