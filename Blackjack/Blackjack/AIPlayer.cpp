#include "AIPlayer.h"

AIPlayer::AIPlayer(const std::string& name, AIType t)
    : Player(name), type(t) {}

Action AIPlayer::decideAction()
{
    int v = getHand().getValue();

    if (getHandsCount() == 1 && canSplitNow())
        return Action::Split;

    if (getHandsCount() == 1)
    {
        if (type == AIType::Safe)
        {
            if (v == 10 || v == 11)
                return Action::DoubleDown;
        }
        else
        {
            if (v == 9 || v == 10 || v == 11)
                return Action::DoubleDown;
        }
    }

    if (type == AIType::Safe)
    {
        if (v <= 15) return Action::Hit;
        return Action::Stand;
    }
    else
    {
        if (v <= 17) return Action::Hit;
        return Action::Stand;
    }
}