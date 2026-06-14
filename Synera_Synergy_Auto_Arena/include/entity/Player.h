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
    unsigned short getXp()const;
    unsigned short getMaxXP()const;
    bool hasWaterBless()const;
    bool hasWindBless()const;
    bool hasThunderBless()const;
    bool hasFireBless()const;

    void changeGold(int addGold);
    void setHp(int newHp);
    void setGold(int newGold);
    void setLevel(int newLevel);
    void setMaxUnit(int newUnit);
    void setXp(int xp);
    void setMaxXP(int MaxXp);
    void setStage(int maj,int min);
    void activateBless(int stage);
    void initialStage();

    void nxtStage();
    void addXp(int addXp);

private:
    int m_hp;
    int m_gold;
    unsigned short m_level;
    unsigned short m_maxLevel;
    unsigned short m_maxUnit;
    unsigned short m_majorStage;
    unsigned short m_minorStage;
    unsigned short m_xp;
    unsigned short m_maxXP;

    bool m_waterBless;
    bool m_windBless;
    bool m_thunderBless;
    bool m_fireBless;
};

#endif // PLAYER_H
