#pragma once
using namespace std;

//Class declarations
class Board; // Defined in Board.h

int movePoints = 6;

class Player {
private:
	const int BOARD_D = 24;
	const int BLINK_TIME = 4;
	Character pChar;
	Rooms room = Null;
	string token;
	int color;
	int x, y;
	int blinkTimer = 0;
	bool isTurn = false;
	bool inRoom = false;
	COORD Exits[4];
	int numExits;
	int selectExit = -1;
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

	bool move(Direction d, Board board); // Defined in Main

	void ExitRoom() {
		if (selectExit != -1) {
			COORD exit = Exits[selectExit];
			x = exit.X;
			y = exit.Y;

			inRoom = false;
			room = Null;
			movePoints--;
		}
	}

	void EnterRoom(Rooms r, Board b);

	void Draw(HANDLE out, int offsetX, int offsetY) {
		GoToXY((x * 2 ) + offsetX, y + offsetY);
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

			if (inRoom) {
				for (int c = 0; c < numExits; c++) {
					GoToXY((Exits[c].X * 2) + offsetX, Exits[c].Y + offsetY);
					if (c == selectExit) {
						if(blinkTimer < BLINK_TIME)
							if(pChar != Ms_White)
								SetConsoleTextAttribute(out, color * 16);
							else
								SetConsoleTextAttribute(out, 1);
						else
							SetConsoleTextAttribute(out, Palette.Default * 16);
					}
					else {
						SetConsoleTextAttribute(out, Palette.Default * 16);
					}
					cout << "  ";
				}
			}

			blinkTimer++;
			if (blinkTimer >= BLINK_TIME * 2) {
				blinkTimer = 0;
			}
		}
	}

	void TurnStart(Board board);

	void TurnEnd() {
		blinkTimer = 0;
		isTurn = false;
	}

	int GetX() {
		return x;
	}

	int GetY() {
		return y;
	}

	bool IsInRoom() {
		return inRoom;
	}

	Rooms GetRoom() {
		return room;
	}

	bool OverExit(COORD mouseLoc) {
		if (inRoom) {
			for (int c = 0; c < numExits; c++) {
				if (mouseLoc.Y == Exits[c].Y + 1 && (mouseLoc.X == (Exits[c].X * 2) + 2 || mouseLoc.X == (Exits[c].X * 2) + 3)) {
					selectExit = c;
					return true;
				}
			}
		}

		selectExit = -1;
		return false;
	}
};