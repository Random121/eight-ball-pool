#pragma once

#include "common.h"

#include <iostream>
#include <string>

namespace menu
{
	void setPlayerNames(std::string& playerName1, std::string& playerName2)
	{
		std::cout << "=========================\n";
		std::cout << "= Player Name Selection =\n";
		std::cout << "=========================\n\n";

		std::cout << "Enter name for Player (1): ";
		std::getline(std::cin, playerName1);

		if (playerName1.empty())
		{
			playerName1 = "1";
			std::cout << "Default name has been selected for Player (1).\n";
		}

		while (true)
		{
			std::cout << "Enter name for Player (2): ";
			std::getline(std::cin, playerName2);

			if (playerName2 == playerName1)
			{
				std::cout << "Players can not have the same name.\n";
			}
			else
			{
				if (playerName2.empty())
				{
					playerName2 = "2";
					std::cout << "Default name has been selected for Player (2).\n";
				}
				break;
			}
		}

		pauseProgram("\nPress [ENTER] to go back to main menu...");
	}

	void displayCredits()
	{
		std::cout << "===========\n";
		std::cout << "= Credits =\n";
		std::cout << "===========\n\n";

		std::cout << "[Allegro Game Library]\n";
		std::cout << "For providing this easy to use game and rendering library so I don't have to learn OpenGL.\n\n";

		std::cout << "[Learncpp]\n";
		std::cout << "Helping me with learning OOP and function templates.\n\n";

		std::cout << "[Stackoverflow Users (https://stackoverflow.com/a/6487534)]\n";
		std::cout << "For providing me with this better alternative to clear the console.\n\n";

		std::cout << "[Youtuber javidx9 (https://youtu.be/LPzyNOHY3A4)]\n";
		std::cout << "Intuitive explanation on the maths for the collision.\n\n";

		std::cout << "[Blog (https://lajbert.github.io/blog/fixed_timestep/#/)]\n";
		std::cout << "Explanations on the benefits of fixed time updates.\n\n";

		std::cout << "[freesound.org]\n";
		std::cout << "Sound effect samples for ball collisions and pocketing.\n\n";

		pauseProgram("Press [ENTER] to go back to main menu...");
	}

	bool initMenu(std::string& playerName1, std::string& playerName2)
	{
		bool menuActive{ true };
		int userSelection;

		while (menuActive)
		{
			std::cout << "====================================================\n";
			std::cout << "= Welcome to Totally Accurate Eight-Ball Simulator =\n";
			std::cout << "====================================================\n\n";

			std::cout << "===Please select one of the options below===\n";
			std::cout << "[1] Play Eight-Ball\n";
			std::cout << "[2] Setup Player Names\n";
			std::cout << "[3] Credits\n";
			std::cout << "[4] Exit\n\n";

			std::cout << "Select Option: ";
			std::cin >> userSelection;

			resetCin();
			clearConsole();

			if (!std::cin.fail())
			{
				switch (userSelection)
				{
				case 1:
					menuActive = false;
					break;
				case 2:
					setPlayerNames(playerName1, playerName2);
					break;
				case 3:
					displayCredits();
					break;
				case 4:
					return true;
				}
			}

			clearConsole();
		}

		return false;
	}
}
