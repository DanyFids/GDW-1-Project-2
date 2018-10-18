#pragma once
using namespace std;
class Tile {
private:
	TileType type;
	Direction enterDir;
public:
	Tile() {
		type = Floor;
		enterDir = None;
	}

	Tile(TileType t) {
		type = t;

		if (t == Entrance) {
			enterDir = Up;
		}
		else {
			enterDir = None;
		}
	}

	Tile(Direction d) {
		type = Entrance;
		enterDir = d;
	}

	bool IsWall() {
		return (type == Wall);
	}

	bool IsEntrance() {
		return (type == Entrance);
	}
};

class Board {
private:
	const int BOARD_WIDTH = 54;
	const int BOARD_HEIGHT = 26;
	const int MARGIN = 1;
	int x, y;
	int numPlayers;
	int curPlayerId;
	Player players[6] = { {Ms_Scarlet, 16, 0}, {Col_Mustard, 24, 7}, {Ms_White, 14, 24}, {Mr_Green, 9, 24}, {Mrs_Peacock, 0, 18}, {Prof_Plum, 0, 5} };
	string boardLayout = "";
	Tile boardTiles[25][25];
public:
	Board() {
		x = 0;
		y = 0;
		numPlayers = 6;
		curPlayerId = 0;

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
		tmp.replace(31, 1, { '\\' });
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

		//Setting board tiles
		//STUDY
		for (int c = 0; c < 4; c++) {
			boardTiles[6][c] = Tile(Wall);
			if (c == 3) {
				for (int d = 0; d < 6; d++)
					boardTiles[d][c] = Tile(Wall);
			}
		}

		//HALL
		for (int c = 0; c < 7; c++) {
			boardTiles[9][c] = Tile(Wall);
			boardTiles[14][c] = Tile(Wall);
			if (c == 6)
				for (int d = 1; d <= 4; d++)
					boardTiles[9 + d][c] = Tile(Wall);
		}

		//LOUNGE
		for (int c = 0; c < 6; c++) {
			boardTiles[17][c] = Tile(Wall);
			if (c == 5) {
				for (int d = 0; d < 8; d++)
					boardTiles[17 + d][c] = Tile(Wall);
			}
		}

		//LIBRARY
		for (int c = 0; c < 5; c++) {
			if (c == 4 || c == 0) {
				for (int d = 0; d < 6; d++)
					boardTiles[d][6 + c] = Tile(Wall);
			}
			else {
				boardTiles[6][6 + c] = Tile(Wall);
			}
		}

		//BILLIARD
		for (int c = 0; c < 5; c++) {
			if (c == 4 || c == 0) {
				for (int d = 0; d < 6; d++)
					boardTiles[d][12 + c] = Tile(Wall);
			}
			else {
				boardTiles[5][12 + c] = Tile(Wall);
			}
		}

		//CONSERVATORY
		for (int c = 0; c < 5; c++) {
			if (c == 0) {
				for (int d = 0; d < 5; d++)
					boardTiles[d][19 + c] = Tile(Wall);
			}
			else {
				boardTiles[5][19 + c] = Tile(Wall);
			}
		}

		//BASEMENT
		for (int c = 0; c < 7; c++) {
			if (c == 1 || c == 6) {
				for (int d = 0; d < 5; d++)
					boardTiles[9 + d][8 + c] = Tile(Wall);
			}
			else {
				boardTiles[9][8 + c] = Tile(Wall);
				boardTiles[13][8 + c] = Tile(Wall);
			}
		}

		//BALLROOM
		for (int c = 0; c < 7; c++) {
			if (c == 0 || c == 5) {
				for (int d = 0; d < 8; d++)
					boardTiles[8 + d][17 + c] = Tile(Wall);
			}
			else if (c == 6) {
				boardTiles[10][17 + c] = Tile(Wall);
				boardTiles[13][17 + c] = Tile(Wall);
			}
			else {
				boardTiles[8][17 + c] = Tile(Wall);
				boardTiles[15][17 + c] = Tile(Wall);
			}
		}

		//DINING ROOM
		for (int c = 0; c < 7; c++) {
			if (c == 0) {
				for (int d = 0; d < 9; d++)
					boardTiles[16 + d][9 + c] = Tile(Wall);
			}
			else if (c == 5) {
				for (int d = 0; d < 3; d++)
					boardTiles[16 + d][9 + c] = Tile(Wall);
			}
			else if (c == 6) {
				for (int d = 3; d < 8; d++)
					boardTiles[16 + d][9 + c] = Tile(Wall);
			}
			else {
				boardTiles[16][9 + c] = Tile(Wall);
			}
		}

		//KITCHEN
		for (int c = 0; c < 6; c++) {
			boardTiles[18][18 + c] = Tile(Wall);
			if (c == 0) {
				for (int d = 0; d < 7; d++)
					boardTiles[18 + d][18 + c] = Tile(Wall);
			}
		}

		//EXTRA WALLS
		for (int c = 0; c < 24; c++) {
			if (c != 9 && c != 14)
				boardTiles[c][24] = Tile(Wall);
		}

		boardTiles[0][4] = Tile(Wall);
		boardTiles[0][11] = Tile(Wall);
		boardTiles[0][17] = Tile(Wall);
		boardTiles[6][23] = Tile(Wall);
		boardTiles[17][23] = Tile(Wall);
		boardTiles[24][16] = Tile(Wall);
		boardTiles[24][8] = Tile(Wall);
		boardTiles[24][6] = Tile(Wall);
		boardTiles[8][0] = Tile(Wall);
		boardTiles[15][0] = Tile(Wall);

	}

	Player* GetCurrentPlayer() {
		return &players[curPlayerId];
	}

	void Draw(HANDLE out) {
		GoToXY(x, y);
		SetConsoleTextAttribute(out, Palette.Board);
		cout << boardLayout;

		for (int c = 0; c < numPlayers; c++) {
			players[c].Draw(out, (x + MARGIN), (y + MARGIN));
		}
	}

	Player* NextPlayer() {
		curPlayerId++;
		if (curPlayerId >= numPlayers) {
			curPlayerId = 0;
		}
		return &players[curPlayerId];
	}

	bool IsWall(int x, int y) {
		return boardTiles[x][y].IsWall();
	}
};