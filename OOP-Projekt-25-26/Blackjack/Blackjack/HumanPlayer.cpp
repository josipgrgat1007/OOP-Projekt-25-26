#include "HumanPlayer.h"
#include <iostream>

HumanPlayer::HumanPlayer(const std::string& name) : Player(name){}

Action HumanPlayer::decideAction(const Deck& deck)
{
	Hand& hand = getHand();

	while (true)
	{
		std::cout << name << " (" << hand.toString() << ") Value = " << hand.getValue()
			<< " -> ";

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

void HumanPlayer::startSplit(Deck& deck)
{
	splitHands.clear();

	Hand first = getHand();
	Card secondCard = first.moveSecondCard();

	Hand second;
	second.addCard(secondCard);

	first.addCard(deck.draw());
	second.addCard(deck.draw());

	splitHands.push_back(first);
	splitHands.push_back(second);
}


