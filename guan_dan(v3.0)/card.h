#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum Suit {
    Heart,    // 红桃
    Diamond,  // 方块
    Club,     // 梅花
    Spade,    // 黑桃
    Joker     // 王牌
};

enum Rank {
    two = 2, three, four, five, six, seven, eight, nine, ten,
    jack, queen, king, ace, smal, big
};

class Game;

/**
 * 扑克牌类
 * 表示游戏中的单张扑克牌，包含花色、点数和特殊属性
 */
class Card
{
private:
    Suit suit;      
    Rank rank;      
    bool isLevel;   
    bool isWild;    
    bool isSelected;

public:
    // 构造函数
    Card(Suit s, Rank r, bool level = false, bool wild = false, bool selected = false)
        : suit(s), rank(r), isLevel(level), isWild(wild), isSelected(selected) {
    }

    // 支持整数点数的构造函数
    Card(Suit s, int r, bool level = false, bool wild = false, bool selected = false)
        : suit(s), rank(static_cast<Rank>(r)), isLevel(level), isWild(wild), isSelected(selected) {
    }

    // 属性访问方法
    Suit getSuit() const;
    Rank getRank() const;
    bool isLevelCard() const;
    bool isWildCard() const;
    bool isSelectedCard() const;

    // 卡牌选择状态控制
    void toggleSelected();
    void setSelected(bool selected);

    // 运算符重载
    friend ostream& operator<<(ostream& os, const Card& card);
    bool operator==(const Card& other) const;
    bool operator>(const Card& other) const;
};

/**
 * 牌组类
 * 管理游戏中的牌堆，包括洗牌和发牌功能
 */
class Deck
{
private:
    vector<Card> cards;    
    Rank currentLevel;    

public:
    // 构造函数
    Deck(Rank l);

    // 牌组操作方法
    void initialize(Rank l); 
    void shuffle();          
    vector<Card> dealCards(int num);
};
