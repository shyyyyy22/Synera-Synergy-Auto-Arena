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
private:
    Unit* m_unit;
    QPoint m_gridPos;
    bool m_isBoard;
    QColor m_color;
};

#endif // UNITITEM_H
