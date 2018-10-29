#pragma once

class NotesScreen : public Screen {
private:
	static const int NUM_BUTTONS = 22;
	Button buttons[NUM_BUTTONS] = {
		Button("CandleSticks",20,3,P_WEAPON, (int)Candlestick),
		Button("    Pipe    ",20,4,P_WEAPON, (int)Pipe),
		Button("   Wrench   ",20,5,P_WEAPON, (int)Wrench),
		Button("    Rope    ",20,6,P_WEAPON, (int)Rope),
		Button("    Knife   ",20,7,P_WEAPON, (int)Knife),
		Button("  Revolver  ",20,8,P_WEAPON, (int)Revolver),
		
		Button("Ms. Scarlet ",38,3,P_CHARACTER, (int)Ms_Scarlet),
		Button("Col. Mustard",38,4,P_CHARACTER, (int)Col_Mustard),
		Button("  Ms. White ",38,5,P_CHARACTER, (int)Ms_White),
		Button("  Mr. Green ",38,6,P_CHARACTER, (int)Mr_Green),
		Button("Mrs. Peacock",38,7,P_CHARACTER, (int)Mrs_Peacock),
		Button(" Prof. Plum ",38,8,P_CHARACTER, (int)Prof_Plum),

		Button("    Hall     ",56,3,P_ROOMS, (int)Hall),
		Button("   Lounge    ",56,4,P_ROOMS, (int)Lounge),
		Button("    Study    ",56,5,P_ROOMS, (int)Study),
		Button("   Library   ",56,6,P_ROOMS, (int)Library),
		Button("Billiard Room",56,7,P_ROOMS, (int)BilliardRoom),
		Button("Conservatory ",56,8,P_ROOMS, (int)Conservatory),
		Button("   Ballroom  ",56,9,P_ROOMS, (int)BallRoom),
		Button("   Kitchen   ",56,10,P_ROOMS, (int)Kitchen),
		Button(" Dining Room ",56,11,P_ROOMS, (int)DinningRoom),

		Button("Return", 50, 13, CANCEL)
	};

	Notes *notes;

	Button* wBtns = &buttons[0];
	Button* cBtns = &buttons[6];
	Button* rBtns = &buttons[12];

	string UI[12] = {
		" |------------------------Notes-------------------------| ",
		{' ', (char)201, (char)205, 'W', 'e', 'a', 'p', 'o', 'n', 's', (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)203, (char)205, 'C', 'h', 'a', 'r', 'a', 'c', 't', 'e', 'r', 's', (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)203, (char)205, 'R', 'o', 'o', 'm', 's', (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)187},
		{(char)186},
		{(char)186},
		{(char)186},
		{(char)186},
		{(char)186},
		{(char)186},
		{(char)186},
		{(char)186},
		{(char)186},
		{' ',(char)200, (char)205, (char)205, (char)205,(char)205, (char)205, (char)205, (char)205, (char)205,(char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)202, (char)205,(char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)202, (char)205,(char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)188, ' ' }
	};

	string buffer = "                                                          ";
	
public:
	NotesScreen() {
		SetNumButtons(NUM_BUTTONS);
	}

	Button * GetButtons() {
		return buttons;
	}

	void Draw(HANDLE out){
		for (int b = 0; b < NUM_BUTTONS; b++) {
			buttons[b].draw(out);
		}

		for (int i = 0; i < 9; i++) {
			if (i < 6) {
				if (!wBtns[i].IsDisabled()) {
					bool wFound = false;
					for (int w = 0; w < notes->sus_w_num; w++) {
						if (notes->sus_w[w] == (Weapon)i) {
							wFound = true;
							break;
						}
					}
					if (wFound) {
						GoToXY(34, 3 + i);
						cout << "[?]";
					}
					else {
						GoToXY(34, 3 + i);
						cout << "[ ]";
					}
				}
				if (!cBtns[i].IsDisabled()) {
					bool cFound = false;
					for (int c = 0; c < notes->sus_c_num; c++) {
						if (notes->sus_c[c] == (Character)i) {
							cFound = true;
							break;
						}
					}
					if (cFound) {
						GoToXY(52, 3 + i);
						cout << "[?]";
					}
					else {
						GoToXY(52, 3 + i);
						cout << "[ ]";
					}
				}
			}

			if (!rBtns[i].IsDisabled()) {
				bool rFound = false;
				for (int r = 0; r < notes->sus_r_num; r++) {
					if (notes->sus_r[r] == (Rooms)(i + 2)) {
						rFound = true;
						break;
					}
				}
				if (rFound) {
					GoToXY(71, 3 + i);
					cout << "[?]";
				}
				else {
					GoToXY(71, 3 + i);
					cout << "[ ]";
				}
			}
		}
	}

	void LoadNotes(Notes* n) {
		notes = n;

		for (int b = 0; b < 23; b++) {
			GoToXY(18, 1 + b);
			cout << buffer;
		}

		for (int d = 0; d < 12; d++) {
			if (d <= 10 && d >= 2) {
				GoToXY(18, 1 + d);
				cout << " " << UI[d];
				GoToXY(37, 1 + d);
				cout << UI[d];
				GoToXY(55, 1 + d);
				cout << UI[d];
				GoToXY(74, 1 + d);
				cout << UI[d] << " ";
			}
			else {
				GoToXY(18, 1 + d);
				cout << UI[d];
			}
		}

		for (int e = 0; e < 9; e++) {
			if (e < 6) {
				wBtns[e].Enable();
				cBtns[e].Enable();
			}
			rBtns[e].Enable();
		}

		for (int i = 0; i < 9; i++) {
			if (i < 6) {
				for (int w = 0; w < n->w_num; w++) {
					if (n->w[w] == (Weapon)i) {
						wBtns[i].Disable();
						GoToXY(34, 3+i);
						cout << "[X]";
						break;
					}
				}
				for (int c = 0; c < n->c_num; c++) {
					if (n->c[c] == (Character)i) {
						cBtns[i].Disable();
						GoToXY(52, 3 + i);
						cout << "[X]";
						break;
					}
				}
			}
			for (int r = 0; r < n->r_num; r++) {
				if (n->r[r] == (Rooms) (i + 2)) {
					rBtns[i].Disable();
					GoToXY(71, 3 + i);
					cout << "[X]";
					break;
				}
			}
		}
	}
};