#include "game.h"
#include "player.h"
#include "card.h"
#include "cardgroup.h"
#include "cardgroupfactory.h"
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

int main()
{
    cout << "========== 掼蛋游戏测试 ==========" << endl << endl;

    // 创建4个玩家
    vector<shared_ptr<Player>> players;
    players.push_back(make_shared<HumanPlayer>("anon", Red));
    //players.push_back(make_shared<AIPlayer>("Anon",Red));
    players.push_back(make_shared<AIPlayer>("Taki",Blue));
    players.push_back(make_shared<AIPlayer>("Soyo",Red));
    players.push_back(make_shared<AIPlayer>("Tomori",Blue));

    Game game(players, two);

   
	game.aWholeGame();

    // 游戏结束后显示最终分数
    cout << "\n游戏结束，最终分数：" << endl;
    for (auto& player : players) {
        cout << player->getName() << ": " << player->getScore() << endl;
    }


    return 0;
}
