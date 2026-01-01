#include "Dealer.h"

Dealer::Dealer() : Player("Dealer"){}

Action Dealer::decideAction()
{
	if (hand.getValue() < 17)
		return Action::Hit;

	return Action::Stand;
}