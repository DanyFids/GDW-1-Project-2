#include<iostream>
#include<string>

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

Rooms BoardLayout[24][24] = {};

Weapon WeaponDeck[6] = {Candlestick, Pipe, Wrench, Rope, Knife, Revolver};
Character CharacterDeck[6] = {Ms_Scarlet, Prof_Plum, Mr_Green, Mrs_Peacock, Col_Mustard, Ms_White };
Rooms RoomDeck[9] = { Hall, Lounge, Study, Library, BilliardRoom, Conservatory, BallRoom, Kitchen, DinningRoom };

int main() {
	return 0;
}