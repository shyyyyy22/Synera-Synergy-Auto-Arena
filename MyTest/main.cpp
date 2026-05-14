#include"Hero.h"
#include"UiManager.h"
#include<QApplication>
#include<QObject>
#include <QGraphicsItem>
#include <QPainter>
#include<QGraphicsScene>
#include<QGraphicsView>
// 1. 继承老祖宗
class MagicBlockItem : public QGraphicsItem {
public:
    MagicBlockItem() {
        // 构造函数，暂时什么都不干
    }

    // ===============================================
    // 问题 1：你占多大地方？（规定物理边界）
    // ===============================================
    QRectF boundingRect() const override {
        // 假设自己的中心点是 (0,0)
        // 告诉 Qt，我的领地是从 (-25, -25) 开始，宽 50，高 50 的正方形。
        return QRectF(-25, -25, 50, 50);
    }

    // ===============================================
    // 问题 2：你长什么样？（开始作画）
    // ===============================================
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        // 参数里的 painter 就是 Qt 递给你的画笔，尽情用它吧！

        // --- 动作 A：准备工具 ---
        QPen pen(Qt::black);     // 选一支黑色的勾线笔，用来画边缘轮廓
        pen.setWidth(2);         // 笔尖变粗一点

        QBrush brush(Qt::blue);  // 选一把蓝色的刷子，用来填充内部

        // 把工具拿在手里
        painter->setPen(pen);
        painter->setBrush(brush);

        // --- 动作 B：开始画画 ---
        // 在我的边界内，画一个圆（椭圆），参数恰好就用我规定的 boundingRect()
        painter->drawEllipse(boundingRect());
        painter->setBrush(Qt::green);
        painter->drawRect(-20,30,40,8);

        // （可选）再用白色画笔，在中心写个字
        painter->setPen(Qt::white);
        painter->drawText(boundingRect(), Qt::AlignCenter, "魔");
    }
};
Hero *garen = new Hero();
UiManager *ui = new UiManager();
int main(int argc ,char* argv[]){
    QApplication a(argc,argv);



    QGraphicsScene scene;
    MagicBlockItem *myBlock = new MagicBlockItem(); // 造物！
    scene.addItem(myBlock); // 登台！

    QGraphicsView view(&scene);
    view.show();
    return a.exec();
}