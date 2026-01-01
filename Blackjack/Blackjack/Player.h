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

	double money{ 100 };
	double bet{ 0 };

public:
	Player(const std::string& name);
	virtual ~Player() = default;

	const std::string& getName() const;

	Hand& getHand();
	const Hand& getHand() const;
	
	double getMoney() const { return money; }
	double getBet() const { return bet; }

	void clearHand();
	void hit(Deck& deck);

	void placeBet(double amount);
	void win(double multiplier);
	void push();
	void lose();
	void addMoney(double amount);
	void clearBet();
	bool addToBet(double amount);
	
	virtual Action decideAction() = 0;

	virtual int getHandsCount() const { return 1; }
	virtual Hand& getHandByIndex(int index)
	{
		return hand;
	}

	virtual const Hand& getHandByIndex(int index) const
	{
		return hand;
	}

	virtual bool canSplitNow() const { return false; }
	virtual bool tryStartSplit(Deck& deck) { return false; }
};

#endif