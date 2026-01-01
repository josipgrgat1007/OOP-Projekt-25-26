#ifndef CARD_H
#define CARD_H

#include <string>

class Card
{
public:
	enum class Suit
	{
		Hearts,
		Diamonds,
		Spades,
		Clubs
	};

	enum class Number
	{
		Two = 2,
		Three,
		Four,
		Five,
		Six,
		Seven,
		Eight,
		Nine,
		Ten,
		Jack,
		Queen,
		King,
		Ace
	};

private:
	Suit suit;
	Number number;

public:
	Card(Suit suit, Number number);

	Suit getSuit() const;
	Number getNumber() const;

	std::string toString() const;
};

#endif