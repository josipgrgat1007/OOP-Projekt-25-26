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
    void printHandLine(const std::string& p, const Hand& h) const;
    void drawWithDelay(const std::string& p);
    Action askHitStand(const std::string& p);
    void takeBets();
    void dealInitial();
    void printDealerHidden() const;
    void printInitialHands() const;

    void playersTurn();
    void dealerTurn();
    void resolveBets();
    void printResult() const;

    bool anyPlayerAlive() const;

    void delay(int ms);

public:
    BlackjackGame();
    bool playRound();
};

#endif