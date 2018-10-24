#pragma once
/*

Tia
Version 1.0: (October 21st, 2018)
Added ASCII art of each weapon and descriptions.

*/

#include<iostream>
#include<string>
#include <windows.h>


// functions that include ASCII art and descriptions of each item.
// weapons
void knife();
void candlestick();
void revolver();
void leadpipe();
void wrench();
void rope();

// characters
void scarlet();
void plum();
void mustard();
void peacock();
void white();
void green();

// rooms
void lounge();
void kitchen();
void diningRoom();
void library();
void billiardRoom();
void conservatory();
void ballroom();
void hall();
void study();

void scarlet() {

}
void plum() {

}
void mustard() {
}
void peacock() {
}
void white() {
}
void green() {
}

void lounge() {
}
void kitchen() {
}
void diningRoom() {
}
void library() {
}
void billiardRoom() {
}
void conservatory() {
}
void ballroom() {
}
void hall() {
}
void study() {
}


void knife() {


	std::cout << "\t \t \t \t \t \t KNIFE" << std::endl << std::endl << std::endl;

	std::cout << "__________ _____________________________________" << std::endl
		<< "l__o___o___|                                  /" << std::endl
		<< "           |_______________________________./" << std::endl
		<< "           |_________________________ /         " << std::endl << std::endl
		<< "A blade, of the cutting edge variety.";

}

void candlestick() {


	std::cout << "\t \t \t \t \t \t CANDLESTICK" << std::endl << std::endl << std::endl;

	std::cout << "           ,,," << std::endl
		<< "           {o}" << std::endl
		<< "        ____|___" << std::endl
		<< "       (_ __ __ )" << std::endl
		<< "       |       U|" << std::endl
		<< "       |       O|" << std::endl
		<< "       |        |" << std::endl
		<< "       |        |" << std::endl
		<< "      _|________|_" << std::endl
		<< "     (__________ `)" << std::endl
		<< "       \\  __   /" << std::endl
		<< "        )     ( " << std::endl
		<< "       (-------) " << std::endl
		<< "         |   | " << std::endl
		<< "    	 |   | " << std::endl
		<< "       __|   |__" << std::endl
		<< "      /__________\\ " << std::endl
		<< "     (____________) " << std::endl << std::endl



		<< "A candlestick, to stick your candles in.";
}


void revolver() {

	std::cout << "\t \t \t \t \t \t REVOLVER" << std::endl << std::endl << std::endl;

	std::cout << " /7______________                    / " << std::endl
		<< "|________________0|  ==  ==  ==  ==   " << std::endl
		<< "\\-------------//                     \\ " << std::endl
		<< "  /    (  )_/  " << std::endl
		<< " /    /" << std::endl
		<< "/ __ /" << std::endl << std::endl

		<< "The world doesn't revolve around you.";
	;

}

void leadpipe() {

	std::cout << "\t \t \t \t \t \t LEADPIPE" << std::endl << std::endl << std::endl;

	std::cout << " __  " << std::endl
		<< "|__|" << std::endl
		<< "|  |" << std::endl
		<< "|  |" << std::endl
		<< "\\  \\ " << std::endl
		<< " \\ _\\ " << std::endl
		<< "  |  |" << std::endl
		<< "  |  |" << std::endl
		<< "  |  |" << std::endl
		<< "  |__|   _" << std::endl
		<< "  |__|__| |" << std::endl
		<< "  (_______)" << std::endl << std::endl

		<< "Lead-y or not, here I come.";

}

void wrench() {

	std::cout << "\t \t \t \t \t \t WRENCH" << std::endl << std::endl << std::endl;

	std::cout << " __       __" << std::endl
		<< "|  |     |  |" << std::endl
		<< "|  |     |  |" << std::endl
		<< "|  |_____|  |" << std::endl
		<< " \\_       _/" << std::endl
		<< "   |     |" << std::endl
		<< "   |     |" << std::endl
		<< "   |     |" << std::endl
		<< "   |     |" << std::endl
		<< "   |     |" << std::endl
		<< "   |     |" << std::endl
		<< "  _|     |_" << std::endl
		<< " /  _____  \\" << std::endl
		<< "|  |     |  |" << std::endl
		<< "|  |     |  |" << std::endl
		<< "|__|     |__|" << std::endl << std::endl

		<< "Throw a wrench in their plans.";

}

void rope() {

	std::cout << "\t \t \t \t \t \t ROPE" << std::endl << std::endl << std::endl;

	//std::cout << "   ,_________" << std::endl
	//	<< "  (__________)" << std::endl
	//	<< " (____________)" << std::endl
	//	<< "(______________)" << std::endl
	//	<< "             \\ \\" << std::endl
	//	<< "             / /" << std::endl
	//	<< "             \\,_\\" << std::endl << std::endl

	//	<< "Rope them in with this.";

	//std::cout << 


}

int main() {
	//HANDLE m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // for colour, will add later

	std::cout << "\t \t \t \t \t \t HAND" << std::endl << "\t\tHere are the items currently in your Hand. Click to inspect." << std::endl;

	// uncomment the code to see each one
	//knife();
	//candlestick();
	//revolver();
	//leadpipe();
	//wrench();
	rope();
	std::cout << std::endl << std::endl;

	std::cout << std::endl << std::endl << std::endl << std::endl << "Back to Hand." << std::endl << std::endl;

	system("pause");
	return 0;
}