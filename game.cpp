#include "game.h"
#include "player.h" // 如果需要Player完整定义


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
	// 重置记牌器，准备新一轮计数
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
			cout << "游戏结束！" << player->getTeam() << "队伍已经到达了最高级别！" << endl;
			cout << "他们是 ";
			for (auto& player2 : players)
			{
				if (player2->getTeam() == player->getTeam())
				{
					cout << player2->getName() << "、";
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

	//不需要进贡的情况
	if (bigJokerCount == 2)
	{
		cout << "两张大王都在" << fourthPlayer->getName() << "手中，不需要进贡。" << endl;
		return;
	}
	if (maxCard.getRank() <= ace)
	{
		cout << fourthPlayer->getName() << "手中没有大过A的牌，不需要进贡。" << endl;
		return;
	}

	cout << fourthPlayer->getName() << "向" << firstPlayer->getName()
		<< "进贡：" << maxCard << endl;
	fourthPlayer->removeCard(maxCard);
	firstPlayer->addCard(maxCard);

	firstPlayer->returnTribute(fourthPlayer);
}

void Game::aRoundGame()
{
	srand(static_cast<unsigned int>(time(nullptr)));
   // 随机选择一个玩家作为第一家
   int currentPlayerIndex = rand() % players.size();

   deck = make_unique<Deck>(currentLevel);
   deck->shuffle();
   dealCards();

   //在非第一局游戏时，进贡
   if(players[0]->getARoundRank()!=NotRanked)
	   tribute();

   for (auto& player : players)
   {
	   player->setARoundRank(NotRanked); // 重置玩家的本局排名
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
	   //如果是已经打完的玩家,跳过
	   if (players[currentPlayerIndex]->getARoundRank() != NotRanked)
	   {
		   currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
		   continue;
	   }

	   //如果一轮过去没人出牌，清空lastplay
	   if (passCount == nowPlayers - 1)
	   {
		   lastPlay = nullptr;
		   passCount = 0;
	   }

	   unique_ptr<CardGroup> bridge = nullptr;

	   bridge = players[currentPlayerIndex]->playCards(move(lastPlay), *this);

		//// 保存当前lastPlay的副本
	 //  unique_ptr<CardGroup> lastPlayCopy = lastPlay ? make_unique<CardGroup>(*lastPlay) : nullptr;

	 //  // 玩家出牌
	 //  unique_ptr<CardGroup> bridge = players[currentPlayerIndex]->playCards(move(lastPlayCopy), *this);

	   //// 创建一个临时引用
	   //unique_ptr<CardGroup>& tempLastPlay = lastPlay;

	   //// 调用playCards
	   //bridge = players[currentPlayerIndex]->playCards(move(lastPlay), *this);


	   if (bridge == nullptr)
	   {
		   passCount = passCount + 1;
		   currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
		   continue;
		   //lastPlay = move(tempLastPlay); // 恢复lastPlay
		   //passCount = passCount + 1;
		   //currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
		   //continue;
	   }
	   else
	   {
			//用bridge更新记牌器
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

   cout << "本局游戏结束！" << endl;
   
   //赛后结算
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
   currentLevel = firstPlayer->getLevel();//更新当前level

}

void Game::setLastPlay(unique_ptr<CardGroup> newPlay)
{
	//if (newPlay) {
	//	// 获取牌组中的所有牌并更新记牌器
	//	updateCardCounter(newPlay->getCards());
	//}

	// 检查是否为特殊牌型，如果是则倍数翻倍
	if (isSpecialPlay(newPlay.get())) {
		doubleScoreFactor();
		cout << "特殊牌型！倍数翻倍，当前倍数: " << scoreFactor << endl;
	}

	lastPlay = move(newPlay);
}

void Game::resetRoundGame()
{
	for (auto& player : players)
	{
		player->clearHandCards();
	}
	//清空deck和lastPlay
	deck.reset();
	lastPlay.reset();
	resetCardCounter(); // 重置记牌器
	scoreFactor = 1; //重置倍数为1
}

void Game::aWholeGame()
{
	bool playAgain = true;

	while (playAgain)
	{
		cout << "\n========== 开始一场完整掼蛋游戏 ==========" << endl;
		cout << "起始级别：" << currentLevel << endl;

		int roundCount = 1;

		while (!isAWholeGameOver())
		{
			cout << "\n========== 第" << roundCount << "局游戏开始 ==========" << endl;
			cout << "当前级别: " << currentLevel << endl;

			// 显示各玩家当前级别
			for (auto& player : players)
			{
				cout << player->getName() << " 当前级别: " << player->getLevel() << endl;
			}

			// 进行一局游戏
			aRoundGame();

			// 显示本局结果
			cout << "\n========== 第" << roundCount << "局游戏结果 ==========" << endl;
			for (auto& player : players)
			{
				cout << player->getName() << " 级别: " << player->getLevel();

				// 显示玩家本局排名
				switch (player->getARoundRank())
				{
				case First:
					cout << " (第一名)";
					break;
				case Second:
					cout << " (第二名)";
					break;
				case Third:
					cout << " (第三名)";
					break;
				case Fourth:
					cout << " (第四名)";
					break;
				default:
					cout << " (未排名)";
					break;
				}
				cout << endl;
			}

			// 添加记牌器查询选项
			char isShowCardCounter;
			cout << "\n是否查看记牌器? (y/n): ";
			cin >> isShowCardCounter;
			cin.ignore();

			if (tolower(isShowCardCounter) == 'y')
			{
				showCardCounter();
			}

			// 重置游戏状态，准备下一局
			resetRoundGame();

			// 判断是否有队伍已经赢了
			if (isAWholeGameOver())
			{
				break;
			}

			roundCount++;

			// 询问是否继续下一局
			char continueGame;
			cout << "\n是否继续下一局? (y/n): ";
			cin >> continueGame;
			cin.ignore();

			if (tolower(continueGame) != 'y')
			{
				cout << "游戏提前结束!" << endl;
				break;
			}
		}

		// 显示整场游戏结果
		cout << "\n========== 整场游戏结果 ==========" << endl;
		cout << "共进行了 " << roundCount << " 局游戏" << endl;

		// 检查哪个队伍赢得了游戏
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
			cout << (winningTeam == Red ? "红队" : "蓝队") << "获得了最终胜利！" << endl;
			cout << "获胜队伍成员: ";
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
			cout << "游戏提前结束，没有队伍达到获胜条件。" << endl;
		}

		showLeaderboard();

		char restart;
		cout << "\n是否再来一场游戏? (y/n): ";
		cin >> restart;
		cin.ignore();

		if (tolower(restart) == 'y')
		{
			// 重置游戏状态以准备新一场游戏
			resetRoundGame();
			playAgain = true;
		}
		else
		{
			playAgain = false;
		}
	}

	cout << "========== 游戏会话结束 ==========" << endl;
}

void Game::resetGame()
{
	// 重置游戏状态
	resetRoundGame();
	currentLevel = two;

	// 重置每个玩家的级别
	for (auto& player : players)
	{
		player->resetLevel();
	}
}

void Game::showLeaderboard()
{
	cout << "\n========== 玩家排行榜 ==========" << endl;


	vector<shared_ptr<Player>> sortedPlayers = players;

	sort(sortedPlayers.begin(), sortedPlayers.end(),
		[](const shared_ptr<Player>& a, const shared_ptr<Player>& b) {
			return a->getTotalWins() > b->getTotalWins();
		});

	cout << "排名\t玩家\t\t胜场数" << endl;
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

	// 检查是否为炸弹
	if (dynamic_cast<const Bomb*>(cardGroup) != nullptr)
		return true;

	// 检查是否为王炸
	if (dynamic_cast<const TwoJokers*>(cardGroup) != nullptr)
		return true;

	// 检查是否为同花顺
	if (dynamic_cast<const Flush*>(cardGroup) != nullptr) 
		return true;

	return false;
}

void Game::settleScore()
{
	// 确定获胜队伍
	Team winningTeam = Red;  // 默认值
	bool foundWinner = false;

	for (const auto& p : players) 
	{
		if (p -> getARoundRank() == First)
		{ // 找到第一名的队伍
			winningTeam = p->getTeam();
			foundWinner = true;
			break;
		}
	}

	if (foundWinner/* && scoreFactor > 0*/) {
		cout << "\n结算分数，当前倍数: " << scoreFactor << endl;

		// 根据倍数和队伍计算并结算分数
		for (auto& player : players) {
			if (player->getTeam() == winningTeam) {
				player->addScore(scoreFactor);
				cout << player->getName() << " (获胜) +";
			}
			else {
				player->addScore(-scoreFactor);
				cout << player->getName() << " (失败) -";
			}
			cout << scoreFactor << "，当前分数: " << player->getScore() << endl;
		}
	}
}

// 实现记牌器更新方法
void Game::updateCardCounter(const vector<Card>& playedCards) {
	cardCounter.markCardsPlayed(playedCards);
}

// 实现显示记牌器方法
void Game::showCardCounter() const {
	cardCounter.displayStatistics();
}

// 实现重置记牌器方法
void Game::resetCardCounter() {
	cardCounter.reset();
}