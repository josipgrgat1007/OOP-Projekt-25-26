#include "BlackjackGame.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

	try
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
    catch (const std::exception& e)
    {
        std::cout << "Error: " << e.what() << "\n";
    }
}
