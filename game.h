#pragma once
#include "cardcounter.h"
#include <unordered_map>
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>  // 添加此行支持shared_ptr和unique_ptr
#include "card.h"  // 首先包含card.h以获取Suit和Rank枚举
#include "graphical.h"


using namespace std;

//game头文件包含了主游戏类Game类，游戏程序的主逻辑
//目前实现了单局游戏的流程，和整局掼蛋游戏的流程，能够记录玩家的总胜场数

//class Card;
class Player;
class CardGroup;
class CardGroupFactory;
class HumanPlayer;
//class Deck;

//enum Suit { Heart, Diamond, Club, Spade, Joker };
////分别是红桃、方块、梅花、黑桃和大小王
//enum Rank { two = 2, three, four, five, six, seven, eight, nine, ten, jack, queen, king, ace, smal, big };
//// 牌的点数从2到A，大小王分别是小王和大王
enum PlayerRank { First, Second, Third, Fourth, NotRanked };
enum Team { Red, Blue};




class Game 
{
private:
    Rank currentLevel;
    vector<shared_ptr<Player>> players;
	unique_ptr<Deck> deck;
    unique_ptr<CardGroup> lastPlay;
    unordered_map < PlayerRank, shared_ptr<Player>> rankingList;
    CardCounter cardCounter;
    int scoreFactor;
	Graphical graphical; // 图形化对象，用于绘制牌桌和显示信息
public:
    Game(vector<shared_ptr<Player>> playerList, Rank startLevel = two);
    ~Game();

    Rank getLevel() const;
    int getScoreFactor() const;//获取当前倍数
    void doubleScoreFactor();   //倍数翻倍
    const CardGroup* getLastPlay() const;

    //获取玩家
	vector<shared_ptr<Player>> getPlayers() const { return players; }

	void resetRoundGame();//重置一局游戏
    void setLastPlay(unique_ptr<CardGroup> newPlay);
	void aWholeGame();//处理整局游戏
	void aRoundGame();//处理一局游戏
	void tribute();//处理贡牌
	void dealCards();//发牌
    bool isAWholeGameOver();
    void resetGame(); //重置游戏
	void showLeaderboard();//展示排行榜

	//检查是否为需要翻倍的特殊牌型
    bool isSpecialPlay(const CardGroup* cardGroup);

    //赛后结算分数
    void settleScore();

    //记牌器相关
    void updateCardCounter(const vector<Card>& playedCards);
    void showCardCounter() const;
    void resetCardCounter();
    void showCardCounterExcludePlayer(const vector<Card>& playerCards) const;

};