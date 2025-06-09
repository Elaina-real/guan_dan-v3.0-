#include "game.h"
#include "card.h"
#include "cardgroup.h"
#include "player.h"
#include <sstream>

static bool compareCards(const Card& a, const Card& b)//排序的依据
{
	if (a.getRank() == b.getRank())
		return a.getSuit() < b.getSuit();
	return a.getRank() < b.getRank();
}

//Player
string Player::getName() const
{
	return name;
}

Team Player::getTeam() const
{
	return team;
}

Rank Player::getLevel() const
{
	return playerCurrentLevel;
}

vector<Card> Player::getHandCards() const
{
	return handCards;
}

int Player::getScore() const
{
	return score;
}

PlayerRank Player::getARoundRank() const
{
	return aRoundRank;
}

void Player::setARoundRank(PlayerRank rank)
{
	aRoundRank = rank;
}

PlayerRank Player::getGameRank() const
{
	return gameRank;
}

void Player::setGameRank(PlayerRank rank)
{
	gameRank = rank;
}

void Player::addScore(int s)
{
	score = s + score;
}

void Player::levelUp(int n)
{
	if (playerCurrentLevel + n >= ace)
		playerCurrentLevel = ace;
	else
		playerCurrentLevel = static_cast<Rank>(playerCurrentLevel + n);
}

void Player::addCard(const Card& card)
{
	handCards.push_back(card);
	sortHandCards();
}

void Player::addCards(const vector<Card>& cards)
{
	handCards.insert(handCards.end(), cards.begin(), cards.end());
	sortHandCards();
}

void Player::removeCard(const Card& card)
{
	auto it = std::find(handCards.begin(), handCards.end(), card);

	if (it != handCards.end())//仅仅在找到的情况下删除
		handCards.erase(it);

	sortHandCards();
}

void Player::removeCards(const vector<Card>& cards)
{
	for (const auto& card : cards)
	{
		removeCard(card);
	}
	sortHandCards();
}

void Player::sortHandCards()
{
	sort(handCards.begin(), handCards.end(), compareCards);
}

void Player::showHand() const
{
	cout  << name << "的手牌: ";
	for (const auto& card : handCards)
	{
		cout << card << " ";
	}
	cout << endl;
}


int Player::getTotalWins() const
{
	return totalWins;
}

void Player::addWin()
{
	totalWins++;
}

void Player::resetLevel()
{
	playerCurrentLevel = two;
	aRoundRank = NotRanked;
}


//HumanPlayer
unique_ptr<CardGroup> HumanPlayer::playCards(const unique_ptr<CardGroup>& lastPlay, Game& game)
{
	cout << "\n======" << name << "的回合======" << endl;
	while (true)
	{
		//显示上家出牌(如果有
		if (lastPlay != nullptr)
		{
			cout << "上家出牌:";
			for (const auto& card : lastPlay->getCards())
			{
				cout << card << " ";
			}
			cout << endl;
		}

		//展示手牌
		showHand();

		// 玩家可以选择"过"
		cout << "可输入'c'查看记牌器,请选择要出的牌（输入牌的编号，从1开始，用空格分隔，输入0表示不出）: ";
		string input;
		getline(cin, input);

		if (input == "c") 
		{
			// 显示记牌器
			game.showCardCounter();
			// 递归调用自身，让玩家继续选择出牌
			return playCards(lastPlay, game);
		}

		if (lastPlay == nullptr && input == "0")
		{
			cout << "您必须出牌\n" << endl;
			continue;//重新输入
		}

		//检查是否选择过
		if (input == "0" || input.empty())
		{
			cout << name << "选择过" << endl << endl;
			return nullptr;//不出牌
		}


		vector<int> selectedNum;
		istringstream iss(input);
		int index;
		while (iss >> index)
		{
			if (index > 0 && index <= handCards.size())
			{
				selectedNum.push_back(index - 1);
			}
			else
			{
				cout << "无效的输入:" << index << endl << endl;
				continue;//重新输入
			}
		}

		//检查是否选择了重复的牌
		sort(selectedNum.begin(), selectedNum.end(), greater<int>());
		if (adjacent_find(selectedNum.begin(), selectedNum.end()) != selectedNum.end())
		{
			cout << "请不要重复选择\n" << endl;
			continue;//重新输入
		}

		vector<Card> selectedCards;
		for (int idx : selectedNum)
		{
			selectedCards.push_back(handCards[idx]);
		}

		//检查是否选择了癞子牌
		int thisWildCardCount = 0;
		vector<size_t> wildCardIndex; // 记录癞子牌的位置
		for (size_t i = 0; i < selectedCards.size(); i++)
		{
			if (selectedCards[i].isWildCard())
			{
				thisWildCardCount++;
				wildCardIndex.push_back(i);
			}
		}

		unique_ptr<CardGroup> cardGroup = nullptr;

		if (thisWildCardCount == 0)//没有癞子牌则正常比较
		{
			cardGroup = CardGroupFactory::createCardGroup(selectedCards, game);

			if (cardGroup == nullptr)
			{
				cout << "无效的牌型，请重新选择\n" << endl;
				continue;//重新输入
			}
			
			if (lastPlay == nullptr) // 如果是第一个出牌
			{
				removeCards(selectedCards);
				cout << name << "出牌: ";
				for (const auto& card : selectedCards)
				{
					cout << card << " ";
				}
				cout << endl << endl;

				game.updateCardCounter(cardGroup->getCards());
				auto cardGroupCopy = CardGroupFactory::createCardGroup(selectedCards, game);
				// 直接使用setLastPlay更新游戏状态
				game.setLastPlay(move(cardGroup));
				return cardGroupCopy;
			}
			else if (*cardGroup > *lastPlay) // 如果可以压制上家
			{
				removeCards(selectedCards);
				cout << name << "出牌: ";
				for (const auto& card : selectedCards)
				{
					cout << card << " ";
				}
				cout << endl << endl;
				game.updateCardCounter(cardGroup->getCards());
				auto cardGroupCopy = CardGroupFactory::createCardGroup(selectedCards, game);
				// 直接使用setLastPlay更新游戏状态
				game.setLastPlay(move(cardGroup));
				return cardGroupCopy;
			}
			else
			{
				cout << "出牌失败，无效牌型或不能压制上家的牌\n" << endl;
				continue;//重新输入
			}
			//if (lastPlay == nullptr || *cardGroup > *lastPlay)
			//{
			//	removeCards(selectedCards);
			//	cout << name << "出牌: ";
			//	for (const auto& card : selectedCards)
			//	{
			//		cout << card << " ";
			//	}
			//	cout << endl << endl;
			//	return cardGroup;
			//}
			//else
			//{
			//	cout << "出牌失败，无效牌型或不能压制上家的牌\n" << endl;
			//	continue;//重新输入
			//}
		}
		else//有的话特殊处理
		{
			cardGroup = make_unique<Single>(selectedCards, game);//随意创建一个子类的对象用以进入比较逻辑

			if (lastPlay == nullptr) // 如果是第一个出牌
			{
				vector<Card> tempCards = cardGroup->getCards();
				unique_ptr<CardGroup> newCardGroup = CardGroupFactory::createCardGroup(tempCards, game);
				removeCards(selectedCards);
				cout << name << "出牌: ";
				for (const auto& card : selectedCards)
				{
					cout << card << " ";
				}
				cout << endl << endl;

				game.updateCardCounter(cardGroup->getCards());
				auto cardGroupCopy = CardGroupFactory::createCardGroup(selectedCards, game);
				// 直接使用setLastPlay更新游戏状态
				game.setLastPlay(move(cardGroupCopy));
				return cardGroup;
			}
			else if (*cardGroup > *lastPlay) // 如果可以压制上家
			{
				vector<Card> tempCards = cardGroup->getCards();
				unique_ptr<CardGroup> newCardGroup = CardGroupFactory::createCardGroup(tempCards, game);
				removeCards(selectedCards);
				cout << name << "出牌: ";
				for (const auto& card : selectedCards)
				{
					cout << card << " ";
				}
				cout << endl << endl;
				return newCardGroup;
			}
			else
			{
				cout << "出牌失败，无效牌型或不能压制上家的牌\n" << endl;
				continue;//重新输入
			}

			//if (lastPlay == nullptr || *cardGroup > *lastPlay)
			//{
			//	vector <Card> tempCards = cardGroup->getCards();
			//	unique_ptr<CardGroup> newCardGroup = CardGroupFactory::createCardGroup(tempCards, game);
			//	removeCards(selectedCards);
			//	cout << name << "出牌: ";
			//	for (const auto& card : selectedCards)
			//	{
			//		cout << card << " ";
			//	}
			//	cout << endl << endl;
			//	return newCardGroup;
			//}
			//else
			//{
			//	cout << "出牌失败，无效牌型或不能压制上家的牌\n" << endl;
			//	continue;//重新输入
			//}
		}
		//unique_ptr<CardGroup> cardGroup = Card(selectedCards, game);

		//unique_ptr<CardGroup> cardGroup = nullptr;


		//if (cardGroup == nullptr)
		//{
		//	cout << "无效的牌型，请重新选择\n" << endl;
		//	continue;//重新输入
		//}
		//else
		//if (lastPlay == nullptr || *cardGroup > *lastPlay)
		//{
		//	removeCards(selectedCards);
		//	cout << name << "出牌: ";
		//	for (const auto& card : selectedCards)
		//	{
		//		cout << card << " ";
		//	}
		//	cout << endl << endl;
		//	return cardGroup;
		//}
		//else
		//{
		//	cout << "出牌失败，无效牌型或不能压制上家的牌\n" << endl;
		//	continue;//重新输入
		//}
	}
}

void HumanPlayer::returnTribute(const shared_ptr<Player>& tributer)
{
	vector<Card> eligibleCards;
	for (const auto& card : handCards)
	{
		if (card.getRank() < ten)
		{
			eligibleCards.push_back(card);
		}
	}

	if (eligibleCards.empty())
	{
		cout << name << "没有可以让出的牌" << endl;
		return;
	}

	cout << name << "的可让出牌: ";
	for (size_t i = 0; i < eligibleCards.size(); ++i)
	{
		cout << i + 1 << ": " << eligibleCards[i] << " ";
	}

	unsigned int index;
	while (1)
	{
		cout << "\n\n" << name << "请选择要让出的一张牌（输入牌的编号，从1开始）: ";
		cin >> index;
		if (index >= 1 && index <= eligibleCards.size())
			break;
		cout << "无效的输入，请重新选择" << endl;
	}

	Card reutrnCard = eligibleCards[index - 1];
	cout << name << "还贡" << reutrnCard << endl;
	removeCard(reutrnCard);
	tributer->addCard(reutrnCard);
	return;
}

//AIPlayer
unique_ptr<CardGroup> AIPlayer::playCards(const unique_ptr<CardGroup>& lastPlay, Game& game)
{
	cout << "\n======" << name << "的回合======" << endl;

	// 展示手牌
	showHand();

	// 显示上家出牌(如果有)
	if (lastPlay != nullptr) {
		cout << "上家出牌: ";
		for (const auto& card : lastPlay->getCards()) {
			cout << card << " ";
		}
		cout << endl;
	}

	// 如果是首家出牌（没有上家牌）或者已经没有玩家能出牌
	if (lastPlay == nullptr) {
		// 简单策略：出手中最小的一张牌或对子
		if (handCards.size() >= 2 && handCards[0].getRank() == handCards[1].getRank())
		{
			vector<Card> selectedCards = { handCards[0], handCards[1] };
			auto cardGroup = CardGroupFactory::createCardGroup(selectedCards, game);
			if (cardGroup)
			{
				removeCards(selectedCards);
				cout << name << "出牌: ";
				for (const auto& card : selectedCards)
				{
					cout << card << " ";
				}
				cout << endl;

				game.updateCardCounter(selectedCards);
				auto cardGroupCopy = CardGroupFactory::createCardGroup(selectedCards, game);
				// 直接使用setLastPlay更新游戏状态
				game.setLastPlay(move(cardGroup));
				return cardGroupCopy;
			}
		}

		// 出单张
		vector<Card> selectedCards = { handCards[0] };
		auto cardGroup = CardGroupFactory::createCardGroup(selectedCards, game);
		removeCards(selectedCards);
		cout << name << "出牌: " << selectedCards[0] << endl;

		game.updateCardCounter(selectedCards);
		auto cardGroupCopy = CardGroupFactory::createCardGroup(selectedCards, game);
		// 直接使用setLastPlay更新游戏状态
		game.setLastPlay(move(cardGroup));
		return cardGroupCopy;
	}
	else {
		// 需要压制上家的牌
		const vector<Card>& lastCards = lastPlay->getCards();
		int lastCardsSize = lastCards.size();

		// 从手牌中尝试找到能压制上家牌的组合
		for (size_t i = 0; i < handCards.size() - lastCardsSize + 1; i++)
		{
			vector<Card> selectedCards;
			for (int j = 0; j < lastCardsSize; j++)
			{
				if (i + j < handCards.size())
				{
					selectedCards.push_back(handCards[i + j]);
				}
			}

			auto cardGroup = CardGroupFactory::createCardGroup(selectedCards, game);
			if (cardGroup && (cardGroup->canBeat(*lastPlay))) {
				removeCards(selectedCards);
				cout << name << "出牌: ";
				for (const auto& card : selectedCards) {
					cout << card << " ";
				}
				cout << endl;
				game.updateCardCounter(selectedCards);
				auto cardGroupCopy = CardGroupFactory::createCardGroup(selectedCards, game);
				// 直接使用setLastPlay更新游戏状态
				game.setLastPlay(move(cardGroup));
				return cardGroupCopy;
			}
		}

		// 找不到能压制的牌，选择过牌
		cout << name << "选择过" << endl;
		return nullptr;
	}
}

void AIPlayer::returnTribute(const shared_ptr<Player>& tributer)
{
	// 找出所有符合条件的牌（点数小于10）
	vector<Card> eligibleCards;
	for (const auto& card : handCards)
	{
		if (card.getRank() < ten)
		{
			eligibleCards.push_back(card);
		}
	}

	// 如果没有符合条件的牌，则不能还贡
	if (eligibleCards.empty())
	{
		cout << name << "没有可以让出的牌" << endl;
		return;
	}

	// 简单AI策略：选择最小的牌还贡
	Card returnCard = eligibleCards[0];

	cout << name << "还贡：" << returnCard << endl;

	removeCard(returnCard);
	tributer->addCard(returnCard);
	return;
}

void Player::clearHandCards()
{
	handCards.clear();
}