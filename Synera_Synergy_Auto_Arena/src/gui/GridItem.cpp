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
{
    for(int i=0;i<6;++i){
        qreal degrees=60.0*i-90.0;
        qreal radians=qDegreesToRadians(degrees);
        m_poly<<QPointF(m_radius*qCos(radians),m_radius*qSin(radians));
    }
    m_bounds=m_poly.boundingRect();
}

//基础功能
QRectF GridItem::boundingRect() const {
    return m_poly.boundingRect();
};
void GridItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*){
    if(m_shape==GridShape::Hexagon){
        QColor fill = m_baseColor;
        QColor border = QColor(40, 40, 40);

        painter->setPen(QPen(border, 2));
        painter->setBrush(fill);
        painter->drawPolygon(m_poly);
    }
    else {
        QColor fill = m_baseColor;
        QColor border = QColor(217,217,217);

        painter->setPen(QPen(border, 2));
        painter->setBrush(fill);
        painter->drawRect(0, 0, 1.732*m_radius, 1.732*m_radius);
    }

    //调试专用
    painter->setPen(Qt::white);
    painter->drawText(boundingRect(), Qt::AlignCenter, QString("%1,%2").arg(m_rows).arg(m_cols));
};
void GridItem::setBaseColor(const QColor& color){
    m_baseColor=color;
};

