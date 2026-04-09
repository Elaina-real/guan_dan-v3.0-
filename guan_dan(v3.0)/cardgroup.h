#pragma once

#include "game.h"
#include "card.h"
#include <unordered_map>

class CardGroupFactory;

/**
 * 卡牌组基类
 */
class CardGroup
{
protected:
    vector<Card> cards;                  // 组成牌组的卡牌
    Game& game;                          // 游戏引用
    unordered_map<Rank, int> rankCount;  // 统计每种点数数量

    /**
     * 计算不同点数的数量
     */
    void countRanks();

public:
    /**
     * 构造函数
     */
    CardGroup(vector<Card>& c, Game& gs) : cards(c), game(gs) { countRanks(); }

    /**
     * 检查牌组是否符合规则
     * @return 如果牌组有效则返回true
     */
    virtual bool isValid() const = 0;

    /**
     * 检查当前牌组是否能够打过另一个牌组
     * @param other 其他牌组
     * @return 如果能打过则返回true
     */
    virtual bool canBeat(const CardGroup& other) const = 0;

    /**
     * 比较运算符，检查当前牌组是否大于其他牌组
     */
    bool operator>(const CardGroup& other);

    /**
     * 获取牌组中的所有卡牌
     * @return 卡牌集合
     */
    vector<Card> getCards() const;

    /**
     * 设置牌组中的卡牌
     * @param c 新的卡牌集合
     */
    void setCards(const vector<Card>& c);

    /**
     * 获取点数统计信息
     * @return 各点数数量的映射表
     */
    unordered_map<Rank, int> getRankCount() const;
};

/**
 * 单张牌组
 */
class Single : public CardGroup
{
public:
    Single(vector<Card>& c, Game& gs) : CardGroup(c, gs) {}
    bool isValid() const override;
    bool canBeat(const CardGroup& other) const override;
};

/**
 * 对子牌组
 */
class Pair : public CardGroup
{
public:
    Pair(vector<Card>& c, Game& gs) : CardGroup(c, gs) {}
    bool isValid() const override;
    bool canBeat(const CardGroup& other) const override;
};

/**
 * 三张牌组
 */
class Triple : public CardGroup
{
public:
    Triple(vector<Card>& c, Game& gs) : CardGroup(c, gs) {}
    bool isValid() const override;
    bool canBeat(const CardGroup& other) const override;
};

/**
 * 三带二牌组
 */
class TripleWithPair : public CardGroup
{
public:
    TripleWithPair(vector<Card>& c, Game& gs) : CardGroup(c, gs) {}
    bool isValid() const override;
    bool canBeat(const CardGroup& other) const override;
};

/**
 * 炸弹牌组
 */
class Bomb : public CardGroup
{
public:
    Bomb(vector<Card>& c, Game& gs) : CardGroup(c, gs) {}
    bool isValid() const override;
    bool canBeat(const CardGroup& other) const override;
};

/**
 * 双王（小王+大王）牌组
 */
class TwoJokers : public CardGroup
{
public:
    TwoJokers(vector<Card>& c, Game& gs) : CardGroup(c, gs) {}
    bool isValid() const override;
    bool canBeat(const CardGroup& other) const override;
};

/**
 * 四王炸（四个王）牌组
 */
class FourJokers : public CardGroup
{
public:
    FourJokers(vector<Card>& c, Game& gs) : CardGroup(c, gs) {}
    bool isValid() const override;
    bool canBeat(const CardGroup& other) const override;
};

/**
 * 顺子牌组
 */
class Straight : public CardGroup
{
public:
    Straight(vector<Card>& c, Game& gs) : CardGroup(c, gs) {}
    bool isValid() const override;
    bool canBeat(const CardGroup& other) const override;
};

/**
 * 同花顺牌组
 */
class Flush : public CardGroup
{
public:
    Flush(vector<Card>& c, Game& gs) : CardGroup(c, gs) {}
    bool isValid() const override;
    bool canBeat(const CardGroup& other) const override;
};

/**
 * 钢板（同点数多张）牌组
 */
class SteelPlate : public CardGroup
{
public:
    SteelPlate(vector<Card>& c, Game& gs) : CardGroup(c, gs) {}
    bool isValid() const override;
    bool canBeat(const CardGroup& other) const override;
};

/**
 * 连对牌组
 */
class ConPairs : public CardGroup
{
public:
    ConPairs(vector<Card>& c, Game& gs) : CardGroup(c, gs) {}
    bool isValid() const override;
    bool canBeat(const CardGroup& other) const override;
};
