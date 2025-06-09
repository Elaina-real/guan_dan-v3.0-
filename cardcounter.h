#pragma once
#include <map>
#include <array>
#include <vector>
#include "card.h"

using namespace std;

class CardCounter {
private:
    // 记录每种牌的剩余数量 [花色][点数] = 数量
    array<array<int, 17>, 5> remainingCards;
    // 总牌数统计
    int totalRemainingCards;

public:
    CardCounter();

    // 重置记牌器（开始新一局游戏时）
    void reset();

    // 标记一张牌已经被打出
    void markCardPlayed(const Card& card);

    // 标记一组牌已经被打出
    void markCardsPlayed(const std::vector<Card>& cards);

    // 获取特定花色和点数的剩余牌数
    int getRemainingCount(Suit suit, Rank rank) const;

    // 获取特定点数的剩余牌数（所有花色）
    int getRemainingCountByRank(Rank rank) const;

    // 获取特定花色的剩余牌数（所有点数）
    int getRemainingCountBySuit(Suit suit) const;

    // 获取所有剩余的牌
    vector<Card> getAllRemainingCards() const;

    // 获取剩余牌总数
    int getTotalRemainingCards() const;

    // 显示记牌器统计信息
    void displayStatistics() const;
};