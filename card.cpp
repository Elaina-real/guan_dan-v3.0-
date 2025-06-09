#include <random>   
#include <chrono>   
#include <algorithm>
#include "card.h"
#include "game.h" // 如果需要用到Game类的方法

Suit Card::getSuit()const
{
	return suit;
}

Rank Card::getRank()const
{
	return rank;
}

bool Card::isLevelCard()const
{
	return isLevel;
}

bool Card::isWildCard()const
{
	return isWild;
}

ostream& operator<<(ostream& os, const Card& card)
{
	switch (card.getSuit())
	{
	case Heart: os << "H"; break;
	case Diamond: os << "♦"; break;//♦ ♣ ♠
	case Club: os << "♣"; break;
	case Spade: os << "♠"; break;
	case Joker: os << " Joker_"; break;
	}
	switch (card.getRank())
	{
	case two: os << "2"; break;
	case three: os << "3"; break;
	case four: os << "4"; break;
	case five: os << "5"; break;
	case six: os << "6"; break;
	case seven: os << "7"; break;
	case eight: os << "8"; break;
	case nine: os << "9"; break;
	case ten: os << "10"; break;
	case jack: os << "J"; break;
	case queen: os << "Q"; break;
	case king: os << "K"; break;
	case ace: os << "A"; break;
	case smal: os << "小"; break;
	case big: os << "大"; break;
	}
	return os;
}

bool Card::operator==(const Card& other)const
{
	return getRank() == other.getRank();
}

bool Card::operator>(const Card& other)const
{
	if (isLevel)
		return other.getRank() <= ace;
	else if (other.isLevelCard())
		return rank > ace;

	return getRank() > other.getRank();
}

//DEck
Deck::Deck(Rank l) : currentLevel(l)
{
	initialize(l);
}

void Deck::initialize(Rank l)
{
	cards.clear();
	//初始化两副牌
	for (int i = 0; i < 2; i++)
	{
		for (int suit = 0; suit < 4; suit++)
		{
			for (int rank = 2; rank <= ace; rank++)
			{
				cards.push_back(Card(static_cast<Suit>(suit), static_cast<Rank>(rank),
					(rank == l),(rank == l&&suit ==Heart)
				));
			}
		}
		cards.push_back(Card(Joker, smal));
		cards.push_back(Card(Joker, big));
	}
}

void Deck::shuffle()
{
	unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
	random_device rd; 

	seed_seq seed_seq{ rd(), rd(), rd(), rd(), static_cast<unsigned>(seed1) };

	mt19937_64 generator(seed_seq);

	std::shuffle(cards.begin(), cards.end(), generator);
	// 第二次洗牌，进一步增强随机性
	std::shuffle(cards.begin(), cards.end(), generator);
}

vector<Card> Deck::dealCards(int num)
{
	vector<Card> dealtCards;

	// 检查剩余牌数是否足够
	int availableCards = min(num, static_cast<int>(cards.size()));

	// 从牌堆顶部取出指定数量或剩余数量的牌
	for (int i = 0; i < availableCards; ++i)
	{
		dealtCards.push_back(cards.back());
		cards.pop_back();
	}

	return dealtCards;
}