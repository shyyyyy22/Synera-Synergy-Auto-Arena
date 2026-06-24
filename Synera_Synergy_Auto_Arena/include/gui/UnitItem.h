#ifndef UNITITEM_H
#define UNITITEM_H

#include <QGraphicsObject>
#include"Unit.h"
#include<QPixmap>
#include<vector>
class UnitItem : public QGraphicsObject
{
    Q_OBJECT
public:
    UnitItem(Unit* unit, bool isBoard,QGraphicsItem* parent = nullptr);
    ~UnitItem()=default;

    //基础功能
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)override;

    //属性相关
    Unit* getUnit()const;
    void setGridPos(const QPoint &gridPos);
    bool getIsBoard ()const;
    void setIsBoard(bool stage);
    void setIsSelected(bool selected);
    bool getIsSelected()const;

    //动画
    void slidePosTo(const QPointF& newPos);

public slots:
    //状态改变刷新
    void unitInfoChanged(Unit* unit);
    void onDamaged(int dmg);
signals:
    void clicked(Unit* unit);
    void dragStarted(int unitId,const QPoint &gridPos,const QPointF &worldPos);
    void dragMoved(int unitId,const QPoint &gridPos,const QPointF &worldPos);
    void dragDropped(int unitId,const QPoint &gridPos,const QPointF &worldPos);
    void unitInfoReflash(Unit* unit);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
    //绘制装备
    void drawMiniItem(QPainter* painter, const QPointF& pos, Equipment type);

    //基础属性
    Unit* m_unit;
    QPoint m_gridPos;
    bool m_isBoard;
    QColor m_color;
    bool m_dragging;
    bool m_isSelected;

    //贴图以及受击相关
    struct FloatingText {
        QString text;
        QColor color;
        qreal xOffset;
        qreal yOffset;
        int alpha;
    };
    std::vector<FloatingText> m_floatingTexts;

    qreal m_auraPulse = 0.0;

    QPixmap m_sprite;
};

#endif // UNITITEM_H
