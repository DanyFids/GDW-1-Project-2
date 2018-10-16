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

enum TileType {
	Floor,
	Wall,
	Entrance
};

enum Rooms {
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
	None,
	Up,
	Down,
	Left,
	Right
};

enum GameState {
	DEBUG,
	Menu,
	Play
};

enum ACTION {
	DEFAULT,
	PLAY,

	COLORS,
	CHARACTERS
};

// Function Prototypes
// DEBUG FUNCTIONS
void displayColors();
void displayChars();
//GAME FUNCTIONS
void Draw(HANDLE out);
void GoToXY(int x, int y);
string toString(Rooms input);
string toString(Character input);
string toString(Weapon input);
int GetColor(Character c);
string GetToken(Character c);
void KeyHandler(KEY_EVENT_RECORD e);
void MouseHandler(MOUSE_EVENT_RECORD e);
void ButtonHandler(ACTION action);
void clear();
void NextTurn();

//Class declarations
class Board;

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
	const int BOARD_D = 24;
	const int BLINK_TIME = 4;
	Character pChar;
	string token;
	int color;
	int x, y;
	bool isTurn = false;
	int blinkTimer = 0;
public:
	Player(Character c, int x, int y) {
		pChar = c;
		this->x = x;
		this->y = y;
		token = GetToken(c);
		color = GetColor(c);
	}

	Player() {
		pChar = Ms_Scarlet;
		x = 0;
		y = 0;
	}
	
	bool move(Direction d, Board board);

	void Draw(HANDLE out, int offsetX, int offsetY) {
		GoToXY((x + offsetX) * 2, y + offsetY);
		if (!isTurn) {
			SetConsoleTextAttribute(out, color);
			cout << GetToken(pChar);
		}
		else {
			if (blinkTimer < BLINK_TIME) {
				SetConsoleTextAttribute(out, color);
				cout << GetToken(pChar);
			}
			else {
				SetConsoleTextAttribute(out, color + 240);
				cout << GetToken(pChar);
			}

			blinkTimer++;
			if (blinkTimer >= BLINK_TIME * 2) {
				blinkTimer = 0;
			}
		}
	}

	void TurnStart() {
		blinkTimer = 0;
		isTurn = true;
	}

	void TurnEnd() {
		blinkTimer = 0;
		isTurn = false;
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

class Tile {
private:
	TileType type;
	Direction enterDir;
public:
	Tile() {
		type = Floor;
		enterDir = None;
	}

	Tile(TileType t) {
		type = t;

		if (t == Entrance) {
			enterDir = Up;
		}
		else {
			enterDir = None;
		}
	}

	Tile(Direction d) {
		type = Entrance;
		enterDir = d;
	}

	bool IsWall() {
		return (type == Wall);
	}

	bool IsEntrance() {
		return (type == Entrance);
	}
};

class Board {
private:
	const int BOARD_WIDTH = 54;
	const int BOARD_HEIGHT = 26;
	const int MARGIN = 1;
	int x, y;
	int numPlayers;
	int curPlayerId;
	Player players[6] = { {Ms_Scarlet, 16, 0}, {Col_Mustard, 24, 7}, {Ms_White, 14, 24}, {Mr_Green, 9, 24}, {Mrs_Peacock, 0, 18}, {Prof_Plum, 0, 5} };
	string boardLayout = "";
	Tile boardTiles[25][25];
public:
	Board() {
		x = 0;
		y = 0;
		numPlayers = 6;
		curPlayerId = 0;

		//The Following Code Builds the board.
		//Special CHaracters
		const char solid = (char)219;
		const char wallD = (char)220;
		const char wallR = (char)222;
		const char wallU = (char)223;
		const char wallL = (char)221;

		string line1 = "";
		string line2 = "";

		for (int i = 0; i < BOARD_WIDTH; i++) {
			line1.insert(line1.end(), solid);
			if (i < 2 || i >= BOARD_WIDTH - 2)
				line2.insert(line2.end(), solid);
			else
				line2.append(" ");
		}

		line1.append("\n");
		line2.append("\n");

		boardLayout.append(line1);
		string tmp = line2;
		//line 2
		tmp.replace(2, 1, "?");
		tmp.replace(13, 3, { wallR, solid, solid });
		tmp.replace(18, 5, { solid, solid, solid, solid, wallL });
		tmp.replace(29, 5, { wallR, solid, solid, solid, solid });
		tmp.replace(36, 3, { solid, solid, wallL });
		tmp.replace(51, 1, "?");
		boardLayout.append(tmp);

		//line 3
		tmp = line2;
		tmp.replace(5, 5, "STUDY");
		tmp.replace(15, 1, { wallR });
		tmp.replace(20, 1, { wallL });
		tmp.replace(31, 1, { wallR });
		tmp.replace(36, 1, { wallL });
		boardLayout.append(tmp);

		//line 4
		tmp.replace(5, 5, "     ");
		tmp.replace(24, 4, "HALL");
		tmp.replace(41, 6, "LOUNGE");
		boardLayout.append(tmp);

		//line 5
		tmp = line2;
		tmp.replace(2, 14, { wallD,wallD,wallD,wallD,wallD,wallD,wallD,wallD,wallD,wallD,wallD,wallD,'/',wallR });
		tmp.replace(20, 1, { wallL });
		tmp.replace(31, 1, { wallR });
		tmp.replace(36, 1, { wallL });
		boardLayout.append(tmp);

		//line 6
		tmp = line2;
		tmp.replace(2, 2, { solid, solid });
		tmp.replace(20, 1, { '/' });
		tmp.replace(31, 1, { '\\' });
		tmp.replace(36, 1, { wallL });
		boardLayout.append(tmp);

		//line 7
		tmp = line2;
		tmp.replace(20, 1, { wallL });
		tmp.replace(31, 1, { wallR });
		tmp.replace(36, 16, { wallL, '\\', wallD, wallD, wallD, wallD, wallD, wallD, wallD, wallD, wallD, wallD, wallD, wallD, wallD, wallD });
		boardLayout.append(tmp);

		//line 8
		tmp = line2;
		tmp.replace(2, 12, { solid, solid, wallU, wallU, wallU, wallU, wallU, wallU, wallU, wallU, wallU, solid });
		tmp.replace(20, 12, { solid, solid, wallD, wallD, '/', ' ', ' ', '\\', wallD, wallD, solid, solid });
		tmp.replace(50, 2, { solid, solid });
		boardLayout.append(tmp);

		//line 9
		tmp = line2;
		tmp.replace(13, 3, { wallU, wallU, solid });
		boardLayout.append(tmp);

		//line 10
		tmp = line2;
		tmp.replace(4, 7, "LIBRARY");
		tmp.replace(15, 1, { '/' });
		tmp.replace(20, 10, { solid, solid, '_', '_', '_', '_', '_', '_', solid, solid });
		tmp.replace(50, 2, { solid, solid });
		boardLayout.append(tmp);

		//line 11
		tmp = line2;
		tmp.replace(13, 3, { wallD, wallD, solid });
		tmp.replace(20, 10, { wallL, 'B', 'A', 'S', 'E', 'M', 'E', 'N', 'T', wallR });
		tmp.replace(34, 18, { solid, wallU, '\\', ' ', wallU, wallU, wallU, wallU, wallU, wallU, wallU, wallU, wallU, wallU, wallU, wallU, wallU, wallU });
		boardLayout.append(tmp);

		//line 12
		tmp = line2;
		tmp.replace(2, 12, { solid, solid, wallD, wallD, wallD, wallD, '/', ' ', wallD, wallD, wallD, solid });
		tmp.replace(20, 10, { solid, solid, solid, wallD, '_', '_', wallD, solid, solid, solid });
		tmp.replace(34, 1, { wallL });
		boardLayout.append(tmp);

		//line 13
		tmp = line2;
		tmp.replace(2, 2, { solid, solid });
		tmp.replace(20, 10, { wallL, (char)201, (char)190, solid, solid, solid, solid, (char)201, (char)190, wallR });
		tmp.replace(34, 1, { wallL });
		tmp.replace(41, 6, "DINING");
		boardLayout.append(tmp);

		//line 14
		tmp = line2;
		tmp.replace(2, 12, { solid, solid, ' ', '/', wallU, wallU, wallU, wallU, wallU, wallU, wallU, solid });
		tmp.replace(20, 10, { wallL, (char)186, ' ', (char)210, ' ', (char)210, (char)210, (char)204, (char)181, wallR });
		tmp.replace(34, 1, { '\\' });
		tmp.replace(42, 4, "ROOM");
		boardLayout.append(tmp);

		//line 15
		tmp = line2;
		tmp.replace(13, 1, { wallR });
		tmp.replace(20, 10, { wallL, (char)200, (char)184, (char)200, (char)184, (char)200, (char)188, (char)200, (char)184, wallR });
		tmp.replace(34, 1, { wallL });
		boardLayout.append(tmp);

		//line 16
		tmp = line2;
		tmp.replace(4, 8, "BILLIARD");
		tmp.replace(13, 1, { wallR });
		tmp.replace(20, 10, { solid, solid, solid, solid, solid, solid, solid, solid, solid, solid });
		tmp.replace(34, 7, { solid, wallD, wallD, wallD, wallD, wallD, wallD });
		boardLayout.append(tmp);

		//line 17
		tmp = line2;
		tmp.replace(13, 1, { '/' });
		tmp.replace(40, 12, { solid, wallD, wallD, wallD, wallD, wallD, wallD, wallD, wallD, wallD, wallD, wallD });
		boardLayout.append(tmp);

		//line 18
		tmp = line2;
		tmp.replace(2, 12, { wallD, wallD, wallD, wallD, wallD, wallD, wallD, wallD, wallD, wallD, wallD, solid });
		tmp.replace(50, 2, { solid, solid });
		boardLayout.append(tmp);

		//line 19
		tmp = line2;
		tmp.replace(2, 2, { solid, solid });
		tmp.replace(18, 16, { solid, wallU, '\\', ' ', wallU, wallU, wallU, wallU, wallU, wallU, wallU, wallU, ' ', '/', wallU, solid });
		boardLayout.append(tmp);

		//line 20
		tmp = line2;
		tmp.replace(18, 1, { wallL });
		tmp.replace(33, 1, { wallR });
		tmp.replace(38, 14, { solid, wallU, '\\', ' ', wallU, wallU, wallU, wallU, wallU, wallU, wallU, wallU, solid, solid });
		boardLayout.append(tmp);

		//line 21
		tmp = line2;
		tmp.replace(2, 10, { wallU, wallU, wallU, wallU, wallU, wallU, wallU, wallU, wallU, solid });
		tmp.replace(18, 1, { '/' });
		tmp.replace(33, 1, { '\\' });
		tmp.replace(38, 1, { wallL });
		tmp.replace(50, 2, { wallU, wallU });
		boardLayout.append(tmp);

		//line 22
		tmp = line2;
		tmp.replace(1, 1, { wallU });
		tmp.replace(11, 3, { wallU, '\\', wallR });
		tmp.replace(18, 1, { wallL });
		tmp.replace(22, 8, "BALLROOM");
		tmp.replace(33, 1, { wallR });
		tmp.replace(38, 1, { wallL });
		boardLayout.append(tmp);

		//line 23
		tmp = line2;
		tmp.replace(1, 12, "CONSERVATORY");
		tmp.replace(13, 1, { wallR });
		tmp.replace(18, 1, { wallL });
		tmp.replace(33, 1, { wallR });
		tmp.replace(38, 1, { wallL });
		tmp.replace(41, 7, "KITCHEN");
		boardLayout.append(tmp);

		//line 24
		tmp = line2;
		tmp.replace(1, 1, " ");
		tmp.replace(13, 1, { wallR });
		tmp.replace(18, 5, { solid, wallD, wallD, wallD, wallD });
		tmp.replace(29, 5, { wallD, wallD, wallD, wallD, solid });
		tmp.replace(38, 1, { wallL });
		boardLayout.append(tmp);

		//line 25
		tmp = line2;
		tmp.replace(2, 1, "?");
		tmp.replace(13, 3, { wallR, solid, solid });
		tmp.replace(22, 1, { solid });
		tmp.replace(29, 1, { solid });
		tmp.replace(36, 3, { solid, solid, wallL });
		tmp.replace(51, 1, "?");
		boardLayout.append(tmp);

		//line 26
		tmp = line1;
		tmp.replace(20, 2, "  ");
		tmp.replace(30, 2, "  ");
		boardLayout.append(tmp);

		//boardLayout.append(line2);
		boardLayout.append(line1);

		//Setting board tiles
		//STUDY
		for (int c = 0; c < 4; c++) {
			boardTiles[6][c] = Tile(Wall);
			if (c == 3) {
				for (int d = 0; d < 6; d++)
					boardTiles[d][c] = Tile(Wall);
			}
		}

		//HALL
		for (int c = 0; c < 7; c++) {
			boardTiles[9][c] = Tile(Wall);
			boardTiles[14][c] = Tile(Wall);
			if(c==6)
				for(int d=1; d <= 4; d++)
					boardTiles[9+d][c] = Tile(Wall);
		}

		//LOUNGE
		for (int c = 0; c < 6; c++) {
			boardTiles[17][c] = Tile(Wall);
			if (c == 5) {
				for (int d = 0; d < 8; d++)
					boardTiles[17+d][c] = Tile(Wall);
			}
		}

		//LIBRARY
		for (int c = 0; c < 5; c++) {
			if (c == 4 || c == 0) {
				for (int d = 0; d < 6; d++)
					boardTiles[d][6+c] = Tile(Wall);
			}
			else {
				boardTiles[6][6+c] = Tile(Wall);
			}
		}

		//BILLIARD
		for (int c = 0; c < 5; c++) {
			if (c == 4 || c == 0) {
				for (int d = 0; d < 6; d++)
					boardTiles[d][12 + c] = Tile(Wall);
			}
			else {
				boardTiles[5][12 + c] = Tile(Wall);
			}
		}

		//CONSERVATORY
		for (int c = 0; c < 5; c++) {
			if (c == 0) {
				for (int d = 0; d < 5; d++)
					boardTiles[d][19 + c] = Tile(Wall);
			}
			else {
				boardTiles[5][19 + c] = Tile(Wall);
			}
		}

		//BASEMENT
		for (int c = 0; c < 7; c++) {
			if (c == 1 || c==6) {
				for (int d = 0; d < 5; d++)
					boardTiles[9+d][8 + c] = Tile(Wall);
			}
			else {
				boardTiles[9][8 + c] = Tile(Wall);
				boardTiles[13][8 + c] = Tile(Wall);
			}
		}

		//BALLROOM
		for (int c = 0; c < 7; c++) {
			if (c == 0 || c==5) {
				for (int d = 0; d < 8; d++)
					boardTiles[8 + d][17 + c] = Tile(Wall);
			}
			else if (c == 6) {
				boardTiles[10][17 + c] = Tile(Wall);
				boardTiles[13][17 + c] = Tile(Wall);
			}
			else {
				boardTiles[8][17 + c] = Tile(Wall);
				boardTiles[15][17 + c] = Tile(Wall);
			}
		}

		//DINING ROOM
		for (int c = 0; c < 7; c++) {
			if (c == 0) {
				for (int d = 0; d < 9; d++)
					boardTiles[16+d][9 + c] = Tile(Wall);
			}
			else if (c == 5) {
				for (int d = 0; d < 3; d++)
					boardTiles[16 + d][9 + c] = Tile(Wall);
			}
			else if (c == 6) {
				for (int d = 3; d < 8; d++)
					boardTiles[16 + d][9 + c] = Tile(Wall);
			}
			else {
				boardTiles[16][9 + c] = Tile(Wall);
			}
		}

		//KITCHEN
		for (int c = 0; c < 6; c++) {
			boardTiles[18][18+c] = Tile(Wall);
			if (c == 0) {
				for (int d = 0; d < 7; d++)
					boardTiles[18+d][18+c] = Tile(Wall);
			}
		}

		//EXTRA WALLS
		for (int c = 0; c < 24; c++) {
			if (c != 9 && c != 14)
				boardTiles[c][24] = Tile(Wall);
		}

		boardTiles[0][4] = Tile(Wall);
		boardTiles[0][11] = Tile(Wall);
		boardTiles[0][17] = Tile(Wall);
		boardTiles[6][23] = Tile(Wall);
		boardTiles[17][23] = Tile(Wall);
		boardTiles[24][16] = Tile(Wall);
		boardTiles[24][8] = Tile(Wall);
		boardTiles[24][6] = Tile(Wall);
		boardTiles[8][0] = Tile(Wall);
		boardTiles[15][0] = Tile(Wall);

	}
	
	Player* GetCurrentPlayer() {
		return &players[curPlayerId];
	}

	void Draw(HANDLE out) {
		GoToXY(x, y);
		SetConsoleTextAttribute(out, Palette.Board);
		cout << boardLayout;
		 
		for (int c = 0; c < numPlayers; c++) {
			players[c].Draw(out, (x + MARGIN), (y + MARGIN));
		}
	}
	
	Player* NextPlayer() {
		curPlayerId++;
		if (curPlayerId >= numPlayers) {
			curPlayerId = 0;
		}
		return &players[curPlayerId];
	}

	bool IsWall(int x, int y) {
		return boardTiles[x][y].IsWall();
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

	GoToXY(0, 0);
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