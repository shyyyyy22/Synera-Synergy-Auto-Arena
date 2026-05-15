#ifndef BOARD_H
#define BOARD_H

#include<QVector>
#include<QPoint>
#include"Unit.h";

class Board
{
public:
    Board(int rows,int cols);
    ~Board()=default;

    //基础功能
    void addUnit(Unit* unit,const QPoint &pos);
    void removeUnit(Unit* unit);
    void moveUnit(Unit* unit,const QPoint &pos);
    Unit* getUnitAt(const QPoint &pos)const;
    bool hasUnitAt(const QPoint &pos)const;

    //工具函数
    bool isValidPosition(const QPoint &pos)const;
    bool isPlayerHalf(const QPoint &pos)const;

    void clear();

private:
    int indexOf(const QPoint &pos)const;

    int m_rows;
    int m_cols;
    QVector<Unit*> m_cells;
};

#endif // BOARD_H
