#ifndef UNITITEM_H
#define UNITITEM_H

#include <QGraphicsObject>
#include"Unit.h"
class UnitItem : public QGraphicsObject
{
    Q_OBJECT
public:
    UnitItem(Unit* unit, bool isBoard,QGraphicsItem* parent = nullptr);

    //基础功能
    QRectF boundingRect() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

    //属性相关
    Unit* getUnit()const;
    void setGridPos(const QPoint &gridPos);
    bool getIsBoard ()const;
    void setIsBoard(bool stage);
    void setIsSelected(bool selected);
    bool getIsSelected()const;

signals:
    void clicked(Unit* unit);
    void dragStarted(int unitId,const QPoint &gridPos,const QPointF &worldPos);
    void dragMoved(int unitId,const QPoint &gridPos,const QPointF &worldPos);
    void dragDropped(int unitId,const QPoint &gridPos,const QPointF &worldPos);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
    Unit* m_unit;
    QPoint m_gridPos;
    bool m_isBoard;
    QColor m_color;
    bool m_dragging;
    bool m_isSelected;
};

#endif // UNITITEM_H
