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
	int x;      // 卡片左上角x坐标
	int y;      // 卡片左上角y坐标 
	int width;  // 卡片宽度
	int height; // 卡片高度
	int index;  // 卡片在数组中的索引
};

struct Button {
	int x;      // 按钮左上角x坐标
	int y;      // 按钮左上角y坐标
	int width;  // 按钮宽度
	int height; // 按钮高度
	string text; // 按钮文本
	bool isEnabled; // 按钮是否可用
};

class Graphical
{
private:
	Game* game;
	CardCounter* counterPtr;

	// 存储每个玩家最近打出的牌
	vector<vector<Card>> lastPlayerPlays;

	//音乐
	bool isMusicPlaying;
	string currentMusic;
public:
	Graphical(Game* g) :game(g),counterPtr(nullptr) { lastPlayerPlays = vector<vector<Card>>(4); }
	void setGame(Game* game);
	void setCardCounterPtr(CardCounter* counter) { counterPtr = counter; } // 添加设置 cardCounter 指针的方法

	
	string getCardImageFileName(const Card& card);
	
	
	void displayCard(const Card& card, int x, int y, int width, int height);
	vector<CardRect> displayCards(const vector<Card>& cards); // 修改为返回卡片区域，参数改为非const引用
	void displayAICards(const vector<Card>& cards, int num, int selectedIndex = -1);

	void drawCards(vector<Card>& cards,int x,int y);

	void drawUI(int index = -1);


	Button createButton(int x, int y, int width, int height, const string& text, bool isEnabled = true);
	void drawButton(const Button& button);
	vector<Button> drawPlayButtons(); // 绘制出牌和不出按钮
	bool isPointInButton(int x, int y, const Button& button); // 检查点击是否在按钮内
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

	// 音乐相关函数
	void playBackgroundMusic(const string& musicFile);
	void stopBackgroundMusic();
	void checkAndUpdateGameMusic(int scoreFactor); // 根据倍数检查和更新游戏音乐

};
//要画玩家手牌，ai玩家的名字排名、牌背面加上剩余牌数，上个出牌的牌