#include "Deck.h"
#include <algorithm>
#include <random>
#include <stdexcept>

Deck::Deck()
{
    reset();
    shuffle();
}

void Deck::reset()
{
    cards.clear();

    for (int i = 0; i < 4; i++)
    {
        for (int j = 2; j <= 14; j++)
        {
            cards.emplace_back(
                static_cast<Card::Suit>(i),
                static_cast<Card::Number>(j)
            );
        }
    }
}

void Deck::shuffle()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::shuffle(cards.begin(), cards.end(), gen);
}

int Deck::remaining() const
{
    return cards.size();
}

Card Deck::draw()
{
    if(cards.empty())
        throw std::runtime_error("Deck is empty!");

    Card card = cards.back();
    cards.pop_back();
    return card;
}

bool Deck::empty() const
{
    return cards.empty();
}