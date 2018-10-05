#include<iostream>
#include<string>
#include<random>

using namespace std;

enum Rooms {
	Null,
	Basement,
	Hall,
	Lounge,
	Study,
	Library,
	BilliardRoom,
	Conservatory,
	BallRoom,
	Kitchen,
	DinningRoom
};

enum Character {
	Ms_Scarlet,
	Prof_Plum,
	Mr_Green,
	Mrs_Peacock,
	Col_Mustard,
	Ms_White
};

enum Weapon {
	Candlestick,
	Pipe,
	Wrench,
	Rope,
	Knife,
	Revolver
};

// Classes
// Deck
template <class T, int Size>
class Deck {
private:
	T *cards;
	int topId = 0;
	int deckSize = Size;
public:
	Deck(T c[]) {
		cards = c;

		shuffle();
	}

	T draw() {
		T hold;
		if (topId < deckSize) {
			hold = cards[topId];
			topId++;
		}

		return hold;
	}

	bool checkDeck() {
		if (topId < deckSize) {
			return true;
		}
		else {
			return false;
		}
		return false;
	}

	void shuffle() {
		random_device gen;
		uniform_int_distribution<int> range(0, deckSize - 1);

		topId = 0;

		for (int c = 0; c < deckSize; c++) {
			int d = range(gen);
			T hold = cards[d];
			cards[d] = cards[c];
			cards[c] = hold;
		}
	}
};

// Solution
class Solution {
private:
	Weapon murderWep;
	Character murderer;
	Rooms murderRoom;

public:
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

// Functions
/**************
   ToString
***************
	Converts Enum values to String.
*/
//Rooms
string toString(Rooms input) {
	switch (input) {
	case Null:
		return "Empty";
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

Rooms BoardLayout[24][24] = {};

Weapon WeapCards[] = { Candlestick, Pipe, Wrench, Rope, Knife, Revolver };
Character CharCards[] = { Ms_Scarlet, Prof_Plum, Mr_Green, Mrs_Peacock, Col_Mustard, Ms_White };
Rooms RoomCards[] = { Hall, Lounge, Study, Library, BilliardRoom, Conservatory, BallRoom, Kitchen, DinningRoom };

Deck<Weapon, 6> WeaponDeck(WeapCards);
Deck<Character, 6> CharacterDeck(CharCards);
Deck<Rooms, 9> RoomDeck(RoomCards);

int main() {
	

	return 0;
}