#ifndef GRIDITEM_H
#define GRIDITEM_H

#include <QGraphicsObject>
#include <QPolygonF>

class GridItem : public QGraphicsObject
{
    Q_OBJECT
public:
    GridItem(int row, int col, qreal radius, QGraphicsItem* parent = nullptr);
    ~GridItem();

private:
    int m_rows;
    int m_cols;
    qreal m_radius;
    QPolygonF m_poly;
    QRectF m_bounds;
    QColor m_baseColor;
};

#endif // GRIDITEM_H
