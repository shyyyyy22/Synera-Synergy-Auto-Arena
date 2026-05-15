#include "Player.h"

Player::Player()
    :m_hp(100)
    ,m_gold(0)
    ,m_level(0)
    ,m_maxUnit(3)
    ,m_majorStage(1)
    ,m_minorStage(1)
    ,m_bench(1,8)
{}

//基础功能
int Player::getHp()const{
    return m_hp;
};
int Player::getGold()const{
    return m_gold;
};
unsigned short Player::getLevel()const{
    return m_level;
};
unsigned short Player::getMaxUnit()const{
    return m_maxUnit;
};