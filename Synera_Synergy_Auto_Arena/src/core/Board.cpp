#include "Board.h"

Board::Board(int rows,int cols)
    :m_rows(rows)
    ,m_cols(cols)
    ,m_cells(rows * cols,nullptr)
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
    int idx=indexOf(pos);
    return (idx>0||m_cells[idx]!=nullptr);
}
//工具函数
bool Board::isValidPosition(const QPoint &pos)const{
    return pos.x()>=0 && pos.x()<m_cols && pos.y()>=0 && pos.y()<m_rows;
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
    return pos.y() * m_cols + pos.x();
}