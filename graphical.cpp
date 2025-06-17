#include "graphical.h"
#include <graphics.h>  // ���� EasyX ͼ�ο��ͷ�ļ�
#include <conio.h> // ���ڻ�ȡ��������
#include "game.h"
#include "player.h"
#include "cardcounter.h"

using namespace std;

void Graphical::setGame(Game* g)
{
	game = g;
}

string Graphical::getCardImageFileName(const Card& card)
{
    string fileName = "resource/";

    // ���ݻ�ɫ�͵���ȷ���ļ���
    if (card.getSuit() == Joker) 
    {
        // ��С����ͼƬ
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
        // ��ͨ�Ƶ�ͼƬ
        string suitStr;
        switch (card.getSuit()) 
        {
        case Heart:   suitStr = "Heart"; break;    // ����
        case Diamond: suitStr = "Diamond"; break;    // ����
        case Club:    suitStr = "Club"; break;    // ÷��
        case Spade:   suitStr = "Spade"; break;    // ����
        default:      suitStr = "Heart"; break;    // Ĭ�Ϻ���
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

// ��ʾ��Ƭ
void Graphical::displayCard(const Card& card, int x, int y, int width, int height) {
    string imagePath = getCardImageFileName(card);
    IMAGE cardImg;

    wstring wideImagePath = wstring(imagePath.begin(), imagePath.end());
    // ���Լ��ؿ�ƬͼƬ
    loadimage(&cardImg, wideImagePath.c_str(), width, height);

    // ��ʾ��Ƭ
    putimage(x, y, &cardImg);

    //// ����Ǽ��ƻ�����ƣ����������
    //if (card.isLevelCard()) {
    //    // �ڿ�Ƭ�ϱ��Ϊ����
    //    settextcolor(RED);
    //    settextstyle(18, 0, _T("����"));
    //    outtextxy(x + 5, y + 5, _T("��"));
    //}

    //if (card.isWildCard()) {
    //    // �ڿ�Ƭ�ϱ��Ϊ�����
    //    settextcolor(GREEN);
    //    settextstyle(18, 0, _T("����"));
    //    outtextxy(x + width - 25, y + 5, _T("�"));
    //}
}

// ��ʾһ�鿨Ƭ
vector<CardRect> Graphical::displayCards(const vector<Card>& cards)
{
    int x = 250;
    int y = 550;
    int cardWidth = 75;
    int cardHeight = 104;
    vector<CardRect> cardRects;

    for (int i = 0; i < cards.size(); i++)
    {
        // ���㿨Ƭλ�ã�ѡ�еĿ�Ƭ����20����
        int yOffset = (cards[i].isSelectedCard()) ? -30 : 0;

        // ��ʾ��Ƭ
        displayCard(cards[i], x, y + yOffset, 75, 104);

        // ���濨Ƭ��������Ϣ
        CardRect rect;
        rect.x = x;
        rect.y = y + yOffset;
        rect.width = cardWidth;
        rect.height = cardHeight;
        rect.index = i;
        cardRects.push_back(rect);

        // ��һ�ſ���λ��
        x += 30;  // ��Ƭ�ص���ʾ
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
	putimage(x, y, &backImg); // �ڴ�������ʾ����ͼƬ

	int centerX = x + 75 / 2; // ����ͼƬ���ĵ�x����
	int centerY = y + 104 / 2; // ����ͼƬ���ĵ�y����
    // �����ı���ʽ
    setbkmode(TRANSPARENT);
    settextcolor(RED); // �����ı���ɫΪ��ɫ
    settextstyle(50, 0, _T("Arial")); // �����ı���СΪ30����ϸΪ0������ΪArial

    // �����ı�
    TCHAR number[10];
	int cardsNum = cards.size();
    _stprintf_s(number, _T("%d"), cardsNum); // ������42ת��Ϊ�ַ���

    // ��ȡ�ı���Ⱥ͸߶��Ա������ʾ
    int textWidth = textwidth(number);
    int textHeight = textheight(number);

    // ��ͼƬ������ʾ���֣���Ҫ��ȥ�ı���Ⱥ͸߶ȵ�һ����ȷ������
    outtextxy(centerX - textWidth / 2, centerY - textHeight / 2, number);
}

// ����һ����ť
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

// ���ư�ť
void Graphical::drawButton(const Button& button) {
    // ���ð�ť��ɫ
    COLORREF btnColor = button.isEnabled ? RGB(0, 122, 204) : RGB(150, 150, 150);
    COLORREF textColor = WHITE;

    // ���ư�ť����
    setfillcolor(btnColor);
    setlinecolor(RGB(30, 30, 30));
    fillroundrect(button.x, button.y, button.x + button.width, button.y + button.height, 10, 10);

    // ���ư�ť�ı�
    settextcolor(textColor);
    settextstyle(20, 0, _T("����"));
    setbkmode(TRANSPARENT);

    // ʹ�� EasyX �Ĺ���ת�������ַ���
    TCHAR* text = new TCHAR[button.text.length() + 1];
#ifdef UNICODE
    MultiByteToWideChar(CP_ACP, 0, button.text.c_str(), -1, text, button.text.length() + 1);
#else
    strcpy(text, button.text.c_str());
#endif

    // �����ı���Ⱥ͸߶��Ա����
    int textWidth = textwidth(text);
    int textHeight = textheight(text);

    // �ڰ�ť������ʾ�ı�
    outtextxy(button.x + (button.width - textWidth) / 2,
        button.y + (button.height - textHeight) / 2,
        text);

    delete[] text;
}

// ���Ƴ��ƺͲ�����ť
vector<Button> Graphical::drawPlayButtons() {
    vector<Button> buttons;

    // ����"����"��ť
    Button playButton = createButton(600, 470, 120, 40, "����");
    // ����"����"��ť
    Button passButton = createButton(450, 470, 120, 40, "����");

    // ���ư�ť
    drawButton(playButton);
    drawButton(passButton);

    // ����ť��ӵ������б�
    buttons.push_back(playButton);
    buttons.push_back(passButton);

    return buttons;
}

// ������Ƿ��ڰ�ť��
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

// �жϵ�����Ƿ��ڿ�Ƭ������
bool Graphical::isPointInRect(int x, int y, const CardRect& rect) {
    return (x >= rect.x && x <= rect.x + rect.width &&
        y >= rect.y && y <= rect.y + rect.height);
}

void Graphical::drawUI(int index)
{
    // ��鲢������Ϸ����
    checkAndUpdateGameMusic(game->getScoreFactor());

    // ��ʼ�������ƣ�����˫���弼������ֹ��˸
    BeginBatchDraw();

    // �����Ļ��׼�����»���
    cleardevice();

    loadBackgroundImage();

    // ��ȡ��Ϸ��Ϣ
    auto players = game->getPlayers();

    // 1. ��ʾ��Ϸ״̬��Ϣ�����𡢱����ȣ�
    drawGameInfo();

    // 2. ��ʾAI��ҵ�����
    for (int i = 1; i < players.size(); i++) 
    {    
        displayAICards(players[i]->getHandCards(), i);

        // ��ʾAI������ֺ�����
        drawPlayerInfo(players[i], i);
		drawPlayerPlay(i);
    }

    // 3. ��ʾ��ǰ�������
    auto cardRects = displayCards(players[0]->getHandCards());

    // 4. ��ʾ�����Ϣ�����֡����������ȣ�
    drawPlayerInfo(players[0], 0);
	drawPlayerPlay(0);

    //// 5. ��ʾ��һ���ƣ�����У�
    //const CardGroup* lastPlay = game->getLastPlay();
    //if (lastPlay)
    //{
    //    vector<Card> lastPlayCards = lastPlay->getCards();
    //    drawLastPlay(lastPlayCards);
    //}

    // 6. ���ư�ť
    drawPlayButtons();

    //// 7. ���Ƽ�����
    drawCardCounter();

	// 8. ��ʾ��ǰ��ҳ�����ʾ
    if(index != -1)
	displayCurrentPlayer(index);

    // �����������ƣ�ˢ����Ļ
    EndBatchDraw();
}

void Graphical::drawGameInfo()
{
    // �����ı���ʽ
    settextcolor(WHITE);
    settextstyle(20, 0, _T("����"));
    setbkmode(TRANSPARENT);

    // ��ʾ��ǰ����
    wstring levelText = L"��ǰ����: ";
    levelText += to_wstring(game->getLevel());
    outtextxy(20, 20, levelText.c_str());

    // ��ʾ��ǰ����
    wstring factorText = L"��ǰ����: ";
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

    // �����ı���ʽ
    settextcolor(WHITE);
    settextstyle(20, 0, _T("����"));
    setbkmode(TRANSPARENT);

    // ׼���ı�����
    wstring playerInfo = L"";
    string playerName = player->getName();  // �Ȼ�ȡ���ƣ��洢�ھֲ�������
    playerInfo += wstring(playerName.begin(), playerName.end());  // ʹ��ͬһ���ַ�������ĵ�����
    playerInfo += L" Lv:";
    playerInfo += to_wstring(player->getLevel());

    // ��ʾ�������������У�
    PlayerRank rank = player->getARoundRank();
    if (rank != NotRanked) {
        playerInfo += L" ����:";
        switch (rank) {
        case First:  playerInfo += L"��һ"; break;
        case Second: playerInfo += L"�ڶ�"; break;
        case Third:  playerInfo += L"����"; break;
        case Fourth: playerInfo += L"����"; break;
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
		// ���û�г��ƣ���ʾ��ʾ��Ϣ
		settextcolor(WHITE);
		settextstyle(20, 0, _T("����"));
		outtextxy(x, y, _T("δ����"));
	}
    else
    {
        // ��ʾ��ҳ���
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

    //��ӡ���֡������С�
	settextcolor(WHITE);
	settextstyle(20, 0, _T("����"));
	setbkmode(TRANSPARENT);
	//wstring currentPlayerText = L"��ǰ���: ";
	//currentPlayerText += to_wstring(index + 1); // ���������0��ʼ�����Լ�1
	outtextxy(x, y, _T("��ǰ��ҡ�"));
}

bool Graphical::handleUserInput(vector<Card>& selectedCards, bool& isPass)
{
    auto players = game->getPlayers();
    vector<Card>& playerCards = players[0]->getHandCards();
    vector<Button> buttons;

    // ���Ƴ�ʼUI����
    drawUI(0);

    BeginBatchDraw();
    // ��ȡ��Ƭ���� - ֻ��ȡһ�Σ���������ѭ����
    vector<CardRect> cardRects = displayCards(playerCards);
    EndBatchDraw();

    Button playButton = createButton(600, 470, 120, 40, "����");
    Button passButton = createButton(450, 470, 120, 40, "����");
    buttons.push_back(playButton);
    buttons.push_back(passButton);

    // �����������
    MOUSEMSG msg;
    bool exitLoop = false;
    bool isPlay = false;
    isPass = false;

    while (!exitLoop) {


        // ��ȡ�����Ϣ
        if (MouseHit()) {
            msg = GetMouseMsg();



            if (msg.uMsg == WM_LBUTTONDOWN) {
                // ����Ƿ����˰�ť
                if (isPointInButton(msg.x, msg.y, playButton)) {
                    isPlay = true;
                    exitLoop = true;
                    continue;
                }

                if (isPointInButton(msg.x, msg.y, passButton)) {
                    // �ж��Ƿ���Թ���
                    if (game->getLastPlay() == nullptr) {
                        // �׼ҳ��Ʋ��ܹ�����ʾ��ʾ
                        settextcolor(RED);
                        settextstyle(24, 0, _T("����"));
                        outtextxy(450, 445, _T("��������ƣ�"));
                        FlushBatchDraw();  // ����ˢ����ʾ��ʾ
                        Sleep(1000);  // ��ʾ1��
                        drawUI(0);  // �ػ���������ʾ
                    }
                    else {
                        isPass = true;
                        exitLoop = true;
                    }
                    continue;
                }

                // ����Ƿ����˿�Ƭ - �����һ�ſ�ʼ��飬�������Դ���������Ŀ�Ƭ
                bool cardClicked = false;
                for (int i = cardRects.size() - 1; i >= 0; i--) {
                    // ʹ�ÿ�Ƭ��������Ƚ��е�����
                    if (msg.x >= cardRects[i].x && msg.x <= cardRects[i].x + cardRects[i].width &&
                        msg.y >= cardRects[i].y && msg.y <= cardRects[i].y + cardRects[i].height) {

                        // �л���Ƭѡ��״̬
                        playerCards[i].toggleSelected();
                        cardClicked = true;

                        drawUI(0);

                        // ʹ��˫������ƣ�������˸
                        BeginBatchDraw();

                      
                        // ��ʾ��ǰ������ƣ����º�ģ�
                        cardRects = displayCards(players[0]->getHandCards());

                      

                        // �����������ƣ�ˢ����Ļ
                        EndBatchDraw();

                        break;  // ֻ�������ϲ�Ŀ�Ƭ

                //bool cardClicked = false;
                //for (int i = cardRects.size() - 1; i >= 0; i--) {
                //    // ��������� - ���һ����ʹ��������ȣ�������ֻʹ��ǰ30����
                //    int effectiveWidth = (i == cardRects.size() - 1) ? cardRects[i].width : 30;

                //    if (msg.x >= cardRects[i].x && msg.x <= cardRects[i].x + effectiveWidth &&
                //        msg.y >= cardRects[i].y && msg.y <= cardRects[i].y + cardRects[i].height) {

                //        // �л���Ƭѡ��״̬
                //        playerCards[i].toggleSelected();
                //        cardClicked = true;

                //        // ���¿�Ƭλ����Ϣ - �ؼ�������Ҫ����cardRects
                //        drawUI();  // �ػ���������
                //        cardRects = displayCards(playerCards);  // ��ȡ�µĿ�Ƭ������Ϣ

                //        break;  // ֻ�������ϲ�Ŀ�Ƭ
                    }
                }
            }
        }
    }

    // ���ѡ����ƣ��ռ�ѡ�еĿ�Ƭ
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
    // ���ر���ͼƬ (ȷ��ͼƬ��С�봰��ƥ��)
    loadimage(&bgImg, _T("resource/bg.jpg"), 1280, 720);

    // ���Ʊ���
    putimage(0, 0, &bgImg);
}

void Graphical::drawCardCounter()
{
    // ���Ȼ�ȡ��ҵ�����
    auto players = game->getPlayers();
    vector<Card> playerCards = players[0]->getHandCards();

	BeginBatchDraw();

    IMAGE img;
    loadimage(&img, _T("resource/cardcounter.png"), 324, 95); // ���ؼ�����ͼƬ
    putimage(920, 10, &img); // �ڴ�������ʾ������ͼƬ

    // �����ı���ʽ
    settextcolor(BLACK);
    settextstyle(16, 0, _T("Arial"));
    setbkmode(TRANSPARENT);

    int baseX = 930; // ������ͼƬ�����Ͻ�X����
    int baseY = 10;  // ������ͼƬ�����Ͻ�Y����

    // ��һ��Y���� - 2��10�ļ���
    int row1Y = 36; // �����Ե������ֵ�����õ�һ���ı���λ��

    // �ڶ���Y���� - J�������ļ���
    int row2Y = 84; // �����Ե������ֵ�����õڶ����ı���λ��

    int adder = 32.4;

    int x2 = baseX;   // 2��X����λ��
    int x3 = baseX + adder;   // 3��X����λ��
    int x4 = baseX + 2*adder;   // 4��X����λ��
    int x5 = baseX + 3*adder;   // 5��X����λ��
    int x6 = baseX + 4*adder;   // 6��X����λ��
    int x7 = baseX + 5*adder;   // 7��X����λ��
    int x8 = baseX + 6*adder;   // 8��X����λ��
    int x9 = baseX + 7*adder;   // 9��X����λ��
    int x10 = baseX + 8*adder;  // 10��X����λ��

    // �ڶ��� - J�������ļ���λ��
    // X���꽫��������д������ֻ��Ԥ��λ��
    int xJ = baseX;    // J��X����λ��
    int xQ = baseX + adder;   // Q��X����λ��
    int xK = baseX + 2*adder;   // K��X����λ��
    int xA = baseX + 3*adder;   // A��X����λ��
    int xSmallJoker = baseX + 4*adder;  // С����X����λ��
    int xBigJoker = baseX + 5*adder;    // ������X����λ��
	int xHeart = baseX + 6 * adder;  
	int xSpade = baseX + 7 * adder; 
	int xDiamond = baseX + 8 * adder;   
	int xClub = baseX + 9 * adder;

    // ��ʾ2��10��ʣ������
    outtextxy(x2, row1Y, to_wstring(counterPtr->getRemainingCountByRankExcludePlayer(two,playerCards)).c_str());
    outtextxy(x3, row1Y, to_wstring(counterPtr->getRemainingCountByRankExcludePlayer(three, playerCards)).c_str());
    outtextxy(x4, row1Y, to_wstring(counterPtr->getRemainingCountByRankExcludePlayer(four, playerCards)).c_str());
    outtextxy(x5, row1Y, to_wstring(counterPtr->getRemainingCountByRankExcludePlayer(five, playerCards)).c_str());
    outtextxy(x6, row1Y, to_wstring(counterPtr->getRemainingCountByRankExcludePlayer(six, playerCards)).c_str());
    outtextxy(x7, row1Y, to_wstring(counterPtr->getRemainingCountByRankExcludePlayer(seven, playerCards)).c_str());
    outtextxy(x8, row1Y, to_wstring(counterPtr->getRemainingCountByRankExcludePlayer(eight, playerCards)).c_str());
    outtextxy(x9, row1Y, to_wstring(counterPtr->getRemainingCountByRankExcludePlayer(nine, playerCards)).c_str());
    outtextxy(x10, row1Y, to_wstring(counterPtr->getRemainingCountByRankExcludePlayer(ten, playerCards)).c_str());

    // ��ʾJ��������ʣ������
    outtextxy(xJ, row2Y, to_wstring(counterPtr->getRemainingCountByRankExcludePlayer(jack, playerCards)).c_str());
    outtextxy(xQ, row2Y, to_wstring(counterPtr->getRemainingCountByRankExcludePlayer(queen, playerCards)).c_str());
    outtextxy(xK, row2Y, to_wstring(counterPtr->getRemainingCountByRankExcludePlayer(king, playerCards)).c_str());
    outtextxy(xA, row2Y, to_wstring(counterPtr->getRemainingCountByRankExcludePlayer(ace, playerCards)).c_str());
    outtextxy(xSmallJoker, row2Y, to_wstring(counterPtr->getRemainingCountByRankExcludePlayer(smal, playerCards)).c_str());
    outtextxy(xBigJoker, row2Y, to_wstring(counterPtr->getRemainingCountByRankExcludePlayer(big, playerCards)).c_str());

	// ��ʾ��ɫ��ʣ������
	outtextxy(xHeart, row2Y, to_wstring(counterPtr->getRemainingCountBySuitExcludePlayer(Heart, playerCards)).c_str());
	outtextxy(xSpade, row2Y, to_wstring(counterPtr->getRemainingCountBySuitExcludePlayer(Spade, playerCards)).c_str());
	outtextxy(xDiamond, row2Y, to_wstring(counterPtr->getRemainingCountBySuitExcludePlayer(Diamond, playerCards)).c_str());
	outtextxy(xClub, row2Y, to_wstring(counterPtr->getRemainingCountBySuitExcludePlayer(Club, playerCards)).c_str());

	// �����������ƣ�ˢ����Ļ
	EndBatchDraw();
}

void Graphical::drawStartInterface() {
    // ���ſ�ʼ��������
    playBackgroundMusic("resource/Welcome.wav");

    // �����Ļ
    cleardevice();

    // ���ر���ͼƬ
    IMAGE bgImg;
    loadimage(&bgImg, _T("resource/bg.jpg"), 1280, 720);
    putimage(0, 0, &bgImg);

    // ������Ϸ����
    settextcolor(WHITE);
    settextstyle(80, 0, _T("�����п�"));
    setbkmode(TRANSPARENT);
    outtextxy(460, 100, _T("��           ��"));

    // ���ư�ť
    Button startButton = createButton(540, 300, 200, 60, "��ʼ��Ϸ");
    Button rulesButton = createButton(540, 380, 200, 60, "��Ϸ����");
    Button rankingButton = createButton(540, 460, 200, 60, "���а�");
    Button quitButton = createButton(540, 540, 200, 60, "�˳���Ϸ");

    drawButton(startButton);
    drawButton(rulesButton);
    drawButton(rankingButton);
    drawButton(quitButton);

    // ���ư�Ȩ��Ϣ
    settextstyle(20, 0, _T("����"));
    outtextxy(520, 650, _T("�汾 3.0 2025"));
}

bool Graphical::handleStartInterfaceInput(int& gameMode) {
    // ������ť
    Button startButton = createButton(540, 300, 200, 60, "��ʼ��Ϸ");
    Button rulesButton = createButton(540, 380, 200, 60, "��Ϸ����");
    Button rankingButton = createButton(540, 460, 200, 60, "���а�");
    Button quitButton = createButton(540, 540, 200, 60, "�˳���Ϸ");

    MOUSEMSG msg;

    // ���������Ϣ
    if (MouseHit()) {
        msg = GetMouseMsg();

        if (msg.uMsg == WM_LBUTTONDOWN) {
            // ������İ�ť
            if (isPointInButton(msg.x, msg.y, startButton)) {
                gameMode = 1; // ��ʼ��Ϸ
                return true;
            }
            else if (isPointInButton(msg.x, msg.y, rulesButton)) {
                // ��ʾ��Ϸ����
                cleardevice();
                loadBackgroundImage();

                settextcolor(WHITE);
                settextstyle(40, 0, _T("����"));
                outtextxy(500, 50, _T("��Ϸ����"));

                settextstyle(20, 0, _T("����"));
                int y = 120;
                outtextxy(300, y, _T("1. �走��һ�������˿�����Ϸ����Ϊ������Ӫ�Կ�")); y += 30;
                outtextxy(300, y, _T("2. ��Ϸʹ�������ƣ�ȥ����С����108���ƣ�")); y += 30;
                outtextxy(300, y, _T("3. ÿ����Ϸ��һ���ض��ļ�����")); y += 30;
                outtextxy(300, y, _T("4. ��һλ�������Ƶ���һ�ʤ�����һλ��")); y += 30;
                outtextxy(300, y, _T("5. ����ƿ���ѡ�У��ٴε��ȡ��ѡ��")); y += 30;
                outtextxy(300, y, _T("6. ������ư�ť���ѡ�е���")); y += 30;

                // ���ذ�ť
                Button backButton = createButton(540, 600, 200, 60, "����");
                drawButton(backButton);

                FlushBatchDraw();

                // �ȴ��������
                bool back = false;
                while (!back) {
                    if (MouseHit()) {
                        MOUSEMSG backMsg = GetMouseMsg();
                        if (backMsg.uMsg == WM_LBUTTONDOWN && isPointInButton(backMsg.x, backMsg.y, backButton)) {
                            back = true;
                        }
                    }
                }

                // �ػ濪ʼ����
                drawStartInterface();
                return false;
            }
            else if (isPointInButton(msg.x, msg.y, rankingButton)) {
                // ��ʾ���а񣨿���������ʵ�����а��ܣ�
                cleardevice();
                loadBackgroundImage();

                settextcolor(WHITE);
                settextstyle(40, 0, _T("����"));
                outtextxy(500, 50, _T("���а�"));

                // �������������а�����
                settextstyle(20, 0, _T("����"));
                outtextxy(450, 200, _T("δʵװXD"));

                // ���ذ�ť
                Button backButton = createButton(540, 600, 200, 60, "����");
                drawButton(backButton);

                FlushBatchDraw();

                // �ȴ��������
                bool back = false;
                while (!back) {
                    if (MouseHit()) {
                        MOUSEMSG backMsg = GetMouseMsg();
                        if (backMsg.uMsg == WM_LBUTTONDOWN && isPointInButton(backMsg.x, backMsg.y, backButton)) {
                            back = true;
                        }
                    }
                }

                // �ػ濪ʼ����
                drawStartInterface();
                return false;
            }
            else if (isPointInButton(msg.x, msg.y, quitButton)) {
                gameMode = 0; // �˳���Ϸ
                return true;
            }
        }
    }

    return false;
}

void Graphical::drawSettlementInterface(bool isGameOver) {
    playBackgroundMusic("resource/Welcome.wav");

    // �����Ļ
    cleardevice();

    // ���ر���ͼƬ
    IMAGE bgImg;
    loadimage(&bgImg, _T("resource/bg.jpg"), 1280, 720);
    putimage(0, 0, &bgImg);

    // ��ȡ��Ϸ��Ϣ
    auto players = game->getPlayers();

    // ���Ʊ���
    settextcolor(WHITE);
    settextstyle(60, 0, _T("�����п�"));
    setbkmode(TRANSPARENT);

    if (isGameOver) {
        outtextxy(400, 50, _T("��Ϸ����"));
    }
    else {
        outtextxy(400, 50, _T("���ֽ���"));
    }

    // ���Ʒָ���
    setlinecolor(WHITE);
    line(300, 130, 980, 130);

    // ���������������Ϣ
    settextstyle(30, 0, _T("����"));
    int yPos = 170;

    // �������������
    vector<shared_ptr<Player>> sortedPlayers = players;
    sort(sortedPlayers.begin(), sortedPlayers.end(),
        [](const shared_ptr<Player>& a, const shared_ptr<Player>& b) {
            return a->getARoundRank() < b->getARoundRank();
        });

    // ��ʾ������������ơ����顢�����ͼ���
    outtextxy(300, yPos, _T("����"));
    outtextxy(400, yPos, _T("���"));
    outtextxy(550, yPos, _T("����"));
    outtextxy(700, yPos, _T("����"));
    outtextxy(850, yPos, _T("����"));
    yPos += 50;

    for (const auto& player : sortedPlayers) {
        // ��ʾ����
        wstring rankText;
        switch (player->getARoundRank()) {
        case First:  rankText = L"��һ"; break;
        case Second: rankText = L"�ڶ�"; break;
        case Third:  rankText = L"����"; break;
        case Fourth: rankText = L"����"; break;
        default:     rankText = L"δ����"; break;
        }
        outtextxy(300, yPos, rankText.c_str());

        // ��ʾ�������
        string playerName = player->getName();
        wstring wPlayerName(playerName.begin(), playerName.end());
        outtextxy(400, yPos, wPlayerName.c_str());

        // ��ʾ����
        wstring teamText = (player->getTeam() == Red) ? L"���" : L"����";
        outtextxy(550, yPos, teamText.c_str());

        // ��ʾ����
        wstring scoreText = to_wstring(player->getScore());
        outtextxy(700, yPos, scoreText.c_str());

        // ��ʾ����
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

    // ��ʾ������Ϸ��Ϣ
    settextstyle(25, 0, _T("����"));
    yPos += 30;

    wstring levelText = L"��ǰ����: ";
    switch (game->getLevel()) {
    case jack:  levelText += L"J"; break;
    case queen: levelText += L"Q"; break;
    case king:  levelText += L"K"; break;
    case ace:   levelText += L"A"; break;
    default:    levelText += to_wstring(game->getLevel()); break;
    }
    outtextxy(400, yPos, levelText.c_str());

    wstring factorText = L"���ձ���: " + to_wstring(game->getScoreFactor());
    outtextxy(700, yPos, factorText.c_str());

    // ���ư�ť
    Button continueButton = createButton(440, 550, 200, 60, "������Ϸ");
    Button exitButton = createButton(680, 550, 200, 60, "�˳���Ϸ");

    drawButton(continueButton);
    drawButton(exitButton);

    FlushBatchDraw();
}

bool Graphical::handleSettlementInterfaceInput(bool& continueGame) {
    // ������ť - ����drawSettlementInterface�еİ�ťλ��һ��
    Button continueButton = createButton(440, 550, 200, 60, "������Ϸ");
    Button exitButton = createButton(680, 550, 200, 60, "�˳���Ϸ");

    MOUSEMSG msg;

    // ���������Ϣ
    if (MouseHit()) {
        msg = GetMouseMsg();

        if (msg.uMsg == WM_LBUTTONDOWN) {
            // ������İ�ť
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

// ���ű�������
void Graphical::playBackgroundMusic(const string& musicFile) {
    if (currentMusic == musicFile && isMusicPlaying) {
        // �����ǰ���ڲ�����������ļ�����ִ���κβ���
        return;
    }

    // ֹͣ��ǰ���ڲ��ŵ�����
    stopBackgroundMusic();

    // ���ļ���ת��Ϊ���ַ���
    wstring wMusicFile(musicFile.begin(), musicFile.end());

    // ���������� (SND_ASYNC|SND_LOOP ��ʾ�첽���Ų�ѭ��)
    if (PlaySound(wMusicFile.c_str(), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP)) {
        isMusicPlaying = true;
        currentMusic = musicFile;
    }
}

// ֹͣ��������
void Graphical::stopBackgroundMusic() {
    if (isMusicPlaying) {
        PlaySound(NULL, NULL, 0); // ֹͣ��������
        isMusicPlaying = false;
        currentMusic = "";
    }
}

// ��鲢������Ϸ����
void Graphical::checkAndUpdateGameMusic(int scoreFactor) {
    if (scoreFactor > 31) {
        // ��������32ʱ�����Ǹ�����
        playBackgroundMusic("resource/dedede.wav");
    }
    else {
        // ���򲥷ų�����Ϸ����
        playBackgroundMusic("resource/Normal.wav");
    }
}

//void Graphical::drawCardCounter()
//{
//    // �����Ͻǻ�һ����͸���ı�����
//    int x = 850;
//    int y = 15;
//    int width = 300;
//    int height = 160;
//
//    // ���ð�͸������
//    setfillcolor(RGB(0, 0, 0, 128)); // ��͸����ɫ
//    solidrectangle(x, y, x + width, y + height);
//
//    // ���ñ߿�
//    setlinecolor(WHITE);
//    rectangle(x, y, x + width, y + height);
//
//    // �����ı���ʽ
//    settextcolor(WHITE);
//    settextstyle(22, 0, _T("����"));
//    setbkmode(TRANSPARENT);
//
//    // ������
//    outtextxy(x + 120, y + 5, L"������");
//
//    // ��ʾ����������ʣ������
//    y += 35; // �Ƶ������·�
//
//    // ���廨ɫ�͵�����ǩ
//    wstring rankLabels[] = { L"", L"", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"10", L"J", L"Q", L"K", L"A" };
//
//    // ���Ʊ��ͷ��
//    settextstyle(18, 0, _T("����"));
//    outtextxy(x + 10, y, L"����");
//    outtextxy(x + 70, y, L"ʣ��");
//    outtextxy(x + 150, y, L"����");
//    outtextxy(x + 210, y, L"ʣ��");
//
//    y += 25;
//
//    // ��ʾ2-A��ʣ��������������
//    for (int r = 2; r <= 14; ++r) {
//        // ���� (2-7)
//        if (r <= 7) {
//            outtextxy(x + 10, y + (r - 2) * 20, rankLabels[r].c_str());
//            int remain = game->cardCounter.getRemainingCountByRank(static_cast<Rank>(r));
//            outtextxy(x + 70, y + (r - 2) * 20, to_wstring(remain).c_str());
//        }
//        // ���� (8-A)
//        else {
//            outtextxy(x + 150, y + (r - 8) * 20, rankLabels[r].c_str());
//            int remain = game->cardCounter.getRemainingCountByRank(static_cast<Rank>(r));
//            outtextxy(x + 210, y + (r - 8) * 20, to_wstring(remain).c_str());
//        }
//    }
//
//    // ��ʾ��С��
//    y += 120;
//    wstring smallJokerText = L"С��: ";
//    smallJokerText += to_wstring(game->cardCounter.getRemainingCount(Joker, smal));
//    outtextxy(x + 10, y, smallJokerText.c_str());
//
//    wstring bigJokerText = L"����: ";
//    bigJokerText += to_wstring(game->cardCounter.getRemainingCount(Joker, big));
//    outtextxy(x + 150, y, bigJokerText.c_str());
//}