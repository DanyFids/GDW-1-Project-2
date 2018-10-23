#pragma once
enum TileType {
	Floor,
	Wall,
	Entrance
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

enum Direction {
	None,
	Up,
	Down,
	Left,
	Right
};

enum GameState {
	DEBUG,
	Menu,
	Play
};

enum ACTION {
	DEFAULT,
	PLAY,
	NOTES,
	S_PASSAGE,
	ROLL,
	DISP_CONTROL,

	COLORS,
	CHARACTERS,
	RULES
};