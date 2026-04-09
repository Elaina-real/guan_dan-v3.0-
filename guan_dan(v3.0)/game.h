#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <unordered_map>

#include "card.h"
#include "cardcounter.h"
#include "graphical.h"

using namespace std;

class Player;
class CardGroup;
class CardGroupFactory;
class HumanPlayer;

/**
 * 玩家排名枚举
 */
enum PlayerRank {
    First,     
    Second,    
    Third,     
    Fourth,    
    NotRanked   // 未排名
};

/**
 * 玩家队伍枚举
 */
enum Team {
    Red,        
    Blue        
};

/**
 * 游戏主类
 */
class Game
{
private:
    // 游戏核心数据
    Rank currentLevel;                                   // 当前级别
    vector<shared_ptr<Player>> players;                  // 玩家列表
    unique_ptr<Deck> deck;                               // 牌堆
    unique_ptr<CardGroup> lastPlay;                      // 最后出的牌组
    unordered_map<PlayerRank, shared_ptr<Player>> rankingList; // 排名列表
    CardCounter cardCounter;                             // 记牌器
    int scoreFactor;                                     // 分数倍数
    Graphical graphical;                                 // 图形界面

public:
    /**
     * 构造函数
     */
    Game(vector<shared_ptr<Player>> playerList, Rank startLevel = two);

    /**
     * 析构函数
     */
    ~Game();

    // 游戏状态访问方法
    Rank getLevel() const;                           // 获取当前级别
    int getScoreFactor() const;                      // 获取当前倍数
    const CardGroup* getLastPlay() const;            // 获取最后一次出牌
    vector<shared_ptr<Player>> getPlayers() const { return players; } // 获取玩家列表

    // 游戏控制方法
    void doubleScoreFactor();                        // 倍数翻倍
    void resetRoundGame();                           // 重置一局游戏
    void setLastPlay(unique_ptr<CardGroup> newPlay); // 设置最后一次出牌

    // 游戏流程方法
    void aWholeGame();                               // 处理整局游戏
    void aRoundGame();                               // 处理一局游戏
    void tribute();                                  // 处理贡牌
    void dealCards();                                // 发牌
    bool isAWholeGameOver();                         // 检查整局游戏是否结束
    void resetGame();                                // 重置游戏
    void showLeaderboard();                          // 展示排行榜

    /**
     * 检查是否为需要翻倍的特殊牌型
     */
    bool isSpecialPlay(const CardGroup* cardGroup);

    /**
     * 赛后结算分数
     */
    void settleScore();

    // 记牌器相关方法
    void updateCardCounter(const vector<Card>& playedCards);             // 更新记牌器
    void showCardCounter() const;                                         // 显示记牌器信息
    void resetCardCounter();                                              // 重置记牌器
    void showCardCounterExcludePlayer(const vector<Card>& playerCards) const; // 显示记牌器(排除指定玩家手牌)
};
