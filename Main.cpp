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
	static const int NUM_BUTTONS = 3;
	Button buttons[NUM_BUTTONS] = {Button("Play!", 30, 10, PLAY), Button("Colors", 30, 12, COLORS), Button("Chars", 30, 14, CHARACTERS)};
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
	int nX = x, nY = y;
	bool colision = false;

	if (board.IsEntrance(x, y) && d == board.GetDir(x, y)) {
		inRoom = true;
		room = board.GetRoom(x, y);
		COORD roomC = board.GetRoomCoord(pChar, room);
		x = roomC.X;
		y = roomC.Y;
		NextTurn();
	}
	else {
		if (!inRoom) {
			switch (d) {
			case Up:
				if (y > 0)
					nY = y - 1;
				break;
			case Down:
				if (y < BOARD_D)
					nY = y + 1;
				break;
			case Right:
				if (x < BOARD_D)
					nX = x + 1;
				break;
			case Left:
				if (x > 0)
					nX = x - 1;
				break;
			}

			if (board.IsWall(nX, nY)) {
				colision = true;
			}

			Player * players = board.getPlayers();
			for (int c = 0; c < board.NumPlayers(); c++) {
				if (nX == players[c].GetX() && nY == players[c].GetY()) {
					colision = true;
				}
			}

			if (!colision) {
				x = nX;
				y = nY;
				movePoints--;

				if (movePoints <= 0) {
					NextTurn();
				}
			}
		}
		else {
			switch (d) {
			case Right:
				selectExit++;
				if (selectExit >= numExits) {
					selectExit = 0;
				}
				break;
			case Left:
				selectExit--;
				if (selectExit < 0) {
					selectExit = numExits - 1;
				}
				break;
			}
		}
	}

	return true;
}

void Player::EnterRoom(Rooms r, Board b) {
	inRoom = true;
	room = r;
	COORD roomC = b.GetRoomCoord(pChar, room);
	x = roomC.X;
	y = roomC.Y;
	b.GetExits(room, numExits, Exits);
	selectExit = -1;
	NextTurn();
}

void Player::TurnStart(Board board) {
	blinkTimer = 0;
	movePoints = 6;
	isTurn = true;

	if (inRoom) {
		board.GetExits(room, numExits, Exits);
		selectExit = -1;
	}
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
		GoToXY(0, 0);
		break;
	case Play:
		gameBoard.Draw(out);
		/*
		GoToXY(40, 0);
		cout << "(" << curPlayer->GetX() << "," << curPlayer->GetY() << ")";
		*/
		GoToXY(0, 0);
		break;

	}

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
		case VK_SPACE:
			if (state == Play) {
				if (curPlayer->IsInRoom()) {
					curPlayer->ExitRoom();
				}
			}
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
		scrn = &gameBoard;
		break;
	}

	Button* btns = scrn->GetButtons();

	if (e.dwEventFlags == MOUSE_MOVED) {
		mouseLoc = e.dwMousePosition;
	}

	if (e.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
		for (int c = 0; c < scrn->GetNumButtons(); c++) {
			if (btns[c].isOver(e.dwMousePosition) && e.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && !btns[c].IsDisabled()) {
				ButtonHandler(btns[c].GetAction());
			}
		}
	}

	if (state == Play && curPlayer->IsInRoom()) {
		if (curPlayer->OverExit(e.dwMousePosition) && e.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
			curPlayer->ExitRoom();
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
		curPlayer->TurnStart(gameBoard);
		break;
	case S_PASSAGE:
		switch (curPlayer->GetRoom()) {
		case Study:
			curPlayer->EnterRoom(Kitchen, gameBoard);
			break;
		case Lounge:
			curPlayer->EnterRoom(Conservatory, gameBoard);
			break;
		case Conservatory:
			curPlayer->EnterRoom(Lounge, gameBoard);
			break;
		case Kitchen:
			curPlayer->EnterRoom(Study, gameBoard);
			break;
		}
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
	curPlayer->TurnStart(gameBoard);
}