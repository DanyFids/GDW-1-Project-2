#pragma once


/*
Win and Lose Titles for the Game.
Date: October 25
Version 1.0
*/


#include<iostream>
#include<string>
#include <windows.h>

void lose();
void win();

void lose() {

	std::cout << "You..." << std::endl
		<< "__         _____    _____   ______" << std::endl
		<< "| |       |  _  |  | ____|  |  ___|" << std::endl
		<< "| |       | | | |  | |___   |  |__" << std::endl
		<< "| |       | | | |  |____ |  |  ___|" << std::endl
		<< "| |____   | |_| |   ___| |  | |___" << std::endl
		<< "|______|  |_____|  |_____|  |_____|" << std::endl;


}

void win() {

	std::cout << "You..." << std::endl
		<< "__        __  _________  ___     _" << std::endl
		<< "| |       | | |__   __| |   \\   | |" << std::endl
		<< "| |  ___  | |    | |    |  \\ \\  | |" << std::endl
		<< "| | /   \\ | |    | |    | | \\ \\ | |" << std::endl
		<< "| |/ / \\ \\| | ___| |__  | |  \\ \\| |" << std::endl
		<< "|___/   \\___| |_______| |_|   \\___|" << std::endl;


}