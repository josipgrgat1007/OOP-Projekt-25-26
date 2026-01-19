#include "Dealer.h"

Dealer::Dealer() : Player("Dealer"){}

Action Dealer::decideAction()
{
	if (getHand().getValue() < 17)
		return Action::Hit;

	return Action::Stand;
}