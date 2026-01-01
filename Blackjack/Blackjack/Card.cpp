#include "Card.h"

Card::Card(Suit suit, Number number) : suit(suit), number(number){}

Card::Suit Card::getSuit() const
{
	return suit;
}

Card::Number Card::getNumber() const
{
	return number;
}

std::string Card::toString() const
{
	std::string suitStr;
	std::string numberStr;

	switch (suit)
	{
	case Suit::Hearts: suitStr = "Hearts"; break;
	case Suit::Diamonds: suitStr = "Diamonds"; break;
	case Suit::Spades: suitStr = "Spades"; break;
	case Suit::Clubs: suitStr = "Clubs"; break;
	}

	switch (number)
	{
	case Number::Two: numberStr = "2"; break;
	case Number::Three: numberStr = "3"; break;
	case Number::Four: numberStr = "4"; break;
	case Number::Five: numberStr = "5"; break;
	case Number::Six: numberStr = "6"; break;
	case Number::Seven: numberStr = "7"; break;
	case Number::Eight: numberStr = "8"; break;
	case Number::Nine: numberStr = "9"; break;
	case Number::Ten: numberStr = "10"; break;
	case Number::Jack: numberStr = "J"; break;
	case Number::Queen: numberStr = "Q"; break;
	case Number::King: numberStr = "K"; break;
	case Number::Ace: numberStr = "A"; break;
	}

	return numberStr + " of " + suitStr;
}