#ifndef HAND_H
#define HAND_H

#include <vector>
#include <string>
#include "Card.h"

class Hand
{
private:
	std::vector<Card> cards;

public:
	Hand() = default;

	void addCard(const Card& card);
	void clear();

	int size() const;
	const Card& getCard(int index) const;

	int getValueForSplit(const Card& c) const;
	int getValue() const;
	bool isBust() const;
	bool isBlackjack() const;
	bool isFirstMove() const;

	bool canSplit() const;
	Card moveSecondCard();

	std::string toString() const;
};

#endif