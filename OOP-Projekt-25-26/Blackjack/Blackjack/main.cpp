#include "BlackjackGame.h"
#include <iostream>

int main()
{
    BlackjackGame game;

    char again = 'y';
    while (again == 'y' || again == 'Y')
    {
        game.playRound();

        std::cout << "\nNova runda? (y/n): ";
        std::cin >> again;
    }

    return 0;
}
