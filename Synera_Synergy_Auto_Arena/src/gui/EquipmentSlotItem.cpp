#include "EquipmentSlotItem.h"
#include<QPainter>

EquipmentSlotItem::EquipmentSlotItem(int index, QGraphicsItem *parent)
    :QGraphicsItem(parent)
    ,m_index(index)
{}

QRectF EquipmentSlotItem::boundingRect() const
{
    return QRectF(-20,-20,40,40);
}

void EquipmentSlotItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing);

    QPen pen(QColor(100, 100, 100, 150), 1.5, Qt::DashLine);
    painter->setPen(pen);
    painter->setBrush(QColor(25, 25, 25, 180));

    painter->drawRoundedRect(boundingRect().adjusted(1,1,-1,-1), 4, 4);

    painter->setPen(QColor(80, 80, 80));
    QFont font = painter->font();
    font.setPointSize(7);
    painter->setFont(font);

    QString roman[] = {"I", "II", "III", "IV"};
    painter->drawText(boundingRect(), Qt::AlignCenter, roman[m_index % 4]);
}
