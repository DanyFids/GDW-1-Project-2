#include<iostream>
#include<string>
#include<random>
#include<conio.h>
#include<windows.h>
#include<stdio.h>

using namespace std;

class Colors {
public:
	const int Default = 7;
	const int Scarlet = 4;
	const int Mustard = 6;
	const int Peacock = 9;
	const int White = 15;
	const int Green = 2;
	const int Plum = 13;
	const int Board = 8;
	const int Button = 240;
};

Colors Palette;

enum Rooms {
	Null,
	Basement,
	Hall,
	Lounge,
	Study,
	Library,
	BilliardRoom,
	Conservatory,
	BallRoom,
	Kitchen,
	DinningRoom
};

enum Character {
	Ms_Scarlet,
	Prof_Plum,
	Mr_Green,
	Mrs_Peacock,
	Col_Mustard,
	Ms_White
};

enum Weapon {
	Candlestick,
	Pipe,
	Wrench,
	Rope,
	Knife,
	Revolver
};

enum Direction {
	Up,
	Down,
	Left,
	Right
};

enum GameState {
	Menu,
	Play
};

enum ACTION {
	DEFAULT,
	PLAY
};

// Function Prototypes
void displayColors();
void Draw(HANDLE out);
void GoToXY(int x, int y);
string toString(Rooms input);
string toString(Character input);
string toString(Weapon input);
int GetColor(Character c);
char GetToken(Character c);
void KeyHandler(KEY_EVENT_RECORD e);
void MouseHandler(MOUSE_EVENT_RECORD e);
void ButtonHandler(ACTION action);
void clear();

// Global Variables Classes Depend on
COORD mouseLoc;

// Classes
// Deck
template <class T, int Size>
class Deck {
private:
	T *cards;
	int topId = 0;
	int deckSize = Size;
public:
	Deck(T c[]) {
		cards = c;

		shuffle();
	}

	T draw() {
		T hold;
		if (topId < deckSize) {
			hold = cards[topId];
			topId++;
		}

		return hold;
	}

	bool checkDeck() {
		if (topId < deckSize) {
			return true;
		}
		else {
			return false;
		}
		return false;
	}

	void shuffle() {
		random_device gen;
		uniform_int_distribution<int> range(0, deckSize - 1);

		topId = 0;

		for (int c = 0; c < deckSize; c++) {
			int d = range(gen);
			T hold = cards[d];
			cards[d] = cards[c];
			cards[c] = hold;
		}
	}
};

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

class Button {
private:
	string text;
	ACTION action;
	int size, x, y;
	bool hov = false;
public:
	Button() {
		text = "Default";
		action = DEFAULT;
		x = 0;
		y = 0;
		size = text.length();
	}

	Button(string t, int x, int y, ACTION a) {
		text = t;
		action = a;
		this->x = x;
		this->y = y;
		size = text.length();
	}

	bool isOver(COORD pos) {
		if (pos.Y == y && pos.X < x + text.length() + 4 && pos.X > x) {
			return true;
		}
		else {
			return false;
		}
	}

	ACTION GetAction() {
		return action;
	}

	void draw(HANDLE out) {
		GoToXY(x, y);


		if (isOver(mouseLoc)) {
			cout << " |";
			SetConsoleTextAttribute(out, Palette.Button);
			cout << text;
			SetConsoleTextAttribute(out, Palette.Default);
			cout << "| ";
		}
		else {
			cout << " |" << text << "| ";
		}
		GoToXY(0, 0);
	}

	
};

class Player {
private:
	Character pChar;
	int x, y;
public:
	Player(Character c, int x, int y) {
		pChar = c;
		this->x = x;
		this->y = y;
	}

	Player() {
		pChar = Ms_Scarlet;
		x = 0;
		y = 0;
	}
	
	bool move(Direction d) {
		switch (d) {
		case Up:
			if(y > 0)
				y -= 1;
			break;
		case Down:
			y += 1;
			break;
		case Right:
			x += 1;
			break;
		case Left:
			if(x > 0) 
				x -= 1;
			break;
		}

		return true;
	}

	void Draw(HANDLE out) {
		GoToXY(x, y);
		SetConsoleTextAttribute(out, GetColor(pChar));
		cout << GetToken(pChar);
		SetConsoleTextAttribute(out, Palette.Default);
		GoToXY(0, 0);
	}
};

/*****************
	Screen Class
******************
	Virtual class to be used as a Base for other screens
*/
class Screen {
private:
	int numButtons;
public:
	virtual Button* GetButtons() = 0;
	virtual void Draw(HANDLE out) = 0;
	
	int GetNumButtons() {
		return numButtons;
	}

	void SetNumButtons(int i) {
		numButtons = i;
	}
};

class MainMenu :public Screen {
private:
	static const int NUM_BUTTONS = 1;
	Button buttons[NUM_BUTTONS] = {Button("Play!", 30, 10, PLAY)};
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


// GLOBAL VARIABLES

//Rooms BoardLayout[24][24] = {};
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

bool play = true;

int numPlayers = 1;
Player players[6] = { Player(Ms_Scarlet, 20, 10) };
Player *curPlayer = &players[0];

int main() {
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
		

		if (!ReadConsoleInput(
			inputH,
			inputR,
			inputR_SIZE,
			&iNumRead
		)) {
			return 201;
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
		Draw
*********************
	Self explanatory,
	Draws the screen.

*/

void Draw(HANDLE out) {
	clear();

	switch (state) {
	case Menu:
		mainMenu.Draw(out);
		break;
	case Play:
		for (int c = 0; c < numPlayers; c++) {
			players[c].Draw(out);
		}
		break;

	}

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
	case Null:
		return "Empty";
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
char GetToken(Character c) {
	switch (c) {
	case Ms_Scarlet:
		return 'S';
	case Col_Mustard:
		return 'M';
	case Mrs_Peacock:
		return 'P';
	case Mr_Green:
		return 'G';
	case Ms_White:
		return 'W';
	case Prof_Plum:
		return 'P';
	default:
		return ' ';
	}

	return ' ';

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
			play = false;
			break;
		case VK_UP:
			if (state == Play) {
				curPlayer->move(Up);
			}
			break;
		case VK_DOWN:
			if (state == Play) {
				curPlayer->move(Down);
			}
			break;
		case VK_LEFT:
			if (state == Play) {
				curPlayer->move(Left);
			}
			break;
		case VK_RIGHT:
			if (state == Play) {
				curPlayer->move(Right);
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