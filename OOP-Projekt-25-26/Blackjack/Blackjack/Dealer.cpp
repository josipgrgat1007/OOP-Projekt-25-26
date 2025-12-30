#include "Dealer.h"

Dealer::Dealer() : Player("Dealer"){}

Action Dealer::decideAction(const Deck& deck)
{
	if (hand.getValue() < 17)
		return Action::Hit;

	return Action::Stand;
}