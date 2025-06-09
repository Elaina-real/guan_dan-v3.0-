#pragma once
#include "game.h"
#include "card.h"
#include "cardgroup.h"
#include "cardgroupfactory.h"
#include <memory>

//playerͷ�ļ�������Player�����������:HumanPlayer��AIPlayer
//��ҳ��ƵĲ�����Ҫ��player�������playCards������ʵ��
//AI���Ŀǰʵ���˼򵥵ĳ����߼����Զ�ѡ����ƣ��������������

class Player
{
protected:
	string name;
	vector<Card> handCards;
	int score;
	Rank playerCurrentLevel;
	PlayerRank aRoundRank;
	PlayerRank gameRank;
	Team team;
	int totalWins;
public:
	Player(string n,Team t) :name(n),team(t), score(1000),playerCurrentLevel(two),aRoundRank(NotRanked),gameRank(NotRanked),
	totalWins(0){}
	virtual ~Player() {}

	string getName() const;
	Team getTeam() const;
	Rank getLevel() const;
	vector<Card> getHandCards() const;
	int getScore() const;
	PlayerRank getARoundRank() const;
	void setARoundRank(PlayerRank rank);
	PlayerRank getGameRank() const;
	void setGameRank(PlayerRank rank);
	void addScore(int s);
	void levelUp(int n);

	void addCard(const Card& card);
	void addCards(const vector<Card>& cards);
	void removeCard(const Card& card);
	void removeCards(const vector<Card>& cards);
	void sortHandCards();
	void clearHandCards();

	void showHand()const;

	int getTotalWins() const;
	void addWin();
	void resetLevel();

	virtual unique_ptr<CardGroup> playCards(const unique_ptr<CardGroup>& lastPlay, Game& game)=0;
	virtual void returnTribute(const shared_ptr<Player>& tributer)=0;
};

class HumanPlayer : public Player
{
public:
	HumanPlayer(string n,Team t) : Player(n,t) {}
	
	//���������ѡ����ƻ��,���س��������ָ��,����������򷵻�nullptr
	unique_ptr<CardGroup> playCards(const unique_ptr<CardGroup>& lastPlay, Game& game) override;
	void returnTribute(const shared_ptr<Player>& tributer);
};

class AIPlayer : public Player
{
public:
    AIPlayer(string n, Team t) : Player(n, t) {}

    unique_ptr<CardGroup> playCards(const unique_ptr<CardGroup>& lastPlay, Game& game) override;
	void returnTribute(const shared_ptr<Player>& tributer);
};
