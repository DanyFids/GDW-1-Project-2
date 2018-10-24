#pragma once

/*
Logo for the Game.
Date: October 23
Version 1.0
*/


#include<iostream>
#include<string>
#include <windows.h>

void logo();


void logo() {

		int bottomLeft = 192;
		int	bottomRight = 218;
		int topLeft = 217;
		int topRight = 191;

		std::cout << (char)192;
		std::cout
		<< " __________        ___               ___      ___       __________" << std::endl
		<< "|    ______|      |   |             |   |    |   |     |     _____|" << std::endl
		<< "|   |             |   |             |   |    |   |     |    |" << std::endl
		<< "|   |             |   |             |   |    |   |     |    |_____" << std::endl
		<< "|   |             |   |             |   |    |   |     |     _____|" << std::endl
		<< "|   |______       |   |______       |   |____|   |     |    |_____" << std::endl
		<< "|__________|      |__________|      |____________|     |__________|" << std::endl;


}