#pragma once

class Suggestion : public Screen {
private:
	Notes notes;
	Weapon wGuess;
	Character cGuess;
	Rooms rGuess;

	bool wSelected;
	bool cSelected;
	bool rSelected;

	string UI[16] = {
		" |-----------------Prediction-----------------| ",
		{' ', (char)201, (char)205, 'W', 'e', 'a', 'p', 'o', 'n', (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char) 203, (char)205, 'C', 'h', 'a', 'r', 'a', 'c', 't', 'e', 'r', (char)205, (char)205, (char)205, (char)205, (char) 203, (char)205, 'R', 'o', 'o', 'm', (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char) 187},
		{(char)186},
		{' ',(char) 204, (char)205, (char)205,(char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char) 206, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char) 206, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char) 185},
		{(char)186},
		{(char)186},
		{(char)186},
		{(char)186},
		{(char)186},
		{(char)186},
		{(char)186},
		{(char)186},
		{(char)186},
		{(char)186},
		{(char)186},
		{' ',(char)200, (char)205, (char)205,(char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)202, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)202, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)188, ' ' }
	};

	static const int NUM_BUTTONS = 23;
	Button buttons[NUM_BUTTONS] = {
		Button("Candlestick", 32, 7, P_WEAPON, (int) Candlestick),
		Button("   Pipe    ", 32, 8, P_WEAPON, (int)Pipe),
		Button("  Wrench   ", 32, 9, P_WEAPON, (int)Wrench),
		Button("   Rope    ", 32, 10, P_WEAPON, (int)Rope),
		Button("   Knife   ", 32, 11, P_WEAPON, (int)Knife),
		Button("  Revolver ", 32, 12, P_WEAPON, (int)Revolver),

		Button("Ms. Scarlet ", 46, 7, P_CHARACTER, (int)Ms_Scarlet),
		Button("Col. Mustard", 46, 8, P_CHARACTER, (int)Col_Mustard),
		Button(" Ms. White  ", 46, 9, P_CHARACTER, (int)Ms_White),
		Button(" Mr. Green  ", 46, 10, P_CHARACTER, (int)Mr_Green),
		Button("Mrs. Peacock", 46, 11, P_CHARACTER, (int)Mrs_Peacock),
		Button(" Prof. Plum ", 46, 12, P_CHARACTER, (int)Prof_Plum),

		Button("     Hall    ", 61, 7, P_ROOMS, (int)Hall),
		Button("    Lounge   ", 61, 8, P_ROOMS, (int)Lounge),
		Button("     Study   ", 61, 9, P_ROOMS, (int)Study),
		Button("   Library   ", 61, 10, P_ROOMS, (int)Library),
		Button("Billiard Room", 61, 11, P_ROOMS, (int)BilliardRoom),
		Button("Conservatory ", 61, 12, P_ROOMS, (int)Conservatory),
		Button("  Ball Room  ", 61, 13, P_ROOMS, (int)BallRoom),
		Button("   Kitchen   ", 61, 14, P_ROOMS, (int)Kitchen),
		Button(" Dining Room ", 61, 15, P_ROOMS, (int)DinningRoom),

		Button("Accuse", 45, 19, ACCUSE),
		Button("Cancel", 55, 19, CANCEL)
	};

public:	
	Suggestion() {
		SetNumButtons(NUM_BUTTONS);
	}

	void LoadNotes(Notes n) {
		notes = n;
	}

	void Draw(HANDLE out) {
		for (int d = 0; d < 16; d++) {
			if ((d <= 14 && d >= 4) || d == 2) {
				GoToXY(30, 3 + d);
				cout << " " << UI[d];
				GoToXY(45, 3 + d);
				cout << UI[d];
				GoToXY(60, 3 + d);
				cout << UI[d];
				GoToXY(76, 3 + d);
				cout << UI[d] << " ";
			}
			else {
				GoToXY(30, 3 + d);
				cout << UI[d];
			}
		}

		if (wSelected) {
			GoToXY(33, 5);
			cout << toString(wGuess);
		}

		if (cSelected) {
			GoToXY(47, 5);
			cout << toString(cGuess);
		}

		if (rSelected) {
			GoToXY(62, 5);
			cout << toString(rGuess);
		}


		for (int b = 0; b < GetNumButtons(); b++) {
			buttons[b].draw(out);
		}
	}

	void SetupPrediction(Rooms r) {
		rGuess = r;
		rSelected = true;
		
		wSelected = false;
		cSelected = false;

		for (int c = 12; c < 21; c++) {
			buttons[c].Disable();
		}
	}

	void SetupAccusation() {
		rSelected = false;
		wSelected = false;
		cSelected = false;

		for (int c = 12; c < 21; c++) {
			buttons[c].Enable();
		}
	}

	void ClearArea() {
		for (int c = 0; c < 19; c++) {
			GoToXY(30, 2 + c);
			cout << "                                                ";
		}
	}

	Button* GetButtons() {
		return buttons;
	}

	void Select(Weapon w) {
		GoToXY(33, 5);
		cout << "           ";
		wGuess = w;
		wSelected = true;
	}

	void Select(Character c) {
		GoToXY(47, 5);
		cout << "            ";
		cGuess = c;
		cSelected = true;
	}

	void Select(Rooms r) {
		GoToXY(62, 5);
		cout << "             ";
		rGuess = r;
		rSelected = true;
	}

	bool Ready() {
		return (wSelected && cSelected && rSelected);
	}

	void GetGuess(Weapon & wG, Character & cG, Rooms & rG) {
		wG = wGuess;
		cG = cGuess;
		rG = rGuess;
	}
};

class ShareInfo : public Screen {
private:
	int numCards;
	Button buttons[3];
	COORD btnPos[3];
	Character from;
	Character to;
public:
	ShareInfo(){
		btnPos[0].X = 32;
		btnPos[0].Y = 7;
		btnPos[1].X = 32;
		btnPos[1].Y = 8;
		btnPos[2].X = 32;
		btnPos[2].Y = 9;
	}

	Button* GetButtons() {
		return buttons;
	}

	void Draw(HANDLE out) {
		for (int b = 0; b < numCards; b++) {
			buttons[b].draw(out);
		}
	}

	void LoadButtons(COORD cards[], int numC) {
		string base = { ' ', ' ', ' ', ' ', ' ', ' ', ' ' , ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ' };
		numCards = numC;
		for (int b = 0; b < numCards; b++) {
			ACTION action;
			string str;
			switch (cards[b].X) {
			case 0:
				action = P_WEAPON;
				str = toString((Weapon)cards[b].Y);
				break;
			case 1:
				action = P_CHARACTER;
				str = toString((Character)cards[b].Y);
				break;
			case 2:
				action = P_ROOMS;
				str = toString((Rooms)cards[b].Y);
				break;
			}
			int baseHalf = base.length() / 2;
			int strHalf = str.length() / 2;

			string btnStr = base;
			btnStr.replace(baseHalf - strHalf, str.length(), str);

			buttons[b] = Button(btnStr, btnPos[b].X, btnPos[b].Y, action, cards[b].Y);
		}

		SetNumButtons(numC);
	}

	void LoadCharacters(Character t, Character f) {
		to = t;
		from = f;
	}

	void RevealCard(HANDLE out, COORD card) {
		PromptPlayer(to);
		DrawPrompt();
		SetConsoleTextAttribute(out, GetColor(from));
		GoToXY(33, 6);
		cout << toString(from);
		SetConsoleTextAttribute(out, Palette.Default);
		cout << " has revealed:";
		GoToXY(33, 7);
		switch (card.X) {
		case 0:
			cout << "[Weapon] " << toString((Weapon)card.Y);
			break;
		case 1:
			cout << "[Character] " << toString((Character)card.Y);
			break;
		case 2:
			cout << "[Rooms] " << toString((Rooms)card.Y);
			break;
		}

		GoToXY(33, 9);
		cout << "Press any key to continue...";

		const int inputR_SIZE = 128;
		INPUT_RECORD inputR[inputR_SIZE];
		DWORD iNumRead;
		HANDLE inputH = GetStdHandle(STD_INPUT_HANDLE);

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
					if (inputR[c].EventType == KEY_EVENT) {
						keyHit = true;
					}
				}
				if (keyHit)
					break;
			}
		}

		clear();
	}
};