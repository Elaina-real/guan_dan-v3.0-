#include "game.h"
#include "player.h"
#include "card.h"
#include "cardgroup.h"
#include "cardgroupfactory.h"
#include <iostream>
#include <memory>
#include <vector>
#include "easyx.h"
#include <conio.h>//键盘输入
#include <graphics.h>  //包含 EasyX 图形库的头文件
#include <string>
#include "graphical.h"
#include <Windows.h>//添加 Sleep 函数
#include <wingdi.h>
using namespace std;




int main()
{
 //  // 创建一个图形窗口
 //  initgraph(1280, 720);


 //  cout << "========== 掼蛋游戏测试 ==========" << endl << endl;

 //  // 创建4个玩家
 //  vector<shared_ptr<Player>> players;
 //  players.push_back(make_shared<HumanPlayer>("anon", Red));
 //  players.push_back(make_shared<AIPlayer>("Taki",Blue));
 //  players.push_back(make_shared<AIPlayer>("Soyo",Red));
 //  players.push_back(make_shared<AIPlayer>("Tomori",Blue));

 //  Game game(players, two);

 // 
	//game.aWholeGame();

 //  // 游戏结束后显示最终分数
 //  cout << "\n游戏结束，最终分数：" << endl;
 //  for (auto& player : players) {
 //      cout << player->getName() << ": " << player->getScore() << endl;
 //  }


 //  return 0;

     // 创建一个图形窗口
    initgraph(1280, 720);

    // 创建初始游戏对象（仅用于显示开始界面）
    vector<shared_ptr<Player>> players;
    players.push_back(make_shared<HumanPlayer>("anon", Red));
    players.push_back(make_shared<AIPlayer>("Taki", Blue));
    players.push_back(make_shared<AIPlayer>("Soyo", Red));
    players.push_back(make_shared<AIPlayer>("Tomori", Blue));

    Game game(players, two);
    Graphical graphical(&game);

    // 显示开始界面
    graphical.drawStartInterface();

    int gameMode = -1;
    bool exitStartInterface = false;

    // 处理开始界面输入
    while (!exitStartInterface) {
        exitStartInterface = graphical.handleStartInterfaceInput(gameMode);
        Sleep(10);
    }

    // 根据用户选择执行相应操作
    if (gameMode == 1) {
        // 开始游戏
        cout << "========== 掼蛋游戏开始 ==========" << endl << endl;
        game.aWholeGame();

        // 游戏结束后显示最终分数
        cout << "\n游戏结束，最终分数：" << endl;
        for (auto& player : players) {
            cout << player->getName() << ": " << player->getScore() << endl;
        }
    }
    else {
        // 退出游戏
        closegraph();
        return 0;
    }

    _getch();
    // 停止音乐播放
    PlaySound(NULL, NULL, 0);
    closegraph();

    return 0;
}
