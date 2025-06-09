#pragma once
#include <map>
#include <array>
#include <vector>
#include "card.h"

using namespace std;

class CardCounter {
private:
    // ��¼ÿ���Ƶ�ʣ������ [��ɫ][����] = ����
    array<array<int, 17>, 5> remainingCards;
    // ������ͳ��
    int totalRemainingCards;

public:
    CardCounter();

    // ���ü���������ʼ��һ����Ϸʱ��
    void reset();

    // ���һ�����Ѿ������
    void markCardPlayed(const Card& card);

    // ���һ�����Ѿ������
    void markCardsPlayed(const std::vector<Card>& cards);

    // ��ȡ�ض���ɫ�͵�����ʣ������
    int getRemainingCount(Suit suit, Rank rank) const;

    // ��ȡ�ض�������ʣ�����������л�ɫ��
    int getRemainingCountByRank(Rank rank) const;

    // ��ȡ�ض���ɫ��ʣ�����������е�����
    int getRemainingCountBySuit(Suit suit) const;

    // ��ȡ����ʣ�����
    vector<Card> getAllRemainingCards() const;

    // ��ȡʣ��������
    int getTotalRemainingCards() const;

    // ��ʾ������ͳ����Ϣ
    void displayStatistics() const;
};