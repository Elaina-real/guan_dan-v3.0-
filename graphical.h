#pragma once
#include "card.h"
#include <unordered_map>
#include <vector>
#include <utility>
#include "graphics.h"

#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib") 

class Player;
class CardCounter;

using namespace std;

class Game;

struct CardRect {
	int x;      // ��Ƭ���Ͻ�x����
	int y;      // ��Ƭ���Ͻ�y���� 
	int width;  // ��Ƭ���
	int height; // ��Ƭ�߶�
	int index;  // ��Ƭ�������е�����
};

struct Button {
	int x;      // ��ť���Ͻ�x����
	int y;      // ��ť���Ͻ�y����
	int width;  // ��ť���
	int height; // ��ť�߶�
	string text; // ��ť�ı�
	bool isEnabled; // ��ť�Ƿ����
};

class Graphical
{
private:
	Game* game;
	CardCounter* counterPtr;

	// �洢ÿ���������������
	vector<vector<Card>> lastPlayerPlays;

	//����
	bool isMusicPlaying;
	string currentMusic;
public:
	Graphical(Game* g) :game(g),counterPtr(nullptr) { lastPlayerPlays = vector<vector<Card>>(4); }
	void setGame(Game* game);
	void setCardCounterPtr(CardCounter* counter) { counterPtr = counter; } // ������� cardCounter ָ��ķ���

	
	string getCardImageFileName(const Card& card);
	
	
	void displayCard(const Card& card, int x, int y, int width, int height);
	vector<CardRect> displayCards(const vector<Card>& cards); // �޸�Ϊ���ؿ�Ƭ���򣬲�����Ϊ��const����
	void displayAICards(const vector<Card>& cards, int num, int selectedIndex = -1);

	void drawCards(vector<Card>& cards,int x,int y);

	void drawUI(int index = -1);


	Button createButton(int x, int y, int width, int height, const string& text, bool isEnabled = true);
	void drawButton(const Button& button);
	vector<Button> drawPlayButtons(); // ���Ƴ��ƺͲ�����ť
	bool isPointInButton(int x, int y, const Button& button); // ������Ƿ��ڰ�ť��
	bool isPointInRect(int x, int y, const CardRect& rect);

	void drawGameInfo();
	void drawPlayerInfo(const shared_ptr<Player>& player, int pos);
	void drawPlayerPlay(int pos);

	void updateLastPlay(int index, const vector<Card>& cards);
	void clearLastPlay(int index);

	void displayCurrentPlayer(int index);

	bool handleUserInput(vector <Card>& selectedCards, bool& isPass);

	void loadBackgroundImage();

	void drawCardCounter();

	void drawStartInterface();
	bool handleStartInterfaceInput(int& gameMode);

	void drawSettlementInterface(bool isGameOver = false);
	bool handleSettlementInterfaceInput(bool& continueGame);

	// ������غ���
	void playBackgroundMusic(const string& musicFile);
	void stopBackgroundMusic();
	void checkAndUpdateGameMusic(int scoreFactor); // ���ݱ������͸�����Ϸ����

};
//Ҫ��������ƣ�ai��ҵ������������Ʊ������ʣ���������ϸ����Ƶ���