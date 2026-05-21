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





