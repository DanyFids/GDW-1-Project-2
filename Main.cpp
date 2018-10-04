#include<iostream>
#include<string>

template <class T>
class Deck {
private:
	T *cards;
	int topId = 0;
public:
	Deck(T c[]) {
		cards = c;

		shuffle();
	}

	T draw() {
		if (topId < cards.length()) {
			T hold = cards[topId];
			topId++;
			return hold;
		}
		else {
			return 0;
		}

		return 0;
	}

	bool checkDeck() {
		if (topId < cards.length()) {
			return true;
		}
		else {
			return false;
		}
		return false;
	}

	void shuffle() {
		topId = 0;
	}
};

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

Rooms BoardLayout[24][24] = {};

Weapon WeapCards[] = { Candlestick, Pipe, Wrench, Rope, Knife, Revolver };
Character CharCards[] = { Ms_Scarlet, Prof_Plum, Mr_Green, Mrs_Peacock, Col_Mustard, Ms_White };
Rooms RoomCards[] = { Hall, Lounge, Study, Library, BilliardRoom, Conservatory, BallRoom, Kitchen, DinningRoom };

Deck<Weapon> WeaponDeck(WeapCards);
Deck<Character> CharacterDeck(CharCards);
Deck<Rooms> RoomDeck(RoomCards);

int main() {
	


	return 0;
}