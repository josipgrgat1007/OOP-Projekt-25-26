#ifndef AIPLAYER_H
#define AIPLAYER_H

#include "Player.h"

enum class AIType { Safe, Aggressive };

class AIPlayer : public Player
{
private:
	AIType type;

public:
	AIPlayer(const std::string& name, AIType t);

	Action decideAction() override;
	AIType getType() const { return type; }

	bool isAI() const override { return true; }
};

#endif