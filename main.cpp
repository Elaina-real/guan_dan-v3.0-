#include "game.h"
#include "player.h"
#include "card.h"
#include "cardgroup.h"
#include "cardgroupfactory.h"
#include <iostream>
#include <memory>
#include <vector>
#include "easyx.h"
#include <conio.h>//��������
#include <graphics.h>  //���� EasyX ͼ�ο��ͷ�ļ�
#include <string>
#include "graphical.h"
#include <Windows.h>//��� Sleep ����
#include <wingdi.h>
using namespace std;




int main()
{
 //  // ����һ��ͼ�δ���
 //  initgraph(1280, 720);


 //  cout << "========== �走��Ϸ���� ==========" << endl << endl;

 //  // ����4�����
 //  vector<shared_ptr<Player>> players;
 //  players.push_back(make_shared<HumanPlayer>("anon", Red));
 //  players.push_back(make_shared<AIPlayer>("Taki",Blue));
 //  players.push_back(make_shared<AIPlayer>("Soyo",Red));
 //  players.push_back(make_shared<AIPlayer>("Tomori",Blue));

 //  Game game(players, two);

 // 
	//game.aWholeGame();

 //  // ��Ϸ��������ʾ���շ���
 //  cout << "\n��Ϸ���������շ�����" << endl;
 //  for (auto& player : players) {
 //      cout << player->getName() << ": " << player->getScore() << endl;
 //  }


 //  return 0;

     // ����һ��ͼ�δ���
    initgraph(1280, 720);

    // ������ʼ��Ϸ���󣨽�������ʾ��ʼ���棩
    vector<shared_ptr<Player>> players;
    players.push_back(make_shared<HumanPlayer>("anon", Red));
    players.push_back(make_shared<AIPlayer>("Taki", Blue));
    players.push_back(make_shared<AIPlayer>("Soyo", Red));
    players.push_back(make_shared<AIPlayer>("Tomori", Blue));

    Game game(players, two);
    Graphical graphical(&game);

    // ��ʾ��ʼ����
    graphical.drawStartInterface();

    int gameMode = -1;
    bool exitStartInterface = false;

    // ����ʼ��������
    while (!exitStartInterface) {
        exitStartInterface = graphical.handleStartInterfaceInput(gameMode);
        Sleep(10);
    }

    // �����û�ѡ��ִ����Ӧ����
    if (gameMode == 1) {
        // ��ʼ��Ϸ
        cout << "========== �走��Ϸ��ʼ ==========" << endl << endl;
        game.aWholeGame();

        // ��Ϸ��������ʾ���շ���
        cout << "\n��Ϸ���������շ�����" << endl;
        for (auto& player : players) {
            cout << player->getName() << ": " << player->getScore() << endl;
        }
    }
    else {
        // �˳���Ϸ
        closegraph();
        return 0;
    }

    _getch();
    // ֹͣ���ֲ���
    PlaySound(NULL, NULL, 0);
    closegraph();

    return 0;
}
