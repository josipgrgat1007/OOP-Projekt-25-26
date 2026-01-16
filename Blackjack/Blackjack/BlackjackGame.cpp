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

    if (!(std::cin >> n))
        throw std::runtime_error("Neispravan unos!");
    if (n <= 0)
        throw std::runtime_error("Broj igraca mora biti veci od 0!");

    for (int i = 0; i < n; i++)
    {
        std::string name;
        std::cout << "Ime igraca " << (i + 1) << ": ";
        std::cin >> name;
           
        players.push_back(std::make_unique<HumanPlayer>(name));
    }

    deck.reset();
    deck.shuffle();
}

std::string BlackjackGame::handLabel(const Player& p, int handIndex) const
{
    if (p.getHandsCount() > 1)
        return p.getName() + " [Split " + std::to_string(handIndex + 1) + "]";
    return p.getName();
}

void BlackjackGame::printHandLine(const std::string& p, const Hand& h) const
{
    std::cout << p << ": " << h.toString()
        << " (Value = " << h.getValue() << ")\n";
}

void BlackjackGame::drawWithDelay(const std::string& p)
{
    std::cout << p << " vuce kartu...\n";
    delay(800);
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
        std::cout << h.getCard(1).toString();
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
            std::string label = handLabel(*p, i);
            Hand& h0 = p->getHandByIndex(i);

            if (p->getHandsCount() > 1)
                printHandLine(label, h0);

            // BLACKJACK
            if (p->getHandsCount() == 1 && h0.isBlackjack())
            {
                std::cout << label << " ima Blackjack! \n";
                break;
            }

            while (!p->getHandByIndex(i).isBust())
            {
                Hand& h = p->getHandByIndex(i);
                label = handLabel(*p, i);

                if (h.getValue() == 21)
                {
                    std::cout << label << " ima 21 i staje.\n";
                    break;
                }

                Action a = (p->getHandsCount() > 1) ? askHitStand(label) : p->decideAction();

                // SPLIT
                if (p->getHandsCount() == 1 && a == Action::Split)
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
                        std::cout << p->getName() << " split-a karte!\n";
                        printHandLine(handLabel(*p, 0), p->getHandByIndex(0));
                        printHandLine(handLabel(*p, 1), p->getHandByIndex(1));

                        continue;
                    }
                    std::cout << "Split nije moguc.\n";
                }

                // DOUBLE DOWN
                if (p->getHandsCount() == 1 && a == Action::DoubleDown)
                {
                    double add = p->getBet();
                    std::cout << p->getName() << " double down-a (jos " << add << ").\n";

                    if (!p->addToBet(add))
                    {
                        std::cout << "Nemate dovoljno novca za double down.\n";
                        continue;
                    }

                    drawWithDelay(p->getName());
                    p->hit(deck);
                    printHandLine(p->getName(), p->getHandByIndex(0));

                    std::cout << p->getName() << " staje nakon double down-a.\n";
                    break;
                }

                // STAND
                if (a == Action::Stand)
                {
                    std::cout << label << " staje (Value = " << h.getValue() << ")\n";
                    break;
                }

                // HIT
                drawWithDelay(label);

                if (p->getHandsCount() > 1)
                    h.addCard(deck.draw());
                else
                    p->hit(deck);

                printHandLine(label, h);
            }

            if (p->getHandByIndex(i).isBust())
                std::cout << handLabel(*p, i) << " bust!\n";

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
        std::cout << "Dealer ima BLACKJACK!";
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
            Hand& h = p->getHandByIndex(i);

            std::string outcome;

            if (dealerBJ)
            {
                if (!isSplit && h.isBlackjack())
                {
                    outcome = "PUSH (I dealer i " + p->getName() + "imaju BLACKJACK)";
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
                if (h.isBust())
                    outcome = "LOSE (Bust)";
                else if (dbust)
                    outcome = "WIN (Dealer bust)";
                else if (h.getValue() > dv)
                    outcome = "WIN";
                else if (h.getValue() < dv)
                    outcome = "LOSE";
                else
                    outcome = "PUSH";

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

            std::string label;
            if (isSplit)
                label = " Split " + std::to_string(i + 1);
            else
                label = p->getName();

            std::string line = label + ": " +
                h.toString() + " (Value = " + std::to_string(h.getValue()) + ") -> " + outcome;

            resultLines.push_back(line);
        }

        if (isSplit)
            p->clearBet();

        resultLines.push_back(" -> Balance: " + std::to_string(static_cast<int>(p->getMoney())));
        resultLines.push_back("");
    }
}

void BlackjackGame::printResult() const
{
    std::cout << "\n=== REZULTATI ===\n";
    std::cout << dealerResultLine << "\n\n";

    for (const std::string& line : resultLines)
        std::cout << line << "\n";

    std::cout << "\n";
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

    if (deck.remaining() < shuffle_threshold)
    {
        std::cout << "\nSpil se ponovno mijesa....";
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