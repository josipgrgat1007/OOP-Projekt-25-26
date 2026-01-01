#include "BlackjackGame.h"
#include <iostream>

int main()
{
    BlackjackGame game;

    char again = 'y';
    while (again == 'y' || again == 'Y')
    {
        if (!game.playRound())
            break;  // nema vise igraca s novcem

        std::cout << "\nNova runda? (y/n): ";
        std::cin >> again;
    }

    return 0;
}
