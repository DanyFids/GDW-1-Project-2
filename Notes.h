#pragma once

class NotesScreen : public Screen {
private:
	static const int NUM_BUTTONS = 21;
	Button buttons[NUM_BUTTONS] = {
		Button("CandleSticks",20,3,P_WEAPON, (int)Candlestick),
		Button("Knife",20,4,P_WEAPON, (int)Knife),
		Button("Revolver",20,5,P_WEAPON, (int)Revolver),
		Button("Rope",20,6,P_WEAPON, (int)Rope),
		Button("Pipe",20,7,P_WEAPON, (int)Pipe),

		Button("Colonel Mustard",40,3,P_CHARACTER, (int)Col_Mustard),
		Button("Mrs.White",40,4,P_CHARACTER, (int)Ms_White),
		Button("Miss.Scarlet",40,5,P_CHARACTER, (int)Ms_Scarlet),
		Button("Mrs. Peacock",40,6,P_CHARACTER, (int)Mrs_Peacock),
		Button("Mr. Green",40,7,P_CHARACTER, (int)Mr_Green),
		Button("Professor Plum",40,8,P_CHARACTER, (int)Prof_Plum),

		Button("Hall",60,3,P_ROOMS, (int)Hall),
		Button("Lounge",60,4,P_ROOMS, (int)Lounge),
		Button("Dining Room",60,5,P_ROOMS, (int)DinningRoom),
		Button("Kitchen",60,6,P_ROOMS, (int)Kitchen),
		Button("Ballroom",60,7,P_ROOMS, (int)BallRoom),
		Button("Conservatory",60,8,P_ROOMS, (int)Conservatory),
		Button("Billiard Room",60,9,P_ROOMS, (int)BilliardRoom),
		Button("Library",60,10,P_ROOMS, (int)Library),
		Button("Study",60,11,P_ROOMS, (int)Study),

		Button("Return", 50, 12, CANCEL)
	};
	
	void LoadNotes(Notes n) {
		for (int w = 0; w < 6; w++) {
			for (int i = 0;i < n.sus_w_num; i++) {
				n.sus_w[i] = (Weapon)w;
		}
		}


	}
public:
	NotesScreen() {
		SetNumButtons(NUM_BUTTONS);
	}

	Button * GetButtons() {
		return buttons;
	}

	void Draw(HANDLE out) {
		Notes n = curPlayer->GetNotes();
		for (int b = 0; b < NUM_BUTTONS; b++) {
			buttons[b].draw(out);
		}
	}
};