#include "Board.h"
#include"Game.h"
Board::Board(int rows,int cols,bool isBoard)
    :m_rows(rows)
    ,m_cols(cols)
    ,m_cells(rows * cols,nullptr)
    ,m_isBoard(isBoard)
{}

//基础功能
void Board::addUnit(Unit* unit,const QPoint &pos){
    int idx=indexOf(pos);
    //要改成交换回来这里
    if(!unit || idx<0 || m_cells[idx]){
        return;
    }
    m_cells[idx]=unit;
    unit->setPos(pos);
}
void Board::removeUnit(Unit* unit){
    if(!unit){
        return;
    }
    int idx=indexOf(unit->getPos());
    if(idx>=0){
        m_cells[idx]=nullptr;
    }
}
void Board::moveUnit(Unit* unit,const QPoint &pos){
    if(!unit){
        return;
    }
    removeUnit(unit);
    addUnit(unit,pos);
}
Unit* Board::getUnitAt(const QPoint &pos)const{
    int idx=indexOf(pos);
    return idx<0?nullptr:m_cells[idx];
}
bool Board::hasUnitAt(const QPoint &pos)const{
    return getUnitAt(pos)!=nullptr;
}

std::vector<QPoint> Board::getNeighborGrid(const QPoint &pos) const
{
    std::vector<QPoint> neighborGrid;
    int count=6;
    int dir[6][2]={{1,0},{-1,0},{0,1},{0,-1},{1,1},{-1,1}};
    if(pos.x()%2==0){
        dir[4][0]=-1;
        dir[4][1]=-1;
        dir[5][0]=1;
        dir[5][1]=-1;
    }
    for(int i=0;i<6;i++){
        int x=dir[i][0]+pos.x();
        int y=dir[i][1]+pos.y();
        if(x>=Board::COLS || y>=Board::ROWS || x<0 || y<0){
            continue;
        }
        else {
            neighborGrid.push_back(QPoint(x,y));
        }
    }
    return neighborGrid;
}
//工具函数
bool Board::isValidPosition(const QPoint &pos)const{
    if(m_isBoard)return pos.x()>=0 && pos.x()<m_cols && pos.y()>=0 && pos.y()<m_rows;
    else return pos.x()>=0 && pos.x()<m_cols && pos.y()==Board::ROWS;
}
bool Board::isPlayerHalf(const QPoint &pos)const{
    if(!isValidPosition(pos)){
        return false;
    }
    return pos.y()>=m_rows/2;
}

void Board::clear(){
    std::fill(m_cells.begin(), m_cells.end(), nullptr);
}

int Board::indexOf(const QPoint &pos)const{
    if(!isValidPosition(pos)){
        return -1;
    }
    if(m_isBoard)return pos.y() * m_cols + pos.x();
    else return pos.x();
}
