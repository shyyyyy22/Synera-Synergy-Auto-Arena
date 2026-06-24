#ifndef EQUIPMENTITEM_H
#define EQUIPMENTITEM_H

#include <QGraphicsObject>
enum class Equipment{None,Sword,Mail,Gloves,Crystal};

class EquipmentItem : public QGraphicsObject
{
    Q_OBJECT
public:
    EquipmentItem(Equipment type,int index,QGraphicsItem* parent=nullptr);

    //基础功能
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    //属性相关
    Equipment getType()const;
    void setType(Equipment newType);

signals:
    void dragStarted(int index,Equipment type,const QPointF &worldPos);
    void dragMoved(int index,Equipment type,const QPointF &worldPos);
    void dragDropped(int index,Equipment type,const QPointF &worldPos);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
    Equipment m_type;
    int m_index;
    QColor m_color;
    QString m_name;

    bool m_dragging;
};

#endif // EQUIPMENTITEM_H
