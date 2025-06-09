#pragma once
#include "game.h"
#include "card.h"
#include <unordered_map>

//cardgroup头文件包含了CardGroup类和它的子类，定义了牌组的类和它的子类，子类是所有的牌型
//重载了>运算符，比较两个牌组的大小，通过调用每个子类的canBeat函数比较牌组大小
//应该实现了癞子牌的功能，通过从大到小尝试创建牌组，看是否能够大过上家出牌
//癞子牌的功能主要在牌组对>的重载中实现

class CardGroupFactory; // 前向声明

class CardGroup
{
protected:
	vector<Card> cards;
	Game& game;
	unordered_map<Rank, int> rankCount; // 统计每种点数数量
	void countRanks(); // 统计函数
public:
	CardGroup(vector<Card>& c,Game& gs) : cards(c), game(gs) { countRanks(); }

	virtual bool isValid() const = 0;
	virtual bool canBeat(const CardGroup& other) const = 0;

	bool operator>(const CardGroup& other);
	vector<Card> getCards() const;
	void setCards(const vector<Card>& c);
	unordered_map<Rank, int> getRankCount() const; // 返回rankCount
};

class Single:public CardGroup
{
public:
	Single(vector<Card>& c, Game& gs) : CardGroup(c, gs) {}
	bool isValid() const override;
	bool canBeat(const CardGroup& other) const override;
};

class Pair :public CardGroup
{
public:
	Pair(vector<Card>& c, Game& gs) : CardGroup(c, gs) {}
	bool isValid() const override;
	bool canBeat(const CardGroup& other) const override;
};

class Triple :public CardGroup
{
public:
	Triple(vector<Card>& c, Game& gs) : CardGroup(c, gs) {}
	bool isValid() const override;
	bool canBeat(const CardGroup& other) const override;
};

class TripleWithPair :public CardGroup
{
public:
	TripleWithPair(vector<Card>& c, Game& gs) : CardGroup(c, gs) {}
	bool isValid() const override;
	bool canBeat(const CardGroup& other) const override;
};

class Bomb :public CardGroup
{
public:
	Bomb(vector<Card>& c, Game& gs) : CardGroup(c, gs) {}
	bool isValid() const override;
	bool canBeat(const CardGroup& other) const override;
};

class TwoJokers :public CardGroup
{
public:
	TwoJokers(vector<Card>& c, Game& gs) : CardGroup(c, gs) {}
	bool isValid() const override;
	bool canBeat(const CardGroup& other) const override;
};

class FourJokers :public CardGroup
{
public:
	FourJokers(vector<Card>& c, Game& gs) : CardGroup(c, gs) {}
	bool isValid() const override;
	bool canBeat(const CardGroup& other) const override;
};

class Straight :public CardGroup
{
public:
	Straight(vector<Card>& c, Game& gs) : CardGroup(c, gs) {}
	bool isValid() const override;
	bool canBeat(const CardGroup& other) const override;
};

class Flush :public CardGroup
{
public:
	Flush(vector<Card>& c, Game& gs) : CardGroup(c, gs) {}
	bool isValid() const override;
	bool canBeat(const CardGroup& other) const override;
};

class SteelPlate :public CardGroup
{
public:
	SteelPlate(vector<Card>& c, Game& gs) : CardGroup(c, gs) {}
	bool isValid() const override;
	bool canBeat(const CardGroup& other) const override;
};

class ConPairs :public CardGroup
{
public:
	ConPairs(vector<Card>& c, Game& gs) : CardGroup(c, gs) {}
	bool isValid() const override;
	bool canBeat(const CardGroup& other) const override;
};
