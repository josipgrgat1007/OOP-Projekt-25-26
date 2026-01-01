#ifndef BLACKJACKGAME_H
#define BLACKJACKGAME_H

#include <vector>
#include <memory>

#include "Deck.h"
#include "Dealer.h"

class Player;

class BlackjackGame
{
private:
    std::vector<std::unique_ptr<Player>> players;
    Deck deck;
    Dealer dealer;

    void takeBets();
    void dealInitial();
    void printDealerHidden() const;
    void printInitialHands() const;

    void playersTurn();
    void dealerTurn();
    void printResult() const;

    bool anyPlayerAlive() const;

    void delay(int ms);

public:
    BlackjackGame();
    bool playRound();
};

#endif