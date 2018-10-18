#pragma once

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
COORD mouseLoc;

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

