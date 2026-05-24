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

    //属性相关
    void setBaseColor(const QColor& color);
    void setHoverActive(bool active);
    void setDropActive(bool active);
    QPoint getPos()const;
    void setRangeActive(bool active);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
private:
    int m_rows;
    int m_cols;
    qreal m_radius;
    QColor m_baseColor;
    QPolygonF m_poly;
    QRectF m_bounds;
    GridShape m_shape;
    bool m_dropActive;
    bool m_hoverActive;
    bool m_pointerHover;
    bool m_rangeActive;
};

#endif // GRIDITEM_H
