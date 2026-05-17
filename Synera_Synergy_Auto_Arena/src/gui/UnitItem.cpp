#include "UnitItem.h"
#include <QPainter>
#include"Unit.h"
#include<QGraphicsSceneMouseEvent>
UnitItem::UnitItem(Unit* unit, bool isBoard,QGraphicsItem* parent)
    :QGraphicsObject(parent)
    ,m_unit(unit)
    ,m_isBoard(isBoard)
    ,m_dragging(false)
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


//拖拽
void UnitItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()!=Qt::LeftButton){
        QGraphicsObject::mousePressEvent(event);
        return;
    }

    m_dragging=true;
    emit dragStarted(getUnit()->getId(),m_gridPos,event->scenePos());
    event->accept();
}

void UnitItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(!m_dragging){
        QGraphicsObject::mouseMoveEvent(event);
        return;
    }

    emit dragMoved(getUnit()->getId(),m_gridPos,event->scenePos());
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





