#include "BlackjackGame.h"
#include <iostream>

BlackjackGame::BlackjackGame()
{
    int n;
    std::cout << "Koliko igraca igra? ";
    std::cin >> n;

    for (int i = 0; i < n; i++)
    {
        std::string name;
        std::cout << "Ime igraca " << (i + 1) << ": ";
        std::cin >> name;
           
        players.push_back(std::make_unique<HumanPlayer>(name));
    }
}

void BlackjackGame::dealInitial()
{
    for (auto& p : players)
        p->clearHand();

    dealer.clearHand();

    for (int i = 0; i < 2; i++)
    {
        for (auto& p : players)
            p->hit(deck);

        dealer.hit(deck);
    }
}

void BlackjackGame::printInitialHands() const
{
    std::cout << "\n=== Pocetno dijeljenje ===\n";
    
    for (const auto& p : players)
    {
        std::cout << p->getName() << ": "
            << p->getHand().toString()
            << " (Value = " << p->getHand().getValue() << ")\n";
    }

    std::cout << "Dealer: "
        << dealer.getHand().toString()
        << " (Value = " << dealer.getHand().getValue() << ")\n";
}

void BlackjackGame::playersTurn()
{
    for (auto& p : players)
    {
        std::cout << "\n=== " << p->getName() << " na potezu ===\n";

        // CISTI BLACKJACK
        if (p->getHand().isBlackjack()) {
            std::cout << p->getName() << " ima Blackjack!\n";
            std::cout << "WIN (isplata x2.5)\n";
            p->win(2.5);
            continue;
        }

        while (!p->getHand().isBust())
        {
            Action a = p->decideAction(deck);

            // SPLIT
            if (a == Action::Split) {
                HumanPlayer* hp = dynamic_cast<HumanPlayer*>(p.get());
                if (hp) {
                    std::cout << hp->getName() << " split-a karte!\n";
                    hp->startSplit(deck);

                    for (Hand& hand : hp->splitHands) {
                        while (!hand.isBust()) {
                            std::cout << hp->getName() << " (split ruka): "
                                << hand.toString()
                                << " Value=" << hand.getValue()
                                << " -> [h]it/[s]tand? ";

                            char c; std::cin >> c;
                            if (c == 's') break;

                            hand.addCard(deck.draw());

                            std::cout << hp->getName() << " (split ruka): "
                                << hand.toString()
                                << " Value=" << hand.getValue() << "\n";
                        }

                        if (hand.isBust())
                            std::cout << hp->getName() << " (split ruka) bust!\n";
                    }
                }
                break;
            }

            // DOUBLE DOWN
            if (a == Action::DoubleDown) {
                std::cout << p->getName() << " double down-a!\n";

                p->placeBet(p->getBet());

                std::cout << p->getName() << " vuce kartu...\n";
                p->hit(deck);

                std::cout << p->getName() << ": "
                    << p->getHand().toString()
                    << " (Value=" << p->getHand().getValue() << ")\n";

                break;
            }

            // STAND
            if (a == Action::Stand)
            {
                std::cout << p->getName()
                    << " staje (value="
                    << p->getHand().getValue() << ")\n";
                break;
            }

            // HIT
            std::cout << p->getName() << " vuce kartu...\n";
            p->hit(deck);

            std::cout << p->getName() << ": "
                << p->getHand().toString()
                << " (Value=" << p->getHand().getValue() << ")\n";
        }

        if (p->getHand().isBust())
            std::cout << p->getName() << " bust!\n";
    }

    std::cout << "-------------------------\n";
}

bool BlackjackGame::anyPlayerAlive() const
{
    for (const auto& p : players)
        if (!p->getHand().isBust())
            return true;
    return false;
}

void BlackjackGame::dealerTurn()
{
    if (!anyPlayerAlive())
    {
        std::cout << "Svi igraci su bust. Dealer pobjeduje.\n";
        return;
    }

    std::cout << "\n=== Dealer na potezu ===\n";

    while (!dealer.getHand().isBust())
    {
        Action a = dealer.decideAction(deck);

        if (a == Action::Stand)
        {
            std::cout << "Dealer staje (Value="
                << dealer.getHand().getValue() << ")\n";
            break;
        }

        std::cout << "Dealer vuce kartu...\n";
        dealer.hit(deck);

        std::cout << "Dealer: "
            << dealer.getHand().toString()
            << " (Value=" << dealer.getHand().getValue() << ")\n";
    }

    if (dealer.getHand().isBust())
        std::cout << "Dealer bust!\n";

    std::cout << "-------------------------\n";
}

void BlackjackGame::printResult() const
{
    int dv = dealer.getHand().getValue();
    bool dbust = dealer.getHand().isBust();

    std::cout << "\n=== REZULTATI ===\n";
    std::cout << "Dealer: "
        << dealer.getHand().toString()
        << " (Value=" << dv << ")\n\n";

    for (const auto& p : players)
    {

        int pv = p->getHand().getValue();
        bool pbust = p->getHand().isBust();

        std::cout << p->getName() << ": "
            << p->getHand().toString()
            << " (Value=" << pv << ") -> ";

        if (pbust)
        {
            std::cout << "LOSE (Bust)";
            p->lose();
        }
        else if (dbust)
        {
            std::cout << "WIN (Dealer bust)";
            p->win(2.0);
        }
        else if (pv > dv)
        {
            std::cout << "WIN";
            p->win(2.0);
        }
        else if (pv < dv)
        {
            std::cout << "LOSE";
            p->lose();
        }
        else
        {
            std::cout << "PUSH";
            p->push();
        }

        std::cout << "\n";
    }
    std::cout << " -> Balance: " << p->getMoney() << "€" << "\n";
}

void BlackjackGame::playRound()
{
    deck.reset();
    deck.shuffle();

    std::cout << "\n=== ULOZI ZELJENU SVOTU NOVCA===\n";

    for (auto& p : players)
    {
        double ulog;
        std::cout << p->getName()
            << " ima" << p->getMoney() << " €"
            << ". Unesi ulog: ";
        std::cin >> ulog;

        p->placeBet(ulog);
    }

    dealInitial();
    printInitialHands();

    playersTurn();
    dealerTurn();

    printResult();
}