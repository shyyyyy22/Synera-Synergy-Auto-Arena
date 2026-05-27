#ifndef PLAYER_H
#define PLAYER_H

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
    unsigned short getMajorStage()const;
    unsigned short getMinorStage()const;

    void addGold(int addGold);
    void setHp(int newHp);

    void nxtStage();

private:
    int m_hp;
    int m_gold;
    unsigned short m_level;
    unsigned short m_maxUnit;
    unsigned short m_majorStage;
    unsigned short m_minorStage;
};

#endif // PLAYER_H
