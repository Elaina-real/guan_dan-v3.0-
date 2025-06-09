#pragma once
#include <iostream>
#include <string>
//#include "game.h"

using namespace std;


// 前向声明
enum Suit { Heart, Diamond, Club, Spade, Joker };
enum Rank { two = 2, three, four, five, six, seven, eight, nine, ten, jack, queen, king, ace, smal, big };
class Game;  // 只需前向声明，不需要包含整个头文件


//card头文件包含了card类和deck类，定义了牌的类和牌堆的类
//card类实现了牌的基本属性，包括了级牌和癞子牌以便后面的比较
//但是不知道为什么不能正常输出牌的花色,输出为"?"

class Card
{
private:
	Suit suit;
	Rank rank;
	bool isLevel;
	bool isWild;
public:
	Card(Suit s, Rank r, bool level = false,bool wild = false) : suit(s), rank(r), isLevel(level),isWild(wild) {}
	Card(Suit s, int r, bool level = false, bool wild = false) : suit(s), rank(static_cast<Rank>(r)), isLevel(level),isWild(wild) {} // Add this constructor to handle integer rank values  

	Suit getSuit() const;
	Rank getRank() const;
	bool isLevelCard()const;
	bool isWildCard()const;
	friend ostream& operator<<(ostream& os, const Card& card);
	bool operator==(const Card& other)const;
	bool operator>(const Card& other)const;
};

class Deck
{
private:
	vector<Card> cards;
	Rank currentLevel;
public:
	Deck(Rank l);

	void initialize(Rank l);
	void shuffle();
	vector<Card> dealCards(int num);
};