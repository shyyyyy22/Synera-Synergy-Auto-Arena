#include "GridItem.h"
#include <QPainter>
#include <QtMath>

GridItem::GridItem(int row, int col, qreal radius, GridShape shape, QGraphicsItem* parent)
    :QGraphicsObject(parent)
    ,m_rows(row)
    ,m_cols(col)
    ,m_radius(radius)
    ,m_baseColor(QColor(60,60,80))
    ,m_shape(shape)
    ,m_dropActive(false)
    ,m_hoverActive(false)
    ,m_pointerHover(false)
    ,m_rangeActive(false)
{
    for(int i=0;i<6;++i){
        qreal degrees=60.0*i-90.0;
        qreal radians=qDegreesToRadians(degrees);
        m_poly<<QPointF(m_radius*qCos(radians),m_radius*qSin(radians));
    }
    m_bounds=m_poly.boundingRect();

    setAcceptHoverEvents(true);
}

//基础功能
QRectF GridItem::boundingRect() const {
    if (m_shape == GridShape::Hexagon) {
        return m_poly.boundingRect();
    } else {
        qreal side = 1.732 * m_radius;
        return QRectF(-side/2.0, -side/2.0, side, side);
    }
};
void GridItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*){    
    if(m_shape==GridShape::Hexagon){
        QColor fill = m_baseColor;
        QColor border = QColor(40, 40, 40);

        if(m_rangeActive){
            setZValue(1.0);
            fill = QColor(0, 180, 255, 35);
            border = QColor(100, 220, 255, 190);
        }
        if(m_dropActive){
            fill = QColor(110, 170, 110);
            border = QColor(100, 255, 100);
        }

        else if(m_hoverActive){
            fill=m_baseColor.lighter(120);
        }

        painter->setPen(QPen(border, 2));
        painter->setBrush(fill);
        painter->drawPolygon(m_poly);

        //高亮边框
        if (m_hoverActive || m_pointerHover) {
            setZValue(1.0);
            painter->setPen(QPen(QColor(220, 220, 220), 2));
            painter->setBrush(Qt::NoBrush);
            painter->drawPolygon(m_poly);
        }
    }
    else {
        QColor fill = m_baseColor;
        QColor border = QColor(40,40,40);

        if(m_dropActive){
            fill = QColor(110, 170, 110);
            border = QColor(100, 255, 100);
        }
        else if(m_hoverActive || m_pointerHover){
            fill=m_baseColor.lighter(120);
        }

        painter->setPen(QPen(border, 2));
        painter->setBrush(fill);
        qreal side = 1.732 * m_radius;
        painter->drawRect(-side/2.0, -side/2.0, side, side);

        //高亮边框
        if (m_hoverActive || m_pointerHover) {
            setZValue(1.0);
            painter->setPen(QPen(QColor(220, 220, 220), 2));
            painter->setBrush(Qt::NoBrush);
            painter->drawRect(-side/2.0, -side/2.0, side, side);
        }
    }


    //调试专用
    //painter->setPen(Qt::white);
    //painter->drawText(boundingRect(), Qt::AlignCenter, QString("%1,%2").arg(m_rows).arg(m_cols));
};

//属性相关
void GridItem::setBaseColor(const QColor& color){
    m_baseColor=color;
}
void GridItem::setHoverActive(bool active)
{
    if(m_hoverActive==active){
        return;
    }
    m_hoverActive=active;
    update();
}

void GridItem::setDropActive(bool active)
{
    if(m_dropActive==active){
        return;
    }
    m_dropActive=active;
    update();
}

QPoint GridItem::getPos()const
{
    return QPoint(m_cols,m_rows);
}

void GridItem::setRangeActive(bool active)
{
    if(m_rangeActive==active){
        return;
    }
    m_rangeActive=active;
    update();
}
//处理高亮
void GridItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    if(!m_pointerHover){
        setZValue(1.0);
        m_pointerHover=true;
        update();
    }
}
void GridItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    if(m_pointerHover){
        setZValue(0.0);
        m_pointerHover=false;
        update();
    }
}

