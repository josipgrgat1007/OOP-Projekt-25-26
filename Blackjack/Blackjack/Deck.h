#ifndef DECK_H
#define DECK_H

#include <vector>
#include "Card.h"

class Deck
{
private:
	std::vector<Card> cards;

public:
	Deck();

	void reset();
	void shuffle();
	Card draw();

	bool empty() const;
};

#endif