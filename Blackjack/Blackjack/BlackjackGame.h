#ifndef BLACKJACKGAME_H
#define BLACKJACKGAME_H

#include <vector>
#include <string>
#include <memory>

#include "Deck.h"
#include "Dealer.h"

class Player;

class BlackjackGame
{
private:
    std::string dealerResultLine;
    std::vector<std::string> resultLines;
    std::vector<std::unique_ptr<Player>> players;
    Deck deck;
    Dealer dealer;
    static constexpr int shuffle_threshold = 15;

    std::string handLabel(const Player& p, int handIndex) const;
    void printHand(const Player& p, int handIndex) const;
    std::string basicOutcome(const Hand& h, int dv, bool dbust) const;
    Action askHitStand(const std::string& p);
    void takeBets();
    void dealInitial();
    void printInitialHands() const;

    void playersTurn();
    void dealerTurn();
    void resolveBets();
    void printResult() const;

    bool anyPlayerAlive() const;
    void delay(int ms) const;

public:
    BlackjackGame();
    bool playRound();
};

#endif