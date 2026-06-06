#include "UnitItem.h"
#include <QPainter>
#include"Unit.h"
#include<QGraphicsSceneMouseEvent>
UnitItem::UnitItem(Unit* unit, bool isBoard,QGraphicsItem* parent)
    :QGraphicsObject(parent)
    ,m_unit(unit)
    ,m_isBoard(isBoard)
    ,m_dragging(false)
    ,m_isSelected(false)
{
    m_color=m_unit->getOwner()==Owner::PlayerCtrl?QColor(100, 150, 200):QColor(200,150,100);
    setAcceptedMouseButtons(Qt::LeftButton);

}

//基础功能
QRectF UnitItem::boundingRect() const {
    return QRectF(-42,-42,84,84);
}
void UnitItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*){
    painter->setRenderHint(QPainter::Antialiasing);

    //画占位符
    QPolygonF badge;
    badge << QPointF(0, -15)
          << QPointF(13, -7)
          << QPointF(13, 7)
          << QPointF(0, 15)
          << QPointF(-13, 7)
          << QPointF(-13, -7);

    painter->setPen(QPen(QColor(18, 18, 18), 1.5));
    painter->setBrush(m_color);
    painter->drawPolygon(badge);

    if (m_unit) {
        painter->setPen(Qt::white);
        QFont font = painter->font();
        font.setPointSize(12);
        font.setBold(true);
        painter->setFont(font);
        painter->drawText(QRectF(-13, -13, 26, 26), Qt::AlignCenter, m_unit->getName().left(1));

        //血条
        qreal hpPercent=qreal(m_unit->getHp())/m_unit->getMaxHp();
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(100, 0, 0));
        painter->drawRect(-15,-25,30,5);
        painter->setBrush(Qt::green);
        painter->drawRect(-15,-25,30*hpPercent,5);
        //蓝条
        qreal manaPercent=qreal(m_unit->getMana())/m_unit->getMaxMana();
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(100, 0, 0));
        painter->drawRect(-15,20,30,5);
        painter->setBrush(QColor(80,130,200));
        painter->drawRect(-15,20,30*manaPercent,5);
        //高亮
        if (m_isSelected) {
            painter->setPen(QPen(Qt::yellow, 4));
            painter->setBrush(Qt::NoBrush);
            painter->drawEllipse(QPoint(m_gridPos.x()-3,m_gridPos.y()-10),30,35);
        }

        //装备
        Equipment items = m_unit->getEquipment();
        int startX = 14;
        int startY = 22;
        QPointF miniIconPos(startX, startY);
        drawMiniItem(painter, miniIconPos, items);
    }
}

//属性相关
Unit* UnitItem::getUnit()const{
    return m_unit;
}
void UnitItem::setGridPos(const QPoint& gridPos){
    m_gridPos=gridPos;
}
bool UnitItem::getIsBoard()const{
    return m_isBoard;
}

void UnitItem::setIsBoard(bool stage)
{
    m_isBoard=stage;
}

void UnitItem::setIsSelected(bool selected)
{
    m_isSelected=selected;
}

bool UnitItem::getIsSelected() const
{
    return m_isSelected;
}

void UnitItem::unitInfoChanged(Unit *unit)
{
    if(m_isSelected){
        emit unitInfoReflash(unit);
    }
}


//拖拽
void UnitItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    if(event->button()!=Qt::LeftButton){
        QGraphicsObject::mousePressEvent(event);
        return;
    }

    m_isSelected=!m_isSelected;
    emit clicked(m_unit);
    if(m_unit->getOwner()==Owner::PlayerCtrl){
        m_dragging=true;
        emit dragStarted(getUnit()->getId(),m_gridPos,event->scenePos());
        event->accept();
    }
}

void UnitItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(!m_dragging){
        QGraphicsObject::mouseMoveEvent(event);
        return;
    }

    m_isSelected=true;
    emit dragMoved(getUnit()->getId(),m_gridPos,event->scenePos());
    emit clicked(m_unit);
    event->accept();
}

void UnitItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(!m_dragging || event->button()!=Qt::LeftButton){
        QGraphicsObject::mouseReleaseEvent(event);
        return;
    }

    m_dragging=false;
    emit dragDropped(getUnit()->getId(),m_gridPos,event->scenePos());
    event->accept();
}

void UnitItem::drawMiniItem(QPainter* painter, const QPointF& pos, Equipment type ) {
    if ( type == Equipment::None) return;

    QRectF rect(pos.x() - 7, pos.y() - 7, 14, 14);

    painter->setPen(QPen(QColor(18, 18, 18), 1));
    painter->setBrush(QColor(25, 25, 25, 230));
    painter->drawRoundedRect(rect, 2, 2);

    QColor color;
    switch(type) {
    case Equipment::Sword:   color = QColor(190, 190, 200); break;
    case Equipment::Mail:    color = QColor(100, 130, 160); break;
    case Equipment::Gloves:  color = QColor(180, 110, 60);  break;
    case Equipment::Crystal: color = QColor(0, 190, 255);   break;
    default: return;
    }

    painter->setBrush(color);
    painter->setPen(Qt::NoPen);

    if (type== Equipment ::Sword) {
        painter->setPen(QPen(color, 2));
        painter->drawLine(rect.left() + 3, rect.bottom() - 3, rect.right() - 3, rect.top() + 3);
    }
    else if (type == Equipment ::Mail) {
        QPolygonF armor;
        armor << QPointF(rect.center().x(), rect.top() + 2)
              << QPointF(rect.right() - 2, rect.top() + 4)
              << QPointF(rect.right() - 2, rect.center().y() + 2)
              << QPointF(rect.center().x(), rect.bottom() - 2)
              << QPointF(rect.left() + 2, rect.center().y() + 2)
              << QPointF(rect.left() + 2, rect.top() + 4);
        painter->drawPolygon(armor);
    }
    else if (type == Equipment ::Gloves) {
        painter->drawEllipse(rect.center(), 3, 3);
        painter->setPen(QPen(color, 1));
        painter->drawLine(rect.center().x() - 3, rect.center().y() + 3, rect.center().x() + 3, rect.center().y() + 3);
    }
    else if ( type == Equipment ::Crystal) {
        QPolygonF gem;
        gem << QPointF(rect.center().x(), rect.top() + 2)
            << QPointF(rect.right() - 2, rect.center().y())
            << QPointF(rect.center().x(), rect.bottom() - 2)
            << QPointF(rect.left() + 2, rect.center().y());
        painter->drawPolygon(gem);
    }
}





