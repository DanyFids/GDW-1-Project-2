#pragma once
using namespace std;

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