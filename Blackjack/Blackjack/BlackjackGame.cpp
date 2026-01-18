#include "BlackjackGame.h"
#include "HumanPlayer.h"
#include "AIPlayer.h"
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <cmath>

BlackjackGame::BlackjackGame()
{
    int n;
    std::cout << "Koliko igraca igra (1-5)? ";

    if (!(std::cin >> n))
        throw std::runtime_error("Neispravan unos!");
    if (n <= 0 || n > 5)
        throw std::runtime_error("Broj igraca mora biti izmedju 1 i 5!");

    for (int i = 0; i < n; i++)
    {
        std::string name;
        std::cout << "Ime igraca " << (i + 1) << ": ";
        std::cin >> name;
           
        players.push_back(std::make_unique<HumanPlayer>(name));
    }

    int aiIndex = 1;
    for (int i = n; i < 5; i++)
    {
        AIType t = (aiIndex % 2 == 1) ? AIType::Safe : AIType::Aggressive;
        std::string aiName = "AIPlayer_" + std::to_string(aiIndex);

        std::cout << "Dodan: " << aiName
            << " (" << (t == AIType::Safe ? "Safe" : "Aggressive") << ")\n";

        players.push_back(std::make_unique<AIPlayer>(aiName, t));
        aiIndex++;
        delay(500);
    }

    deck.reset();
    deck.shuffle();
}

std::string BlackjackGame::handLabel(const Player& p, int handIndex) const
{
    std::string label = p.getName();
    if (p.getHandsCount() > 1)
        label += " [Split " + std::to_string(handIndex + 1) + "]";
    return label;
}

void BlackjackGame::printHand(const Player& p, int handIndex) const
{
    const Hand& h = p.getHandByIndex(handIndex);

    std::cout << handLabel(p, handIndex) << ": " << h.toString()
        << " (Value = " << h.getValue() << ")\n";
}

std::string BlackjackGame::basicOutcome(const Hand& h, int dv, bool dbust) const
{
    if (h.isBust()) return "LOSE (Bust)";
    if (dbust) return "WIN (Dealer bust)";
    if (h.getValue() > dv) return "WIN";
    if (h.getValue() < dv) return "LOSE";
    return "PUSH";
}

Action BlackjackGame::askHitStand(const std::string& p)
{
    while (true)
    {
        std::cout << p << " -> [h]it/[s]tand? ";
        char c;
        std::cin >> c;

        if (c == 'h' || c == 'H') return Action::Hit;
        if (c == 's' || c == 'S') return Action::Stand;

        std::cout << "Upisi h ili s.\n";
    }
}

void BlackjackGame::takeBets()
{
    for (auto& p : players)
    {
        if (p->getMoney() <= 0)
            continue;

        if (p->isAI())
        {
            std::cout << p->getName()
                << " ima " << p->getMoney();

            double money = p->getMoney();
            
            double minPercent = 0.05;
            double maxPercent = 0.30;

            double percent = minPercent + (double)std::rand() / RAND_MAX * (maxPercent - minPercent);

            double amount = std::round(money * percent);

            if (amount < 1) amount = 1;
            if (amount > money) amount = money;

            p->placeBet(amount);
            std::cout << " i ulaze " << amount << "\n";
            delay(800);
            continue;
        }
        
        double amount;
        while (true)
        {
            std::cout << p->getName()
                << " ima " << p->getMoney()
                << ". Unesi ulog: ";

            if (!(std::cin >> amount))
            {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Neispravan unos. Unesi broj.\n";
                continue;
            }

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
            p->hit(deck, 0);

        dealer.hit(deck, 0);
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
        delay(800);
    }

    const Hand& dh = dealer.getHand();
    std::cout << "Dealer: ";
    
    if (dh.size() == 0)
    {
        std::cout << "\n";
        return;
    }

    std::cout << "[?]";
    if (dh.size() >= 2)
        std::cout << ", " << dh.getCard(1).toString();
    std::cout << "\n";

    delay(800);
}

void BlackjackGame::playersTurn()
{
    for (auto& p : players)
    {
        std::cout << "\n=== " << p->getName() << " na potezu ===\n";

        for (int i = 0; i < p->getHandsCount(); i++)
        {
            bool didSplit = false;

            printHand(*p, i);

            Hand& h = p->getHandByIndex(i);

            // BLACKJACK
            if (p->getHandsCount() == 1 && h.isBlackjack())
            {
                std::cout << p->getName() << " ima Blackjack! \n";
                std::cout << "-------------------------\n";
                continue;
            }

            while (!h.isBust())
            {
                std::string label = handLabel(*p, i);

                if (h.getValue() == 21)
                {
                    std::cout << label << " ima 21 i staje.\n";
                    std::cout << "-------------------------\n";
                    break;
                }

                Action a = (p->getHandsCount() > 1) 
                    ? askHitStand(label) 
                    : p->decideAction();

                if (p->getHandsCount() == 1)
                {
                    // SPLIT
                    if (a == Action::Split)
                    {
                        if (!p->canSplitNow())
                        {
                            std::cout << "Split nije moguc (karte nisu iste).\n";
                            continue;
                        }

                        if (p->getMoney() < p->getBet())
                        {
                            std::cout << "Split nije moguc (nedovoljno novca).\n";
                            continue;
                        }

                        if (p->tryStartSplit(deck))
                        {
                            std::cout << label << " split-a karte!\n";
                            printHand(*p, 0);
                            printHand(*p, 1);
                            std::cout << "-------------------------\n";

                            didSplit = true;
                            break;
                        }

                        std::cout << "Split nije moguc.\n";
                        continue;
                    }

                    // DOUBLE DOWN
                    if (a == Action::DoubleDown)
                    {
                        double add = p->getBet();
                        std::cout << label << " double down-a (jos " << add << ").\n";

                        if (!p->addToBet(add))
                        {
                            std::cout << "Nemate dovoljno novca za double down.\n";
                            continue;
                        }

                        std::cout << label << " vuce kartu...\n";
                        delay(800);

                        p->hit(deck, i);
                        printHand(*p, i);

                        std::cout << label << " staje nakon double down-a.\n";
                        std::cout << "-------------------------\n";
                        break;
                    }
                }

                // STAND
                if (a == Action::Stand)
                {
                    std::cout << label << " staje (Value = " << h.getValue() << ")\n";
                    std::cout << "-------------------------\n";
                    break;
                }

                // HIT
                std::cout << label << " vuce kartu...\n";
                delay(800);

                p->hit(deck, i);
                printHand(*p, i);
            }

            if (didSplit)
            {
                i = -1;
                continue;
            }

            if (!didSplit && h.isBust())
            {
                std::cout << handLabel(*p, i) << " bust!\n";
                std::cout << "-------------------------\n";
                delay(800);
            }
        }

        delay(1000);
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

    if (dealer.getHand().isBlackjack())
    {
        std::cout << "Dealer ima BLACKJACK!\n";
        std::cout << "-------------------------\n";
        return;
    }

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

        dealer.hit(deck, 0);

        std::cout << "Dealer: "
            << dealer.getHand().toString()
            << " (Value=" << dealer.getHand().getValue() << ")\n";

        delay(800);
    }

    if (dealer.getHand().isBust())
        std::cout << "Dealer bust!\n";

    std::cout << "-------------------------\n";
}

void BlackjackGame::resolveBets()
{
    resultLines.clear();

    int dv = dealer.getHand().getValue();
    bool dbust = dealer.getHand().isBust();
    bool dealerBJ = dealer.getHand().isBlackjack();

    dealerResultLine = "Dealer: " + dealer.getHand().toString() + " (Value = " + std::to_string(dv) + ")";

    for (const auto& p : players)
    {
        bool isSplit = (p->getHandsCount() > 1);
        double baseBet = p->getBet();

        if (isSplit)
            resultLines.push_back(p->getName() + ":");

        for (int i = 0; i < p->getHandsCount(); i++)
        {
            const Hand& h = p->getHandByIndex(i);

            std::string outcome;

            if (dealerBJ)
            {
                if (!isSplit && h.isBlackjack())
                {
                    outcome = "PUSH (I dealer i " + p->getName() + " imaju BLACKJACK)";
                    p->push();
                }
                else
                {
                    outcome = "LOSE (Dealer Blackjack)";
                    if (!isSplit)
                        p->lose();
                }
            }
            else if (!isSplit && h.isBlackjack())
            {
                outcome = "BLACKJACK (x2.5)";
                p->win(2.5);
            }
            else
            {
                outcome = basicOutcome(h, dv, dbust);

                if (!isSplit)
                {
                    if (outcome == "WIN" || outcome == "WIN (Dealer bust)")
                        p->win(2.0);
                    else if (outcome == "PUSH")
                        p->push();
                    else
                        p->lose();
                }
                else
                {
                    if (outcome == "WIN" || outcome == "WIN (Dealer bust)")
                        p->addMoney(baseBet * 2);
                    else if (outcome == "PUSH")
                        p->addMoney(baseBet);
                }
            }

            std::string label = isSplit 
                ? (" Split " + std::to_string(i + 1))
                : p->getName();

            std::string line = label + ": " + h.toString() 
                + " (Value = " + std::to_string(h.getValue()) + ") -> " + outcome;

            resultLines.push_back(line);
        }

        if (isSplit)
            p->clearBet();

        resultLines.push_back(" -> Balance: " + std::to_string(static_cast<int>(p->getMoney())));
        resultLines.push_back("-------------------------");
    }
}

void BlackjackGame::printResult() const
{
    std::cout << "\n=== REZULTATI ===\n";
    std::cout << dealerResultLine << "\n";

    for (const std::string& line : resultLines)
        std::cout << line << "\n";
}

bool BlackjackGame::anyPlayerAlive() const
{
    for (const auto& p : players)
        for (int i = 0; i < p->getHandsCount(); i++)
            if (!p->getHandByIndex(i).isBust())
                return true;
    return false;
}

void BlackjackGame::delay(int ms) const
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

    if (deck.remaining() < shuffle_threshold)
    {
        std::cout << "\nU spilu je ostalo " << deck.remaining() << " karata.";
        std::cout << "\nSpil se ponovno mijesa....\n";
        delay(1200);

        deck.reset();
        deck.shuffle();
    }

    std::cout << "\n=== ULOZI ZELJENU SVOTU NOVCA===\n";
    takeBets();

    dealInitial();
    printInitialHands();

    playersTurn();
    dealerTurn();

    delay(1500);
    resolveBets();
    printResult();

    return true;
}