#pragma once
using namespace std;

//Class declarations
class Board; // Defined in Board.h

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