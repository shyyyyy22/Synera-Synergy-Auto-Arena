#include "EquipmentItem.h"
#include<QPainter>
#include<QGraphicsSceneMouseEvent>
EquipmentItem::EquipmentItem(Equipment type, int index, QGraphicsItem *parent)
    :QGraphicsObject(parent)
    ,m_type(type)
    ,m_index(index)
    ,m_dragging(false)
{
    switch(m_type){
    case Equipment::Sword:
        m_color=QColor(170,170,180);
        m_name="铁剑";
        break;
    case Equipment::Mail:
        m_color=QColor(100,115,130);
        m_name="锁子甲";
        break;
    case Equipment::Gloves:
        m_color=QColor(160,90,50);
        m_name="急速手套";
        break;
    case Equipment::Crystal:
        m_color=QColor(0,190,255);
        m_name="蓝水晶";
        break;
    default:
        break;
    }

    setAcceptedMouseButtons(Qt::LeftButton);
}

QRectF EquipmentItem::boundingRect() const
{
    return QRectF(-18,-18,36,36);
}

void EquipmentItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(m_type==Equipment::None){
        return;
    }
    painter->setRenderHint(QPainter::Antialiasing);

    painter->setPen(QPen(QColor(18, 18, 18), 1.5));
    painter->setBrush(QColor(30, 30, 30, 230));
    painter->drawRoundedRect(boundingRect().adjusted(1, 1, -1, -1), 4, 4);

    painter->setBrush(m_color);
    if (m_type == Equipment::Sword) {
        QPolygonF blade;
        blade << QPointF(-3, 8) << QPointF(-3, -12) << QPointF(0, -16)
              << QPointF(3, -12) << QPointF(3, 8);
        painter->drawPolygon(blade);
        painter->setBrush(QColor(110, 80, 50));
        painter->drawRect(-6, 8, 12, 3);
        painter->drawRect(-1, 11, 2, 4);
        painter->setPen(Qt::white);
        painter->drawText(QRectF(36,-18,60,30), Qt::AlignCenter, "攻击力+15");
    }
    else if (m_type == Equipment::Mail) {
        QPolygonF armor;
        armor << QPointF(-10, -10) << QPointF(10, -10) << QPointF(12, 2)
              << QPointF(0, 14) << QPointF(-12, 2);
        painter->drawPolygon(armor);
        painter->setPen(Qt::white);
        painter->drawText(QRectF(36,-18,70,30), Qt::AlignCenter, "生命值+150");
    }
    else if (m_type == Equipment::Gloves) {
        painter->drawEllipse(-10, -8, 20, 18);
        painter->drawRect(-7, 4, 14, 6);
        painter->setPen(Qt::white);
        painter->drawText(QRectF(36,-18,100,30), Qt::AlignCenter, "攻击速度提升20%");
    }
    else if (m_type == Equipment::Crystal) {
        QPolygonF gem;
        gem << QPointF(0, -14) << QPointF(10, 0) << QPointF(0, 14) << QPointF(-10, 0);
        painter->drawPolygon(gem);
        painter->setPen(Qt::white);
        painter->drawText(QRectF(36,-18,80,30), Qt::AlignCenter, "最大法力值-20");
    }

    painter->setPen(Qt::white);
    QFont font = painter->font();
    font.setPointSize(7);
    font.setBold(true);
    painter->setFont(font);
    QPointF pos=QPointF(boundingRect().x(),boundingRect().y());
    painter->drawText(QRectF(-18,0,36,36),Qt::AlignCenter, m_name);
}

Equipment EquipmentItem::getType() const
{
    return m_type;
}

void EquipmentItem::setType(Equipment newType)
{
    m_type=newType;
    switch (m_type) {
    case Equipment::Sword:
        m_color = QColor(170, 170, 180); m_name = "铁剑"; break;
    case Equipment::Mail:
        m_color = QColor(100, 115, 130); m_name = "锁子甲"; break;
    case Equipment::Gloves:
        m_color = QColor(160, 90, 50);   m_name = "急速手套"; break;
    case Equipment::Crystal:
        m_color = QColor(0, 190, 255);   m_name = "蓝水晶"; break;
    default:
        m_color = Qt::transparent; m_name = ""; break;
    }
}
void EquipmentItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    if(event->button()!=Qt::LeftButton){
        QGraphicsObject::mousePressEvent(event);
        return;
    }

    m_dragging=true;
    emit dragStarted(m_index,m_type,event->scenePos());
    event->accept();
}

void EquipmentItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(!m_dragging){
        QGraphicsObject::mouseMoveEvent(event);
        return;
    }

    emit dragMoved(m_index,m_type,event->scenePos());
    event->accept();
}

void EquipmentItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(!m_dragging || event->button()!=Qt::LeftButton){
        QGraphicsObject::mouseReleaseEvent(event);
        return;
    }

    m_dragging=false;
    emit dragDropped(m_index,m_type,event->scenePos());
    event->accept();
}
