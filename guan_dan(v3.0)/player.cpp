#include "game.h"
#include "card.h"
#include "cardgroup.h"
#include "player.h"
#include <sstream>
#include <conio.h>
#include <graphics.h>

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

vector<Card>& Player::getHandCards()
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
//unique_ptr<CardGroup> HumanPlayer::playCards(const unique_ptr<CardGroup>& lastPlay, Game& game)
//{
//	cout << "\n======" << name << "的回合======" << endl;
//	while (true)
//	{
//		//显示上家出牌(如果有
//		if (lastPlay != nullptr)
//		{
//			cout << "上家出牌:";
//			for (const auto& card : lastPlay->getCards())
//			{
//				cout << card << " ";
//			}
//			cout << endl;
//		}
//
//		//展示手牌
//		showHand();
//
//		// 玩家可以选择"过"
//		cout << "可输入'c'查看记牌器,请选择要出的牌（输入牌的编号，从1开始，用空格分隔，输入0表示不出）: ";
//		string input;
//		getline(cin, input);
//
//		if (input == "c") 
//		{
//			// 显示记牌器
//			//game.showCardCounter();
//			game.showCardCounterExcludePlayer(handCards);
//			// 递归调用自身，让玩家继续选择出牌
//			return playCards(lastPlay, game);
//		}
//
//		if (lastPlay == nullptr && input == "0")
//		{
//			cout << "您必须出牌\n" << endl;
//			continue;//重新输入
//		}
//
//		//检查是否选择过
//		if (input == "0" || input.empty())
//		{
//			cout << name << "选择过" << endl << endl;
//			return nullptr;//不出牌
//		}
//
//
//		vector<int> selectedNum;
//		istringstream iss(input);
//		int index;
//		while (iss >> index)
//		{
//			if (index > 0 && index <= handCards.size())
//			{
//				selectedNum.push_back(index - 1);
//			}
//			else
//			{
//				cout << "无效的输入:" << index << endl << endl;
//				continue;//重新输入
//			}
//		}
//
//		//检查是否选择了重复的牌
//		sort(selectedNum.begin(), selectedNum.end(), greater<int>());
//		if (adjacent_find(selectedNum.begin(), selectedNum.end()) != selectedNum.end())
//		{
//			cout << "请不要重复选择\n" << endl;
//			continue;//重新输入
//		}
//
//		vector<Card> selectedCards;
//		for (int idx : selectedNum)
//		{
//			selectedCards.push_back(handCards[idx]);
//		}
//
//		//检查是否选择了癞子牌
//		int thisWildCardCount = 0;
//		vector<size_t> wildCardIndex; // 记录癞子牌的位置
//		for (size_t i = 0; i < selectedCards.size(); i++)
//		{
//			if (selectedCards[i].isWildCard())
//			{
//				thisWildCardCount++;
//				wildCardIndex.push_back(i);
//			}
//		}
//
//		unique_ptr<CardGroup> cardGroup = nullptr;
//
//		if (thisWildCardCount == 0)//没有癞子牌则正常比较
//		{
//			cardGroup = CardGroupFactory::createCardGroup(selectedCards, game);
//
//			if (cardGroup == nullptr)
//			{
//				cout << "无效的牌型，请重新选择\n" << endl;
//				continue;//重新输入
//			}
//			
//			if (lastPlay == nullptr) // 如果是第一个出牌
//			{
//				removeCards(selectedCards);
//				cout << name << "出牌: ";
//				for (const auto& card : selectedCards)
//				{
//					cout << card << " ";
//				}
//				cout << endl << endl;
//
//				game.updateCardCounter(cardGroup->getCards());
//				auto cardGroupCopy = CardGroupFactory::createCardGroup(selectedCards, game);
//				// 直接使用setLastPlay更新游戏状态
//				game.setLastPlay(move(cardGroup));
//				return cardGroupCopy;
//			}
//			else if (*cardGroup > *lastPlay) // 如果可以压制上家
//			{
//				removeCards(selectedCards);
//				cout << name << "出牌: ";
//				for (const auto& card : selectedCards)
//				{
//					cout << card << " ";
//				}
//				cout << endl << endl;
//				game.updateCardCounter(cardGroup->getCards());
//				auto cardGroupCopy = CardGroupFactory::createCardGroup(selectedCards, game);
//				// 直接使用setLastPlay更新游戏状态
//				game.setLastPlay(move(cardGroup));
//				return cardGroupCopy;
//			}
//			else
//			{
//				cout << "出牌失败，无效牌型或不能压制上家的牌\n" << endl;
//				continue;//重新输入
//			}
//			//if (lastPlay == nullptr || *cardGroup > *lastPlay)
//			//{
//			//	removeCards(selectedCards);
//			//	cout << name << "出牌: ";
//			//	for (const auto& card : selectedCards)
//			//	{
//			//		cout << card << " ";
//			//	}
//			//	cout << endl << endl;
//			//	return cardGroup;
//			//}
//			//else
//			//{
//			//	cout << "出牌失败，无效牌型或不能压制上家的牌\n" << endl;
//			//	continue;//重新输入
//			//}
//		}
//		else//有的话特殊处理
//		{
//			cardGroup = make_unique<Single>(selectedCards, game);//随意创建一个子类的对象用以进入比较逻辑
//
//			if (lastPlay == nullptr) // 如果是第一个出牌
//			{
//				vector<Card> tempCards = cardGroup->getCards();
//				unique_ptr<CardGroup> newCardGroup = CardGroupFactory::createCardGroup(tempCards, game);
//				removeCards(selectedCards);
//				cout << name << "出牌: ";
//				for (const auto& card : selectedCards)
//				{
//					cout << card << " ";
//				}
//				cout << endl << endl;
//
//				game.updateCardCounter(cardGroup->getCards());
//				auto cardGroupCopy = CardGroupFactory::createCardGroup(selectedCards, game);
//				// 直接使用setLastPlay更新游戏状态
//				game.setLastPlay(move(cardGroupCopy));
//				return cardGroup;
//			}
//			else if (*cardGroup > *lastPlay) // 如果可以压制上家
//			{
//				vector<Card> tempCards = cardGroup->getCards();
//				unique_ptr<CardGroup> newCardGroup = CardGroupFactory::createCardGroup(tempCards, game);
//				removeCards(selectedCards);
//				cout << name << "出牌: ";
//				for (const auto& card : selectedCards)
//				{
//					cout << card << " ";
//				}
//				cout << endl << endl;
//				return newCardGroup;
//			}
//			else
//			{
//				cout << "出牌失败，无效牌型或不能压制上家的牌\n" << endl;
//				continue;//重新输入
//			}
//
//			//if (lastPlay == nullptr || *cardGroup > *lastPlay)
//			//{
//			//	vector <Card> tempCards = cardGroup->getCards();
//			//	unique_ptr<CardGroup> newCardGroup = CardGroupFactory::createCardGroup(tempCards, game);
//			//	removeCards(selectedCards);
//			//	cout << name << "出牌: ";
//			//	for (const auto& card : selectedCards)
//			//	{
//			//		cout << card << " ";
//			//	}
//			//	cout << endl << endl;
//			//	return newCardGroup;
//			//}
//			//else
//			//{
//			//	cout << "出牌失败，无效牌型或不能压制上家的牌\n" << endl;
//			//	continue;//重新输入
//			//}
//		}
//		//unique_ptr<CardGroup> cardGroup = Card(selectedCards, game);
//
//		//unique_ptr<CardGroup> cardGroup = nullptr;
//
//
//		//if (cardGroup == nullptr)
//		//{
//		//	cout << "无效的牌型，请重新选择\n" << endl;
//		//	continue;//重新输入
//		//}
//		//else
//		//if (lastPlay == nullptr || *cardGroup > *lastPlay)
//		//{
//		//	removeCards(selectedCards);
//		//	cout << name << "出牌: ";
//		//	for (const auto& card : selectedCards)
//		//	{
//		//		cout << card << " ";
//		//	}
//		//	cout << endl << endl;
//		//	return cardGroup;
//		//}
//		//else
//		//{
//		//	cout << "出牌失败，无效牌型或不能压制上家的牌\n" << endl;
//		//	continue;//重新输入
//		//}
//	}
//}

unique_ptr<CardGroup> HumanPlayer::playCards(const unique_ptr<CardGroup>& lastPlay, Game& game)
{
	cout << "\n======" << name << "的回合======" << endl;

	graphicalPtr->clearLastPlay(0); // 清除上家出牌的显示

	bool isSkip = false;
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

		graphicalPtr->drawUI(0);


		vector<Card> selectedCards0;

		//展示手牌
		showHand();

		graphicalPtr->handleUserInput(selectedCards0, isSkip);


		if (isSkip) // 如果选择了不出牌
		{
			cout << name << "选择过" << endl << endl;
			return nullptr; // 不出牌
		}

		vector<Card> selectedCards;
		for (auto& card : handCards)
		{
			if (card.isSelectedCard())
			{
				selectedCards.push_back(card);
				card.setSelected(false);
			}
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

				graphicalPtr->updateLastPlay(0, selectedCards);
				graphicalPtr->drawUI(0);

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

				graphicalPtr->updateLastPlay(0, selectedCards0);
				graphicalPtr->drawUI(0);

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

				graphicalPtr->updateLastPlay(0, newCardGroup->getCards());
				graphicalPtr->drawUI(0);

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

				graphicalPtr->updateLastPlay(0, newCardGroup->getCards());
				graphicalPtr->drawUI(0);

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
		//		cout << card << " ";graphicalPtr->
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

	// 使用图形界面让玩家选择还贡的牌
	Card returnCard = graphicalPtr->handleReturnTributeSelection(shared_from_this());

	// 获取玩家在玩家数组中的索引
	int fromPlayerIndex = -1;
	int toPlayerIndex = -1;
	for (size_t i = 0; i < gamePtr->getPlayers().size(); i++) {
		if (gamePtr->getPlayers()[i]->getName() == name) {
			fromPlayerIndex = static_cast<int>(i);
		}
		if (gamePtr->getPlayers()[i]->getName() == tributer->getName()) {
			toPlayerIndex = static_cast<int>(i);
		}
	}

	// 执行还贡动画
	graphicalPtr->animateCardTransfer(fromPlayerIndex, toPlayerIndex, returnCard, false);

	// 显示还贡界面
	graphicalPtr->drawReturnTributeInterface(shared_from_this(), tributer, returnCard);

	// 处理卡牌转移
	removeCard(returnCard);
	tributer->addCard(returnCard);

	/*cout << name << "的可让出牌: ";
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
	return;*/
}

unique_ptr<CardGroup> AIPlayer::playCards(const unique_ptr<CardGroup>& lastPlay, Game& game)
{
    cout << "\n======" << name << "的回合======" << endl;

    // 获取自己在玩家数组中的索引
    int index = -1;
    for (size_t i = 0; i < game.getPlayers().size(); ++i) {
        if (game.getPlayers()[i]->getName() == name) {
            index = static_cast<int>(i);
            break;
        }
    }

    graphicalPtr->clearLastPlay(index);

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

    graphicalPtr->drawUI(index);
    Sleep(300);

    // 分析手牌情况
    unordered_map<Rank, int> rankCount;
    for (const auto& card : handCards) {
        rankCount[card.getRank()]++;
    }

    // 找出所有的炸弹
    vector<vector<Card>> bombs;
    for (auto& pair : rankCount) {
        if (pair.second >= 4) {
            vector<Card> bomb;
            for (const auto& card : handCards) {
                if (card.getRank() == pair.first && bomb.size() < 4) {
                    bomb.push_back(card);
                }
            }
            if (bomb.size() == 4) bombs.push_back(bomb);
        }
    }

    // 找出所有的三张
    vector<vector<Card>> triples;
    for (auto& pair : rankCount) {
        if (pair.second >= 3) {
            vector<Card> triple;
            for (const auto& card : handCards) {
                if (card.getRank() == pair.first && triple.size() < 3) {
                    triple.push_back(card);
                }
            }
            triples.push_back(triple);
        }
    }

    // 找出所有的对子
    vector<vector<Card>> pairs;
    for (auto& pair : rankCount) {
        if (pair.second >= 2) {
            vector<Card> pairCards;
            for (const auto& card : handCards) {
                if (card.getRank() == pair.first && pairCards.size() < 2) {
                    pairCards.push_back(card);
                }
            }
            pairs.push_back(pairCards);
        }
    }

    // 计算每位玩家的剩余牌数
    vector<int> remainingCardCounts;
    vector<shared_ptr<Player>> allPlayers = game.getPlayers();
    for (const auto& player : allPlayers) {
        remainingCardCounts.push_back(player->getHandCards().size());
    }

    // 检查队友的手牌情况
    bool teammateAlmostDone = false;
    int teammateMinCards = INT_MAX;


    int teammateIndex = (index + 2) % 4; // 队友的索引

    if (allPlayers[teammateIndex]->getHandCards().size() <= 3) {
        teammateAlmostDone = true;
    }
    teammateMinCards = allPlayers[teammateIndex]->getHandCards().size();

    // 使用记牌器分析场上情况
    // 分析某种点数的剩余牌数情况
    vector<pair<Rank, int>> rankDanger; // 存储每种点数的剩余数量

    for (int r = two; r <= ace; r++) {
        Rank rank = static_cast<Rank>(r);
        // 获取某点数在其他玩家手中的剩余数量
        int remainingCount = 0;

		// 减去自己手中的数量
        int myCount = 0;
        for (const auto& card : handCards) {
            if (card.getRank() == rank) {
                myCount++;
            }
        }

        // 计算这个点数在其他玩家手中的数量
        remainingCount = 8 - myCount;
        for (const auto& card : handCards) {
            if (card.getRank() == rank) {
                remainingCount--;
            }
        }

        rankDanger.push_back({ rank, remainingCount });
    }

    // 按剩余牌数排序，剩余最少的排在前面
    sort(rankDanger.begin(), rankDanger.end(),
        [](const pair<Rank, int>& a, const pair<Rank, int>& b) {
            return a.second < b.second;
        });

    // 如果是首家出牌（没有上家牌）
    if (lastPlay == nullptr) {
        // 1: 如果只剩1-2张牌，直接出完
        if (handCards.size() <= 2) {
            vector<Card> selectedCards = handCards;
            auto cardGroup = CardGroupFactory::createCardGroup(selectedCards, game);
            if (cardGroup) {
                removeCards(selectedCards);
                cout << name << "出牌: ";
                for (const auto& card : selectedCards) {
                    cout << card << " ";
                }
                cout << endl;

                game.updateCardCounter(selectedCards);
                graphicalPtr->updateLastPlay(index, selectedCards);
                graphicalPtr->drawUI(index);
                Sleep(300);

                auto cardGroupCopy = CardGroupFactory::createCardGroup(selectedCards, game);
                game.setLastPlay(move(cardGroup));
                return cardGroupCopy;
            }
        }

        // 2: 优先出对子，尤其是小对子
        if (!pairs.empty()) {
            // 按点数排序
            sort(pairs.begin(), pairs.end(), [&rankDanger](const vector<Card>& a, const vector<Card>& b) {
                // 按剩余数量和点数大小综合排序
                // 优先选择其他玩家手中最少的点数
                int aIndex = -1, bIndex = -1;
                for (size_t i = 0; i < rankDanger.size(); i++) {
                    if (rankDanger[i].first == a[0].getRank()) aIndex = i;
                    if (rankDanger[i].first == b[0].getRank()) bIndex = i;
                }

                // 优先选择危险度低的点数
                if (aIndex < bIndex) return true;
                if (aIndex > bIndex) return false;

                // 危险度相同，选择点数小的
                return a[0].getRank() < b[0].getRank();
                });

            // 选择合适的对子出牌
            vector<Card> selectedCards = pairs[0];
            auto cardGroup = CardGroupFactory::createCardGroup(selectedCards, game);
            removeCards(selectedCards);
            cout << name << "出牌(对子): ";
            for (const auto& card : selectedCards) {
                cout << card << " ";
            }
            cout << endl;

            game.updateCardCounter(selectedCards);
            graphicalPtr->updateLastPlay(index, selectedCards);
            graphicalPtr->drawUI(index);
            Sleep(300);

            auto cardGroupCopy = CardGroupFactory::createCardGroup(selectedCards, game);
            game.setLastPlay(move(cardGroup));
            return cardGroupCopy;
        }

        // 3: 尝试出三张，优先选择其他玩家手中较少的点数
        if (!triples.empty()) {
            sort(triples.begin(), triples.end(), [&rankDanger](const vector<Card>& a, const vector<Card>& b) {
                // 按剩余数量和点数大小综合排序
                int aIndex = -1, bIndex = -1;
                for (size_t i = 0; i < rankDanger.size(); i++) {
                    if (rankDanger[i].first == a[0].getRank()) aIndex = i;
                    if (rankDanger[i].first == b[0].getRank()) bIndex = i;
                }

                if (aIndex < bIndex) return true;
                if (aIndex > bIndex) return false;
                return a[0].getRank() < b[0].getRank();
                });

            vector<Card> selectedCards = triples[0];
            auto cardGroup = CardGroupFactory::createCardGroup(selectedCards, game);
            removeCards(selectedCards);
            cout << name << "出牌(三张): ";
            for (const auto& card : selectedCards) {
                cout << card << " ";
            }
            cout << endl;

            game.updateCardCounter(selectedCards);
            graphicalPtr->updateLastPlay(index, selectedCards);
            graphicalPtr->drawUI(index);
            Sleep(300);

            auto cardGroupCopy = CardGroupFactory::createCardGroup(selectedCards, game);
            game.setLastPlay(move(cardGroup));
            return cardGroupCopy;
        }

        // 4: 出单张，基于记牌器选择合适的牌
        // 优先出其他玩家手中较少的点数
        vector<Card> candidateSingles;
        for (const auto& card : handCards) {
            candidateSingles.push_back(card);
        }

        sort(candidateSingles.begin(), candidateSingles.end(), [&rankDanger](const Card& a, const Card& b) {
            // 按点数在其他玩家手中的剩余数量排序
            int aIndex = -1, bIndex = -1;
            for (size_t i = 0; i < rankDanger.size(); i++) {
                if (rankDanger[i].first == a.getRank()) aIndex = i;
                if (rankDanger[i].first == b.getRank()) bIndex = i;
            }

            if (aIndex < bIndex) return true;
            if (aIndex > bIndex) return false;
            return a.getRank() < b.getRank();
            });

        vector<Card> selectedCards = { candidateSingles[0] };
        auto cardGroup = CardGroupFactory::createCardGroup(selectedCards, game);
        removeCards(selectedCards);
        cout << name << "出牌(单张): " << selectedCards[0] << endl;

        game.updateCardCounter(selectedCards);
        graphicalPtr->updateLastPlay(index, selectedCards);
        graphicalPtr->drawUI(index);
        Sleep(300);

        auto cardGroupCopy = CardGroupFactory::createCardGroup(selectedCards, game);
        game.setLastPlay(move(cardGroup));
        return cardGroupCopy;
    }
    else {
        // 需要压制上家的牌
        const vector<Card>& lastCards = lastPlay->getCards();
        int lastCardsSize = lastCards.size();

        // 检查上一次出牌是不是来自队友
        bool isLastPlayFromTeammate = false;

        // 获取最后出牌前的出牌顺序
        int previousPlayerCount = 0;
        int currentTurnIndex = index;

        // 往回查找，看看有多少个人选择了"过"
        for (int i = 1; i <= 3; i++) {
            int prevIndex = (currentTurnIndex - i + 4) % 4;

            if (prevIndex == teammateIndex) {
                if (previousPlayerCount == 2) {
                    isLastPlayFromTeammate = true;
                    break;
                }
            }

            previousPlayerCount++;
        }

        // 如果上一次出牌是来自队友且队友快要出完牌，考虑放行
        if (isLastPlayFromTeammate && teammateAlmostDone && handCards.size() > 5) {
            cout << name << "选择过（放队友出牌）" << endl;
            graphicalPtr->drawUI(index);
            Sleep(300);
            return nullptr;
        }

        // 分析当前局面
        // 判断是否要保留炸弹
        bool saveBombs = (handCards.size() <= 10);

        // 寻找能压制上家牌的普通组合（非炸弹）
        vector<vector<Card>> validPlays;

        // 按照牌型处理
        int lastCardRank = lastCards[0].getRank();

        // 如果是单张
        if (lastCardsSize == 1) {
            for (const auto& card : handCards) {
                if (card.getRank() > lastCardRank) {
                    vector<Card> play = { card };
                    auto cardGroup = CardGroupFactory::createCardGroup(play, game);
                    if (cardGroup && cardGroup->canBeat(*lastPlay)) {
                        validPlays.push_back(play);
                    }
                }
            }
        }
        // 如果是对子
        else if (lastCardsSize == 2 && lastCards[0].getRank() == lastCards[1].getRank()) {
            for (auto& pair : pairs) {
                if (pair[0].getRank() > lastCardRank) {
                    auto cardGroup = CardGroupFactory::createCardGroup(pair, game);
                    if (cardGroup && cardGroup->canBeat(*lastPlay)) {
                        validPlays.push_back(pair);
                    }
                }
            }
        }
        // 如果是三张
        else if (lastCardsSize == 3 &&
            lastCards[0].getRank() == lastCards[1].getRank() &&
            lastCards[1].getRank() == lastCards[2].getRank()) {
            for (auto& triple : triples) {
                if (triple[0].getRank() > lastCardRank) {
                    auto cardGroup = CardGroupFactory::createCardGroup(triple, game);
                    if (cardGroup && cardGroup->canBeat(*lastPlay)) {
                        validPlays.push_back(triple);
                    }
                }
            }
        }
        // 如果是其他牌型，使用通用寻找方法
        else {
            for (size_t i = 0; i < handCards.size(); i++) {
                if (i + lastCardsSize > handCards.size()) break;

                vector<Card> candidateCards;
                for (int j = 0; j < lastCardsSize; j++) {
                    candidateCards.push_back(handCards[i + j]);
                }

                auto cardGroup = CardGroupFactory::createCardGroup(candidateCards, game);
                if (cardGroup && cardGroup->canBeat(*lastPlay)) {
                    validPlays.push_back(candidateCards);
                }
            }
        }

        //基于记牌器信息优化牌的选择
        // 排序有效牌组，优先考虑其他玩家手中剩余较少的点数
        sort(validPlays.begin(), validPlays.end(), [&rankDanger](const vector<Card>& a, const vector<Card>& b) {
            Rank aRank = a[0].getRank();
            Rank bRank = b[0].getRank();

            // 在rankDanger中查找这些点数的索引
            int aIndex = -1, bIndex = -1;
            for (size_t i = 0; i < rankDanger.size(); i++) {
                if (rankDanger[i].first == aRank) aIndex = i;
                if (rankDanger[i].first == bRank) bIndex = i;
            }

            // 危险度低的牌优先使用
            if (aIndex < bIndex) return true;
            if (aIndex > bIndex) return false;

            // 危险度相同时，使用点数较小的
            return aRank < bRank;
            });

        // 如果有普通牌型可以压制，使用排序后的第一组
        if (!validPlays.empty()) {
            vector<Card> selectedCards = validPlays[0];
            auto cardGroup = CardGroupFactory::createCardGroup(selectedCards, game);
            removeCards(selectedCards);
            cout << name << "出牌(基于记牌器): ";
            for (const auto& card : selectedCards) {
                cout << card << " ";
            }
            cout << endl;

            game.updateCardCounter(selectedCards);
            graphicalPtr->updateLastPlay(index, selectedCards);
            graphicalPtr->drawUI(index);
            Sleep(300);

            auto cardGroupCopy = CardGroupFactory::createCardGroup(selectedCards, game);
            game.setLastPlay(move(cardGroup));
            return cardGroupCopy;
        }

        //如果普通牌型无法压制，考虑使用炸弹
        if (!bombs.empty() && !saveBombs) {
            bool opponentAlmostDone = false;
            int opponentIndex1 = (index + 1) % 4;
            int opponentIndex2 = (index + 3) % 4;

            if (allPlayers[opponentIndex1]->getHandCards().size() <= 3 ||
                allPlayers[opponentIndex2]->getHandCards().size() <= 3) {
                opponentAlmostDone = true;
            }

            //关键时刻才使用炸弹
            if (opponentAlmostDone || handCards.size() <= 5 || (!isLastPlayFromTeammate)) {
                vector<Card> selectedCards = bombs[0];
                auto cardGroup = CardGroupFactory::createCardGroup(selectedCards, game);
                if (cardGroup && cardGroup->canBeat(*lastPlay)) {
                    removeCards(selectedCards);
                    cout << name << "出牌(炸弹): ";
                    for (const auto& card : selectedCards) {
                        cout << card << " ";
                    }
                    cout << endl;

                    game.updateCardCounter(selectedCards);
                    graphicalPtr->updateLastPlay(index, selectedCards);
                    graphicalPtr->drawUI(index);
                    Sleep(300);

                    auto cardGroupCopy = CardGroupFactory::createCardGroup(selectedCards, game);
                    game.setLastPlay(move(cardGroup));
                    return cardGroupCopy;
                }
            }
        }

        // 找不到能压制的牌，选择过牌
        cout << name << "选择过" << endl;
        graphicalPtr->drawUI(index);
        Sleep(300);
        return nullptr;
    }
}



//AIPlayer
//unique_ptr<CardGroup> AIPlayer::playCards(const unique_ptr<CardGroup>& lastPlay, Game& game)
//{
//	cout << "\n======" << name << "的回合======" << endl;
//
//	//获取自己在玩家数组中的索引
//	int index = -1;
//	for (size_t i = 0; i < game.getPlayers().size(); ++i) {
//		if (game.getPlayers()[i]->getName() == name) {
//			index = static_cast<int>(i);
//			break;
//		}
//	}
//
//	graphicalPtr->clearLastPlay(index);
//
//	// 展示手牌
//	showHand();
//
//	// 显示上家出牌(如果有)
//	if (lastPlay != nullptr) {
//		cout << "上家出牌: ";
//		for (const auto& card : lastPlay->getCards()) {
//			cout << card << " ";
//		}
//		cout << endl;
//	}
//
//	graphicalPtr->drawUI(index);
//	Sleep(300); // 让玩家有时间看清当前状态
//
//	// 如果是首家出牌（没有上家牌）或者已经没有玩家能出牌
//	if (lastPlay == nullptr) {
//		// 简单策略：出手中最小的一张牌或对子
//		if (handCards.size() >= 2 && handCards[0].getRank() == handCards[1].getRank())
//		{
//			vector<Card> selectedCards = { handCards[0], handCards[1] };
//			auto cardGroup = CardGroupFactory::createCardGroup(selectedCards, game);
//			if (cardGroup)
//			{
//				removeCards(selectedCards);
//				cout << name << "出牌: ";
//				for (const auto& card : selectedCards)
//				{
//					cout << card << " ";
//				}
//				cout << endl;
//
//				game.updateCardCounter(selectedCards);
//
//				graphicalPtr->updateLastPlay(index, selectedCards);
//				graphicalPtr->drawUI(index);
//				Sleep(300); // 让玩家有时间看清当前状态
//
//
//				auto cardGroupCopy = CardGroupFactory::createCardGroup(selectedCards, game);
//				// 直接使用setLastPlay更新游戏状态
//				game.setLastPlay(move(cardGroup));
//				return cardGroupCopy;
//			}
//		}
//
//		// 出单张
//		vector<Card> selectedCards = { handCards[0] };
//		auto cardGroup = CardGroupFactory::createCardGroup(selectedCards, game);
//		removeCards(selectedCards);
//		cout << name << "出牌: " << selectedCards[0] << endl;
//
//		game.updateCardCounter(selectedCards);
//
//		graphicalPtr->updateLastPlay(index, selectedCards);
//		graphicalPtr->drawUI(index);
//		Sleep(300); // 让玩家有时间看清当前状态
//
//
//		auto cardGroupCopy = CardGroupFactory::createCardGroup(selectedCards, game);
//		// 直接使用setLastPlay更新游戏状态
//		game.setLastPlay(move(cardGroup));
//		return cardGroupCopy;
//	}
//	else {
//		// 需要压制上家的牌
//		const vector<Card>& lastCards = lastPlay->getCards();
//		int lastCardsSize = lastCards.size();
//
//		// 从手牌中尝试找到能压制上家牌的组合
//		for (size_t i = 0; i < handCards.size() - lastCardsSize + 1; i++)
//		{
//			vector<Card> selectedCards;
//			for (int j = 0; j < lastCardsSize; j++)
//			{
//				if (i + j < handCards.size())
//				{
//					selectedCards.push_back(handCards[i + j]);
//				}
//			}
//
//			auto cardGroup = CardGroupFactory::createCardGroup(selectedCards, game);
//			if (cardGroup && (cardGroup->canBeat(*lastPlay))) {
//				removeCards(selectedCards);
//				cout << name << "出牌: ";
//				for (const auto& card : selectedCards) {
//					cout << card << " ";
//				}
//
//				cout << endl;
//
//				game.updateCardCounter(selectedCards);
//
//				graphicalPtr->updateLastPlay(index, selectedCards);
//				graphicalPtr->drawUI(index);
//				Sleep(300); // 让玩家有时间看清当前状态
//
//				auto cardGroupCopy = CardGroupFactory::createCardGroup(selectedCards, game);
//				// 直接使用setLastPlay更新游戏状态
//				game.setLastPlay(move(cardGroup));
//				return cardGroupCopy;
//			}
//		}
//
//		// 找不到能压制的牌，选择过牌
//		cout << name << "选择过" << endl;
//
//
//
//		graphicalPtr->drawUI(index);
//		Sleep(300); // 让玩家有时间看清当前状态
//
//		return nullptr;
//	}
//}

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

	// 获取玩家在玩家数组中的索引
	int fromPlayerIndex = -1;
	int toPlayerIndex = -1;
	for (size_t i = 0; i < gamePtr->getPlayers().size(); i++) {
		if (gamePtr->getPlayers()[i]->getName() == name) {
			fromPlayerIndex = static_cast<int>(i);
		}
		if (gamePtr->getPlayers()[i]->getName() == tributer->getName()) {
			toPlayerIndex = static_cast<int>(i);
		}
	}

	// 执行还贡动画
	graphicalPtr->animateCardTransfer(fromPlayerIndex, toPlayerIndex, returnCard, false);

	// 显示还贡界面
	graphicalPtr->drawReturnTributeInterface(shared_from_this(), tributer, returnCard);

	// 处理卡牌转移
	removeCard(returnCard);
	tributer->addCard(returnCard);

	/*cout << name << "还贡：" << returnCard << endl;

	removeCard(returnCard);
	tributer->addCard(returnCard);
	return;*/
}

void Player::clearHandCards()
{
	handCards.clear();
}