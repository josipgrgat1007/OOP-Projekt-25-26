#ifndef BLACKJACKGAME_H
#define BLACKJACKGAME_H

#include <vector>
#include <memory>

#include "Deck.h"
#include "HumanPlayer.h"
#include "Dealer.h"

class BlackjackGame
{
private:
    std::vector<std::unique_ptr<Player>> players;
    Deck deck;
    Dealer dealer;

    void dealInitial();
    void printInitialHands() const;

    void playersTurn();
    void dealerTurn();
    bool anyPlayerAlive() const;

    void printResult() const;
    void takeBets();
    void printDealerHidden() const;
    void delay(int ms);

public:
    BlackjackGame();

    bool playRound();   // odigra jednu rundu
};

#endif