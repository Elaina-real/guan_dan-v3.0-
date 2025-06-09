#include "game.h"
#include "player.h" // �����ҪPlayer��������


Game::Game(vector<shared_ptr<Player>> playerList, Rank startLevel)
	: players(playerList), currentLevel(startLevel),scoreFactor(1)	 {}

Game::~Game(){}

Rank Game::getLevel() const
{
	return currentLevel;
}

int Game::getScoreFactor()const
{
	return scoreFactor;
}

void Game::doubleScoreFactor()
{
	scoreFactor *= 2;
}

void Game::dealCards()
{
	// ���ü�������׼����һ�ּ���
	resetCardCounter();
	for (auto& player : players)
	{
		player->addCards(deck->dealCards(26));
	}
}

bool Game::isAWholeGameOver()
{
	for (auto& player : players)
	{
		if (player->getLevel() == ace)
		{
			cout << "��Ϸ������" << player->getTeam() << "�����Ѿ���������߼���" << endl;
			cout << "������ ";
			for (auto& player2 : players)
			{
				if (player2->getTeam() == player->getTeam())
				{
					cout << player2->getName() << "��";
				}
			}
			cout << "!\n";
			return true;
		}
	}
	return false;
}

void Game::tribute()
{
	shared_ptr<Player> firstPlayer = nullptr;
	shared_ptr<Player> fourthPlayer = nullptr;
	for (auto& player : players)
	{
		if (player->getARoundRank() == First)
		{
			firstPlayer = player;
			continue;
		}
		if (player->getARoundRank() == Fourth)
		{
			fourthPlayer = player;
			continue;
		}
	}
	
	vector<Card> fourthCards = fourthPlayer->getHandCards();
	int bigJokerCount = 0;
	Card maxCard = fourthCards[0];
	for (const auto& card : fourthCards)
	{
		if (card.getSuit() == Joker && card.getRank() == big)
		{
			bigJokerCount++;
		}
		if (card > maxCard)
		{
			maxCard = card;
		}
	}

	//����Ҫ���������
	if (bigJokerCount == 2)
	{
		cout << "���Ŵ�������" << fourthPlayer->getName() << "���У�����Ҫ������" << endl;
		return;
	}
	if (maxCard.getRank() <= ace)
	{
		cout << fourthPlayer->getName() << "����û�д��A���ƣ�����Ҫ������" << endl;
		return;
	}

	cout << fourthPlayer->getName() << "��" << firstPlayer->getName()
		<< "������" << maxCard << endl;
	fourthPlayer->removeCard(maxCard);
	firstPlayer->addCard(maxCard);

	firstPlayer->returnTribute(fourthPlayer);
}

void Game::aRoundGame()
{
	srand(static_cast<unsigned int>(time(nullptr)));
   // ���ѡ��һ�������Ϊ��һ��
   int currentPlayerIndex = rand() % players.size();

   deck = make_unique<Deck>(currentLevel);
   deck->shuffle();
   dealCards();

   //�ڷǵ�һ����Ϸʱ������
   if(players[0]->getARoundRank()!=NotRanked)
	   tribute();

   for (auto& player : players)
   {
	   player->setARoundRank(NotRanked); // ������ҵı�������
   }

   for (auto& player : players) 
   {
	   player->setARoundRank(NotRanked);
   }

   lastPlay = nullptr;
   PlayerRank nextPlayerRank = First;
   unsigned int nowPlayers = players.size();
   unsigned int passCount = 0;

   while (nowPlayers>1)
   {
	   //������Ѿ���������,����
	   if (players[currentPlayerIndex]->getARoundRank() != NotRanked)
	   {
		   currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
		   continue;
	   }

	   //���һ�ֹ�ȥû�˳��ƣ����lastplay
	   if (passCount == nowPlayers - 1)
	   {
		   lastPlay = nullptr;
		   passCount = 0;
	   }

	   unique_ptr<CardGroup> bridge = nullptr;

	   bridge = players[currentPlayerIndex]->playCards(move(lastPlay), *this);

		//// ���浱ǰlastPlay�ĸ���
	 //  unique_ptr<CardGroup> lastPlayCopy = lastPlay ? make_unique<CardGroup>(*lastPlay) : nullptr;

	 //  // ��ҳ���
	 //  unique_ptr<CardGroup> bridge = players[currentPlayerIndex]->playCards(move(lastPlayCopy), *this);

	   //// ����һ����ʱ����
	   //unique_ptr<CardGroup>& tempLastPlay = lastPlay;

	   //// ����playCards
	   //bridge = players[currentPlayerIndex]->playCards(move(lastPlay), *this);


	   if (bridge == nullptr)
	   {
		   passCount = passCount + 1;
		   currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
		   continue;
		   //lastPlay = move(tempLastPlay); // �ָ�lastPlay
		   //passCount = passCount + 1;
		   //currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
		   //continue;
	   }
	   else
	   {
			//��bridge���¼�����
		   //updateCardCounter(bridge->getCards());

		   passCount = 0;
		   if (players[currentPlayerIndex]->getHandCards().size() == 0)
		   {
			   players[currentPlayerIndex]->setARoundRank(nextPlayerRank);
			   nextPlayerRank = static_cast<PlayerRank>(nextPlayerRank + 1);
			   currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
			   nowPlayers--;
		   }
		   else
		   {
			   currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
		   }

	   }


   }

   for (auto& player : players)
   {
	   if (player->getARoundRank() == NotRanked)
	   {
		   player->setARoundRank(nextPlayerRank);
	   }
   }

   cout << "������Ϸ������" << endl;
   
   //�������
   Team winningTeam;
   shared_ptr<Player> firstPlayer;
   settleScore();
   for (auto& player : players)
   {
	   if (player->getARoundRank() == First)
	   {
		   winningTeam = player->getTeam();
		   firstPlayer = player;
		   break;
	   }
   }
   for (auto& player : players)
   {
	   if (player->getTeam() == winningTeam && player->getARoundRank() != First)
	   {
		   switch (player->getARoundRank())
		   {
		   case Second:
			   player->levelUp(3);
			   firstPlayer->levelUp(3);
			   break;
		   case Third:
			   player->levelUp(2);
			   firstPlayer->levelUp(2);
			   break;
		   case Fourth:
			   player->levelUp(1);
			   firstPlayer->levelUp(1);
			   break;
		   }
		   break;
	   }
   }
   currentLevel = firstPlayer->getLevel();//���µ�ǰlevel

}

void Game::setLastPlay(unique_ptr<CardGroup> newPlay)
{
	//if (newPlay) {
	//	// ��ȡ�����е������Ʋ����¼�����
	//	updateCardCounter(newPlay->getCards());
	//}

	// ����Ƿ�Ϊ�������ͣ��������������
	if (isSpecialPlay(newPlay.get())) {
		doubleScoreFactor();
		cout << "�������ͣ�������������ǰ����: " << scoreFactor << endl;
	}

	lastPlay = move(newPlay);
}

void Game::resetRoundGame()
{
	for (auto& player : players)
	{
		player->clearHandCards();
	}
	//���deck��lastPlay
	deck.reset();
	lastPlay.reset();
	resetCardCounter(); // ���ü�����
	scoreFactor = 1; //���ñ���Ϊ1
}

void Game::aWholeGame()
{
	bool playAgain = true;

	while (playAgain)
	{
		cout << "\n========== ��ʼһ�������走��Ϸ ==========" << endl;
		cout << "��ʼ����" << currentLevel << endl;

		int roundCount = 1;

		while (!isAWholeGameOver())
		{
			cout << "\n========== ��" << roundCount << "����Ϸ��ʼ ==========" << endl;
			cout << "��ǰ����: " << currentLevel << endl;

			// ��ʾ����ҵ�ǰ����
			for (auto& player : players)
			{
				cout << player->getName() << " ��ǰ����: " << player->getLevel() << endl;
			}

			// ����һ����Ϸ
			aRoundGame();

			// ��ʾ���ֽ��
			cout << "\n========== ��" << roundCount << "����Ϸ��� ==========" << endl;
			for (auto& player : players)
			{
				cout << player->getName() << " ����: " << player->getLevel();

				// ��ʾ��ұ�������
				switch (player->getARoundRank())
				{
				case First:
					cout << " (��һ��)";
					break;
				case Second:
					cout << " (�ڶ���)";
					break;
				case Third:
					cout << " (������)";
					break;
				case Fourth:
					cout << " (������)";
					break;
				default:
					cout << " (δ����)";
					break;
				}
				cout << endl;
			}

			// ��Ӽ�������ѯѡ��
			char isShowCardCounter;
			cout << "\n�Ƿ�鿴������? (y/n): ";
			cin >> isShowCardCounter;
			cin.ignore();

			if (tolower(isShowCardCounter) == 'y')
			{
				showCardCounter();
			}

			// ������Ϸ״̬��׼����һ��
			resetRoundGame();

			// �ж��Ƿ��ж����Ѿ�Ӯ��
			if (isAWholeGameOver())
			{
				break;
			}

			roundCount++;

			// ѯ���Ƿ������һ��
			char continueGame;
			cout << "\n�Ƿ������һ��? (y/n): ";
			cin >> continueGame;
			cin.ignore();

			if (tolower(continueGame) != 'y')
			{
				cout << "��Ϸ��ǰ����!" << endl;
				break;
			}
		}

		// ��ʾ������Ϸ���
		cout << "\n========== ������Ϸ��� ==========" << endl;
		cout << "�������� " << roundCount << " ����Ϸ" << endl;

		// ����ĸ�����Ӯ������Ϸ
		Team winningTeam = Red;
		bool hasWinner = false;

		for (auto& player : players)
		{
			if (player->getLevel() == ace)
			{
				winningTeam = player->getTeam();
				hasWinner = true;
				break;
			}
		}

		if (hasWinner)
		{
			cout << (winningTeam == Red ? "���" : "����") << "���������ʤ����" << endl;
			cout << "��ʤ�����Ա: ";
			for (auto& player : players)
			{
				if (player->getTeam() == winningTeam)
				{
					player->addWin();
					cout << player->getName() << " ";
				}
			}
			cout << endl;
		}
		else
		{
			cout << "��Ϸ��ǰ������û�ж���ﵽ��ʤ������" << endl;
		}

		showLeaderboard();

		char restart;
		cout << "\n�Ƿ�����һ����Ϸ? (y/n): ";
		cin >> restart;
		cin.ignore();

		if (tolower(restart) == 'y')
		{
			// ������Ϸ״̬��׼����һ����Ϸ
			resetRoundGame();
			playAgain = true;
		}
		else
		{
			playAgain = false;
		}
	}

	cout << "========== ��Ϸ�Ự���� ==========" << endl;
}

void Game::resetGame()
{
	// ������Ϸ״̬
	resetRoundGame();
	currentLevel = two;

	// ����ÿ����ҵļ���
	for (auto& player : players)
	{
		player->resetLevel();
	}
}

void Game::showLeaderboard()
{
	cout << "\n========== ������а� ==========" << endl;


	vector<shared_ptr<Player>> sortedPlayers = players;

	sort(sortedPlayers.begin(), sortedPlayers.end(),
		[](const shared_ptr<Player>& a, const shared_ptr<Player>& b) {
			return a->getTotalWins() > b->getTotalWins();
		});

	cout << "����\t���\t\tʤ����" << endl;
	cout << "--------------------------------" << endl;

	for (size_t i = 0; i < sortedPlayers.size(); ++i)
	{
		cout << i + 1 << "\t" << sortedPlayers[i]->getName() << "\t\t"
			<< sortedPlayers[i]->getTotalWins() << endl;
	}

	cout << "================================" << endl;
}

bool Game::isSpecialPlay(const CardGroup* cardGroup)
{
	if (!cardGroup)
		return false;

	// ����Ƿ�Ϊը��
	if (dynamic_cast<const Bomb*>(cardGroup) != nullptr)
		return true;

	// ����Ƿ�Ϊ��ը
	if (dynamic_cast<const TwoJokers*>(cardGroup) != nullptr)
		return true;

	// ����Ƿ�Ϊͬ��˳
	if (dynamic_cast<const Flush*>(cardGroup) != nullptr) 
		return true;

	return false;
}

void Game::settleScore()
{
	// ȷ����ʤ����
	Team winningTeam = Red;  // Ĭ��ֵ
	bool foundWinner = false;

	for (const auto& p : players) 
	{
		if (p -> getARoundRank() == First)
		{ // �ҵ���һ���Ķ���
			winningTeam = p->getTeam();
			foundWinner = true;
			break;
		}
	}

	if (foundWinner/* && scoreFactor > 0*/) {
		cout << "\n�����������ǰ����: " << scoreFactor << endl;

		// ���ݱ����Ͷ�����㲢�������
		for (auto& player : players) {
			if (player->getTeam() == winningTeam) {
				player->addScore(scoreFactor);
				cout << player->getName() << " (��ʤ) +";
			}
			else {
				player->addScore(-scoreFactor);
				cout << player->getName() << " (ʧ��) -";
			}
			cout << scoreFactor << "����ǰ����: " << player->getScore() << endl;
		}
	}
}

// ʵ�ּ��������·���
void Game::updateCardCounter(const vector<Card>& playedCards) {
	cardCounter.markCardsPlayed(playedCards);
}

// ʵ����ʾ����������
void Game::showCardCounter() const {
	cardCounter.displayStatistics();
}

// ʵ�����ü���������
void Game::resetCardCounter() {
	cardCounter.reset();
}