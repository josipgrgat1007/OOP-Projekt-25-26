#include "Player.h"

#include <stdexcept>

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
    if (amount <= 0)
        throw std::invalid_argument("Bet mora biti veci od 0.");
    if (amount > money)
        throw std::invalid_argument("Nemate dovoljno novca.");

    bet = amount;
    money -= bet;
}

void Player::win(double multiplier)
{
    money += bet * multiplier;
    clearBet();
}

void Player::push()
{
    money += bet;
    clearBet();
}

void Player::lose()
{
    clearBet();
}

void Player::addMoney(double amount)
{
    if (money <= 0) return;
    money += amount;
}

void Player::clearBet()
{
    bet = 0;
}

bool Player::addToBet(double amount)
{
    if (amount <= 0 || money < amount)
        return false;

    money -= amount;
    bet += amount;
    return true;
}