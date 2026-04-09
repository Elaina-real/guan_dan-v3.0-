#include "graphical.h"
#include <graphics.h>  // 包含 EasyX 图形库的头文件
#include <conio.h> // 用于获取键盘输入
#include "game.h"
#include "player.h"
#include "cardcounter.h"

using namespace std;

Graphical::Graphical(Game* g) :game(g), counterPtr(nullptr),
currentMusic(""),
volumeLevel(0.5f),
showVolumeSlider(false)
{
    lastPlayerPlays = vector<vector<Card>>(4);
    // 初始化音量控制按钮
    volumeButton = createButton(1180, 670, 80, 30, "音量", true);

    // 初始化音量滑块区域
    volumeSliderRect.left = 1080;
    volumeSliderRect.top = 630;
    volumeSliderRect.right = 1230;
    volumeSliderRect.bottom = 650;
}

void Graphical::setGame(Game* g)
{
	game = g;
}

string Graphical::getCardImageFileName(const Card& card)
{
    string fileName = "resource/";

    // 根据花色和点数确定文件名
    if (card.getSuit() == Joker) 
    {
        // 大小王的图片
        if (card.getRank() == big) 
        {
            fileName += "JOKER-A.png";
        }
        else
        {
            fileName += "JOKER-B.png";
        }
    }
    else {
        // 普通牌的图片
        string suitStr;
        switch (card.getSuit()) 
        {
        case Heart:   suitStr = "Heart"; break;    // 红桃
        case Diamond: suitStr = "Diamond"; break;    // 方块
        case Club:    suitStr = "Club"; break;    // 梅花
        case Spade:   suitStr = "Spade"; break;    // 黑桃
        default:      suitStr = "Heart"; break;    // 默认红桃
        }

        string rankStr;
        switch (card.getRank()) 
        {
        case jack:  rankStr = "J"; break;
        case queen: rankStr = "Q"; break;
        case king:  rankStr = "K"; break;
        case ace:   rankStr = "A"; break;
        default:    rankStr = to_string(card.getRank()); break;
        }

        fileName += suitStr + rankStr + ".png";
    }

    return fileName;
}

// 显示卡片
void Graphical::displayCard(const Card& card, int x, int y, int width, int height) {
    string imagePath = getCardImageFileName(card);
    IMAGE cardImg;

    wstring wideImagePath = wstring(imagePath.begin(), imagePath.end());
    // 尝试加载卡片图片
    loadimage(&cardImg, wideImagePath.c_str(), width, height);

    // 显示卡片
    putimage(x, y, &cardImg);

    //// 如果是级牌或癞子牌，添加特殊标记
    //if (card.isLevelCard()) {
    //    // 在卡片上标记为级牌
    //    settextcolor(RED);
    //    settextstyle(18, 0, _T("宋体"));
    //    outtextxy(x + 5, y + 5, _T("级"));
    //}

    //if (card.isWildCard()) {
    //    // 在卡片上标记为癞子牌
    //    settextcolor(GREEN);
    //    settextstyle(18, 0, _T("宋体"));
    //    outtextxy(x + width - 25, y + 5, _T("癞"));
    //}
}

// 显示一组卡片
vector<CardRect> Graphical::displayCards(const vector<Card>& cards)
{
    int x = 250;
    int y = 550;
    int cardWidth = 75;
    int cardHeight = 104;
    vector<CardRect> cardRects;

    for (int i = 0; i < cards.size(); i++)
    {
        // 计算卡片位置，选中的卡片上移20像素
        int yOffset = (cards[i].isSelectedCard()) ? -30 : 0;

        // 显示卡片
        displayCard(cards[i], x, y + yOffset, 75, 104);

        // 保存卡片的区域信息
        CardRect rect;
        rect.x = x;
        rect.y = y + yOffset;
        rect.width = cardWidth;
        rect.height = cardHeight;
        rect.index = i;
        cardRects.push_back(rect);

        // 下一张卡的位置
        x += 30;  // 卡片重叠显示
    }
	return cardRects;
}



void Graphical::displayAICards(const vector<Card>& cards,int num, int selectedIndex)
{
    IMAGE backImg;
    loadimage(&backImg, _T("resource/Background.png"), 75, 104);

    int x = 0, y = 0;
    switch (num)
    {
    case 1:
        x = 1150;
        y = 350;
        break;
    case 2:
        x = 600;
        y = 35;
		break;
    case 3:
        x = 50;
        y = 350;
		break;
    }
	putimage(x, y, &backImg); // 在窗口中显示背景图片

	int centerX = x + 75 / 2; // 计算图片中心点x坐标
	int centerY = y + 104 / 2; // 计算图片中心点y坐标
    // 设置文本样式
    setbkmode(TRANSPARENT);
    settextcolor(RED); // 设置文本颜色为红色
    settextstyle(50, 0, _T("Arial")); // 设置文本大小为30，粗细为0，字体为Arial

    // 创建文本
    TCHAR number[10];
	int cardsNum = cards.size();
    _stprintf_s(number, _T("%d"), cardsNum); // 将数字42转换为字符串

    // 获取文本宽度和高度以便居中显示
    int textWidth = textwidth(number);
    int textHeight = textheight(number);

    // 在图片中央显示数字，需要减去文本宽度和高度的一半以确保居中
    outtextxy(centerX - textWidth / 2, centerY - textHeight / 2, number);
}

// 创建一个按钮
Button Graphical::createButton(int x, int y, int width, int height, const string& text, bool isEnabled) {
    Button button;
    button.x = x;
    button.y = y;
    button.width = width;
    button.height = height;
    button.text = text;
    button.isEnabled = isEnabled;
    return button;
}

// 绘制按钮
void Graphical::drawButton(const Button& button) {
    // 设置按钮颜色
    COLORREF btnColor = button.isEnabled ? RGB(0, 122, 204) : RGB(150, 150, 150);
    COLORREF textColor = WHITE;

    // 绘制按钮矩形
    setfillcolor(btnColor);
    setlinecolor(RGB(30, 30, 30));
    fillroundrect(button.x, button.y, button.x + button.width, button.y + button.height, 10, 10);

    // 绘制按钮文本
    settextcolor(textColor);
    settextstyle(20, 0, _T("宋体"));
    setbkmode(TRANSPARENT);

    // 使用 EasyX 的功能转换中文字符串
    TCHAR* text = new TCHAR[button.text.length() + 1];
#ifdef UNICODE
    MultiByteToWideChar(CP_ACP, 0, button.text.c_str(), -1, text, button.text.length() + 1);
#else
    strcpy(text, button.text.c_str());
#endif

    // 计算文本宽度和高度以便居中
    int textWidth = textwidth(text);
    int textHeight = textheight(text);

    // 在按钮中央显示文本
    outtextxy(button.x + (button.width - textWidth) / 2,
        button.y + (button.height - textHeight) / 2,
        text);

    delete[] text;
}

// 绘制出牌和不出按钮
vector<Button> Graphical::drawPlayButtons() {
    vector<Button> buttons;

    // 创建"出牌"按钮
    Button playButton = createButton(600, 470, 120, 40, "出牌");
    // 创建"不出"按钮
    Button passButton = createButton(450, 470, 120, 40, "不出");

    // 绘制按钮
    drawButton(playButton);
    drawButton(passButton);

    // 将按钮添加到返回列表
    buttons.push_back(playButton);
    buttons.push_back(passButton);

    return buttons;
}

// 检查点击是否在按钮内
bool Graphical::isPointInButton(int x, int y, const Button& button) {
    return (x >= button.x && x <= button.x + button.width &&
        y >= button.y && y <= button.y + button.height);
}

//void Graphical::drawUI(vector<shared_ptr<Player>> players)
//{
//	vector<Card> player1Cards = players[0]->getHandCards();
//	vector<Card> AI1Cards = players[1]->getHandCards();
//	vector<Card> AI2Cards = players[2]->getHandCards();
//	vector<Card> AI3Cards = players[3]->getHandCards();
//
//
//    vector<CardRect> cardRects = displayCards(player1Cards);
//    displayAICards(AI1Cards, 1);
//    displayAICards(AI2Cards, 2);
//    displayAICards(AI3Cards, 3);
//}


void Graphical::drawCards(vector<Card>& cards,int x,int y)
{
    for (int i = 0; i < cards.size(); i++)
    {
		displayCard(cards[i], x + i * 30, y, 75, 104);
    }
}

// 判断点击点是否在卡片区域内
bool Graphical::isPointInRect(int x, int y, const CardRect& rect) {
    return (x >= rect.x && x <= rect.x + rect.width &&
        y >= rect.y && y <= rect.y + rect.height);
}

void Graphical::drawUI(int index)
{
    // 检查并更新游戏音乐
    checkAndUpdateGameMusic(game->getScoreFactor());

    // 开始批量绘制，启用双缓冲技术，防止闪烁
    BeginBatchDraw();

    // 清空屏幕，准备重新绘制
    cleardevice();

    loadBackgroundImage();

    auto players = game->getPlayers();

    // 1. 显示游戏状态信息（级别、倍数等）
    drawGameInfo();

    // 2. 显示AI玩家
    for (int i = 1; i < players.size(); i++) 
    {    
        displayAICards(players[i]->getHandCards(), i);

        // 显示AI玩家名字和排名
        drawPlayerInfo(players[i], i);
		drawPlayerPlay(i);
    }

    // 3. 显示当前玩家手牌
    auto cardRects = displayCards(players[0]->getHandCards());

    // 4. 显示玩家信息（名字、级别、排名等）
    drawPlayerInfo(players[0], 0);
	drawPlayerPlay(0);

    //// 5. 显示上一手牌（如果有）
    //const CardGroup* lastPlay = game->getLastPlay();
    //if (lastPlay)
    //{
    //    vector<Card> lastPlayCards = lastPlay->getCards();
    //    drawLastPlay(lastPlayCards);
    //}

    // 6. 绘制按钮
    drawPlayButtons();

    //// 7. 绘制记牌器
    drawCardCounter();

	// 8. 显示当前玩家出牌提示
    if(index != -1)
	displayCurrentPlayer(index);

    // 9. 绘制音量控制
    drawVolumeControl();

    // 结束批量绘制，刷新屏幕
    EndBatchDraw();
}

void Graphical::drawGameInfo()
{
    // 设置文本样式
    settextcolor(WHITE);
    settextstyle(20, 0, _T("宋体"));
    setbkmode(TRANSPARENT);

    // 显示当前级别
    wstring levelText = L"当前级别: ";
    levelText += to_wstring(game->getLevel());
    outtextxy(20, 20, levelText.c_str());

    // 显示当前倍数
    wstring factorText = L"当前倍数: ";
    factorText += to_wstring(game->getScoreFactor());
    outtextxy(20, 50, factorText.c_str());
}

void Graphical::drawPlayerInfo(const shared_ptr<Player>& player, int pos)
{
    int x = 0, y = 0;

    switch (pos)
    {
    case 0:
        x = 250;
        y = 650;
        break;
    case 1:
        x = 1150;
        y = 250;
        break;
    case 2:
        x = 500;
        y = 35;
        break;
    case 3:
        x = 50;
        y = 250;
        break;
    }

    // 设置文本样式
    settextcolor(WHITE);
    settextstyle(20, 0, _T("宋体"));
    setbkmode(TRANSPARENT);

    // 准备文本内容
    wstring playerInfo = L"";
    string playerName = player->getName();  // 先获取名称，存储在局部变量中
    playerInfo += wstring(playerName.begin(), playerName.end());  // 使用同一个字符串对象的迭代器
    playerInfo += L" Lv:";
    playerInfo += to_wstring(player->getLevel());

    // 显示玩家排名（如果有）
    PlayerRank rank = player->getARoundRank();
    if (rank != NotRanked) {
        playerInfo += L" 排名:";
        switch (rank) {
        case First:  playerInfo += L"第一"; break;
        case Second: playerInfo += L"第二"; break;
        case Third:  playerInfo += L"第三"; break;
        case Fourth: playerInfo += L"第四"; break;
        }
    }

    outtextxy(x, y, playerInfo.c_str());
}

void Graphical::drawPlayerPlay(int pos)
{
    int x = 0, y = 0;
    switch (pos)
    {
    case 0:
        x = 550;
        y = 350;
        break;
    case 1:
        x = 900;
        y = 300;
        break;
    case 2:
        x = 550;
        y = 170;
        break;
    case 3:
        x = 180;
        y = 300;
        break;
    }

	if (lastPlayerPlays[pos].empty())
	{
		// 如果没有出牌，显示提示信息
		settextcolor(WHITE);
		settextstyle(20, 0, _T("宋体"));
		outtextxy(x, y, _T("未出牌"));
	}
    else
    {
        // 显示玩家出牌
        drawCards(lastPlayerPlays[pos], x, y);
    }
}

void Graphical::updateLastPlay(int index, const vector<Card>& cards)
{
	lastPlayerPlays[index] = cards;
}

void Graphical::clearLastPlay(int index)
{
	lastPlayerPlays[index].clear();
}

void Graphical::displayCurrentPlayer(int index)
{
	int x = 0, y = 0;

    switch (index)
    {
    case 0:
        x = 250;
        y = 680;
        break;
    case 1:
        x = 1150;
        y = 280;
        break;
    case 2:
        x = 500;
        y = 65;
        break;
    case 3:
        x = 50;
        y = 280;
        break;
    }

    //打印文字“出牌中”
	settextcolor(WHITE);
	settextstyle(20, 0, _T("宋体"));
	setbkmode(TRANSPARENT);
	//wstring currentPlayerText = L"当前玩家: ";
	//currentPlayerText += to_wstring(index + 1); // 玩家索引从0开始，所以加1
	outtextxy(x, y, _T("当前玩家↑"));
}

bool Graphical::handleUserInput(vector<Card>& selectedCards, bool& isPass)
{
    auto players = game->getPlayers();
    vector<Card>& playerCards = players[0]->getHandCards();
    vector<Button> buttons;

    // 绘制初始UI界面
    drawUI(0);

    BeginBatchDraw();
    // 获取卡片区域 - 只获取一次，而不是在循环中
    vector<CardRect> cardRects = displayCards(playerCards);
    EndBatchDraw();

    Button playButton = createButton(600, 470, 120, 40, "出牌");
    Button passButton = createButton(450, 470, 120, 40, "不出");
    buttons.push_back(playButton);
    buttons.push_back(passButton);

    // 处理鼠标输入
    MOUSEMSG msg;
    bool exitLoop = false;
    bool isPlay = false;
    isPass = false;

    while (!exitLoop) {


        // 获取鼠标消息
        if (MouseHit()) {
            msg = GetMouseMsg();



            if (msg.uMsg == WM_LBUTTONDOWN) {
				// 检查是否点击了音量按钮
				if (isPointInButton(msg.x, msg.y, volumeButton)) {
					showVolumeSlider = !showVolumeSlider;  // 切换音量滑块显示状态
					drawUI(0);  // 重绘界面以更新音量控制显示
					continue;
				}

                // 检查是否点击了按钮
                if (isPointInButton(msg.x, msg.y, playButton)) {
                    isPlay = true;
                    exitLoop = true;
                    continue;
                }

                if (isPointInButton(msg.x, msg.y, passButton)) {
                    // 判断是否可以过牌
                    if (game->getLastPlay() == nullptr) {
                        // 首家出牌不能过，显示提示
                        settextcolor(RED);
                        settextstyle(24, 0, _T("宋体"));
                        outtextxy(450, 445, _T("您必须出牌！"));
                        FlushBatchDraw();  // 立即刷新显示提示
                        Sleep(1000);  // 显示1秒
                        drawUI(0);  // 重绘界面清除提示
                    }
                    else {
                        isPass = true;
                        exitLoop = true;
                    }
                    continue;
                }

                // 检查是否点击了卡片 - 从最后一张开始检查，这样可以处理最上面的卡片
                bool cardClicked = false;
                for (int i = cardRects.size() - 1; i >= 0; i--) {
                    // 使用卡片的完整宽度进行点击检测
                    if (msg.x >= cardRects[i].x && msg.x <= cardRects[i].x + cardRects[i].width &&
                        msg.y >= cardRects[i].y && msg.y <= cardRects[i].y + cardRects[i].height) {

                        // 切换卡片选中状态
                        playerCards[i].toggleSelected();
                        cardClicked = true;

                        drawUI(0);

                        // 使用双缓冲绘制，避免闪烁
                        BeginBatchDraw();


                        // 显示当前玩家手牌（更新后的）
                        cardRects = displayCards(players[0]->getHandCards());



                        // 结束批量绘制，刷新屏幕
                        EndBatchDraw();

                        break;  // 只处理最上层的卡片

                        //bool cardClicked = false;
                        //for (int i = cardRects.size() - 1; i >= 0; i--) {
                        //    // 检查点击区域 - 最后一张牌使用整个宽度，其他牌只使用前30像素
                        //    int effectiveWidth = (i == cardRects.size() - 1) ? cardRects[i].width : 30;

                        //    if (msg.x >= cardRects[i].x && msg.x <= cardRects[i].x + effectiveWidth &&
                        //        msg.y >= cardRects[i].y && msg.y <= cardRects[i].y + cardRects[i].height) {

                        //        // 切换卡片选中状态
                        //        playerCards[i].toggleSelected();
                        //        cardClicked = true;

                        //        // 更新卡片位置信息 - 关键是这里要更新cardRects
                        //        drawUI();  // 重绘整个界面
                        //        cardRects = displayCards(playerCards);  // 获取新的卡片区域信息

                        //        break;  // 只处理最上层的卡片
                    }
                }
            }
            else if (msg.uMsg == WM_MOUSEMOVE && (msg.mkLButton)) {
                // 处理拖动音量滑块
                if (showVolumeSlider &&
                    msg.x >= volumeSliderRect.left && msg.x <= volumeSliderRect.right) {

                    // 计算新的音量级别
                    float newVolume = (float)(msg.x - volumeSliderRect.left) /
                        (float)(volumeSliderRect.right - volumeSliderRect.left);

                    // 设置新音量
                    setVolume(newVolume);

                    // 重绘UI以更新音量控制显示
                    drawUI(0);
                    continue;
                }
            }
        }
    }

    // 如果选择出牌，收集选中的卡片
    if (isPlay) {
        selectedCards.clear();
        for (auto& card : playerCards) {
            if (card.isSelectedCard()) {
                selectedCards.push_back(card);
            }
        }
        return true;
    }

    return false;
}

void Graphical::loadBackgroundImage() {
    IMAGE bgImg,cImg;
    // 加载背景图片 (确保图片大小与窗口匹配)
    loadimage(&bgImg, _T("resource/bg.jpg"), 1280, 720);

    // 绘制背景
    putimage(0, 0, &bgImg);
}

void Graphical::drawCardCounter()
{
    // 首先获取玩家的手牌
    auto players = game->getPlayers();
    vector<Card> playerCards = players[0]->getHandCards();

	BeginBatchDraw();

    IMAGE img;
    loadimage(&img, _T("resource/cardcounter.png"), 324, 95); // 加载记牌器图片
    putimage(920, 10, &img); // 在窗口中显示记牌器图片

    // 设置文本样式
    settextcolor(BLACK);
    settextstyle(16, 0, _T("Arial"));
    setbkmode(TRANSPARENT);

    int baseX = 930; // 记牌器图片的左上角X坐标
    int baseY = 10;  // 记牌器图片的左上角Y坐标

    // 第一行Y坐标 - 2到10的计数
    int row1Y = 36; // 您可以调整这个值来设置第一行文本的位置

    // 第二行Y坐标 - J到大王的计数
    int row2Y = 84; // 您可以调整这个值来设置第二行文本的位置

    int adder = 32.4;

    int x2 = baseX;   // 2的X坐标位置
    int x3 = baseX + adder;   // 3的X坐标位置
    int x4 = baseX + 2*adder;   // 4的X坐标位置
    int x5 = baseX + 3*adder;   // 5的X坐标位置
    int x6 = baseX + 4*adder;   // 6的X坐标位置
    int x7 = baseX + 5*adder;   // 7的X坐标位置
    int x8 = baseX + 6*adder;   // 8的X坐标位置
    int x9 = baseX + 7*adder;   // 9的X坐标位置
    int x10 = baseX + 8*adder;  // 10的X坐标位置

    // 第二行 - J到大王的计数位置
    // X坐标将由您来填写，这里只是预留位置
    int xJ = baseX;    // J的X坐标位置
    int xQ = baseX + adder;   // Q的X坐标位置
    int xK = baseX + 2*adder;   // K的X坐标位置
    int xA = baseX + 3*adder;   // A的X坐标位置
    int xSmallJoker = baseX + 4*adder;  // 小王的X坐标位置
    int xBigJoker = baseX + 5*adder;    // 大王的X坐标位置
	int xHeart = baseX + 6 * adder;  
	int xSpade = baseX + 7 * adder; 
	int xDiamond = baseX + 8 * adder;   
	int xClub = baseX + 9 * adder;

    // 显示2到10的剩余数量
    outtextxy(x2, row1Y, to_wstring(counterPtr->getRemainingCountByRankExcludePlayer(two,playerCards)).c_str());
    outtextxy(x3, row1Y, to_wstring(counterPtr->getRemainingCountByRankExcludePlayer(three, playerCards)).c_str());
    outtextxy(x4, row1Y, to_wstring(counterPtr->getRemainingCountByRankExcludePlayer(four, playerCards)).c_str());
    outtextxy(x5, row1Y, to_wstring(counterPtr->getRemainingCountByRankExcludePlayer(five, playerCards)).c_str());
    outtextxy(x6, row1Y, to_wstring(counterPtr->getRemainingCountByRankExcludePlayer(six, playerCards)).c_str());
    outtextxy(x7, row1Y, to_wstring(counterPtr->getRemainingCountByRankExcludePlayer(seven, playerCards)).c_str());
    outtextxy(x8, row1Y, to_wstring(counterPtr->getRemainingCountByRankExcludePlayer(eight, playerCards)).c_str());
    outtextxy(x9, row1Y, to_wstring(counterPtr->getRemainingCountByRankExcludePlayer(nine, playerCards)).c_str());
    outtextxy(x10, row1Y, to_wstring(counterPtr->getRemainingCountByRankExcludePlayer(ten, playerCards)).c_str());

    // 显示J到大王的剩余数量
    outtextxy(xJ, row2Y, to_wstring(counterPtr->getRemainingCountByRankExcludePlayer(jack, playerCards)).c_str());
    outtextxy(xQ, row2Y, to_wstring(counterPtr->getRemainingCountByRankExcludePlayer(queen, playerCards)).c_str());
    outtextxy(xK, row2Y, to_wstring(counterPtr->getRemainingCountByRankExcludePlayer(king, playerCards)).c_str());
    outtextxy(xA, row2Y, to_wstring(counterPtr->getRemainingCountByRankExcludePlayer(ace, playerCards)).c_str());
    outtextxy(xSmallJoker, row2Y, to_wstring(counterPtr->getRemainingCountByRankExcludePlayer(smal, playerCards)).c_str());
    outtextxy(xBigJoker, row2Y, to_wstring(counterPtr->getRemainingCountByRankExcludePlayer(big, playerCards)).c_str());

	// 显示花色的剩余数量
	outtextxy(xHeart, row2Y, to_wstring(counterPtr->getRemainingCountBySuitExcludePlayer(Heart, playerCards)).c_str());
	outtextxy(xSpade, row2Y, to_wstring(counterPtr->getRemainingCountBySuitExcludePlayer(Spade, playerCards)).c_str());
	outtextxy(xDiamond, row2Y, to_wstring(counterPtr->getRemainingCountBySuitExcludePlayer(Diamond, playerCards)).c_str());
	outtextxy(xClub, row2Y, to_wstring(counterPtr->getRemainingCountBySuitExcludePlayer(Club, playerCards)).c_str());

	// 结束批量绘制，刷新屏幕
	EndBatchDraw();
}

void Graphical::drawStartInterface() {
    // 播放开始界面音乐
    playBackgroundMusic("resource/Welcome.wav");

    // 清空屏幕
    cleardevice();

    // 加载背景图片
    IMAGE bgImg;
    loadimage(&bgImg, _T("resource/bg.jpg"), 1280, 720);
    putimage(0, 0, &bgImg);

    // 绘制游戏标题
    settextcolor(WHITE);
    settextstyle(80, 0, _T("华文行楷"));
    setbkmode(TRANSPARENT);
    outtextxy(460, 100, _T("掼           蛋"));

    // 绘制按钮
    Button startButton = createButton(540, 300, 200, 60, "开始游戏");
    Button rulesButton = createButton(540, 380, 200, 60, "游戏规则");
    Button rankingButton = createButton(540, 460, 200, 60, "排行榜");
    Button quitButton = createButton(540, 540, 200, 60, "退出游戏");

    drawButton(startButton);
    drawButton(rulesButton);
    drawButton(rankingButton);
    drawButton(quitButton);

    // 绘制版权信息
    settextstyle(20, 0, _T("宋体"));
    outtextxy(520, 650, _T("版本 3.0 2025"));

    // 绘制音量控制
    drawVolumeControl();

    FlushBatchDraw();
}

bool Graphical::handleStartInterfaceInput(int& gameMode) {
    // 创建按钮
    Button startButton = createButton(540, 300, 200, 60, "开始游戏");
    Button rulesButton = createButton(540, 380, 200, 60, "游戏规则");
    Button rankingButton = createButton(540, 460, 200, 60, "排行榜");
    Button quitButton = createButton(540, 540, 200, 60, "退出游戏");

    MOUSEMSG msg;

    // 处理鼠标消息
    if (MouseHit()) {
        msg = GetMouseMsg();

        if (msg.uMsg == WM_LBUTTONDOWN) {
            // 检查音量控制
            if (handleVolumeControl(msg.x, msg.y)) {
                // 重绘界面
                drawStartInterface(); // 或 drawSettlementInterface();
                return false;
            }

            // 检查点击的按钮
            if (isPointInButton(msg.x, msg.y, startButton)) {
                gameMode = 1; // 开始游戏
                return true;
            }
            else if (isPointInButton(msg.x, msg.y, rulesButton)) {
                // 显示游戏规则
                cleardevice();
                loadBackgroundImage();

                settextcolor(WHITE);
                settextstyle(40, 0, _T("宋体"));
                outtextxy(500, 50, _T("游戏规则"));

                settextstyle(20, 0, _T("宋体"));
                int y = 120;
                outtextxy(300, y, _T("1. 掼蛋是一种四人扑克牌游戏，分为两个阵营对抗，您与对面的玩家同阵营")); y += 30;
                outtextxy(300, y, _T("2. 游戏使用两副牌（共108张牌）")); y += 30;
                outtextxy(300, y, _T("3. 每轮游戏有一个特定的级别牌，级牌大于A小于小王")); y += 30;
                outtextxy(300, y, _T("4. 按照打完手牌的顺序排名")); y += 30;
                outtextxy(300, y, _T("5. 点击牌可以选中，再次点击取消选中")); y += 30;
                outtextxy(300, y, _T("6. 点击出牌按钮打出选中的牌")); y += 30;
                outtextxy(300, y, _T("7. 牌型包括单牌、对子、三张、三带二、炸弹、钢板、顺子与连对")); y += 30;
                outtextxy(300, y, _T("8. 拥有癞子规则，红桃级牌为癞子，可以替代除大小王以外的任何牌")); y += 30;
                outtextxy(300, y, _T("9. 拥有升级规则，一二位升三级，一三位升两级，一四位升一级")); y += 30;
				outtextxy(300, y, _T("10. 拥有进贡规则，最后一个出完牌的玩家向第一个出完牌的玩家进贡")); y += 30;

                // 返回按钮
                Button backButton = createButton(540, 600, 200, 60, "返回");
                drawButton(backButton);

                FlushBatchDraw();

                // 等待点击返回
                bool back = false;
                while (!back) {
                    if (MouseHit()) {
                        MOUSEMSG backMsg = GetMouseMsg();
                        if (backMsg.uMsg == WM_LBUTTONDOWN && isPointInButton(backMsg.x, backMsg.y, backButton)) {
                            back = true;
                        }
                    }
                }

                // 重绘开始界面
                drawStartInterface();
                return false;
            }
            else if (isPointInButton(msg.x, msg.y, rankingButton)) {
                // 显示排行榜
                cleardevice();
                loadBackgroundImage();

                settextcolor(WHITE);
                settextstyle(40, 0, _T("宋体"));
                outtextxy(500, 50, _T("排行榜"));

                settextstyle(20, 0, _T("宋体"));
                outtextxy(450, 200, _T("未实装XD"));

                // 返回按钮
                Button backButton = createButton(540, 600, 200, 60, "返回");
                drawButton(backButton);

                FlushBatchDraw();

                // 等待点击返回
                bool back = false;
                while (!back) {
                    if (MouseHit()) {
                        MOUSEMSG backMsg = GetMouseMsg();
                        if (backMsg.uMsg == WM_LBUTTONDOWN && isPointInButton(backMsg.x, backMsg.y, backButton)) {
                            back = true;
                        }
                    }
                }

                // 重绘开始界面
                drawStartInterface();
                return false;
            }
            else if (isPointInButton(msg.x, msg.y, quitButton)) {
                gameMode = 0; // 退出游戏
                return true;
            }
        }
        else if (msg.uMsg == WM_MOUSEMOVE && (msg.mkLButton)) {
            // 处理拖动音量滑块
            if (showVolumeSlider &&
                msg.x >= volumeSliderRect.left && msg.x <= volumeSliderRect.right) {

                // 计算新的音量级别
                float newVolume = (float)(msg.x - volumeSliderRect.left) /
                    (float)(volumeSliderRect.right - volumeSliderRect.left);

                // 设置新音量
                setVolume(newVolume);

                // 重绘界面
                drawStartInterface();
                return false;
            }
        }
    }

    return false;
}

void Graphical::drawSettlementInterface(bool isGameOver) {
    playBackgroundMusic("resource/Welcome.wav");

    // 清空屏幕
    cleardevice();

    // 加载背景图片
    IMAGE bgImg;
    loadimage(&bgImg, _T("resource/bg.jpg"), 1280, 720);
    putimage(0, 0, &bgImg);

    // 获取游戏信息
    auto players = game->getPlayers();

    // 绘制标题
    settextcolor(WHITE);
    settextstyle(60, 0, _T("华文行楷"));
    setbkmode(TRANSPARENT);

    if (isGameOver) {
        outtextxy(400, 50, _T("游戏结束"));
    }
    else {
        outtextxy(400, 50, _T("本局结算"));
    }

    // 绘制分隔线
    setlinecolor(WHITE);
    line(300, 130, 980, 130);

    // 绘制玩家排名和信息
    settextstyle(30, 0, _T("宋体"));
    int yPos = 170;

    // 按排名排序玩家
    vector<shared_ptr<Player>> sortedPlayers = players;
    sort(sortedPlayers.begin(), sortedPlayers.end(),
        [](const shared_ptr<Player>& a, const shared_ptr<Player>& b) {
            return a->getARoundRank() < b->getARoundRank();
        });

    // 显示排名、玩家名称、队伍、分数和级别
    outtextxy(300, yPos, _T("排名"));
    outtextxy(400, yPos, _T("玩家"));
    outtextxy(550, yPos, _T("队伍"));
    outtextxy(700, yPos, _T("分数"));
    outtextxy(850, yPos, _T("级别"));
    yPos += 50;

    for (const auto& player : sortedPlayers) {
        // 显示排名
        wstring rankText;
        switch (player->getARoundRank()) {
        case First:  rankText = L"第一"; break;
        case Second: rankText = L"第二"; break;
        case Third:  rankText = L"第三"; break;
        case Fourth: rankText = L"第四"; break;
        default:     rankText = L"未排名"; break;
        }
        outtextxy(300, yPos, rankText.c_str());

        // 显示玩家名称
        string playerName = player->getName();
        wstring wPlayerName(playerName.begin(), playerName.end());
        outtextxy(400, yPos, wPlayerName.c_str());

        // 显示队伍
        wstring teamText = (player->getTeam() == Red) ? L"红队" : L"蓝队";
        outtextxy(550, yPos, teamText.c_str());

        // 显示分数
        wstring scoreText = to_wstring(player->getScore());
        outtextxy(700, yPos, scoreText.c_str());

        // 显示级别
        wstring levelText;
        switch (player->getLevel()) {
        case jack:  levelText = L"J"; break;
        case queen: levelText = L"Q"; break;
        case king:  levelText = L"K"; break;
        case ace:   levelText = L"A"; break;
        default:    levelText = to_wstring(player->getLevel()); break;
        }
        outtextxy(850, yPos, levelText.c_str());

        yPos += 50;
    }

    // 显示本局游戏信息
    settextstyle(25, 0, _T("宋体"));
    yPos += 30;

    wstring levelText = L"当前级别: ";
    switch (game->getLevel()) {
    case jack:  levelText += L"J"; break;
    case queen: levelText += L"Q"; break;
    case king:  levelText += L"K"; break;
    case ace:   levelText += L"A"; break;
    default:    levelText += to_wstring(game->getLevel()); break;
    }
    outtextxy(400, yPos, levelText.c_str());

    wstring factorText = L"最终倍数: " + to_wstring(game->getScoreFactor());
    outtextxy(700, yPos, factorText.c_str());

    // 绘制按钮
    Button continueButton = createButton(440, 550, 200, 60, "继续游戏");
    Button exitButton = createButton(680, 550, 200, 60, "退出游戏");

    drawButton(continueButton);
    drawButton(exitButton);

    FlushBatchDraw();
}

bool Graphical::handleSettlementInterfaceInput(bool& continueGame) {
    // 创建按钮 - 需与drawSettlementInterface中的按钮位置一致
    Button continueButton = createButton(440, 550, 200, 60, "继续游戏");
    Button exitButton = createButton(680, 550, 200, 60, "退出游戏");

    MOUSEMSG msg;

    // 处理鼠标消息
    if (MouseHit()) {
        msg = GetMouseMsg();

        if (msg.uMsg == WM_LBUTTONDOWN) {
            // 检查点击的按钮
            if (isPointInButton(msg.x, msg.y, continueButton)) {
                continueGame = true;
                return true;
            }
            else if (isPointInButton(msg.x, msg.y, exitButton)) {
                continueGame = false;
                return true;
            }
        }
    }

    return false;
}

// 播放背景音乐
void Graphical::playBackgroundMusic(const string& musicFile) {
    if (currentMusic == musicFile && isMusicPlaying) {
        // 如果当前已在播放这个音乐文件，则不执行任何操作
        return;
    }

    stopBackgroundMusic();

    // 将文件名转换为宽字符串
    wstring wMusicFile(musicFile.begin(), musicFile.end());

    // 播放新音乐
    if (PlaySound(wMusicFile.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP)) {
        isMusicPlaying = true;
        currentMusic = musicFile;

        // 应用当前音量设置
        setVolume(volumeLevel);
    }
}

// 停止背景音乐
void Graphical::stopBackgroundMusic() {
    if (isMusicPlaying) {
        PlaySound(NULL, NULL, 0); // 停止所有声音
        isMusicPlaying = false;
        currentMusic = "";
    }
}

// 检查并更新游戏音乐
void Graphical::checkAndUpdateGameMusic(int scoreFactor) {
    if (scoreFactor > 31) {
        // 倍数到达32时播放那个音乐
        playBackgroundMusic("resource/dedede.wav");
    }
    else {
        // 播放常规音乐
        playBackgroundMusic("resource/Normal.wav");
    }
}

//void Graphical::drawCardCounter()
//{
//    // 在右上角画一个半透明的背景框
//    int x = 850;
//    int y = 15;
//    int width = 300;
//    int height = 160;
//
//    // 设置半透明矩形
//    setfillcolor(RGB(0, 0, 0, 128)); // 半透明黑色
//    solidrectangle(x, y, x + width, y + height);
//
//    // 设置边框
//    setlinecolor(WHITE);
//    rectangle(x, y, x + width, y + height);
//
//    // 设置文本样式
//    settextcolor(WHITE);
//    settextstyle(22, 0, _T("宋体"));
//    setbkmode(TRANSPARENT);
//
//    // 画标题
//    outtextxy(x + 120, y + 5, L"记牌器");
//
//    // 显示各个点数的剩余数量
//    y += 35; // 移到标题下方
//
//    // 定义花色和点数标签
//    wstring rankLabels[] = { L"", L"", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"10", L"J", L"Q", L"K", L"A" };
//
//    // 绘制表格头部
//    settextstyle(18, 0, _T("宋体"));
//    outtextxy(x + 10, y, L"点数");
//    outtextxy(x + 70, y, L"剩余");
//    outtextxy(x + 150, y, L"点数");
//    outtextxy(x + 210, y, L"剩余");
//
//    y += 25;
//
//    // 显示2-A的剩余数量，分两列
//    for (int r = 2; r <= 14; ++r) {
//        // 左列 (2-7)
//        if (r <= 7) {
//            outtextxy(x + 10, y + (r - 2) * 20, rankLabels[r].c_str());
//            int remain = game->cardCounter.getRemainingCountByRank(static_cast<Rank>(r));
//            outtextxy(x + 70, y + (r - 2) * 20, to_wstring(remain).c_str());
//        }
//        // 右列 (8-A)
//        else {
//            outtextxy(x + 150, y + (r - 8) * 20, rankLabels[r].c_str());
//            int remain = game->cardCounter.getRemainingCountByRank(static_cast<Rank>(r));
//            outtextxy(x + 210, y + (r - 8) * 20, to_wstring(remain).c_str());
//        }
//    }
//
//    // 显示大小王
//    y += 120;
//    wstring smallJokerText = L"小王: ";
//    smallJokerText += to_wstring(game->cardCounter.getRemainingCount(Joker, smal));
//    outtextxy(x + 10, y, smallJokerText.c_str());
//
//    wstring bigJokerText = L"大王: ";
//    bigJokerText += to_wstring(game->cardCounter.getRemainingCount(Joker, big));
//    outtextxy(x + 150, y, bigJokerText.c_str());
//}

// 绘制上贡界面
void Graphical::drawTributeInterface(const shared_ptr<Player>& fromPlayer, const shared_ptr<Player>& toPlayer, const Card& tributeCard) {
    // 清空屏幕并绘制背景
    cleardevice();
    loadBackgroundImage();

    // 设置文本样式
    settextcolor(WHITE);
    settextstyle(30, 0, _T("宋体"));
    setbkmode(TRANSPARENT);

    // 显示上贡标题
    wstring titleText = L"上贡";
    outtextxy(600, 50, titleText.c_str());

    // 显示玩家信息
    string fromPlayerName = fromPlayer->getName();
    wstring fromPlayerText = L"进贡方: ";
    fromPlayerText += wstring(fromPlayerName.begin(), fromPlayerName.end());
    outtextxy(400, 120, fromPlayerText.c_str());

    string toPlayerName = toPlayer->getName();
    wstring toPlayerText = L"接收方: ";
    toPlayerText += wstring(toPlayerName.begin(), toPlayerName.end());
    outtextxy(400, 170, toPlayerText.c_str());

    // 显示上贡卡牌
    settextstyle(20, 0, _T("宋体"));
    outtextxy(400, 230, L"上贡卡牌:");
    displayCard(tributeCard, 550, 220, 75, 104);

    // 显示提示文本
    settextstyle(20, 0, _T("宋体"));
    outtextxy(400, 350, L"按任意键继续...");

    // 显示动画效果
    FlushBatchDraw();

    Sleep(500);
    return;
}

// 绘制还贡界面
void Graphical::drawReturnTributeInterface(const shared_ptr<Player>& fromPlayer, const shared_ptr<Player>& toPlayer, const Card& returnCard) {
    // 清空屏幕并绘制背景
    cleardevice();
    loadBackgroundImage();

    // 设置文本样式
    settextcolor(WHITE);
    settextstyle(30, 0, _T("宋体"));
    setbkmode(TRANSPARENT);

    // 显示还贡标题
    wstring titleText = L"还贡";
    outtextxy(600, 50, titleText.c_str());

    // 显示玩家信息
    string fromPlayerName = fromPlayer->getName();
    wstring fromPlayerText = L"还贡方: ";
    fromPlayerText += wstring(fromPlayerName.begin(), fromPlayerName.end());
    outtextxy(400, 120, fromPlayerText.c_str());

    string toPlayerName = toPlayer->getName();
    wstring toPlayerText = L"接收方: ";
    toPlayerText += wstring(toPlayerName.begin(), toPlayerName.end());
    outtextxy(400, 170, toPlayerText.c_str());

    // 显示还贡卡牌
    settextstyle(20, 0, _T("宋体"));
    outtextxy(400, 230, L"还贡卡牌:");
    displayCard(returnCard, 550, 220, 75, 104);

    // 显示提示文本
    settextstyle(20, 0, _T("宋体"));
    outtextxy(400, 350, L"按任意键继续...");

    // 显示动画效果
    FlushBatchDraw();

    Sleep(500);
    return;
}

// 处理人类玩家上贡选择
Card Graphical::handleTributeSelection(const shared_ptr<Player>& player) {
    vector<Card>& playerCards = player->getHandCards();

    // 找出玩家手牌中最大的牌
    Card maxCard = playerCards[0];
    for (const auto& card : playerCards) {
        if (card > maxCard) {
            maxCard = card;
        }
    }

    // 清空屏幕并绘制背景
    cleardevice();
    loadBackgroundImage();

    // 设置文本样式
    settextcolor(WHITE);
    settextstyle(30, 0, _T("宋体"));
    setbkmode(TRANSPARENT);

    // 显示上贡标题
    wstring titleText = L"上贡选择";
    outtextxy(550, 50, titleText.c_str());

    // 显示说明文本
    settextstyle(20, 0, _T("宋体"));
    wstring descText = L"您需要贡献手牌中最大的牌:";
    outtextxy(400, 120, descText.c_str());

    // 显示将要上贡的卡牌
    displayCard(maxCard, 550, 180, 75, 104);

    // 创建确认按钮
    Button confirmButton = createButton(550, 320, 120, 40, "确认上贡");
    drawButton(confirmButton);

    // 显示画面
    FlushBatchDraw();

    // 等待用户点击确认
    MOUSEMSG msg;
    bool confirmed = false;

    while (!confirmed) {
        if (MouseHit()) {
            msg = GetMouseMsg();
            if (msg.uMsg == WM_LBUTTONDOWN && isPointInButton(msg.x, msg.y, confirmButton)) {
                confirmed = true;
            }
        }
        Sleep(10); // 减轻CPU负担
    }

    return maxCard;
}

// 处理人类玩家还贡选择
Card Graphical::handleReturnTributeSelection(const shared_ptr<Player>& player) {
    vector<Card>& playerCards = player->getHandCards();
    vector<Card> eligibleCards;

    // 找出所有符合条件的牌（点数小于10）
    for (const auto& card : playerCards) {
        if (card.getRank() < ten) {
            eligibleCards.push_back(card);
        }
    }

    // 如果没有符合条件的牌，返回第一张牌（这种情况会在Player类中处理）
    if (eligibleCards.empty()) {
        settextcolor(RED);
        settextstyle(24, 0, _T("宋体"));
        outtextxy(400, 400, _T("没有可用的牌来还贡!"));
        FlushBatchDraw();
        Sleep(2000);
        return Card(Heart, two); // 返回默认牌，但不会被实际使用
    }

    // 清空屏幕并绘制背景
    cleardevice();
    loadBackgroundImage();

    // 设置文本样式
    settextcolor(WHITE);
    settextstyle(30, 0, _T("宋体"));
    setbkmode(TRANSPARENT);

    // 显示还贡标题
    wstring titleText = L"还贡选择";
    outtextxy(550, 50, titleText.c_str());

    // 显示说明文本
    settextstyle(20, 0, _T("宋体"));
    wstring descText = L"请选择一张点数小于10的牌进行还贡:";
    outtextxy(350, 100, descText.c_str());

    // 显示所有可选牌
    const int startX = 300;
    const int startY = 150;
    const int cardWidth = 75;
    const int cardHeight = 104;
    const int cardSpacing = 85;

    vector<CardRect> cardRects;

    for (int i = 0; i < eligibleCards.size(); i++) {
        int x = startX + (i % 8) * cardSpacing;
        int y = startY + (i / 8) * (cardHeight + 10);

        displayCard(eligibleCards[i], x, y, cardWidth, cardHeight);

        // 保存卡片区域信息
        CardRect rect;
        rect.x = x;
        rect.y = y;
        rect.width = cardWidth;
        rect.height = cardHeight;
        rect.index = i;
        cardRects.push_back(rect);
    }

    // 显示提示文本
    settextstyle(20, 0, _T("宋体"));
    outtextxy(400, 400, L"点击一张牌选择还贡");

    // 显示画面
    FlushBatchDraw();

    // 等待用户选择卡牌
    MOUSEMSG msg;
    int selectedCardIndex = -1;

    while (selectedCardIndex == -1) {
        if (MouseHit()) {
            msg = GetMouseMsg();
            if (msg.uMsg == WM_LBUTTONDOWN) {
                // 检查是否点击了卡牌
                for (const auto& rect : cardRects) {
                    if (isPointInRect(msg.x, msg.y, rect)) {
                        selectedCardIndex = rect.index;
                        break;
                    }
                }
            }
        }
        Sleep(10); // 减轻CPU负担
    }

    return eligibleCards[selectedCardIndex];
}

// 显示上贡或还贡的卡牌动画
void Graphical::animateCardTransfer(int fromPlayerIndex, int toPlayerIndex, const Card& card, bool isTribute) {
    //// 获取源位置和目标位置
    //int startX, startY, endX, endY;

    //// 根据玩家索引确定起始和终点位置
    //switch (fromPlayerIndex) {
    //case 0:
    //    startX = 600; startY = 600;
    //    break;
    //case 1:
    //    startX = 1150; startY = 350;
    //    break;
    //case 2:
    //    startX = 600; startY = 100;
    //    break;
    //case 3:
    //    startX = 50; startY = 350;
    //    break;
    //}

    //switch (toPlayerIndex) {
    //case 0:
    //    endX = 600; endY = 600;
    //    break;
    //case 1:
    //    endX = 1150; endY = 350;
    //    break;
    //case 2:
    //    endX = 600; endY = 100;
    //    break;
    //case 3:
    //    endX = 50; endY = 350;
    //    break;
    //}

    //// 显示动画的文本（上贡或还贡）
    //cleardevice();
    //loadBackgroundImage();

    //settextcolor(WHITE);
    //settextstyle(30, 0, _T("宋体"));
    //setbkmode(TRANSPARENT);

    //if (isTribute) {
    //    outtextxy(560, 50, _T("上贡"));
    //}
    //else {
    //    outtextxy(560, 50, _T("还贡"));
    //}

    //const int animationFrames = 30;
    //const int cardWidth = 75;
    //const int cardHeight = 104;

    //// 执行卡牌移动动画
    //for (int frame = 0; frame <= animationFrames; frame++) {
    //    // 清空之前的绘制内容，但保留背景
    //    BeginBatchDraw();
    //    loadBackgroundImage();

    //    if (isTribute) {
    //        outtextxy(560, 50, _T("上贡"));
    //    }
    //    else {
    //        outtextxy(560, 50, _T("还贡"));
    //    }

    //    // 计算当前帧卡牌的位置
    //    int currentX = startX + (endX - startX) * frame / animationFrames;
    //    int currentY = startY + (endY - startY) * frame / animationFrames;

    //    // 绘制卡牌
    //    displayCard(card, currentX, currentY, cardWidth, cardHeight);

    //    // 结束批量绘制，刷新屏幕
    //    EndBatchDraw();

    //    // 延迟以控制动画速度
    //    Sleep(20);
    //}

    //// 动画结束后暂停一下
    //Sleep(500);
}

// 设置音量
void Graphical::setVolume(float level) {
    volumeLevel = max(0.0f, min(1.0f, level));

    // 使用Windows API设置系统音量
    DWORD volume = (DWORD)(volumeLevel * 0xFFFF); // 转换为Windows音量格式 (0-65535)

    // 设置所有音频设备的音量
    waveOutSetVolume(NULL, volume | (volume << 16));
}

// 获取当前音量
float Graphical::getVolume() const {
    return volumeLevel;
}

// 绘制音量控制元素
void Graphical::drawVolumeControl() {
    // 绘制音量按钮
    drawButton(volumeButton);

    // 如果音量滑块可见，绘制音量滑块
    if (showVolumeSlider) {
        // 绘制滑块背景
        setfillcolor(RGB(100, 100, 100));
        setlinecolor(WHITE);
        fillrectangle(volumeSliderRect.left, volumeSliderRect.top,
            volumeSliderRect.right, volumeSliderRect.bottom);

        // 计算当前音量位置
        int sliderWidth = volumeSliderRect.right - volumeSliderRect.left;
        int sliderPos = volumeSliderRect.left + (int)(volumeLevel * sliderWidth);

        // 绘制滑块填充部分（已设置的音量）
        setfillcolor(RGB(0, 122, 204));
        fillrectangle(volumeSliderRect.left, volumeSliderRect.top,
            sliderPos, volumeSliderRect.bottom);

        // 绘制滑块控制点
        setfillcolor(WHITE);
        fillcircle(sliderPos, (volumeSliderRect.top + volumeSliderRect.bottom) / 2, 8);

        // 显示当前音量百分比
        settextcolor(WHITE);
        settextstyle(16, 0, _T("宋体"));
        setbkmode(TRANSPARENT);

        TCHAR volumeText[20];
        _stprintf_s(volumeText, _T("%d%%"), (int)(volumeLevel * 100));

        // 将音量百分比文本放在滑块上方
        int textX = volumeSliderRect.left;
        int textY = volumeSliderRect.top - 25;
        outtextxy(textX, textY, volumeText);
    }
}

// 处理音量控制的鼠标事件
bool Graphical::handleVolumeControl(int x, int y) {
    // 检查是否点击了音量按钮
    if (isPointInButton(x, y, volumeButton)) {
        showVolumeSlider = !showVolumeSlider;
        return true;
    }

    // 如果音量滑块可见且点击或拖动在滑块区域内
    if (showVolumeSlider &&
        x >= volumeSliderRect.left && x <= volumeSliderRect.right &&
        y >= volumeSliderRect.top && y <= volumeSliderRect.bottom) {

        // 计算新的音量级别
        float newVolume = (float)(x - volumeSliderRect.left) /
            (float)(volumeSliderRect.right - volumeSliderRect.left);

        // 设置新音量
        setVolume(newVolume);
        return true;
    }

    return false;
}