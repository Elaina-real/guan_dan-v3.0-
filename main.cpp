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
    cout << "========== �走��Ϸ���� ==========" << endl << endl;

    // ����4�����
    vector<shared_ptr<Player>> players;
    players.push_back(make_shared<HumanPlayer>("anon", Red));
    //players.push_back(make_shared<AIPlayer>("Anon",Red));
    players.push_back(make_shared<AIPlayer>("Taki",Blue));
    players.push_back(make_shared<AIPlayer>("Soyo",Red));
    players.push_back(make_shared<AIPlayer>("Tomori",Blue));

    Game game(players, two);

   
	game.aWholeGame();

    // ��Ϸ��������ʾ���շ���
    cout << "\n��Ϸ���������շ�����" << endl;
    for (auto& player : players) {
        cout << player->getName() << ": " << player->getScore() << endl;
    }


    return 0;
}
