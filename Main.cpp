#include<iostream>
#include<string>
#include<random>
#include<conio.h>
#include<windows.h>
#include<stdio.h>
//Our Classes
#include"Enums.h"
#include"FunctionProto.h"
#include"UI.h"
#include"Player.h"
#include"Board.h"
#include"Deck.h"

using namespace std;



// Global Variables Classes Depend on


// Classes


// Solution
class Solution {
private:
	Weapon murderWep;
	Character murderer;
	Rooms murderRoom;

public:
	Solution(Weapon w, Character c, Rooms r) {
		murderWep = w;
		murderer = c;
		murderRoom = r;
	}

	bool guess(Weapon w, Character c, Rooms r) {
		if (murderWep == w && murderer == c && murderRoom == r) {
			return true;
		}
		else {
			return false;
		}

		return false;
	}
};






class MainMenu :public Screen {
private:
	static const int NUM_BUTTONS = 4;
	Button buttons[NUM_BUTTONS] = {Button("Play!", 30, 10, PLAY), Button("Colors", 30, 12, COLORS), Button("Chars", 30, 14, CHARACTERS), Button("Rules", 30, 16, RULES)};
public:
	MainMenu() {
		SetNumButtons(NUM_BUTTONS);
	}

	Button* GetButtons() {
		return buttons;
	}

	void Draw(HANDLE out) {
		for (int c = 0; c < GetNumButtons(); c++) {
			buttons[c].draw(out);
		}
	}
};



//Class Functions
bool Player::move(Direction d, Board board) {
	switch (d) {
	case Up:
		if (y > 0 && !board.IsWall(x, y - 1))
			y -= 1;
		break;
	case Down:
		if (y < 24 && !board.IsWall(x, y + 1))
			y += 1;
		break;
	case Right:
		if (x < 24 && !board.IsWall(x + 1, y))
			x += 1;
		break;
	case Left:
		if (x > 0 && !board.IsWall(x - 1, y))
			x -= 1;
		break;
	}

	return true;
}

// GLOBAL VARIABLES

GameState state = Menu;

Weapon WeapCards[] = { Candlestick, Pipe, Wrench, Rope, Knife, Revolver };
Character CharCards[] = { Ms_Scarlet, Prof_Plum, Mr_Green, Mrs_Peacock, Col_Mustard, Ms_White };
Rooms RoomCards[] = { Hall, Lounge, Study, Library, BilliardRoom, Conservatory, BallRoom, Kitchen, DinningRoom };

//Decks
Deck<Weapon, 6> WeaponDeck(WeapCards);
Deck<Character, 6> CharacterDeck(CharCards);
Deck<Rooms, 9> RoomDeck(RoomCards);

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE inputH;
COORD CursorLoc, KeyLoc, EndLoc;

//Screens
MainMenu mainMenu;

Board gameBoard;

bool play = true;

Player *curPlayer = gameBoard.GetCurrentPlayer();

int main() {
	SetConsoleCP(437);

	const int inputR_SIZE = 128;
	DWORD iNumRead, consoleModeSave, consoleMode;
	INPUT_RECORD inputR[inputR_SIZE];

	inputH = GetStdHandle(STD_INPUT_HANDLE);

	if (inputH == INVALID_HANDLE_VALUE) {
		return 101;
	}

	if (!GetConsoleMode(inputH, &consoleModeSave)) {
		return 102;
	}

	/*DWORD fdwMode;

	fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
	if (!SetConsoleMode(inputH, fdwMode))
		return 103;*/

	consoleMode = ENABLE_EXTENDED_FLAGS;
	if (!SetConsoleMode(inputH, consoleMode))
		return 103;

	consoleMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
	if (!SetConsoleMode(inputH, consoleMode)) {
		return 103;
	}

	while (play) {
		DWORD unreadInputs;

		if (!GetNumberOfConsoleInputEvents(
			inputH,
			&unreadInputs
		)) {
			return 201;
		}
		
		if (unreadInputs > 0) {
			if (!ReadConsoleInput(
				inputH,
				inputR,
				inputR_SIZE,
				&iNumRead
			)) {
				return 202;
			}

			for (DWORD c = 0; c < iNumRead; c++) {
				switch (inputR[c].EventType) {
				case KEY_EVENT:
					KeyHandler(inputR[c].Event.KeyEvent);
					break;
				case MOUSE_EVENT:
					MouseHandler(inputR[c].Event.MouseEvent);
					break;
				default:
					break;
				}
			}
		}

		Draw(console);
		Sleep(40);
	}

	system("Pause");
	SetConsoleMode(inputH, consoleModeSave);
	return 0;
}

/******************
	displayColors
*******************
	### DEBUG FUNCTION ###
	Displays all possible color combinations for windows console.
*/
void displayColors() {
	for (int k = 1; k < 255; k++) {
		cout << k << ". ";
		SetConsoleTextAttribute(console, k);
		cout << "Color Test Display";
		SetConsoleTextAttribute(console, 7);
		cout << endl;
	}
}

/********************
	displayChars
**********************
	## DEBUG FUNCTION ##
	displays characters and their associated codes
*/
void displayChars() {
	for (int c = 1; c < 255; c++) {
		cout << c << ". " << (char)c << endl;
	}
}

/********************
		Draw
*********************
	Self explanatory,
	Draws the screen.

*/
void Draw(HANDLE out) {
	//clear();

	switch (state) {
	case Menu:
		mainMenu.Draw(out);
		break;
	case Play:
		gameBoard.Draw(out);
		break;

	}

	//GoToXY(0, 0);
	SetConsoleTextAttribute(console, Palette.Default);
}

/***************
	GoToXY
****************
	Sets console cursor to X, Y position.
*/
void GoToXY(int x, int y) {
	CursorLoc.X = x;
	CursorLoc.Y = y;

	SetConsoleCursorPosition(console, CursorLoc);
}

/**************
   ToString
***************
	Converts Enum values to String.
*/
//Rooms
string toString(Rooms input) {
	switch (input) {
	case Basement:
		return "Basement";
	case Hall:
		return "Hall";
	case Lounge:
		return "Lounge";
	case Study:
		return "Study";
	case Library:
		return "Library";
	case BilliardRoom:
		return "Billiard Room";
	case Conservatory:
		return "Conservatory";
	case BallRoom:
		return "Ball Room";
	case Kitchen:
		return "Kitchen";
	case DinningRoom:
		return "Dinning Room";
	default: return "";
	}
}
//Weapon
string toString(Weapon input) {
	switch (input) {
	case Candlestick:
		return "Candlestick";
	case Pipe:
		return "Lead Pipe";
	case Wrench:
		return "Wrench";
	case Rope:
		return "Rope";
	case Knife:
		return "Knife";
	case Revolver:
		return "Revolver";
	default:
		return "";
	}
}
//Character
string toString(Character input) {
	switch (input) {
	case Ms_Scarlet: return "Ms. Scarlet";
	case Prof_Plum: return "Prof. Plum";
	case Mr_Green: return "Mr. Green";
	case Mrs_Peacock: return "Mrs. Peacock";
	case Col_Mustard: return "Col. Mustard";
	case Ms_White: return "Ms. White";
	default: return "";
	}
}

/*********************
	  GetColor
**********************
	Returns color associated to
	a given character.
*/
int GetColor(Character c) {
	switch (c) {
	case Ms_Scarlet:
		return Palette.Scarlet;
	case Col_Mustard:
		return Palette.Mustard;
	case Mrs_Peacock:
		return Palette.Peacock;
	case Mr_Green:
		return Palette.Green;
	case Ms_White:
		return Palette.White;
	case Prof_Plum:
		return Palette.Plum;
	default:
		return Palette.Default;
	}

	return Palette.Default;
}

/*********************
	  GetToken
**********************
	Returns Token associated
	with a given character.
*/
string GetToken(Character c) {
	switch (c) {
	case Ms_Scarlet:
		return "Sc";
	case Col_Mustard:
		return "Mu";
	case Mrs_Peacock:
		return "Pe";
	case Mr_Green:
		return "Gr";
	case Ms_White:
		return "Wh";
	case Prof_Plum:
		return "Pl";
	default:
		return "  ";
	}

	return "  ";

}

/************************
	   KeyHandler
*************************
	Handles keyboard input.
*/
void KeyHandler(KEY_EVENT_RECORD e) {
	if (e.bKeyDown) {
		switch(e.wVirtualKeyCode) {
		case VK_ESCAPE:
			clear();
			state = Menu;
			break;
		case VK_UP:
			if (state == Play) {
				curPlayer->move(Up, gameBoard);
			}
			break;
		case VK_DOWN:
			if (state == Play) {
				curPlayer->move(Down, gameBoard);
			}
			break;
		case VK_LEFT:
			if (state == Play) {
				curPlayer->move(Left, gameBoard);
			}
			break;
		case VK_RIGHT:
			if (state == Play) {
				curPlayer->move(Right, gameBoard);
			}
			break;
		case VK_TAB:
			if (state == Play) {
				NextTurn();
			}
			break;
		}
	}
}

/************************
	   MouseHandler
*************************
	Handles Mouse input.
*/
void MouseHandler(MOUSE_EVENT_RECORD e) {
	Screen* scrn;

	switch (state) {
	case Menu:
		scrn = &mainMenu;
		break;
	default:
		scrn = &mainMenu;
		break;
	}

	Button* btns = scrn->GetButtons();

	if (e.dwEventFlags == MOUSE_MOVED) {
		mouseLoc = e.dwMousePosition;
	}

	if (e.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
		for (int c = 0; c < scrn->GetNumButtons(); c++) {
			if (btns[c].isOver(e.dwMousePosition) && e.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
				ButtonHandler(btns[c].GetAction());
			}
		}
	}
}

/************************
	  ButtonHandler
*************************
	Handles Action from Button
*/

void ButtonHandler(ACTION action) {
	switch (action) {
	case PLAY:
		state = Play;
		curPlayer->TurnStart();
		break;
	case COLORS:
		state = DEBUG;
		clear();
		displayColors();
		break;
	case CHARACTERS:
		state = DEBUG;
		clear();
		displayChars();
		break;
	case RULES:
		state = DEBUG;
		clear();
		cout << "Instructions for who starts the game are whoever has Miss Scarlet (red) rolls the die and is the first to move. \n" 
			"\n"
			"Movement \n"
			"Players move their suspect token across the squares the amount shown on the die in their roll.You may move your \n"
			"token forwards, backwards or sideways all in one turn using the arrow keys. \n"
			"Moving your token diagonally is against the Clue rules.You cannot move your token to a particular space twice in the \n" 
			"same turn nor occupy or move through the same square as another player. \n"
			"This rule does not apply to rooms as multiple players and weapons may occupy the same room. \n"
			"\n"
			"Rooms \n"
			"The Clue board game rules state that there are two ways to enter or exit a room : 1) entering through the doorway \n"
			"by moving your token the number shown on the die across the squares, and too one of the doors. You then leave \n" 
			"the room by going through any of the doors in that room. 2) Through the secret passage which is an option in some \n"
			"of the rooms and will teleport you to another room. \n"
			"\n"
			"Guessing the Killer \n"
			"Once a player enters a room, they are then eligible to make a “suggestion” on who committed the Clue game murder. \n"
			"This is done when the player names a Suspect, a Weapon, and the Room \n"
			"that they are currently in(“I suggest that the crime was committed in the Library, by Prof.Plum with the Rope”). \n"
			"Forfeiting or not taking your turn is against the rules; all players must roll the die.After entering a room and \n"
			"making a suggestions, you may not make another suggestions until entering \n"
			"another room or using at least two turns(leaving and then re - entering the same room). \n"
			"\n"
			"It is not against the rules, to deliberately make a \n"
			"Suggestion naming one or two cards that you hold in your own hand to mislead \n"
			"other players or to narrow your search for one specific suspect, room, or weapon. \n"
			"\n"
			"When an Accusation is made, the player(on their turn) states that an Accusation is being made and states the three \n"
			"cards that he thinks committed the murder.The game will then say if the guess was correct or wrong. \n"
			"Unlike the previously stated rules, a player may make an accusation whether or not their token is in the room they \n"
			"mention.If the 3 cards named are correct \n"
			"that player is the winner.If the accusation is wrong, they lose the game. \n" << endl;
		break;
	}
}

/*************************
		clear
**************************
	clears screen.
*/
void clear() {
	COORD origin = { 0, 0 }; // top left corner of screen
	CONSOLE_SCREEN_BUFFER_INFO scrn;
	DWORD buff;

	GetConsoleScreenBufferInfo(console, &scrn);

	//Fills Console with Whitespace
	FillConsoleOutputCharacterA(
		console, ' ', scrn.dwSize.X * scrn.dwSize.Y, origin, &buff
	);

	//Returns text colors to default
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		scrn.dwSize.X * scrn.dwSize.Y, origin, &buff
	);

	//return to 0,0
	SetConsoleCursorPosition(console, origin);
}

void NextTurn() {
	curPlayer->TurnEnd();
	curPlayer = gameBoard.NextPlayer();
	curPlayer->TurnStart();
}
