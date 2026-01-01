#include "HumanPlayer.h"
#include <iostream>

HumanPlayer::HumanPlayer(const std::string& name) : Player(name){}

void HumanPlayer::clearHand()
{
	hand.clear();
	splitHands.clear();
}

Action HumanPlayer::decideAction()
{
	Hand& hand = getHand();

	while (true)
	{
		std::cout << name << ": " << hand.toString()
			<< " (Value = " << hand.getValue() << ")";

		if (hand.isFirstMove()) 
			if (hand.canSplit()) 
				std::cout << "  -> [d]ouble/s[p]lit/[h]it/[s]tand?";
			else 
				std::cout << " -> [d]ouble/[h]it/[s]tand? ";
		else 
			std::cout << " -> [h]it/[s]tand? ";
		
		char c;
		std::cin >> c;

		if ((c == 'p' || c == 'P') && hand.canSplit()) return Action::Split;
		if ((c == 'd' || c == 'D') && hand.isFirstMove()) return Action::DoubleDown;
		if (c == 'h' || c == 'H') return Action::Hit;
		if (c == 's' || c == 'S') return Action::Stand;

		std::cout << "Upisi ponudeno slovo.\n";
	}
}

int HumanPlayer::getHandsCount() const
{
	return splitHands.empty() ? 1 : static_cast<int>(splitHands.size());
}

Hand& HumanPlayer::getHandByIndex(int index)
{
	if (splitHands.empty()) return hand;

	return splitHands.at(index);
}

const Hand& HumanPlayer::getHandByIndex(int index) const
{
	if (splitHands.empty()) return hand;

	return splitHands.at(index);
}

bool HumanPlayer::canSplitNow() const
{
	return splitHands.empty() && hand.canSplit();
}

bool HumanPlayer::tryStartSplit(Deck& deck)
{
	if (!canSplitNow()) 
		return false;

	if (bet > money) 
		return false;
	money -= bet;

	splitHands.clear();

	Hand first = hand;
	Card secondCard = first.moveSecondCard();

	first.addCard(deck.draw());

	Hand second;
	second.addCard(secondCard);
	second.addCard(deck.draw());

	splitHands.push_back(first);
	splitHands.push_back(second);

	hand.clear();

	return true;
}
