#include "Player.h"

Player::Player(const std::string& name) : name(name) {}

const std::string& Player::getName() const
{
    return name;
}

Hand& Player::getHand()
{
    return hand;
}

const Hand& Player::getHand() const
{
    return hand;
}

void Player::clearHand()
{
    hand.clear();
}

void Player::hit(Deck& deck)
{
    hand.addCard(deck.draw());
}

void Player::placeBet(double amount)
{
    if (amount > money)
        amount = money;

    bet = amount;
    money -= bet;
}

void Player::win(double multiplier)
{
    money += bet * multiplier;
    bet = 0;
}

void Player::push()
{
    money += bet;
    bet = 0;
}

void Player::lose()
{
    bet = 0;
}