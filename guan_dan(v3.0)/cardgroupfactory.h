#pragma once
#include "game.h"
#include "card.h"
#include "cardgroup.h"
#include "player.h"
#include <memory>

class CardGroupFactory
{
public:
    static unique_ptr<CardGroup> createCardGroup(vector<Card>& cards, Game& game);

private:
    static unique_ptr<CardGroup> tryCreateSingle(vector<Card>& cards, Game& game);
    static unique_ptr<CardGroup> tryCreatePair(vector<Card>& cards, Game& game);
    static unique_ptr<CardGroup> tryCreateTriple(vector<Card>& cards, Game& game);
    static unique_ptr<CardGroup> tryCreateTripleWithPair(vector<Card>& cards, Game& game);
    static unique_ptr<CardGroup> tryCreateBomb(vector<Card>& cards, Game& game);
    static unique_ptr<CardGroup> tryCreateTwoJokers(vector<Card>& cards, Game& game);
    static unique_ptr<CardGroup> tryCreateFourJokers(vector<Card>& cards, Game& game);
    static unique_ptr<CardGroup> tryCreateStraight(vector<Card>& cards, Game& game);
    static unique_ptr<CardGroup> tryCreateFlush(vector<Card>& cards, Game& game);
    static unique_ptr<CardGroup> tryCreateSteelPlate(vector<Card>& cards, Game& game);
    static unique_ptr<CardGroup> tryCreateConPairs(vector<Card>& cards, Game& game);
};