#pragma once
using namespace std;
class Tile {
private:
	TileType type;
	Direction enterDir;
	Rooms room;

	Tile(TileType t, Direction d, Rooms r) {
		type = t;
		enterDir = d;
		room = r;
	}
public:
	Tile() {
		type = Floor;
		enterDir = None;
		room = Null;
	}

	Tile createWall() {
		Tile temp(Wall, None, Null);
		return temp;
	}

	Tile createEntrance(Direction d, Rooms r) {
		Tile temp(Entrance, d, r);
		return temp;
	}

	bool IsWall() {
		return (type == Wall);
	}

	bool IsEntrance() {
		return (type == Entrance);
	}

	Direction getDir() {
		return enterDir;
	}

	Rooms getRoom() {
		return room;
	}
};

COORD buildXY(int x, int y) {
	COORD temp;
	temp.X = x;
	temp.Y = y;

	return temp;
}

class Board :public Screen{
private:
	const int BOARD_WIDTH = 54;
	const int BOARD_HEIGHT = 26;
	const int MARGIN = 1;
	bool drawBoard = true;

	int x, y;
	int numPlayers;
	int curPlayerId;
	Player players[6] = { {Ms_Scarlet, 16, 0}, {Col_Mustard, 24, 7}, {Ms_White, 14, 24}, {Mr_Green, 9, 24}, {Mrs_Peacock, 0, 18}, {Prof_Plum, 0, 5} };
	string boardLayout = "";
	Tile boardTiles[25][25];
	COORD roomPlacements[6][11] = 
	{
		{buildXY(0,0),buildXY(11, 10), buildXY(10,1), buildXY(18,1), buildXY(1,0), buildXY(0,7), buildXY(0,13), buildXY(1,20), buildXY(10,19), buildXY(19,19), buildXY(17,10)},
		{buildXY(0,0),buildXY(11, 10), buildXY(13,1), buildXY(21,1), buildXY(4,0), buildXY(2,7), buildXY(2,13), buildXY(3,20), buildXY(13,19), buildXY(21,19), buildXY(20,10)},
		{buildXY(0,0),buildXY(11, 10), buildXY(10,3), buildXY(24,1), buildXY(0,1), buildXY(4,7), buildXY(4,13), buildXY(0,22), buildXY(9,20), buildXY(23,19), buildXY(23,10)},
		{buildXY(0,0),buildXY(11, 10), buildXY(13,3), buildXY(18,4), buildXY(5,0), buildXY(0,9), buildXY(0,15), buildXY(4,22), buildXY(14,20), buildXY(19,23), buildXY(17,13)},
		{buildXY(0,0),buildXY(11, 10), buildXY(10,5), buildXY(21,4), buildXY(1,2), buildXY(2,9), buildXY(2,15), buildXY(1,23), buildXY(10,21), buildXY(21,23), buildXY(20,13)},
		{buildXY(0,0),buildXY(11, 10), buildXY(13,5), buildXY(24,4), buildXY(4,2), buildXY(4,9), buildXY(4,15), buildXY(3,23), buildXY(13,21), buildXY(23,23), buildXY(23,13)}
	};
	COORD entrances[11][4];
	int numExits[11] = {};

	int deadPlayers[6] = {};
	int numDead = 0;

	static const int NUM_BUTTONS = 7;
	Button buttons[NUM_BUTTONS] = {
		Button("  View Notes  ", 56, 5, NOTES), 
		Button("Secret Passage", 56, 6, S_PASSAGE), 
		Button("   Controls   ", 56, 4, DISP_CONTROL), 
		Button("   Roll Die   ", 56, 3, ROLL),
		Button("   Go Back    ", 56, 7, RESET_MOVE),
		Button("  Prediction  ", 56, 8, PREDICTION),
		Button("   End Turn   ", 56, 9, END_TURN)
	};
	Button* rollBtn = &buttons[3];
	Button* sPassageBtn = &buttons[1];
	Button* predictBtn = &buttons[5];
	Button* ResetMvBtn = &buttons[4];
	Button* EndTurnBtn = &buttons[6];

	static const int UILines = 11;
	string UILayout[UILines] = {
		{(char) 201 , 'M', 'o', 'v', 'e', (char)205, (char)187 },
		{(char) 186, ' ', ' ', ' ', ' ', ' ', (char) 186},
		{(char) 204, 'A', 'c', 't', 'i', 'o', 'n', 's', (char) 205, (char)205 , (char)205, (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205, (char)187 },
		{(char)186},
		{(char)186},
		{(char)186},
		{(char)186},
		{(char)186},
		{(char)186},
		{(char)186},
		//{(char)186, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', (char)186},
		{(char)200, (char)205, (char)205 , (char)205, (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205, (char)205, (char)205 , (char)205, (char)205, (char) 188}

	};
public:
	Board() {
		x = 0;
		y = 0;
		numPlayers = 6;
		curPlayerId = 0;
		SetNumButtons(NUM_BUTTONS);

		sPassageBtn->Disable();
		predictBtn->Disable();

		//The Following Code Builds the board.
		//Special CHaracters
		const char solid = (char)219;
		const char wallD = (char)220;
		const char wallR = (char)222;
		const char wallU = (char)223;
		const char wallL = (char)221;

		string line1 = "";
		string line2 = "";

		for (int i = 0; i < BOARD_WIDTH; i++) {
			line1.insert(line1.end(), solid);
			if (i < 2 || i >= BOARD_WIDTH - 2)
				line2.insert(line2.end(), solid);
			else
				line2.append(" ");
		}

		line1.append("\n");
		line2.append("\n");

		boardLayout.append(line1);
		string tmp = line2;
		//line 2
		tmp.replace(2, 1, "?");
		tmp.replace(13, 3, { wallR, solid, solid });
		tmp.replace(18, 5, { solid, solid, solid, solid, wallL });
		tmp.replace(29, 5, { wallR, solid, solid, solid, solid });
		tmp.replace(36, 3, { solid, solid, wallL });
		tmp.replace(51, 1, "?");
		boardLayout.append(tmp);

		//line 3
		tmp = line2;
		tmp.replace(5, 5, "STUDY");
		tmp.replace(15, 1, { wallR });
		tmp.replace(20, 1, { wallL });
		tmp.replace(31, 1, { wallR });
		tmp.replace(36, 1, { wallL });
		boardLayout.append(tmp);

		//line 4
		tmp.replace(5, 5, "     ");
		tmp.replace(24, 4, "HALL");
		tmp.replace(41, 6, "LOUNGE");
		boardLayout.append(tmp);

		//line 5
		tmp = line2;
		tmp.replace(2, 14, { wallD,wallD,wallD,wallD,wallD,wallD,wallD,wallD,wallD,wallD,wallD,wallD,'/',wallR });
		tmp.replace(20, 1, { wallL });
		tmp.replace(31, 1, { wallR });
		tmp.replace(36, 1, { wallL });
		boardLayout.append(tmp);

		//line 6
		tmp = line2;
		tmp.replace(2, 2, { solid, solid });
		tmp.replace(20, 1, { '/' });
		tmp.replace(31, 1, { wallR });
		tmp.replace(36, 1, { wallL });
		boardLayout.append(tmp);

		//line 7
		tmp = line2;
		tmp.replace(20, 1, { wallL });
		tmp.replace(31, 1, { wallR });
		tmp.replace(36, 16, { wallL, '\\', wallD, wallD, wallD, wallD, wallD, wallD, wallD, wallD, wallD, wallD, wallD, wallD, wallD, wallD });
		boardLayout.append(tmp);

		//line 8
		tmp = line2;
		tmp.replace(2, 12, { solid, solid, wallU, wallU, wallU, wallU, wallU, wallU, wallU, wallU, wallU, solid });
		tmp.replace(20, 12, { solid, solid, wallD, wallD, '/', ' ', ' ', '\\', wallD, wallD, solid, solid });
		tmp.replace(50, 2, { solid, solid });
		boardLayout.append(tmp);

		//line 9
		tmp = line2;
		tmp.replace(13, 3, { wallU, wallU, solid });
		boardLayout.append(tmp);

		//line 10
		tmp = line2;
		tmp.replace(4, 7, "LIBRARY");
		tmp.replace(15, 1, { '/' });
		tmp.replace(20, 10, { solid, solid, '_', '_', '_', '_', '_', '_', solid, solid });
		tmp.replace(50, 2, { solid, solid });
		boardLayout.append(tmp);

		//line 11
		tmp = line2;
		tmp.replace(13, 3, { wallD, wallD, solid });
		tmp.replace(20, 10, { wallL, 'B', 'A', 'S', 'E', 'M', 'E', 'N', 'T', wallR });
		tmp.replace(34, 18, { solid, wallU, '\\', ' ', wallU, wallU, wallU, wallU, wallU, wallU, wallU, wallU, wallU, wallU, wallU, wallU, wallU, wallU });
		boardLayout.append(tmp);

		//line 12
		tmp = line2;
		tmp.replace(2, 12, { solid, solid, wallD, wallD, wallD, wallD, '/', ' ', wallD, wallD, wallD, solid });
		tmp.replace(20, 10, { solid, solid, solid, wallD, '_', '_', wallD, solid, solid, solid });
		tmp.replace(34, 1, { wallL });
		boardLayout.append(tmp);

		//line 13
		tmp = line2;
		tmp.replace(2, 2, { solid, solid });
		tmp.replace(20, 10, { wallL, (char)201, (char)190, solid, solid, solid, solid, (char)201, (char)190, wallR });
		tmp.replace(34, 1, { wallL });
		tmp.replace(41, 6, "DINING");
		boardLayout.append(tmp);

		//line 14
		tmp = line2;
		tmp.replace(2, 12, { solid, solid, ' ', '/', wallU, wallU, wallU, wallU, wallU, wallU, wallU, solid });
		tmp.replace(20, 10, { wallL, (char)186, ' ', (char)210, ' ', (char)210, (char)210, (char)204, (char)181, wallR });
		tmp.replace(34, 1, { '\\' });
		tmp.replace(42, 4, "ROOM");
		boardLayout.append(tmp);

		//line 15
		tmp = line2;
		tmp.replace(13, 1, { wallR });
		tmp.replace(20, 10, { wallL, (char)200, (char)184, (char)200, (char)184, (char)200, (char)188, (char)200, (char)184, wallR });
		tmp.replace(34, 1, { wallL });
		boardLayout.append(tmp);

		//line 16
		tmp = line2;
		tmp.replace(4, 8, "BILLIARD");
		tmp.replace(13, 1, { wallR });
		tmp.replace(20, 10, { solid, solid, solid, solid, solid, solid, solid, solid, solid, solid });
		tmp.replace(34, 7, { solid, wallD, wallD, wallD, wallD, wallD, wallD });
		boardLayout.append(tmp);

		//line 17
		tmp = line2;
		tmp.replace(13, 1, { '/' });
		tmp.replace(40, 12, { solid, wallD, wallD, wallD, wallD, wallD, wallD, wallD, wallD, wallD, wallD, wallD });
		boardLayout.append(tmp);

		//line 18
		tmp = line2;
		tmp.replace(2, 12, { wallD, wallD, wallD, wallD, wallD, wallD, wallD, wallD, wallD, wallD, wallD, solid });
		tmp.replace(50, 2, { solid, solid });
		boardLayout.append(tmp);

		//line 19
		tmp = line2;
		tmp.replace(2, 2, { solid, solid });
		tmp.replace(18, 16, { solid, wallU, '\\', ' ', wallU, wallU, wallU, wallU, wallU, wallU, wallU, wallU, ' ', '/', wallU, solid });
		boardLayout.append(tmp);

		//line 20
		tmp = line2;
		tmp.replace(18, 1, { wallL });
		tmp.replace(33, 1, { wallR });
		tmp.replace(38, 14, { solid, wallU, '\\', ' ', wallU, wallU, wallU, wallU, wallU, wallU, wallU, wallU, solid, solid });
		boardLayout.append(tmp);

		//line 21
		tmp = line2;
		tmp.replace(2, 10, { wallU, wallU, wallU, wallU, wallU, wallU, wallU, wallU, wallU, solid });
		tmp.replace(18, 1, { '/' });
		tmp.replace(33, 1, { '\\' });
		tmp.replace(38, 1, { wallL });
		tmp.replace(50, 2, { wallU, wallU });
		boardLayout.append(tmp);

		//line 22
		tmp = line2;
		tmp.replace(1, 1, { wallU });
		tmp.replace(11, 3, { wallU, '\\', wallR });
		tmp.replace(18, 1, { wallL });
		tmp.replace(22, 8, "BALLROOM");
		tmp.replace(33, 1, { wallR });
		tmp.replace(38, 1, { wallL });
		boardLayout.append(tmp);

		//line 23
		tmp = line2;
		tmp.replace(1, 12, "CONSERVATORY");
		tmp.replace(13, 1, { wallR });
		tmp.replace(18, 1, { wallL });
		tmp.replace(33, 1, { wallR });
		tmp.replace(38, 1, { wallL });
		tmp.replace(41, 7, "KITCHEN");
		boardLayout.append(tmp);

		//line 24
		tmp = line2;
		tmp.replace(1, 1, " ");
		tmp.replace(13, 1, { wallR });
		tmp.replace(18, 5, { solid, wallD, wallD, wallD, wallD });
		tmp.replace(29, 5, { wallD, wallD, wallD, wallD, solid });
		tmp.replace(38, 1, { wallL });
		boardLayout.append(tmp);

		//line 25
		tmp = line2;
		tmp.replace(2, 1, "?");
		tmp.replace(13, 3, { wallR, solid, solid });
		tmp.replace(22, 1, { solid });
		tmp.replace(29, 1, { solid });
		tmp.replace(36, 3, { solid, solid, wallL });
		tmp.replace(51, 1, "?");
		boardLayout.append(tmp);

		//line 26
		tmp = line1;
		tmp.replace(20, 2, "  ");
		tmp.replace(30, 2, "  ");
		boardLayout.append(tmp);

		//boardLayout.append(line2);
		boardLayout.append(line1);

		Tile Builder;

		//Setting board tiles
		//STUDY
		for (int c = 0; c < 4; c++) {
			boardTiles[6][c] = Builder.createWall();
			if (c == 3) {
				for (int d = 0; d < 6; d++)
					boardTiles[d][c] = Builder.createWall();
			}
		}
		boardTiles[6][4] = Builder.createEntrance(Up, Study);

		//HALL
		for (int c = 0; c < 7; c++) {
			boardTiles[9][c] = Builder.createWall();
			boardTiles[14][c] = Builder.createWall();
			if (c == 6)
				for (int d = 1; d <= 4; d++)
					boardTiles[9 + d][c] = Builder.createWall();
		}
		boardTiles[8][4] = Builder.createEntrance(Right, Hall);
		//boardTiles[15][4] = Builder.createEntrance(Left, Hall);
		boardTiles[11][7] = Builder.createEntrance(Up, Hall);
		boardTiles[12][7] = Builder.createEntrance(Up, Hall);

		//LOUNGE
		for (int c = 0; c < 6; c++) {
			boardTiles[17][c] = Builder.createWall();
			if (c == 5) {
				for (int d = 0; d < 8; d++)
					boardTiles[17 + d][c] = Builder.createWall();
			}
		}
		boardTiles[17][6] = Builder.createEntrance(Up, Lounge);

		//LIBRARY
		for (int c = 0; c < 5; c++) {
			if (c == 4 || c == 0) {
				for (int d = 0; d < 6; d++)
					boardTiles[d][6 + c] = Builder.createWall();
			}
			else {
				boardTiles[6][6 + c] = Builder.createWall();
			}
		}
		boardTiles[7][8] = Builder.createEntrance(Left, Library);
		boardTiles[3][11] = Builder.createEntrance(Up, Library);

		//BILLIARD
		for (int c = 0; c < 5; c++) {
			if (c == 4 || c == 0) {
				for (int d = 0; d < 6; d++)
					boardTiles[d][12 + c] = Builder.createWall();
			}
			else {
				boardTiles[5][12 + c] = Builder.createWall();
			}
		}
		boardTiles[1][11] = Builder.createEntrance(Down, BilliardRoom);
		boardTiles[6][15] = Builder.createEntrance(Left, BilliardRoom);

		//CONSERVATORY
		for (int c = 0; c < 5; c++) {
			if (c == 0) {
				for (int d = 0; d < 5; d++)
					boardTiles[d][19 + c] = Builder.createWall();
			}
			else {
				boardTiles[5][19 + c] = Builder.createWall();
			}
		}
		boardTiles[5][19] = Builder.createEntrance(Down, Conservatory);

		//BASEMENT
		for (int c = 0; c < 7; c++) {
			if (c == 1 || c == 6) {
				for (int d = 0; d < 5; d++)
					boardTiles[9 + d][8 + c] = Builder.createWall();
			}
			else {
				boardTiles[9][8 + c] = Builder.createWall();
				boardTiles[13][8 + c] = Builder.createWall();
			}
		}
		boardTiles[10][8] = Builder.createEntrance(Down, Basement);
		boardTiles[11][8] = Builder.createEntrance(Down, Basement);
		boardTiles[12][8] = Builder.createEntrance(Down, Basement);

		//BALLROOM
		for (int c = 0; c < 7; c++) {
			if (c == 0 || c == 5) {
				for (int d = 0; d < 8; d++)
					boardTiles[8 + d][17 + c] = Builder.createWall();
			}
			else if (c == 6) {
				boardTiles[10][17 + c] = Builder.createWall();
				boardTiles[13][17 + c] = Builder.createWall();
			}
			else {
				boardTiles[8][17 + c] = Builder.createWall();
				boardTiles[15][17 + c] = Builder.createWall();
			}
		}
		boardTiles[7][19] = Builder.createEntrance(Right, BallRoom);
		boardTiles[9][16] = Builder.createEntrance(Down, BallRoom);
		boardTiles[14][16] = Builder.createEntrance(Down, BallRoom);
		boardTiles[16][19] = Builder.createEntrance(Left, BallRoom);
		

		//DINING ROOM
		for (int c = 0; c < 7; c++) {
			if (c == 0) {
				for (int d = 0; d < 9; d++)
					boardTiles[16 + d][9 + c] = Builder.createWall();
			}
			else if (c == 5) {
				for (int d = 0; d < 3; d++)
					boardTiles[16 + d][9 + c] = Builder.createWall();
			}
			else if (c == 6) {
				for (int d = 3; d < 8; d++)
					boardTiles[16 + d][9 + c] = Builder.createWall();
			}
			else {
				boardTiles[16][9 + c] = Builder.createWall();
			}
		}
		boardTiles[17][8] = Builder.createEntrance(Down, DinningRoom);
		boardTiles[15][12] = Builder.createEntrance(Right, DinningRoom);

		//KITCHEN
		for (int c = 0; c < 6; c++) {
			boardTiles[18][18 + c] = Builder.createWall();
			if (c == 0) {
				for (int d = 0; d < 7; d++)
					boardTiles[18 + d][18 + c] = Builder.createWall();
			}
		}
		boardTiles[19][17] = Builder.createEntrance(Down, Kitchen);

		//EXTRA WALLS
		for (int c = 0; c < 24; c++) {
			if (c != 9 && c != 14)
				boardTiles[c][24] = Builder.createWall();
		}

		boardTiles[0][4] = Builder.createWall();
		boardTiles[0][11] = Builder.createWall();
		boardTiles[0][17] = Builder.createWall();
		boardTiles[6][23] = Builder.createWall();
		boardTiles[17][23] = Builder.createWall();
		boardTiles[24][16] = Builder.createWall();
		boardTiles[24][8] = Builder.createWall();
		boardTiles[24][6] = Builder.createWall();
		boardTiles[8][0] = Builder.createWall();
		boardTiles[15][0] = Builder.createWall();


		// finding and storing entrances
		
		for (int i = 0; i < 25; i++) {
			for (int j = 0; j < 25; j++) {
				if (boardTiles[i][j].IsEntrance()) {
					Rooms r = boardTiles[i][j].getRoom();
					entrances[r][numExits[r]] = buildXY(i, j);
					numExits[r]++;
				}
			}
		}
	}

	Player* GetCurrentPlayer() {
		return &players[curPlayerId];
	}

	void Draw(HANDLE out) {
		if (drawBoard) {
			GoToXY(x, y);
			SetConsoleTextAttribute(out, Palette.Board);
			cout << boardLayout;
			drawBoard = false;

			DrawLog(out);
		}

		for (int c = 0; c < numPlayers; c++) {
			if (c != curPlayerId)
				players[c].Draw(out, (x + (MARGIN * 2)), (y + MARGIN));
		}
		players[curPlayerId].Draw(out, (x + (MARGIN * 2)), (y + MARGIN));

		SetConsoleTextAttribute(out, Palette.Default);
		for (int u = 0; u < UILines; u++) {
			GoToXY(55, u);
			cout << UILayout[u];
			if (u <= 9 && u >= 3) {
				GoToXY(72, u);
				cout << UILayout[u];
			}
			else if (u == 1) {

			}
		}

		if (movePoints > 0) {
			GoToXY(58, 1);
			cout << movePoints;
		}

		for (int b = 0; b < NUM_BUTTONS; b++) {
			buttons[b].draw(out);
		}


	}

	Player* NextPlayer() {
		curPlayerId++;
		if (curPlayerId >= numPlayers) {
			curPlayerId = 0;
		}

		while (true) {
			bool isDead = false;
			for (int d = 0; d < numDead; d++) {
				if (curPlayerId == deadPlayers[d])
					isDead = true;
			}
			if (isDead) {
				curPlayerId++;
				if (curPlayerId >= numPlayers) {
					curPlayerId = 0;
				}
			}
			else
				break;
		}

		if (players[curPlayerId].IsInRoom()) {
			Rooms r = players[curPlayerId].GetRoom();
			if (r == Study || r == Lounge || r == Conservatory || r == Kitchen) {
				sPassageBtn->Enable();
			}
			else {
				sPassageBtn->Disable();
			}
		}
		else {
			sPassageBtn->Disable();
		}

		return &players[curPlayerId];
	}

	bool IsWall(int x, int y) {
		return boardTiles[x][y].IsWall();
	}

	bool IsEntrance(int x, int y) {
		return boardTiles[x][y].IsEntrance();
	}

	Direction GetDir(int x, int y) {
		return boardTiles[x][y].getDir();
	}

	Rooms GetRoom(int x, int y) {
		return boardTiles[x][y].getRoom();
	}

	int NumPlayers() {
		return numPlayers;
	}

	int GetCurPlayerId() {
		return curPlayerId;
	}

	Player* getPlayers() {
		return players;
	}

	COORD GetRoomCoord(Character c, Rooms r) {
		return roomPlacements[c][r];
	}

	void GetExits(Rooms r, int & size, COORD e[]) {
		int numCollisions = 0;
		for (int c = 0; c < numExits[r]; c++) {
			bool collision = false;
			for (int p = 0; p < numPlayers; p++) {
				if (entrances[r][c].X == players[p].GetX() && entrances[r][c].Y == players[p].GetY() && p != curPlayerId) {
					collision = true;
				}
			}
			if (collision) {
				numCollisions++;
			}else
				e[c - numCollisions] = entrances[r][c];
		}

		size = numExits[r] - numCollisions;
	}

	Button* GetButtons() {
		return buttons;
	}

	COORD GetStartLoc(Character c) {
		COORD temp;
		switch (c) {
		case Ms_Scarlet:
			temp.X = 16;
			temp.Y = 0;
			break;
		case Col_Mustard:
			temp.X = 24;
			temp.Y = 7;
			break;
		case Ms_White:
			temp.X = 14;
			temp.Y = 24;
			break;
		case Mr_Green:
			temp.X = 9;
			temp.Y = 24;
			break;
		case Mrs_Peacock:
			temp.X = 0;
			temp.Y = 18;
			break;
		case Prof_Plum:
			temp.X = 0;
			temp.Y = 5;
			break;
		}

		return temp;
	}

	void SetBoard(Character p[], int numP) {
		numPlayers = numP;
		numDead = 0;

		for (int i = 0; i < numP; i++) {
			for (int j = i; j < numP; j++) {
				if (i != j) {
					if (p[j] < p[i]) {
						Character temp = p[i];
						p[i] = p[j];
						p[j] = temp;
					}
				}
			}
		}

		for (int c = 0; c < numP; c++) {
			COORD start = GetStartLoc(p[c]);
			Player temp = Player(p[c], start.X, start.Y);
			players[c] = temp;
		}
		
	}

	void DisableRoll() {
		rollBtn->Disable();
	}

	void EnableRoll() {
		rollBtn->Enable();
	}

	void DisableSecretPassage() {
		sPassageBtn->Disable();
	}

	void EnablePrediction() {
		predictBtn->Enable();
	}

	void DisablePrediction() {
		predictBtn->Disable();
	}

	void DisableResetMv() {
		ResetMvBtn->Disable();
	}

	void EnableResetMv() {
		ResetMvBtn->Enable();
	}

	void EnableSecretPass() {
		sPassageBtn->Enable();
	}

	void KillPlayer() {
		deadPlayers[numDead] = curPlayerId;
		numDead++;
	}

	bool CheckGameOver() {
		if (numDead == numPlayers) {
			return true;
		}
		else {
			return false;
		}
	}

	void primeUpdate() {
		drawBoard = true;
	}

	void DisableEndTurn() {
		EndTurnBtn->Disable();
	}

	void EnableEndTurn() {
		EndTurnBtn->Enable();
	}

	bool CheckEndTurnAvailable() {
		if (EndTurnBtn->IsDisabled()) {
			return false;
		}
		else {
			return true;
		}
	}

	bool CheckRollAvailable() {
		if (rollBtn->IsDisabled()) {
			return false;
		}
		else {
			return true;
		}
	}

	bool CheckSecretPassageAvailable() {
		if (sPassageBtn->IsDisabled()) {
			return false;
		}
		else {
			return true;
		}
	}

	bool CheckPredictAvailable() {
		if (predictBtn->IsDisabled()) {
			return false;
		}
		else {
			return true;
		}
	}

	bool CheckResetMvAvailable() {
		if (ResetMvBtn->IsDisabled()) {
			return false;
		}
		else {
			return true;
		}
	}
};