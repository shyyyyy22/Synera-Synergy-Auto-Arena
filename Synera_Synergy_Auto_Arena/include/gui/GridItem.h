#ifndef GRIDITEM_H
#define GRIDITEM_H

#include <QGraphicsObject>
#include <QPolygonF>

enum class GridShape { Hexagon, Square };

class GridItem : public QGraphicsObject
{
    Q_OBJECT
public:
    GridItem(int row, int col, qreal radius, GridShape shape, QGraphicsItem* parent = nullptr);
    ~GridItem()=default;

    //基础功能
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    void setBaseColor(const QColor& color);

private:
    int m_rows;
    int m_cols;
    qreal m_radius;
    QColor m_baseColor;
    QPolygonF m_poly;
    QRectF m_bounds;
    GridShape m_shape;
};

#endif // GRIDITEM_H
