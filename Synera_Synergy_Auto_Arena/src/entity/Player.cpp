#include "Player.h"

Player::Player()
    :m_hp(100)
    ,m_gold(1000)
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

unsigned short Player::getXp() const
{
    return m_xp;
}

unsigned short Player::getMaxXP() const
{
    return m_maxXP;
}

void Player::changeGold(int addGold)
{
    m_gold+=addGold;
}

void Player::setHp(int newHp)
{
    m_hp=newHp;
}

void Player::setGold(int newGold)
{
    m_gold=newGold;
}

void Player::setLevel(int newLevel)
{
    m_level=newLevel;
}

void Player::setMaxUnit(int newUnit)
{
    m_maxUnit=newUnit;
}

void Player::setXp(int xp)
{
    m_xp=xp;
}

void Player::setMaxXP(int MaxXp)
{
    m_maxXP=MaxXp;
}

void Player::setStage(int maj, int min)
{
    m_majorStage=maj;
    m_minorStage=min;
}

void Player::initialStage()
{
    m_minorStage=1;
    m_majorStage=1;
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