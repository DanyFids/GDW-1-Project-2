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
#include"Suggestion.h"
#include"Notes.h"

using namespace std;

// Solution
class Solution {
private:
	Weapon murderWep;
	Character murderer;
	Rooms murderRoom;

public:
	Solution() {
		murderWep = Candlestick;
		murderer = Ms_Scarlet;
		murderRoom = Hall;
	}

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
	int nX = x, nY = y;
	bool colision = false;

	if (board.IsEntrance(x, y) && d == board.GetDir(x, y) && movePoints > 0 && board.GetRoom(x, y) != startRoom) {
		inRoom = true;
		room = board.GetRoom(x, y);
		COORD roomC = board.GetRoomCoord(pChar, room);
		x = roomC.X;
		y = roomC.Y;
		canMove = false;
		movePoints = 0;
		board.EnablePrediction();
	}
	else {
		if (movePoints > 0) {
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
	movePoints = 0;
	canMove = false;
	b.DisableRoll();
	b.DisableSecretPassage();
	if (r != Basement) {
		b.EnablePrediction();
	}
	else {
		LastGuess();
	}
}

void Player::ExitRoom(Board b) {
	if (selectExit != -1) {
		COORD exit = Exits[selectExit];
		x = exit.X;
		y = exit.Y;

		inRoom = false;
		room = Null;
		movePoints--;
		b.DisableSecretPassage();
		b.DisablePrediction();
	}
}

void Player::TurnStart(Board board) {
	blinkTimer = 0;
	isTurn = true;
	canMove = false;
	board.EnableRoll();

	if (inRoom) {
		board.GetExits(room, numExits, Exits);
		selectExit = -1;
		startXY.X = 0;
		startXY.Y = 0;
		startRoom = room;
	}
	else {
		startRoom = Null;
		startXY.X = x;
		startXY.Y = y;
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
Deck<COORD, 18> InPlayDeck;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE inputH;
COORD CursorLoc, KeyLoc, EndLoc;

//Screens
MainMenu mainMenu;
Board gameBoard;
Suggestion prediction;
ShareInfo reveal;
NotesScreen notebook;

Solution answer;

bool play = true;

Player *curPlayer = gameBoard.GetCurrentPlayer();

int main() {
	SetConsoleCP(437);

	//set random seed
	srand((unsigned int)time(NULL));

	const int inputR_SIZE = 128;
	DWORD iNumRead, consoleModeSave, consoleMode;
	INPUT_RECORD inputR[inputR_SIZE];

	//hide cursor
	CONSOLE_CURSOR_INFO cursor;
	GetConsoleCursorInfo(console, & cursor);
	cursor.bVisible = false;
	SetConsoleCursorInfo(console, & cursor);


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

	StartGame();

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
	case Prediction:
		prediction.Draw(out);
		GoToXY(0, 0);
		break;
	case Share:
		reveal.Draw(out);
		GoToXY(0, 0);
		break;
	case NotesS:
		notebook.Draw(out);
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
					curPlayer->ExitRoom(gameBoard);
				}
			}
			break;
		case 0x50: // P key
			PromptPlayer(Ms_Scarlet);
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
	case Play:
		scrn = &gameBoard;
		break;
	case Prediction:
		scrn = &prediction;
		break;
	case Share:
		scrn = &reveal;
		break;
	case NotesS:
		scrn = &notebook;
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
			if (btns[c].isOver(e.dwMousePosition) && e.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && !btns[c].IsDisabled()) {
				ButtonHandler(btns[c].GetAction(), btns[c].GetExtra());
			}
		}
	}

	if (state == Play && curPlayer->IsInRoom()) {
		if (curPlayer->OverExit(e.dwMousePosition) && e.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
			curPlayer->ExitRoom(gameBoard);
		}
	}
}

/************************
	  ButtonHandler
*************************
	Handles Action from Button
*/

void ButtonHandler(ACTION action, int extra) {
	int roll = 0;
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
		gameBoard.DisableSecretPassage();
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
			<< "\n"
			<< "Movement \n"
			<< "Players move their suspect token across the squares the amount shown on the die in their roll.You may move your \n"
			<< "token forwards, backwards or sideways all in one turn using the arrow keys. \n"
			<< "Moving your token diagonally is against the Clue rules.You cannot move your token to a particular space twice in the \n" 
			<< "same turn nor occupy or move through the same square as another player. \n"
			<< "This rule does not apply to rooms as multiple players and weapons may occupy the same room. \n"
			<< "\n"
			<< "Rooms \n"
			<< "The Clue board game rules state that there are two ways to enter or exit a room : 1) entering through the doorway \n"
			<< "by moving your token the number shown on the die across the squares, and too one of the doors. You then leave \n"
			<< "the room by going through any of the doors in that room. 2) Through the secret passage which is an option in some \n"
			<< "of the rooms and will teleport you to another room. \n"
			<< "\n"
			<< "Guessing the Killer \n"
			<< "Once a player enters a room, they are then eligible to make a “suggestion” on who committed the Clue game murder. \n"
			<< "This is done when the player names a Suspect, a Weapon, and the Room \n"
			<< "that they are currently in(“I suggest that the crime was committed in the Library, by Prof.Plum with the Rope”). \n"
			<< "Forfeiting or not taking your turn is against the rules; all players must roll the die.After entering a room and \n"
			<< "making a suggestions, you may not make another suggestions until entering \n"
			<< "another room or using at least two turns(leaving and then re - entering the same room). \n"
			<< "\n"
			<< "It is not against the rules, to deliberately make a \n"
			<< "Suggestion naming one or two cards that you hold in your own hand to mislead \n"
			<< "other players or to narrow your search for one specific suspect, room, or weapon. \n"
			<< "\n"
			<< "When an Accusation is made, the player(on their turn) states that an Accusation is being made and states the three \n"
			<< "cards that he thinks committed the murder.The game will then say if the guess was correct or wrong. \n"
			<< "Unlike the previously stated rules, a player may make an accusation whether or not their token is in the room they \n"
			<< "mention.If the 3 cards named are correct \n"
			<< "that player is the winner.If the accusation is wrong, they lose the game. \n" << endl;
		break;
	case ROLL:
		roll = RollDie();
		curPlayer->CanMove();
		curPlayer->SetStartMove(roll);
		movePoints = roll;
		gameBoard.DisableRoll();
		gameBoard.DisablePrediction();
		break;
	case RESET_MOVE:
		ResetMove();
		break;
	case END_TURN:
		NextTurn();
		break;
	case PREDICTION:
		state = Prediction;
		prediction.LoadNotes(curPlayer->GetNotes());
		prediction.ClearArea();
		if (extra == 0) {
			prediction.SetupPrediction(curPlayer->GetRoom());
		}
		else {
			prediction.SetupAccusation();
		}
		break;
	case P_WEAPON:
		if (state == Prediction) {
			prediction.Select((Weapon)extra);
		}
		else if (state == Share) {
			COORD card;
			card.X = 0;
			card.Y = extra;
			reveal.RevealCard(console, card);
			curPlayer->AddDiscovery(card);
			state = Play;
		}
		break;
	case P_CHARACTER:
		if (state == Prediction) {
			prediction.Select((Character)extra);
		}
		else if (state == Share) {
			COORD card;
			card.X = 1;
			card.Y = extra;
			reveal.RevealCard(console, card);
			curPlayer->AddDiscovery(card);
			state = Play;
		}
		break;
	case P_ROOMS:
		if (state == Prediction) {
			prediction.Select((Rooms)extra);
		}
		else if(state == Share){
			COORD card;
			card.X = 2;
			card.Y = extra;
			reveal.RevealCard(console, card);
			curPlayer->AddDiscovery(card);
			state = Play;
		}
		break;
	case CANCEL:
		switch (state) {
		case Prediction:
			state = Play;
			clear();
			break;
		case Warning:
			state = Play;
			clear();
			ResetMove();
			break;
		case NotesS:
			state = Play;
			clear();
		}
		break;
	case ACCEPT:
		switch (state) {
		case Warning:
			prediction.SetupAccusation();
			state = Prediction;
		}
	case ACCUSE:
		Accuse();
		gameBoard.DisablePrediction();
		gameBoard.DisableResetMv();
		break;
	case NOTES:
		state = NotesS;
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

void StartGame() {
	Character pChars[] = { Mr_Green, Ms_Scarlet, Ms_White, Col_Mustard};
	gameBoard.SetBoard(pChars, 4);

	WeaponDeck.shuffle();
	CharacterDeck.shuffle();
	RoomDeck.shuffle();

	answer = Solution(WeaponDeck.draw(), CharacterDeck.draw(), RoomDeck.draw());

	int numCards = 0;
	COORD tempDeck[18];
	while (WeaponDeck.checkDeck()) {
		COORD temp;
		temp.X = Weap;
		temp.Y = WeaponDeck.draw();
		tempDeck[numCards] = temp;
		numCards++;
	}

	while (CharacterDeck.checkDeck()) {
		COORD temp;
		temp.X = Char;
		temp.Y = CharacterDeck.draw();
		tempDeck[numCards] = temp;
		numCards++;
	}

	while (RoomDeck.checkDeck()) {
		COORD temp;
		temp.X = Room;
		temp.Y = RoomDeck.draw();
		tempDeck[numCards] = temp;
		numCards++;
	}

	InPlayDeck = Deck<COORD, 18>(tempDeck);
	Player* players = gameBoard.getPlayers();
	int playerId = 0;

	while (InPlayDeck.checkDeck()) {
		players[playerId].GiveCard(InPlayDeck.draw());
		playerId++;

		if (playerId >= gameBoard.NumPlayers()) {
			playerId = 0;
		}
	}

}

void NextTurn() {
	curPlayer->TurnEnd();
	curPlayer = gameBoard.NextPlayer();
	curPlayer->TurnStart(gameBoard);
	movePoints = 0;
	gameBoard.EnableResetMv();
	gameBoard.DisablePrediction();
	if (curPlayer->CheckWasMoved()) {
		gameBoard.EnablePrediction();
	}
	PromptPlayer(curPlayer->GetChar());
}

void ResetMove() {
	if (curPlayer->GetStartRoom() != Null) {
		curPlayer->EnterRoom(curPlayer->GetStartRoom(), gameBoard);
		if (curPlayer->GetStartRoom() == Study || curPlayer->GetStartRoom() == Lounge || curPlayer->GetStartRoom() == Kitchen || curPlayer->GetStartRoom() == Conservatory) {
			gameBoard.EnableSecretPass();
		}
	}
	else {
		COORD XY = curPlayer->GetStartXY();
		curPlayer->SetInRoom(false);
		curPlayer->SetXY(XY.X, XY.Y);
		
	}
	gameBoard.DisablePrediction();
	movePoints = curPlayer->GetStartMove();
	curPlayer->CanMove();
}

int RollDie() {
	int randomNum;
	randomNum = rand() % 6 + 1;
	cout << randomNum << endl;
	return randomNum;
}

void Accuse() {
	if (prediction.Ready()) {
		Weapon wGuess;
		Character cGuess;
		Rooms rGuess;

		prediction.GetGuess(wGuess, cGuess, rGuess);

		Player * players = gameBoard.getPlayers();

		int numP = gameBoard.NumPlayers();

		for (int p = 0; p < numP; p++) {
			if (players[p].GetChar() == cGuess) {
				players[p].EnterRoom(rGuess, gameBoard);
				players[p].SetWasMoved(true);
			}
		}

		int curId = gameBoard.GetCurPlayerId();
		COORD cards[3];
		int numCards = 0;

		bool cardFound = false;

		for (int p = curId; p < numP + curId; p++) {
			int pId;
			if (p >= numP) {
				pId = p - numP;
			}
			else {
				pId = p;
			}

			int handSize;
			COORD hand[10];
			players[pId].GetHand(hand, handSize);

			if (pId != curId) {

				for (int c = 0; c < handSize; c++) {
					if ((hand[c].X == 0 && (Weapon)hand[c].Y == wGuess) ||
						(hand[c].X == 1 && (Character)hand[c].Y == cGuess) ||
						(hand[c].X == 2 && (Rooms)hand[c].Y == rGuess)) {
						cards[numCards] = hand[c];
						numCards++;
					}

				}
				if (numCards > 0) {
					cardFound = true;
					clear();
					gameBoard.Draw(console);
					Show(players[pId].GetChar(), cards, numCards);
					break;
				}
			}
		}

		if (!cardFound) {
			state = Play;
			clear();
		}

		gameBoard.DisablePrediction();
	}
}

void DrawPrompt() {
	clear();
	gameBoard.Draw(console);

	string top = { ' ', (char)201, (char)205, (char)205, (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)187, ' ' };
	string mid = { ' ', (char)186, ' ', ' ', ' ' , ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', (char)186, ' ' };
	string bot = { ' ', (char)200, (char)205, (char)205, (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)188, ' ' };
	string buffer = { ' ', ' ', ' ', ' ', ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' };

	GoToXY(30, 4);
	cout << buffer;
	for (int i = 0; i < 6; i++) {
		GoToXY(30, 5 + i);
		if (i == 0) {
			cout << top;
		}
		else if (i == 5) {
			cout << bot;
		}
		else {
			cout << mid;
		}
	}
	GoToXY(30, 11);
	cout << buffer;
}

void Show(Character p, COORD cards[], int numC) {
	PromptPlayer(p);

	DrawPrompt();

	GoToXY(33, 6);
	cout << "Reveal to ";
	SetConsoleTextAttribute(console, GetColor(curPlayer->GetChar()));
	cout << toString(curPlayer->GetChar());
	SetConsoleTextAttribute(console, Palette.Default);

	reveal.LoadButtons(cards, numC);
	reveal.LoadCharacters(curPlayer->GetChar(), p);
	state = Share;
}

/***************************
		PromptPlayer
****************************
	Prompts Current user to switch with the next User based on player Character
*/
void PromptPlayer(Character c) {
	DrawPrompt();

	GoToXY(33, 7);
	cout << "  Switch with ";
	SetConsoleTextAttribute(console, GetColor(c));
	cout << toString(c);
	SetConsoleTextAttribute(console, Palette.Default);

	GoToXY(33, 9);
	cout << "Press any key to continue...";

	const int inputR_SIZE = 128;
	INPUT_RECORD inputR[inputR_SIZE];
	DWORD iNumRead;

	//Clear input buffer
	ReadConsoleInput(
		inputH,
		inputR,
		inputR_SIZE,
		&iNumRead
	);

	while (true) {
		ReadConsoleInput(
			inputH,
			inputR,
			inputR_SIZE,
			&iNumRead
		);

		if (iNumRead > 0) {
			bool keyHit = false;
			for (DWORD c = 0; c < iNumRead; c++) {
				if (inputR[c].EventType == KEY_EVENT || (inputR[c].EventType == MOUSE_EVENT && inputR[c].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)) {
					keyHit = true;
				}
			}
			if(keyHit)
				break;
		}
	}

	clear();
}

void LastGuess() {
	DrawPrompt();

	GoToXY(33, 6);
	SetConsoleTextAttribute(console, 12);
	cout << "WARNING! Entering the BASEMENT will trigger your FINAL ACCUSATION. There's NO GOING BACK.";

	state = Warning;
}

 