#include "BlackjackGame.h"
#include "HumanPlayer.h"
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>

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

void BlackjackGame::takeBets()
{
    for (auto& p : players)
    {
        if (p->getMoney() <= 0)
            continue;

        double amount;

        while (true)
        {
            std::cout << p->getName()
                << " ima " << p->getMoney()
                << ". Unesi ulog: ";

            std::cin >> amount;

            if (amount <= 0)
            {
                std::cout << "Ulog mora biti veci od 0.\n";
            }
            else if (amount > p->getMoney())
            {
                std::cout << "Nemate dovoljno novca. Maksimalno: "
                    << p->getMoney() << "\n";
            }
            else
            {
                break;
            }
        }

        p->placeBet(amount);
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

void BlackjackGame::printDealerHidden() const
{
    const Hand& h = dealer.getHand();

    if (h.size() == 0) return;

    std::cout << "[?], ";
    if (h.size() >= 2)
        std::cout << " " << h.getCard(1).toString();
    std::cout << "\n";
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

    std::cout << "Dealer: ";
    printDealerHidden();
}

void BlackjackGame::playersTurn()
{
    for (auto& p : players)
    {
        std::cout << "\n=== " << p->getName() << " na potezu ===\n";

        for (int i = 0; i < p->getHandsCount(); i++)
        {
            bool splitMode(p->getHandsCount() > 1);

            Hand& h0 = p->getHandByIndex(i);

            if (splitMode)
            {
                std::cout << "\n" << p->getName() << " [Split " << (i + 1) << "]: "
                    << h0.toString() << " (Value = " << h0.getValue() << ")\n";
            }

            //BLACKJACK
            if (h0.isBlackjack())
            {
                if (splitMode)
                    std::cout << p->getName() << " [Split " << (i + 1) << "] ima Blackjack!\n";
                else
                    std::cout << p->getName() << " ima Blackjack!\n";

                std::cout << "WIN (isplata x2.5)\n";
                p->win(2.5);
                continue;
            }

            while (true)
            {
                Hand& h = p->getHandByIndex(i);

                if (h.isBust())
                    break;

                splitMode = (p->getHandsCount() > 1);
                Action a = Action::Stand;

                if (splitMode)
                {
                    std::cout << p->getName() << " [Split " << (i + 1) << "] -> [h]it/[s]tand? ";

                    char c;
                    std::cin >> c;

                    if (c == 'h' || c == 'H') a = Action::Hit;
                    else if (c == 's' || c == 'S') a = Action::Stand;
                    else
                    {
                        std::cout << "Upisi h ili s.\n";
                        continue;
                    }
                }
                else
                {
                    a = p->decideAction();
                }

                //SPLIT
                if (!splitMode && a == Action::Split)
                {
                    if (p->canSplitNow() && p->tryStartSplit(deck))
                    {
                        std::cout << p->getName() << " split-a karte!\n";

                        std::cout << "Split 1: " << p->getHandByIndex(0).toString()
                            << " (Value = " << p->getHandByIndex(0).getValue() << ")\n";
                        std::cout << "Split 2: " << p->getHandByIndex(1).toString()
                            << " (Value = " << p->getHandByIndex(1).getValue() << ")\n";

                        continue;
                    }
                    else
                    {
                        std::cout << "Split nije moguc.\n";
                        continue;
                    }
                }

                //DOUBLE DOWN
                if (!splitMode && a == Action::DoubleDown)
                {
                    std::cout << p->getName() << " double down-a!\n";

                    double add = p->getBet();

                    std::cout << p->getName() << " ulaze jos " << add << "\n";

                    if (!p->addToBet(add))
                    {
                        std::cout << "Nemate dovoljno novca za double down.\n";
                        continue;
                    }

                    std::cout << p->getName() << " vuce kartu...\n";
                    delay(800);
                    p->hit(deck);

                    Hand& hDD = p->getHandByIndex(0);

                    std::cout << p->getName() << ": " << hDD.toString()
                        << " (Value = " << hDD.getValue() << ")\n";

                    std::cout << p->getName() << " staje nakon double down-a.\n";
                    break;
                }

                //STAND
                if (a == Action::Stand)
                {
                    if (splitMode)
                        std::cout << p->getName() << " [Split " << (i + 1)
                        << "] staje (Value = " << h.getValue() << ")\n";
                    else
                        std::cout << p->getName() << " staje (Value = " << h.getValue() << ")\n";

                    break;
                }

                //HIT
                if (splitMode)
                {
                    std::cout << p->getName() << " [Split " << (i + 1)
                        << "] vuce kartu...\n";
                    delay(800);
                    h.addCard(deck.draw());
                }
                else
                {
                    std::cout << p->getName() << " vuce kartu...\n";
                    delay(800);
                    p->hit(deck);
                }

                std::cout << p->getName();
                if (splitMode)
                    std::cout << " [Split " << (i + 1) << "]";
                std::cout << ": " << h.toString() << " (Value = " << h.getValue() << ")\n";
            }

            Hand& end = p->getHandByIndex(i);
            if (end.isBust())
            {
                if (p->getHandsCount() > 1)
                    std::cout << p->getName() << " [Split " << (i + 1) << "] bust!\n";
                else
                    std::cout << p->getName() << " bust!\n";
            }
        }
    }
    std::cout << "-------------------------\n";
}

void BlackjackGame::dealerTurn()
{
    if (!anyPlayerAlive())
    {
        std::cout << "Svi igraci su bust. Dealer pobjeduje.\n";
        return;
    }

    std::cout << "\n=== Dealer otkriva skrivenu kartu. ===\n";
    std::cout << "Dealer: " << dealer.getHand().toString()
        << " (Value = " << dealer.getHand().getValue() << ")\n";

    while (!dealer.getHand().isBust())
    {
        Action a = dealer.decideAction();

        if (a == Action::Stand)
        {
            std::cout << "Dealer staje (Value="
                << dealer.getHand().getValue() << ")\n";
            break;
        }

        std::cout << "Dealer vuce kartu...\n";
        delay(800);

        dealer.hit(deck);

        std::cout << "Dealer: "
            << dealer.getHand().toString()
            << " (Value=" << dealer.getHand().getValue() << ")\n";

        delay(800);
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
        if (p->getHandsCount() == 1)
        {
            Hand& h = p->getHandByIndex(0);

            std::cout << p->getName() << ": "
                << h.toString()
                << " (Value=" << h.getValue() << ") -> ";

            if (h.isBust())
            {
                std::cout << "LOSE (Bust)\n";
                p->lose();
            }
            else if (dbust)
            {
                std::cout << "WIN (Dealer bust)\n";
                p->win(2.0);
            }
            else if (h.getValue() > dv)
            {
                std::cout << "WIN\n";
                p->win(2.0);
            }
            else if (h.getValue() < dv)
            {
                std::cout << "LOSE\n";
                p->lose();
            }
            else
            {
                std::cout << "PUSH\n";
                p->push();
            }

            std::cout << " -> Balance: " << p->getMoney() << "\n\n";
            continue;
        }

        std::cout << p->getName() << ":\n";

        double baseBet = p->getBet();

        for (int i = 0; i < p->getHandsCount(); i++)
        {
            Hand& h = p->getHandByIndex(i);

            std::cout << "  Split " << (i + 1) << ": "
                << h.toString()
                << " (Value=" << h.getValue() << ") -> ";

            if (h.isBust())
            {
                std::cout << "LOSE (Bust)";
            }
            else if (dbust)
            {
                std::cout << "WIN (Dealer bust)";
                p->addMoney(baseBet * 2);
            }
            else if (h.getValue() > dv)
            {
                std::cout << "WIN";
                p->addMoney(baseBet * 2);
            }
            else if (h.getValue() < dv)
            {
                std::cout << "LOSE";
            }
            else
            {
                std::cout << "PUSH";
                p->addMoney(baseBet);
            }

            std::cout << "\n";
        }
        p->clearBet();

        std::cout << " -> Balance: " << p->getMoney() << "\n\n";
    }
}

bool BlackjackGame::anyPlayerAlive() const
{
    for (const auto& p : players)
        for (int i = 0; i < p->getHandsCount(); i++)
            if (!p->getHandByIndex(i).isBust())
                return true;
    return false;
}

void BlackjackGame::delay(int ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

bool BlackjackGame::playRound()
{
    
    players.erase(
        std::remove_if(players.begin(), players.end(),
            [](const std::unique_ptr<Player>& p)
            {
                return p->getMoney() <= 0;
            }),
        players.end()
    );

    if (players.empty())
    {
        std::cout << "Nema vise igraca s novcem. Igra zavrsava.\n";
        return false;
    }
    
    deck.reset();
    deck.shuffle();

    std::cout << "\n=== ULOZI ZELJENU SVOTU NOVCA===\n";
    
    takeBets();
    dealInitial();
    printInitialHands();

    playersTurn();
    dealerTurn();

    printResult();

    return true;
}