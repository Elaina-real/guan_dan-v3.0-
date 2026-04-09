#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <Windows.h>
#include <conio.h>

#include <graphics.h>
#include <easyx.h>
#include <wingdi.h>

#include "game.h"
#include "player.h"
#include "card.h"
#include "cardgroup.h"
#include "cardgroupfactory.h"
#include "graphical.h"

using namespace std;

int main()
{
    // 创建一个图形窗口
    initgraph(1280, 720);

    // 创建初始游戏玩家
    vector<shared_ptr<Player>> players;
    players.push_back(make_shared<HumanPlayer>("anon", Red));   // 人类玩家
    players.push_back(make_shared<AIPlayer>("Taki", Blue));     // AI玩家
    players.push_back(make_shared<AIPlayer>("Soyo", Red));      // AI玩家
    players.push_back(make_shared<AIPlayer>("Tomori", Blue));   // AI玩家

    // 初始化游戏和图形界面
    Game game(players, two);
    Graphical graphical(&game);

    // 显示游戏开始界面
    graphical.drawStartInterface();

    // 处理开始界面输入
    int gameMode = -1;
    bool exitStartInterface = false;

    while (!exitStartInterface) {
        exitStartInterface = graphical.handleStartInterfaceInput(gameMode);
        Sleep(10); 
    }

    // 根据用户选择执行相应操作
    if (gameMode == 1) {
        // 开始新游戏
        cout << "========== 掼蛋游戏开始 ==========" << endl << endl;

        game.aWholeGame();

        // 游戏结束后显示最终分数
        cout << "\n游戏结束，最终分数：" << endl;
        for (auto& player : players) {
            cout << player->getName() << ": " << player->getScore() << endl;
        }
    }
    else {
        // 用户选择退出游戏
        closegraph();
        return 0;
    }

    PlaySound(NULL, NULL, 0);  
    closegraph();              

    return 0;
}
