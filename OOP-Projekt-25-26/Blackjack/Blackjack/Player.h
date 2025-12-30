#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Deck.h"
#include "Hand.h"
#include "Action.h"

class Player
{
protected:
	std::string name;
	Hand hand;

	double money = 100;
	double bet = 0;

public:
	Player(const std::string& name);
	virtual ~Player() = default;

	const std::string& getName() const;

	Hand& getHand();
	const Hand& getHand() const;

	void clearHand();
	
	virtual Action decideAction(const Deck& deck) = 0;

	void hit(Deck& deck);

	double getMoney() const { return money; }
	double getBet() const { return bet; }

	void placeBet(double amount);
	void win(double multiplier);
	void push();
	void lose();
};

#endif