#include "cardgroup.h"
#include "cardgroupfactory.h"
#include "card.h"
void CardGroup::countRanks() 
{
	rankCount.clear();
	for (const auto& card : cards) 
	{
		rankCount[card.getRank()]++;
	}
}

void CardGroup::setCards(const vector<Card>& c)
{
	cards = c;
	rankCount.clear();
	countRanks();
}


bool CardGroup::operator>(const CardGroup& other)
{
	if (isValid() == true && other.isValid() == true && canBeat(other))
	{
		// ʹ��CardGroupFactory���������ʵľ��忨�������
		//vector<Card> cardsCopy = cards;
		//unique_ptr<CardGroup> newGroup = CardGroupFactory::createCardGroup(cardsCopy, game);
		//if (newGroup) 
		//{
		//	game.setLastPlay(move(newGroup));
		//}
		return true;
	}

	//����Ƿ��������
	int thisWildCardCount = 0;
	vector<size_t> wildCardIndex; // ��¼����Ƶ�λ��
	for (size_t i = 0; i < cards.size(); i++)
	{
		if (cards[i].isWildCard())
		{
			thisWildCardCount++;
			wildCardIndex.push_back(i);
		}
	}

	if (thisWildCardCount == 0)//û������ƣ������Ƚ�
	{
		if (this->isValid() == false || other.isValid() == false)
			return false;
		if (this->canBeat(other))
		{
			game.updateCardCounter(cards);
			return true;
		}
		return false;
	}
	else if (thisWildCardCount == 1)//����һ�������
	{
		// ���ȳ���ԭʼ�����Ƿ��Ѿ�����սʤ�Է�
		if (this->canBeat(other))
		{
			vector<Card> cardsCopy = cards;
			unique_ptr<CardGroup> newGroup = CardGroupFactory::createCardGroup(cardsCopy, game);
			if (newGroup)
			{
				game.setLastPlay(move(newGroup));
			}
			game.updateCardCounter(cards);
			return true;
		}

		vector<Card> tempCards = cards;
		// ���Դ�A��2�����п���ֵ�滻�����
		for (int rank = ace; rank >= two; rank--)
		{

			if (rank == smal || rank == big)
				continue;

			// �����µ��滻��
			Card replacementCard(Heart, static_cast<Rank>(rank), false, false);
			tempCards[wildCardIndex[0]] = replacementCard;

			Game& nonConstGame = const_cast<Game&>(game); 
			unique_ptr<CardGroup> newGroup = CardGroupFactory::createCardGroup(tempCards, nonConstGame);

			// ����������Ƿ���Ч�����ܷ�սʤ�Է�.
			if (newGroup && newGroup->isValid() && newGroup->canBeat(other))
			{
				// ���¼�����ʱʹ��ԭʼ���飨��������ƣ�
				game.updateCardCounter(cards);
				nonConstGame.setLastPlay(move(newGroup));
				const_cast<CardGroup*>(this)->setCards(tempCards);
				return true;
			}
		}
		return false;
	}
	else if (thisWildCardCount == 2)//�������������
	{
		// ���ȳ���ԭʼ�����Ƿ��Ѿ�����սʤ�Է�
		if (this->canBeat(other))
		{
			vector<Card> cardsCopy = cards;
			unique_ptr<CardGroup> newGroup = CardGroupFactory::createCardGroup(cardsCopy, game);
			if (newGroup) 
			{
				game.setLastPlay(move(newGroup));
			}
			game.updateCardCounter(cards);
			return true;
		}

		// ����һ�����������滻�����
		vector<Card> tempCards = cards;

		// �������п��ܵ�����������滻���
		for (int rank1 = ace; rank1 >= two; rank1--)
		{
			if (rank1 == smal || rank1 == big)
				continue;

			for (int rank2 = ace; rank2 >= two; rank2--)
			{
				if (rank2 == smal || rank2 == big)
					continue;

				// �����µ��滻��
				Card replacementCard1(Heart, static_cast<Rank>(rank1), false, false);
				Card replacementCard2(Heart, static_cast<Rank>(rank2), false, false);

				tempCards[wildCardIndex[0]] = replacementCard1;
				tempCards[wildCardIndex[1]] = replacementCard2;

				// ʹ��CardGroupFactory�����µ��������
				Game& nonConstGame = const_cast<Game&>(game);
				unique_ptr<CardGroup> newGroup = CardGroupFactory::createCardGroup(tempCards, nonConstGame);

				// ����������Ƿ���Ч�����ܷ�սʤ�Է�
				if (newGroup && newGroup->isValid() && newGroup->canBeat(other))
				{
					game.updateCardCounter(cards);
					game.setLastPlay(move(newGroup));
					return true;
				}
			}
		}
		return false;
	}
}

vector<Card> CardGroup::getCards() const
{
	return cards;
}

unordered_map<Rank, int> CardGroup::getRankCount() const
{
	return rankCount;
}

//Single
bool Single::isValid() const 
{
	return cards.size() == 1;
}

bool Single::canBeat(const CardGroup& other)const
{
	// �ж�other�Ƿ�ΪSingle����
	if (dynamic_cast<const Single*>(&other) == nullptr)
		return false;

	if (cards[0].getRank() == other.getCards()[0].getRank())
		return false;
	else if (cards[0].isLevelCard())
		return other.getCards()[0].getRank() < ace;
	else if (other.getCards()[0].isLevelCard())
		return cards[0].getRank() > ace;
	else return cards[0].getRank() > other.getCards()[0].getRank();
}

//Pair
bool Pair::isValid() const
{
	return cards.size() == 2 && rankCount.size() == 1;
}

bool Pair::canBeat(const CardGroup& other)const
{
	// �ж�other�Ƿ�ΪPair����
	if (dynamic_cast<const Pair*>(&other) == nullptr)
		return false;



	if (cards[0].getRank() == other.getCards()[0].getRank())
		return false;
	else if (cards[0].isLevelCard())
		return other.getCards()[0].getRank() < ace;
	else if (other.getCards()[0].isLevelCard())
		return cards[0].getRank() > ace;
	else return cards[0].getRank() > other.getCards()[0].getRank();
}

//Triple
bool Triple::isValid() const
{
	return cards.size() == 3 && rankCount.size() == 1;
}

bool Triple::canBeat(const CardGroup& other)const
{
	// �ж�other�Ƿ�ΪTriple����
	if (dynamic_cast<const Triple*>(&other) == nullptr)
		return false;

	if (cards[0].getRank() == other.getCards()[0].getRank())
		return false;
	else if (cards[0].isLevelCard())
		return other.getCards()[0].getRank() > ace;
	else if (other.getCards()[0].isLevelCard())
		return cards[0].getRank() > ace;
	else return cards[0].getRank() > other.getCards()[0].getRank();
}

//TripleWithPair
bool TripleWithPair::isValid() const
{
	return cards.size() == 5 && rankCount.size() == 2 &&
		(any_of(rankCount.begin(), rankCount.end(), [](const auto& p) { return p.second == 3; }) &&
		any_of(rankCount.begin(), rankCount.end(), [](const auto& p) { return p.second == 2; }));
}

bool TripleWithPair::canBeat(const CardGroup& other)const
{
	// �ж�other�Ƿ�ΪTripleWithPair����
	if (dynamic_cast<const TripleWithPair*>(&other) == nullptr)
		return false;

	// ��ȡ�Լ��������ƺͶ���
	vector<Card> myTripleCards; 
	vector<Card> myPairCards; 
	Rank myTripleRank = static_cast<Rank>(-1);
	Rank myPairRank = static_cast<Rank>(-1);

	// �ҳ������ƺͶ��ӵĵ���
	for (const auto& kv : rankCount)
	{
		if (kv.second == 3)
		{
			myTripleRank = kv.first;
		}
		else if (kv.second == 2)
		{
			myPairRank = kv.first;
		}
	}

	// ��ȡ��Ӧ�Ŀ���
	for (const auto& card : cards)
	{
		if (card.getRank() == myTripleRank)
		{
			myTripleCards.push_back(card);
		}
		else if (card.getRank() == myPairRank)
		{
			myPairCards.push_back(card);
		}
	}

	// ��ȡ�Է��������ƺͶ���
	vector<Card> otherTripleCards;  
	vector<Card> otherPairCards;   
	Rank otherTripleRank = static_cast<Rank>(-1);
	Rank otherPairRank = static_cast<Rank>(-1);

	// �ҳ��Է������ƺͶ��ӵĵ���
	for (const auto& kv : other.getRankCount())
	{
		if (kv.second == 3)
		{
			otherTripleRank = kv.first;
		}
		else if (kv.second == 2)
		{
			otherPairRank = kv.first;
		}
	}

	// ��ȡ�Է���Ӧ�Ŀ���
	const vector<Card>& otherCards = other.getCards();
	for (const auto& card : otherCards)
	{
		if (card.getRank() == otherTripleRank)
		{
			otherTripleCards.push_back(card);
		}
		else if (card.getRank() == otherPairRank)
		{
			otherPairCards.push_back(card);
		}
	}

	if (myTripleRank == otherTripleRank)
		return myPairCards[0] > otherPairCards[0];
	else
		return myTripleCards[0] > otherTripleCards[0];

}

//Bomb
bool Bomb::isValid() const
{
	return cards.size() >= 4 && cards.size() <= 8 && rankCount.size() == 1;
}

bool Bomb::canBeat(const CardGroup& other)const
{
	// �ж�other�Ƿ�ΪBomb����
	if (dynamic_cast<const Bomb*>(&other) != nullptr)
	{
		if (cards.size() != other.getCards().size())
			return cards.size() > other.getCards().size();
		else return cards[0].getRank() > other.getCards()[0].getRank();
	}
	else
	{
		if (dynamic_cast<const Flush*>(&other) != nullptr)
			return cards.size() > 5;
		else if (!(dynamic_cast<const TwoJokers*>(&other) == nullptr))
			return cards.size() > 4;
		else if (!(dynamic_cast<const FourJokers*>(&other) == nullptr))
			return false;
		else return true;
	}
}

//TwoJokers
bool TwoJokers::isValid() const
{
	return cards.size() == 2
		&& rankCount.size() == 2
		&& rankCount.count(smal) == 1
		&& rankCount.at(smal) == 1
		&& rankCount.count(big) == 1
		&& rankCount.at(big) == 1;
}

bool TwoJokers::canBeat(const CardGroup& other)const
{
	// �ж�other�Ƿ�ΪTwoJokers����
	if (dynamic_cast<const TwoJokers*>(&other) == nullptr && dynamic_cast<const FourJokers*>(&other) == nullptr
		&& dynamic_cast<const Flush*>(&other) == nullptr && dynamic_cast<const Bomb*>(&other) == nullptr)
		return true;
	else if (dynamic_cast<const Bomb*>(&other) == nullptr)
		return other.getCards().size() > 4;
	else
		return true;
}

//FourJokers
bool FourJokers::isValid() const
{
	return cards.size() == 4
		&& rankCount.size() == 2
		&& rankCount.count(smal) == 1
		&& rankCount.at(smal) == 2
		&& rankCount.count(big) == 1
		&& rankCount.at(big) == 2;
}

bool FourJokers::canBeat(const CardGroup& other)const
{
	return true;
}

//Straight
bool Straight::isValid() const
{
	if (cards.size() != 5) return false; // ����������

	// �����д�С��
	if (rankCount.count(smal) || rankCount.count(big)) return false;

	// ����Ψһ
	if (rankCount.size() != 5) return false;

	// �ų�ͬ��˳
	Suit firstSuit = cards[0].getSuit();
	bool allSameSuit = true;
	for (const auto& card : cards)
	{
		if (card.getSuit() != firstSuit)
		{
			allSameSuit = false;
			break;
		}
	}
	if (allSameSuit)
		return false;

	// �ռ�����������
	vector<int> ranks;
	for (const auto& kv : rankCount) 
	{
		ranks.push_back(static_cast<int>(kv.first));
	}
	sort(ranks.begin(), ranks.end());

	// ��ͨ˳��
	bool normal = true;
	for (int i = 1; i < 5; ++i) 
	{
		if (ranks[i] - ranks[i - 1] != 1) 
		{
			normal = false;
			break;
		}
	}
	// ����˳��A2345
	bool a2345 = (ranks == vector<int>{2, 3, 4, 5, 14});

	return normal || a2345;
}

bool Straight::canBeat(const CardGroup& other)const
{
	if (dynamic_cast<const Straight*>(&other) == nullptr)
		return false;

	// �ռ�������˳�ӵĵ���
	vector<int> myRanks;
	for (const auto& kv : rankCount) 
	{
		myRanks.push_back(static_cast<int>(kv.first));
	}
	std::sort(myRanks.begin(), myRanks.end());

	// �ռ�������Է�˳�ӵĵ���
	vector<int> otherRanks;
	for (const auto& kv : other.getRankCount()) 
	{
		otherRanks.push_back(static_cast<int>(kv.first));
	}
	sort(otherRanks.begin(), otherRanks.end());

	// �ж��Ƿ�A2345
	bool myA2345 = (myRanks == vector<int>{2, 3, 4, 5, 14});
	bool otherA2345 = (otherRanks == vector<int>{2, 3, 4, 5, 14});

	// �Ƚϴ�С
	int myValue = myA2345 ? 5 : myRanks.back();
	int otherValue = otherA2345 ? 5 : otherRanks.back();

	return myValue > otherValue;
}

//Flush
bool Flush::isValid()const
{
	if (cards.size() != 5) return false; // ����������

	// �����д�С��
	if (rankCount.count(smal) || rankCount.count(big)) return false;

	// ����Ψһ
	if (rankCount.size() != 5) return false;

	// �õ�ͬ��˳
	Suit firstSuit = cards[0].getSuit();
	bool allSameSuit = true;
	for (const auto& card : cards)
	{
		if (card.getSuit() != firstSuit)
		{
			allSameSuit = false;
			break;
		}
	}
	if (!allSameSuit)
		return false;

	// �ռ�����������
	vector<int> ranks;
	for (const auto& kv : rankCount)
	{
		ranks.push_back(static_cast<int>(kv.first));
	}
	sort(ranks.begin(), ranks.end());

	// ��ͨ˳��
	bool normal = true;
	for (int i = 1; i < 5; ++i) {
		if (ranks[i] - ranks[i - 1] != 1) {
			normal = false;
			break;
		}
	}
	// ����˳��A2345
	bool a2345 = (ranks == vector<int>{2, 3, 4, 5, 14});

	return normal || a2345;
}

bool Flush::canBeat(const CardGroup& other)const
{
	// ����ѹ������ը
	if (dynamic_cast<const FourJokers*>(&other) != nullptr)
		return false;

	// ��ѹ��˫��ը
	if (dynamic_cast<const TwoJokers*>(&other) != nullptr)
		return true;

	// ����ѹ����ը������
	if (const Bomb* bomb = dynamic_cast<const Bomb*>(&other))
	{
		return other.getCards().size() <= 5;
	}

	// ��ͬ��˳�ȴ�С
	if (dynamic_cast<const Flush*>(&other) != nullptr) 
	{
		// �ռ�������˳�ӵĵ���
		vector<int> myRanks;
		for (const auto& kv : rankCount)
		{
			myRanks.push_back(static_cast<int>(kv.first));
		}
		std::sort(myRanks.begin(), myRanks.end());

		// �ռ�������Է�˳�ӵĵ���
		vector<int> otherRanks;
		for (const auto& kv : other.getRankCount())
		{
			otherRanks.push_back(static_cast<int>(kv.first));
		}
		sort(otherRanks.begin(), otherRanks.end());

		// �ж��Ƿ�A2345
		bool myA2345 = (myRanks == vector<int>{2, 3, 4, 5, 14});
		bool otherA2345 = (otherRanks == vector<int>{2, 3, 4, 5, 14});

		// �Ƚϴ�С
		int myValue = myA2345 ? 5 : myRanks.back();
		int otherValue = otherA2345 ? 5 : otherRanks.back();

		return myValue > otherValue;
	}

	// �����������Ͷ���ѹ��
	return true;
}

//SteelPlate
bool SteelPlate::isValid()const
{
	// ����6����
	if (cards.size() != 6) return false;
	// �����������ֵ���
	if (rankCount.size() != 2) return false;

	vector<int> ranks;
	for (const auto& kv : rankCount) {
		if (kv.second != 3) return false; // ÿ�����3��
		ranks.push_back(static_cast<int>(kv.first));
	}
	sort(ranks.begin(), ranks.end());

	return (ranks[1] - ranks[0] == 1) || (ranks[0] == 2 && ranks[1] == 14);
}

bool SteelPlate::canBeat(const CardGroup& other)const
{
	if (dynamic_cast<const SteelPlate*>(&other) == nullptr)
		return false;

	// �ռ������򱾸ְ�ĵ���
	vector<int> myRanks;
	for (const auto& kv : rankCount) {
		myRanks.push_back(static_cast<int>(kv.first));
	}
	sort(myRanks.begin(), myRanks.end());

	// �ռ�������Է��ְ�ĵ���
	vector<int> otherRanks;
	for (const auto& kv : other.getRankCount()) {
		otherRanks.push_back(static_cast<int>(kv.first));
	}
	sort(otherRanks.begin(), otherRanks.end());

	// AAA222����С�ĸְ�
	bool myIsAAA222 = (myRanks[0] == 2 && myRanks[1] == 14);
	bool otherIsAAA222 = (otherRanks[0] == 2 && otherRanks[1] == 14);

	if (myIsAAA222 && !otherIsAAA222) 
		return false;
	if (!myIsAAA222 && otherIsAAA222) 
		return true;

	// �������ֱ�ӱ�������
	return myRanks[1] > otherRanks[1];
}

//ConPairs
bool ConPairs::isValid()const
{
	// ����6����
	if (cards.size() != 6) return false;
	// �����������ֵ���
	if (rankCount.size() != 3) return false;

	vector<int> ranks;
	for (const auto& kv : rankCount) {
		if (kv.second != 2) return false; // ÿ�����2��
		ranks.push_back(static_cast<int>(kv.first));
	}
	sort(ranks.begin(), ranks.end());

	return (ranks[1] - ranks[0] == 1 && ranks[2] - ranks[1] == 1) || (ranks[0] == 2 && ranks[1] == 3 && ranks[2] == 14);
}

bool ConPairs::canBeat(const CardGroup& other)const
{
	if (dynamic_cast<const ConPairs*>(&other) == nullptr)
		return false;

	// �ռ����������Եĵ���
	vector<int> myRanks;
	for (const auto& kv : rankCount) {
		myRanks.push_back(static_cast<int>(kv.first));
	}
	sort(myRanks.begin(), myRanks.end());

	// �ռ�������Է����Եĵ���
	vector<int> otherRanks;
	for (const auto& kv : other.getRankCount()) {
		otherRanks.push_back(static_cast<int>(kv.first));
	}
	sort(otherRanks.begin(), otherRanks.end());

	// AA2233����С�ĸְ�
	bool myIsAA2233 = (myRanks[0] == 2 && myRanks[1] == 3 && myRanks[2] == 14);
	bool otherIsAA2233 = (otherRanks[0] == 2 && otherRanks[1] == 3 && otherRanks[2] == 14);

	if (myIsAA2233 && !otherIsAA2233)
		return false;
	if (!myIsAA2233 && otherIsAA2233)
		return true;

	// �������ֱ�ӱ�������
	return myRanks[2] > otherRanks[2];
}