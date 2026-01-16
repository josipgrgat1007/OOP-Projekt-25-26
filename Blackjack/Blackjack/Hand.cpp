#include "Hand.h"

void Hand::addCard(const Card& card)
{
	cards.push_back(card);
}

void Hand::clear()
{
	cards.clear();
}

int Hand::size() const
{
	return static_cast<int>(cards.size());
}

const Card& Hand::getCard(int index) const
{
	return cards.at(index);
}

int Hand::getValueForSplit(const Card& c) const
{
	Card::Number number = c.getNumber();

	if (number == Card::Number::Ace)
		return 11;
	if (number == Card::Number::Jack || number == Card::Number::Queen || number == Card::Number::King)
		return 10;

	return static_cast<int>(number);
}

int Hand::getValue() const
{
	int total = 0;
	int aceCount = 0;

	for (const Card& c : cards)
	{
		Card::Number n = c.getNumber();
	
		if (n == Card::Number::Ace)
		{
			aceCount++;
		}
		else if (n == Card::Number::Jack || n == Card::Number::Queen || n == Card::Number::King)
		{
			total += 10;
		}
		else
		{
			total += static_cast<int>(n);
		}
	}

	for (int i = 0; i < aceCount; i++)
	{
		if (total + 11 <= 21)
			total += 11;
		else
			total += 1;
	}

	return total;
}

bool Hand::isBust() const
{
	return getValue() > 21;
}

bool Hand::isBlackjack() const
{
	return (cards.size() == 2 && getValue() == 21);
}

bool Hand::isFirstMove() const
{
	return cards.size() == 2;
}

bool Hand::canSplit() const 
{
	if (cards.size() != 2)
		return false;

	return getValueForSplit(cards[0]) == getValueForSplit(cards[1]);
}

Card Hand::moveSecondCard()
{
	Card c = cards.back();
	cards.pop_back();

	return c;
}

std::string Hand::toString() const
{
	std::string s;

	for (size_t i = 0; i < cards.size(); i++)
	{
		s += cards[i].toString();
		if (i + 1 < cards.size())
			s += ", ";
	}

	return s;
}