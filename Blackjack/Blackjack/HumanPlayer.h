#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "Player.h"
#include <vector>

class HumanPlayer : public Player
{
private:
    std::vector<Hand> splitHands;

public:
    HumanPlayer(const std::string& name);

    Action decideAction() override;

    int getHandsCount() const override;
    Hand& getHandByIndex(int index) override;
    const Hand& getHandByIndex(int index) const override;

    bool canSplitNow() const override;
    bool tryStartSplit(Deck& deck) override;
};

#endif