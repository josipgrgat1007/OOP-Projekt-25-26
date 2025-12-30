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

	int getValue() const;
	bool isBust() const;
	bool isBlackjack() const;

	std::string toString() const;
	int size() const;

	bool isFirstMove() const;

	bool canSplit() const;
	Card moveSecondCard();
};

#endif