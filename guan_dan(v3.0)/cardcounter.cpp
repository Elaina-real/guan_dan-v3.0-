#include "cardcounter.h"
#include <iostream>
#include <iomanip>

CardCounter::CardCounter() {
    reset();
}

void CardCounter::reset() {
    // 初始化所有牌的计数
    // 掼蛋游戏使用两副牌，每副牌包含52张普通牌和2张王
    for (int s = 0; s < 4; s++) {
        for (int r = two; r <= ace; r++) {
            remainingCards[s][r] = 2; // 每种普通牌有2张
        }
    }

    // 初始化大小王
    remainingCards[Joker][smal] = 2;
    remainingCards[Joker][big] = 2;

    // 计算总牌数 = (13种点数 × 4种花色 + 2种王) × 2副 = 108张
    totalRemainingCards = 108;
}

void CardCounter::markCardPlayed(const Card& card) {
    Suit suit = card.getSuit();
    Rank rank = card.getRank();

    if (remainingCards[suit][rank] > 0) {
        remainingCards[suit][rank]--;
        totalRemainingCards--;
    }
}

void CardCounter::markCardsPlayed(const vector<Card>& cards) {
    for (const auto& card : cards) {
        markCardPlayed(card);
    }
}

int CardCounter::getRemainingCount(Suit suit, Rank rank) const {
    return remainingCards[suit][rank];
}

int CardCounter::getRemainingCountByRank(Rank rank) const {
    int count = 0;

    // 针对普通牌
    if (rank >= two && rank <= ace) {
        for (int s = 0; s < 4; s++) { // 心、方、梅、黑四种花色
            count += remainingCards[s][rank];
        }
    }
    // 针对王牌
    else if (rank == smal || rank == big) {
        count = remainingCards[Joker][rank];
    }

    return count;
}

int CardCounter::getRemainingCountBySuit(Suit suit) const {
    int count = 0;
    if (suit == Joker) {
        count = remainingCards[suit][smal] + remainingCards[suit][big];
    }
    else {
        for (int r = two; r <= ace; r++) {
            count += remainingCards[suit][r];
        }
    }
    return count;
}

vector<Card> CardCounter::getAllRemainingCards() const {
    vector<Card> cards;

    // 添加所有剩余的普通牌
    for (int s = 0; s < 4; s++) {
        for (int r = two; r <= ace; r++) {
            for (int i = 0; i < remainingCards[s][r]; i++) {
                cards.push_back(Card(static_cast<Suit>(s), static_cast<Rank>(r)));
            }
        }
    }

    // 添加剩余的大小王
    for (int i = 0; i < remainingCards[Joker][smal]; i++) {
        cards.push_back(Card(Joker, smal));
    }
    for (int i = 0; i < remainingCards[Joker][big]; i++) {
        cards.push_back(Card(Joker, big));
    }

    return cards;
}

int CardCounter::getTotalRemainingCards() const {
    return totalRemainingCards;
}

void CardCounter::displayStatistics() const {
    std::cout << "\n========== 记牌器统计 ==========" << std::endl;
    std::cout << "剩余牌总数: " << totalRemainingCards << "/108" << std::endl << std::endl;

    // 显示按点数统计的剩余牌
    std::cout << "按点数统计: " << std::endl;
    std::cout << "-----------------" << std::endl;
    std::cout << "点数\t剩余数量" << std::endl;
    for (int r = two; r <= ace; r++) {
        std::cout << std::left << std::setw(8);
        switch (r) {
        case jack: cout << "J"; break;
        case queen: cout << "Q"; break;
        case king: cout << "K"; break;
        case ace: cout << "A"; break;
        default: cout << r; break;
        }
        cout << getRemainingCountByRank(static_cast<Rank>(r)) << "/8" << endl;
    }
    cout << "小王\t" << remainingCards[Joker][smal] << "/2" << endl;
    cout << "大王\t" << remainingCards[Joker][big] << "/2" << endl;
    cout << "\n按花色统计: " << endl;
    cout << "-----------------" << endl;
    cout << "花色\t剩余数量" << endl;
    cout << "♥\t" << getRemainingCountBySuit(Heart) << "/26" << endl;
    cout << "♦\t" << getRemainingCountBySuit(Diamond) << "/26" << endl;
    cout << "♣\t" << getRemainingCountBySuit(Club) << "/26" << endl;
    cout << "♠\t" << getRemainingCountBySuit(Spade) << "/26" << endl;
    cout << "王牌\t" << getRemainingCountBySuit(Joker) << "/4" << endl;
    cout << "================================" << endl;
}