#pragma once
using namespace std;

//Class declarations
class Board; // Defined in Board.h

int movePoints = 0;

struct Notes {
	Weapon w[6];
	Character c[6];
	Rooms r[9];
	int w_num = 0;
	int c_num = 0;
	int r_num = 0;
};

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
	bool canMove = false;
	COORD Exits[4];
	int numExits;
	int selectExit = -1;

	bool wasMoved = false;

	Notes notes;

	Rooms startRoom;
	COORD startXY;
	int startMove;

	int handSize = 0;
	COORD hand[10];
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

	Player operator= (Player & p)
	{
		x = p.GetX();
		y = p.GetY();
		pChar = p.GetChar();
		token = GetToken(pChar);
		color = GetColor(pChar);
		return p;
	}

	bool move(Direction d, Board board); // Defined in Main

	void ExitRoom(Board b);

	void EnterRoom(Rooms r, Board b);

	void Draw(HANDLE out, int offsetX, int offsetY) {
		GoToXY((x * 2 ) + offsetX, y + offsetY);
		if (!isTurn) {
			SetConsoleTextAttribute(out, color);
			cout << token;
		}
		else {
			if (blinkTimer < BLINK_TIME) {
				SetConsoleTextAttribute(out, color);
				cout << token;
			}
			else {
				SetConsoleTextAttribute(out, color + 240);
				cout << token;
			}

			if (inRoom && canMove) {
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


			SetConsoleTextAttribute(out, Palette.Default);
			for (int h = 0; h < handSize; h++) {
				GoToXY(80, 0 + h);
				switch (hand[h].X) {
				case 0:
					cout << "[Weapon] " << toString((Weapon)hand[h].Y);
					break;
				case 1:
					cout << "[Character] " << toString((Character)hand[h].Y);
					break;
				case 2:
					cout << "[Room] " << toString((Rooms)hand[h].Y);
					break;
				}
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

	Character GetChar() {
		return pChar;
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

	void AddDiscovery(COORD card) {
		bool known = false;
		switch (card.X) {
		case 0:
			for (int c = 0; c < notes.w_num; c++) {
				if (notes.w[c] == (Weapon)card.Y)
					known = true;
			}
			if (!known) {
				notes.w[notes.w_num] = (Weapon)card.Y;
				notes.w_num++;
			}
			break;
		case 1:
			for (int c = 0; c < notes.c_num; c++) {
				if (notes.c[c] == (Character)card.Y)
					known = true;
			}
			if (!known) {
				notes.c[notes.c_num] = (Character)card.Y;
				notes.c_num++;
			}
			break;
		case 2:
			for (int c = 0; c < notes.r_num; c++) {
				if (notes.r[c] == (Rooms)card.Y)
					known = true;
			}
			if (!known) {
				notes.r[notes.r_num] = (Rooms)card.Y;
				notes.r_num++;
			}
			break;
		}
	}

	void GiveCard(COORD card) {
		hand[handSize] = card;
		handSize++;
		AddDiscovery(card);
	}

	void GetHand(COORD h[]) {
		h = hand;
	}

	void GetHand(COORD h[], int & hs) {
		for (int c = 0; c < handSize; c++) {
			h[c] = hand[c];
		}
		hs = handSize;
	}

	void CanMove() {
		canMove = true;
	}

	void SetStartMove(int m) {
		startMove = m;
	}

	int GetStartMove() {
		return startMove;
	}

	Rooms GetStartRoom() {
		return startRoom;
	}

	COORD GetStartXY() {
		return startXY;
	}

	void SetXY(int X, int Y) {
		x = X;
		y = Y;
	}

	void SetInRoom(bool r) {
		inRoom = r;
	}

	Notes GetNotes() {
		return notes;
	}

	bool CheckWasMoved() {
		bool ret = wasMoved;
		if (wasMoved) {
			wasMoved = false;
		}
		return ret;
	}

	void SetWasMoved(bool b) {
		wasMoved = b;
	}
};