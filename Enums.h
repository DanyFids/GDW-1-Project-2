#pragma once
enum TileType {
	Floor,
	Wall,
	Entrance
};

enum CardType {
	Weap,
	Char,
	Room
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
	Col_Mustard,
	Ms_White,
	Mr_Green,
	Mrs_Peacock,
	Prof_Plum
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
	Play,
	Prediction,
	Pause,
	Share,
	Warning,
	NotesS,
	Char_Sel
};

enum ACTION {
	DEFAULT,
	PLAY,
	NOTES,
	S_PASSAGE,
	ROLL,
	DISP_CONTROL,
	RESET_MOVE,
	END_TURN,
	PREDICTION,
	ACCUSE,
	CANCEL,
	P_WEAPON,
	P_CHARACTER,
	P_ROOMS,
	START,
	ACCEPT,
	CREDITS,
	SELECT,
	EXIT,
	
	
	COLORS,
	CHARACTERS,
	RULES
};