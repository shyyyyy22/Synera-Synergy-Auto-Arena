#ifndef PLAYER_H
#define PLAYER_H

#include"Board.h"

class Player
{
public:
    Player();
    ~Player()=default;

    //基础功能
    int getHp()const;
    int getGold()const;
    unsigned short getLevel()const;
    unsigned short getMaxUnit()const;

private:
    int m_hp;
    int m_gold;
    unsigned short m_level;
    unsigned short m_maxUnit;
    unsigned short m_majorStage;
    unsigned short m_minorStage;
    Board m_bench;
};

#endif // PLAYER_H
