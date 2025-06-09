#pragma once
#include "game.h"
#include "card.h"
#include <unordered_map>

//cardgroupͷ�ļ�������CardGroup����������࣬���������������������࣬���������е�����
//������>��������Ƚ���������Ĵ�С��ͨ������ÿ�������canBeat�����Ƚ������С
//Ӧ��ʵ��������ƵĹ��ܣ�ͨ���Ӵ�С���Դ������飬���Ƿ��ܹ�����ϼҳ���
//����ƵĹ�����Ҫ�������>��������ʵ��

class CardGroupFactory; // ǰ������

class CardGroup
{
protected:
	vector<Card> cards;
	Game& game;
	unordered_map<Rank, int> rankCount; // ͳ��ÿ�ֵ�������
	void countRanks(); // ͳ�ƺ���
public:
	CardGroup(vector<Card>& c,Game& gs) : cards(c), game(gs) { countRanks(); }

	virtual bool isValid() const = 0;
	virtual bool canBeat(const CardGroup& other) const = 0;

	bool operator>(const CardGroup& other);
	vector<Card> getCards() const;
	void setCards(const vector<Card>& c);
	unordered_map<Rank, int> getRankCount() const; // ����rankCount
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
