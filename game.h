#pragma once
#include "cardcounter.h"
#include <unordered_map>
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>  // ��Ӵ���֧��shared_ptr��unique_ptr
#include "card.h"  // ���Ȱ���card.h�Ի�ȡSuit��Rankö��
#include "graphical.h"


using namespace std;

//gameͷ�ļ�����������Ϸ��Game�࣬��Ϸ��������߼�
//Ŀǰʵ���˵�����Ϸ�����̣��������走��Ϸ�����̣��ܹ���¼��ҵ���ʤ����

//class Card;
class Player;
class CardGroup;
class CardGroupFactory;
class HumanPlayer;
//class Deck;

//enum Suit { Heart, Diamond, Club, Spade, Joker };
////�ֱ��Ǻ��ҡ����顢÷�������Һʹ�С��
//enum Rank { two = 2, three, four, five, six, seven, eight, nine, ten, jack, queen, king, ace, smal, big };
//// �Ƶĵ�����2��A����С���ֱ���С���ʹ���
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
	Graphical graphical; // ͼ�λ��������ڻ�����������ʾ��Ϣ
public:
    Game(vector<shared_ptr<Player>> playerList, Rank startLevel = two);
    ~Game();

    Rank getLevel() const;
    int getScoreFactor() const;//��ȡ��ǰ����
    void doubleScoreFactor();   //��������
    const CardGroup* getLastPlay() const;

    //��ȡ���
	vector<shared_ptr<Player>> getPlayers() const { return players; }

	void resetRoundGame();//����һ����Ϸ
    void setLastPlay(unique_ptr<CardGroup> newPlay);
	void aWholeGame();//����������Ϸ
	void aRoundGame();//����һ����Ϸ
	void tribute();//������
	void dealCards();//����
    bool isAWholeGameOver();
    void resetGame(); //������Ϸ
	void showLeaderboard();//չʾ���а�

	//����Ƿ�Ϊ��Ҫ��������������
    bool isSpecialPlay(const CardGroup* cardGroup);

    //����������
    void settleScore();

    //���������
    void updateCardCounter(const vector<Card>& playedCards);
    void showCardCounter() const;
    void resetCardCounter();
    void showCardCounterExcludePlayer(const vector<Card>& playerCards) const;

};