#include "game.h"
#include "card.h"
#include "cardgroup.h"
#include "player.h"
#include "cardgroupfactory.h"


unique_ptr<CardGroup> CardGroupFactory::createCardGroup(vector<Card>& cards, Game& game)
{
    unique_ptr<CardGroup> cardgroup = nullptr;

	switch (cards.size())//按照牌的数量来初步判断牌型，尝试的顺序可能有点问题
    {
    case 1:
        cardgroup = tryCreateSingle(cards, game);
        break;
    case 2:
        cardgroup = tryCreatePair(cards, game);
        if (cardgroup == nullptr)
            cardgroup = tryCreateTwoJokers(cards, game);
        break;
    case 3:
        cardgroup = tryCreateTriple(cards, game);
        break;
    case 4:
        cardgroup = tryCreateBomb(cards, game);
        if (cardgroup == nullptr)
            cardgroup = tryCreateFourJokers(cards, game);
        break;
    case 5:
        cardgroup = tryCreateFlush(cards, game);
        if (cardgroup == nullptr)
            cardgroup = tryCreateBomb(cards, game);
        if (cardgroup == nullptr)
            cardgroup = tryCreateStraight(cards, game);
        if (cardgroup == nullptr)
            cardgroup = tryCreateTripleWithPair(cards, game);
        break;
    case 6:
        cardgroup = tryCreateBomb(cards, game);
        if (cardgroup == nullptr)
            cardgroup = tryCreateSteelPlate(cards, game);
        if (cardgroup == nullptr)
            cardgroup = tryCreateConPairs(cards, game);
        break;
    default:
        cardgroup = tryCreateBomb(cards, game);
        break;
    }
    return cardgroup;
}

unique_ptr<CardGroup> CardGroupFactory::tryCreateSingle(vector<Card>& cards, Game& game)
{
    auto single = make_unique<Single>(cards, game);
    if (single->isValid()) {
        return single;
    }
    return nullptr;
}

unique_ptr<CardGroup> CardGroupFactory::tryCreatePair(vector<Card>& cards, Game& game)
{
    auto pair = make_unique<Pair>(cards, game);
    if (pair->isValid()) {
        return pair;
    }
    return nullptr;
}

unique_ptr<CardGroup> CardGroupFactory::tryCreateTriple(vector<Card>& cards, Game& game)
{
    auto triple = make_unique<Triple>(cards, game);
    if (triple->isValid()) {
        return triple;
    }
    return nullptr;
}

unique_ptr<CardGroup> CardGroupFactory::tryCreateTripleWithPair(vector<Card>& cards, Game& game)
{
    auto tripleWithPair = make_unique<TripleWithPair>(cards, game);
    if (tripleWithPair->isValid()) {
        return tripleWithPair;
    }
    return nullptr;
}

unique_ptr<CardGroup> CardGroupFactory::tryCreateBomb(vector<Card>& cards, Game& game)
{
    auto bomb = make_unique<Bomb>(cards, game);
    if (bomb->isValid()) {
        return bomb;
    }
    return nullptr;
}

unique_ptr<CardGroup> CardGroupFactory::tryCreateTwoJokers(vector<Card>& cards, Game& game)
{
    auto twoJokers = make_unique<TwoJokers>(cards, game);
    if (twoJokers->isValid()) {
        return twoJokers;
    }
    return nullptr;
}

unique_ptr<CardGroup> CardGroupFactory::tryCreateFourJokers(vector<Card>& cards, Game& game)
{
    auto fourJokers = make_unique<FourJokers>(cards, game);
    if (fourJokers->isValid()) {
        return fourJokers;
    }
    return nullptr;
}

unique_ptr<CardGroup> CardGroupFactory::tryCreateStraight(vector<Card>& cards, Game& game)
{
    auto straight = make_unique<Straight>(cards, game);
    if (straight->isValid()) {
        return straight;
    }
    return nullptr;
}

unique_ptr<CardGroup> CardGroupFactory::tryCreateFlush(vector<Card>& cards, Game& game)
{
    auto flush = make_unique<Flush>(cards, game);
    if (flush->isValid()) {
        return flush;
    }
    return nullptr;
}

unique_ptr<CardGroup> CardGroupFactory::tryCreateSteelPlate(vector<Card>& cards, Game& game)
{
    auto steelPlate = make_unique<SteelPlate>(cards, game);
    if (steelPlate->isValid()) {
        return steelPlate;
    }
    return nullptr;
}

unique_ptr<CardGroup> CardGroupFactory::tryCreateConPairs(vector<Card>& cards, Game& game)
{
    auto conPairs = make_unique<ConPairs>(cards, game);
    if (conPairs->isValid()) {
        return conPairs;
    }
    return nullptr;
}