#pragma once

#include "game.h"
#include "card.h"
#include "cardgroup.h"
#include "cardgroupfactory.h"
#include "graphical.h"
#include <memory>

/**
 * 玩家基类
 */
class Player : public std::enable_shared_from_this<Player>
{
protected:
    string name;                // 玩家姓名
    vector<Card> handCards;     // 玩家手牌
    int score;                  // 玩家分数
    Rank playerCurrentLevel;    // 玩家当前级别
    PlayerRank aRoundRank;      // 玩家在单轮游戏中的排名
    PlayerRank gameRank;        // 玩家在整个游戏中的排名
    Team team;                  // 玩家所属队伍
    int totalWins;              // 总胜场数
    Game* gamePtr;              // 游戏指针，初始可以为nullptr
    Graphical* graphicalPtr;    // 图形界面指针

public:
    /**
     * 构造函数
     */
    Player(string n, Team t)
        : name(n), team(t), score(1000), playerCurrentLevel(two),
        aRoundRank(NotRanked), gameRank(NotRanked),
        totalWins(0), gamePtr(nullptr), graphicalPtr(nullptr) {
    }

    /**
     * 析构函数
     */
    virtual ~Player() {}

    /**
     * 设置游戏引用
     */
    void setGame(Game* g) { gamePtr = g; }

    /**
     * 设置图形界面引用
     */
    void setGraphical(Graphical* g) { graphicalPtr = g; }

    // 基本属性访问方法
    string getName() const;
    Team getTeam() const;
    Rank getLevel() const;
    vector<Card>& getHandCards();
    int getScore() const;
    PlayerRank getARoundRank() const;
    void setARoundRank(PlayerRank rank);
    PlayerRank getGameRank() const;
    void setGameRank(PlayerRank rank);

    // 分数和级别相关方法
    void addScore(int s);
    void levelUp(int n);
    int getTotalWins() const;
    void addWin();
    void resetLevel();

    // 手牌管理方法
    void addCard(const Card& card);
    void addCards(const vector<Card>& cards);
    void removeCard(const Card& card);
    void removeCards(const vector<Card>& cards);
    void sortHandCards();
    void clearHandCards();
    void showHand() const;

    /**
     * 出牌方法
     */
    virtual unique_ptr<CardGroup> playCards(const unique_ptr<CardGroup>& lastPlay, Game& game) = 0;

    /**
     * 还贡方法
     * @param tributer 进贡者
     */
    virtual void returnTribute(const shared_ptr<Player>& tributer) = 0;
};

/**
 * 人类玩家
 */
class HumanPlayer : public Player
{
public:
    HumanPlayer(string n, Team t) : Player(n, t) {}

    /**
     * 人类玩家出牌
     */
    unique_ptr<CardGroup> playCards(const unique_ptr<CardGroup>& lastPlay, Game& game) override;

    /**
     * 人类玩家还贡
     */
    void returnTribute(const shared_ptr<Player>& tributer) override;
};

/**
 * AI玩家
 */
class AIPlayer : public Player
{
public:
    AIPlayer(string n, Team t) : Player(n, t) {}

    /**
     * AI玩家出牌
     */
    unique_ptr<CardGroup> playCards(const unique_ptr<CardGroup>& lastPlay, Game& game) override;

    /**
     * AI玩家还贡
     */
    void returnTribute(const shared_ptr<Player>& tributer) override;
};
