#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "Player.h"
#include <vector>

class HumanPlayer : public Player
{
public:
    HumanPlayer(const std::string& name);

    Action decideAction(const Deck& deck) override;

    void startSplit(Deck& deck);

    std::vector<Hand> splitHands;
};

#endif