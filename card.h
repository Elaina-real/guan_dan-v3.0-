#pragma once
#include <iostream>
#include <string>
//#include "game.h"

using namespace std;


// ǰ������
enum Suit { Heart, Diamond, Club, Spade, Joker };
enum Rank { two = 2, three, four, five, six, seven, eight, nine, ten, jack, queen, king, ace, smal, big };
class Game;  // ֻ��ǰ������������Ҫ��������ͷ�ļ�


//cardͷ�ļ�������card���deck�࣬�������Ƶ�����ƶѵ���
//card��ʵ�����ƵĻ������ԣ������˼��ƺ�������Ա����ıȽ�
//���ǲ�֪��Ϊʲô������������ƵĻ�ɫ,���Ϊ"?"

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