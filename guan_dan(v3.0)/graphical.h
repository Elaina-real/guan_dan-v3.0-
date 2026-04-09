#pragma once

#include <unordered_map>
#include <vector>
#include <utility>
#include <windows.h>
#include <mmsystem.h>

#include "card.h"
#include "graphics.h"

#pragma comment(lib, "winmm.lib") 

using namespace std;

// 前向声明
class Player;
class CardCounter;
class Game;

/**
 * 卡片矩形区域结构体
 * 用于存储卡片的位置和大小信息
 */
struct CardRect {
    int x;      // 卡片左上角x坐标
    int y;      // 卡片左上角y坐标 
    int width;  // 卡片宽度
    int height; // 卡片高度
    int index;  // 卡片在数组中的索引
};

/**
 * 按钮结构体
 * 用于存储按钮的位置、大小和状态信息
 */
struct Button {
    int x;          // 按钮左上角x坐标
    int y;          // 按钮左上角y坐标
    int width;      // 按钮宽度
    int height;     // 按钮高度
    string text;    // 按钮文本
    bool isEnabled; // 按钮是否可用
};

/**
 * 图形界面类
 * 负责游戏的所有图形绘制和用户交互
 */
class Graphical
{
private:
    Game* game;
    CardCounter* counterPtr;

    // 游戏状态数据
    vector<vector<Card>> lastPlayerPlays;  // 存储每个玩家最近打出的牌

    // 音乐相关
    bool isMusicPlaying;
    string currentMusic;

    // 音量控制
    float volumeLevel;        // 音量级别
    Button volumeButton;      // 音量按钮
    bool showVolumeSlider;    // 是否显示音量滑块
    RECT volumeSliderRect;    // 音量滑块区域

public:
    //初始化函数
    Graphical(Game* g);
    void setGame(Game* game);
    void setCardCounterPtr(CardCounter* counter) { counterPtr = counter; }
    void loadBackgroundImage();

    //卡片绘制函数
    string getCardImageFileName(const Card& card);
    void displayCard(const Card& card, int x, int y, int width, int height);
    vector<CardRect> displayCards(const vector<Card>& cards);
    void displayAICards(const vector<Card>& cards, int num, int selectedIndex = -1);
    void drawCards(vector<Card>& cards, int x, int y);

    //界面绘制函数
    void drawUI(int index = -1);
    void drawGameInfo();
    void drawPlayerInfo(const shared_ptr<Player>& player, int pos);
    void drawPlayerPlay(int pos);
    void displayCurrentPlayer(int index);
    void drawCardCounter();

    //按钮相关函数
    Button createButton(int x, int y, int width, int height, const string& text, bool isEnabled = true);
    void drawButton(const Button& button);
    vector<Button> drawPlayButtons();
    bool isPointInButton(int x, int y, const Button& button);
    bool isPointInRect(int x, int y, const CardRect& rect);

    //玩家出牌管理
    void updateLastPlay(int index, const vector<Card>& cards);
    void clearLastPlay(int index);
    bool handleUserInput(vector<Card>& selectedCards, bool& isPass);

    //开始和结束界面
    void drawStartInterface();
    bool handleStartInterfaceInput(int& gameMode);
    void drawSettlementInterface(bool isGameOver = false);
    bool handleSettlementInterfaceInput(bool& continueGame);

    //音乐控制函数
    void playBackgroundMusic(const string& musicFile);
    void stopBackgroundMusic();
    void checkAndUpdateGameMusic(int scoreFactor);

    //音量控制函数
    void setVolume(float level);
    float getVolume() const;
    void drawVolumeControl();
    bool handleVolumeControl(int x, int y);

    //上贡还贡相关
    void drawTributeInterface(const shared_ptr<Player>& fromPlayer, const shared_ptr<Player>& toPlayer, const Card& tributeCard);
    void drawReturnTributeInterface(const shared_ptr<Player>& fromPlayer, const shared_ptr<Player>& toPlayer, const Card& returnCard);
    Card handleTributeSelection(const shared_ptr<Player>& player);
    Card handleReturnTributeSelection(const shared_ptr<Player>& player);
    void animateCardTransfer(int fromPlayerIndex, int toPlayerIndex, const Card& card, bool isTribute);
};