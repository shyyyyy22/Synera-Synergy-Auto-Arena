#ifndef BOARD_H
#define BOARD_H

#include<QPoint>
#include"Unit.h"
#include<QSet>
#include<vector>

class Board
{
public:
    //常量
    static constexpr int ROWS = 8;
    static constexpr int COLS = 8;

    Board(int rows,int cols,bool isBoard);
    ~Board()=default;

    //基础功能
    void addUnit(Unit* unit,const QPoint &pos);
    void removeUnit(Unit* unit);
    void moveUnit(Unit* unit,const QPoint &pos);
    Unit* getUnitAt(const QPoint &pos)const;
    bool hasUnitAt(const QPoint &pos)const;
    void clear();

    //棋盘计算
    std::vector<QPoint> getNeighborGrid(const QPoint& pos)const;
    QSet<QPoint> getRangeGrid(const QPoint& pos,int range)const;
    bool isValidPosition(const QPoint &pos)const;
    bool isPlayerHalf(const QPoint &pos)const;

private:
    int indexOf(const QPoint &pos)const;

    int m_rows;
    int m_cols;
    std::vector<Unit*> m_cells;
    bool m_isBoard;
};

#endif // BOARD_H
