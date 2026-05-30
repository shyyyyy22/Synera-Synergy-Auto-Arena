#include "Player.h"

Player::Player()
    :m_hp(100)
    ,m_gold(10)
    ,m_level(1)
    ,m_maxLevel(6)
    ,m_maxUnit(3)
    ,m_majorStage(1)
    ,m_minorStage(1)
    ,m_xp(0)
    ,m_maxXP(2)
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
}

unsigned short Player::getMajorStage() const
{
    return m_majorStage;
}

unsigned short Player::getMinorStage() const
{
    return m_minorStage;
}

void Player::changeGold(int addGold)
{
    m_gold+=addGold;
}

void Player::setHp(int newHp)
{
    m_hp=newHp;
}

void Player::nxtStage()
{
    if(m_minorStage==4){
        m_majorStage++;
        m_minorStage=1;
    }
    else {
        m_minorStage++;
    }
}

void Player::addXp(int addXp)
{
    if(m_xp+addXp>=m_maxXP && m_level<m_maxLevel){
        m_xp=m_xp+addXp-m_maxXP;
        m_maxXP*=2;
        m_level++;
        m_maxUnit++;
    }
    else {
        m_xp+=addXp;
    }
};