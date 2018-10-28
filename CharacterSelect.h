#pragma once
using namespace std;

enum SelectState {
	Num_Players,
	Character_Sel
};

class CharacterSelect : public Screen {
private:
	SelectState stt;

	Character PlayerChars[6];
	int num_sel = 0;
	int num_players = 0;

	static const int NUM_BUTTONS = 6;
	int nBArr[2] = {
		5, 6
	};

	Button buttons[2][NUM_BUTTONS]{
		{
			Button("       2 Players       ", 35, 15, SELECT, 2), 
			Button("       3 Players       ", 35, 16, SELECT, 3),
			Button("       4 Players       ", 35, 17, SELECT, 4),
			Button("       5 Players       ", 35, 18, SELECT, 5),
			Button("       6 Players       ", 35, 19, SELECT, 6)
		},
		{
			Button("      Ms. Scarlet      ", 35, 15, SELECT, (int) Ms_Scarlet),
			Button("      Col. Mustard     ", 35, 16, SELECT, (int) Col_Mustard),
			Button("       Ms. White       ", 35, 17, SELECT, (int) Ms_White),
			Button("       Mr. Green       ", 35, 18, SELECT, (int) Mr_Green),
			Button("      Mrs. Peacock     ", 35, 19, SELECT, (int) Mrs_Peacock),
			Button("       Prof. Plum      ", 35, 20, SELECT, (int) Prof_Plum)
		},
	};

	void DrawSelectScreen() {
		clear();

		DrawLogo(10, 1);

		string top = { ' ', (char)201, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)187, ' ' };
		string mid = { ' ', (char)186, ' ', ' ', ' ' , ' ', ' ' , ' ', ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', (char)186, ' ' };
		string bot = { ' ', (char)200, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)188, ' ' };
		string buffer = { ' ', ' ', ' ', ' ', ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ',  ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' };

		GoToXY(25, 10);
		cout << buffer;
		for (int i = 0; i < 13; i++) {
			GoToXY(25, 11 + i);
			if (i == 0) {
				cout << top;
			}
			else if (i == 12) {
				cout << bot;
			}
			else {
				cout << mid;
			}
		}
		GoToXY(25, 24);
		cout << buffer;
	}

public:
	CharacterSelect() {
		SetNumButtons(NUM_BUTTONS);
	}

	void Draw(HANDLE out) {
		if (stt == Num_Players) {
			for (int i = 0; i < GetNumButtons(); i++) {
				buttons[stt][i].draw(out);
			}
		}
		else {
			GoToXY(40, 13);
			cout << num_sel + 1;
			for (int i = 0; i < GetNumButtons(); i++) {
				buttons[stt][i].DrawColor(out, GetColor((Character) i));
			}
		}
	}

	Button * GetButtons() {
		return buttons[stt];
	}

	void SetPlayerNumSel() {
		stt = Num_Players;
		SetNumButtons(nBArr[stt]);

		DrawSelectScreen();

		GoToXY(33, 13);
		cout << "Select the Number of Players:";
	}

	void SetCharacterSelect(int numP) {

		stt = Character_Sel;
		SetNumButtons(nBArr[stt]);

		num_players = numP;

		DrawSelectScreen();

		GoToXY(33, 13);
		cout << "Player 1 Select Your Character:";
	}

	bool CharSelected(Character c) {
		PlayerChars[num_sel] = c;
		num_sel++;

		buttons[stt][c].Disable();

		if (num_sel == num_players)
			return true;
		else
			return false;
	}

	SelectState GetState() {
		return stt;
	}

	int GetNumPlayers() {
		return num_players;
	}

	Character * GetChosenCharacters() {
		return PlayerChars;
	}

	void Reset() {
		for (int i = 0; i < 6; i++) {
			buttons[1][i].Enable();
		}

		num_sel = 0;
	}
};