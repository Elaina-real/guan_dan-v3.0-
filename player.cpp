#include "game.h"
#include "card.h"
#include "cardgroup.h"
#include "player.h"
#include <sstream>

static bool compareCards(const Card& a, const Card& b)//���������
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

	if (it != handCards.end())//�������ҵ��������ɾ��
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
	cout  << name << "������: ";
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
	cout << "\n======" << name << "�Ļغ�======" << endl;
	while (true)
	{
		//��ʾ�ϼҳ���(�����
		if (lastPlay != nullptr)
		{
			cout << "�ϼҳ���:";
			for (const auto& card : lastPlay->getCards())
			{
				cout << card << " ";
			}
			cout << endl;
		}

		//չʾ����
		showHand();

		// ��ҿ���ѡ��"��"
		cout << "������'c'�鿴������,��ѡ��Ҫ�����ƣ������Ƶı�ţ���1��ʼ���ÿո�ָ�������0��ʾ������: ";
		string input;
		getline(cin, input);

		if (input == "c") 
		{
			// ��ʾ������
			game.showCardCounter();
			// �ݹ������������Ҽ���ѡ�����
			return playCards(lastPlay, game);
		}

		if (lastPlay == nullptr && input == "0")
		{
			cout << "���������\n" << endl;
			continue;//��������
		}

		//����Ƿ�ѡ���
		if (input == "0" || input.empty())
		{
			cout << name << "ѡ���" << endl << endl;
			return nullptr;//������
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
				cout << "��Ч������:" << index << endl << endl;
				continue;//��������
			}
		}

		//����Ƿ�ѡ�����ظ�����
		sort(selectedNum.begin(), selectedNum.end(), greater<int>());
		if (adjacent_find(selectedNum.begin(), selectedNum.end()) != selectedNum.end())
		{
			cout << "�벻Ҫ�ظ�ѡ��\n" << endl;
			continue;//��������
		}

		vector<Card> selectedCards;
		for (int idx : selectedNum)
		{
			selectedCards.push_back(handCards[idx]);
		}

		//����Ƿ�ѡ���������
		int thisWildCardCount = 0;
		vector<size_t> wildCardIndex; // ��¼����Ƶ�λ��
		for (size_t i = 0; i < selectedCards.size(); i++)
		{
			if (selectedCards[i].isWildCard())
			{
				thisWildCardCount++;
				wildCardIndex.push_back(i);
			}
		}

		unique_ptr<CardGroup> cardGroup = nullptr;

		if (thisWildCardCount == 0)//û��������������Ƚ�
		{
			cardGroup = CardGroupFactory::createCardGroup(selectedCards, game);

			if (cardGroup == nullptr)
			{
				cout << "��Ч�����ͣ�������ѡ��\n" << endl;
				continue;//��������
			}
			
			if (lastPlay == nullptr) // ����ǵ�һ������
			{
				removeCards(selectedCards);
				cout << name << "����: ";
				for (const auto& card : selectedCards)
				{
					cout << card << " ";
				}
				cout << endl << endl;

				game.updateCardCounter(cardGroup->getCards());
				auto cardGroupCopy = CardGroupFactory::createCardGroup(selectedCards, game);
				// ֱ��ʹ��setLastPlay������Ϸ״̬
				game.setLastPlay(move(cardGroup));
				return cardGroupCopy;
			}
			else if (*cardGroup > *lastPlay) // �������ѹ���ϼ�
			{
				removeCards(selectedCards);
				cout << name << "����: ";
				for (const auto& card : selectedCards)
				{
					cout << card << " ";
				}
				cout << endl << endl;
				game.updateCardCounter(cardGroup->getCards());
				auto cardGroupCopy = CardGroupFactory::createCardGroup(selectedCards, game);
				// ֱ��ʹ��setLastPlay������Ϸ״̬
				game.setLastPlay(move(cardGroup));
				return cardGroupCopy;
			}
			else
			{
				cout << "����ʧ�ܣ���Ч���ͻ���ѹ���ϼҵ���\n" << endl;
				continue;//��������
			}
			//if (lastPlay == nullptr || *cardGroup > *lastPlay)
			//{
			//	removeCards(selectedCards);
			//	cout << name << "����: ";
			//	for (const auto& card : selectedCards)
			//	{
			//		cout << card << " ";
			//	}
			//	cout << endl << endl;
			//	return cardGroup;
			//}
			//else
			//{
			//	cout << "����ʧ�ܣ���Ч���ͻ���ѹ���ϼҵ���\n" << endl;
			//	continue;//��������
			//}
		}
		else//�еĻ����⴦��
		{
			cardGroup = make_unique<Single>(selectedCards, game);//���ⴴ��һ������Ķ������Խ���Ƚ��߼�

			if (lastPlay == nullptr) // ����ǵ�һ������
			{
				vector<Card> tempCards = cardGroup->getCards();
				unique_ptr<CardGroup> newCardGroup = CardGroupFactory::createCardGroup(tempCards, game);
				removeCards(selectedCards);
				cout << name << "����: ";
				for (const auto& card : selectedCards)
				{
					cout << card << " ";
				}
				cout << endl << endl;

				game.updateCardCounter(cardGroup->getCards());
				auto cardGroupCopy = CardGroupFactory::createCardGroup(selectedCards, game);
				// ֱ��ʹ��setLastPlay������Ϸ״̬
				game.setLastPlay(move(cardGroupCopy));
				return cardGroup;
			}
			else if (*cardGroup > *lastPlay) // �������ѹ���ϼ�
			{
				vector<Card> tempCards = cardGroup->getCards();
				unique_ptr<CardGroup> newCardGroup = CardGroupFactory::createCardGroup(tempCards, game);
				removeCards(selectedCards);
				cout << name << "����: ";
				for (const auto& card : selectedCards)
				{
					cout << card << " ";
				}
				cout << endl << endl;
				return newCardGroup;
			}
			else
			{
				cout << "����ʧ�ܣ���Ч���ͻ���ѹ���ϼҵ���\n" << endl;
				continue;//��������
			}

			//if (lastPlay == nullptr || *cardGroup > *lastPlay)
			//{
			//	vector <Card> tempCards = cardGroup->getCards();
			//	unique_ptr<CardGroup> newCardGroup = CardGroupFactory::createCardGroup(tempCards, game);
			//	removeCards(selectedCards);
			//	cout << name << "����: ";
			//	for (const auto& card : selectedCards)
			//	{
			//		cout << card << " ";
			//	}
			//	cout << endl << endl;
			//	return newCardGroup;
			//}
			//else
			//{
			//	cout << "����ʧ�ܣ���Ч���ͻ���ѹ���ϼҵ���\n" << endl;
			//	continue;//��������
			//}
		}
		//unique_ptr<CardGroup> cardGroup = Card(selectedCards, game);

		//unique_ptr<CardGroup> cardGroup = nullptr;


		//if (cardGroup == nullptr)
		//{
		//	cout << "��Ч�����ͣ�������ѡ��\n" << endl;
		//	continue;//��������
		//}
		//else
		//if (lastPlay == nullptr || *cardGroup > *lastPlay)
		//{
		//	removeCards(selectedCards);
		//	cout << name << "����: ";
		//	for (const auto& card : selectedCards)
		//	{
		//		cout << card << " ";
		//	}
		//	cout << endl << endl;
		//	return cardGroup;
		//}
		//else
		//{
		//	cout << "����ʧ�ܣ���Ч���ͻ���ѹ���ϼҵ���\n" << endl;
		//	continue;//��������
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
		cout << name << "û�п����ó�����" << endl;
		return;
	}

	cout << name << "�Ŀ��ó���: ";
	for (size_t i = 0; i < eligibleCards.size(); ++i)
	{
		cout << i + 1 << ": " << eligibleCards[i] << " ";
	}

	unsigned int index;
	while (1)
	{
		cout << "\n\n" << name << "��ѡ��Ҫ�ó���һ���ƣ������Ƶı�ţ���1��ʼ��: ";
		cin >> index;
		if (index >= 1 && index <= eligibleCards.size())
			break;
		cout << "��Ч�����룬������ѡ��" << endl;
	}

	Card reutrnCard = eligibleCards[index - 1];
	cout << name << "����" << reutrnCard << endl;
	removeCard(reutrnCard);
	tributer->addCard(reutrnCard);
	return;
}

//AIPlayer
unique_ptr<CardGroup> AIPlayer::playCards(const unique_ptr<CardGroup>& lastPlay, Game& game)
{
	cout << "\n======" << name << "�Ļغ�======" << endl;

	// չʾ����
	showHand();

	// ��ʾ�ϼҳ���(�����)
	if (lastPlay != nullptr) {
		cout << "�ϼҳ���: ";
		for (const auto& card : lastPlay->getCards()) {
			cout << card << " ";
		}
		cout << endl;
	}

	// ������׼ҳ��ƣ�û���ϼ��ƣ������Ѿ�û������ܳ���
	if (lastPlay == nullptr) {
		// �򵥲��ԣ���������С��һ���ƻ����
		if (handCards.size() >= 2 && handCards[0].getRank() == handCards[1].getRank())
		{
			vector<Card> selectedCards = { handCards[0], handCards[1] };
			auto cardGroup = CardGroupFactory::createCardGroup(selectedCards, game);
			if (cardGroup)
			{
				removeCards(selectedCards);
				cout << name << "����: ";
				for (const auto& card : selectedCards)
				{
					cout << card << " ";
				}
				cout << endl;

				game.updateCardCounter(selectedCards);
				auto cardGroupCopy = CardGroupFactory::createCardGroup(selectedCards, game);
				// ֱ��ʹ��setLastPlay������Ϸ״̬
				game.setLastPlay(move(cardGroup));
				return cardGroupCopy;
			}
		}

		// ������
		vector<Card> selectedCards = { handCards[0] };
		auto cardGroup = CardGroupFactory::createCardGroup(selectedCards, game);
		removeCards(selectedCards);
		cout << name << "����: " << selectedCards[0] << endl;

		game.updateCardCounter(selectedCards);
		auto cardGroupCopy = CardGroupFactory::createCardGroup(selectedCards, game);
		// ֱ��ʹ��setLastPlay������Ϸ״̬
		game.setLastPlay(move(cardGroup));
		return cardGroupCopy;
	}
	else {
		// ��Ҫѹ���ϼҵ���
		const vector<Card>& lastCards = lastPlay->getCards();
		int lastCardsSize = lastCards.size();

		// �������г����ҵ���ѹ���ϼ��Ƶ����
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
				cout << name << "����: ";
				for (const auto& card : selectedCards) {
					cout << card << " ";
				}
				cout << endl;
				game.updateCardCounter(selectedCards);
				auto cardGroupCopy = CardGroupFactory::createCardGroup(selectedCards, game);
				// ֱ��ʹ��setLastPlay������Ϸ״̬
				game.setLastPlay(move(cardGroup));
				return cardGroupCopy;
			}
		}

		// �Ҳ�����ѹ�Ƶ��ƣ�ѡ�����
		cout << name << "ѡ���" << endl;
		return nullptr;
	}
}

void AIPlayer::returnTribute(const shared_ptr<Player>& tributer)
{
	// �ҳ����з����������ƣ�����С��10��
	vector<Card> eligibleCards;
	for (const auto& card : handCards)
	{
		if (card.getRank() < ten)
		{
			eligibleCards.push_back(card);
		}
	}

	// ���û�з����������ƣ����ܻ���
	if (eligibleCards.empty())
	{
		cout << name << "û�п����ó�����" << endl;
		return;
	}

	// ��AI���ԣ�ѡ����С���ƻ���
	Card returnCard = eligibleCards[0];

	cout << name << "������" << returnCard << endl;

	removeCard(returnCard);
	tributer->addCard(returnCard);
	return;
}

void Player::clearHandCards()
{
	handCards.clear();
}