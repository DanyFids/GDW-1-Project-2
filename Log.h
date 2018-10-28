#pragma once
const int LOG_SIZE = 15;
string gLog[LOG_SIZE] = {};
int numLogs = 0;

void AddLog(string l) {
	if (numLogs != LOG_SIZE) {
		gLog[numLogs] = l;
		numLogs++;
	}
	else {
		for (int i = 0; i < LOG_SIZE; i++) {
			if (i == LOG_SIZE - 1) {
				gLog[i] = l;
			}
			else {
				gLog[i] = gLog[i + 1];
			}
		}
	}
}

void DrawLog(HANDLE out) {
	string top = { (char)201, 'L', 'o', 'g', (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)187 };
	string mid = { (char)186, ' ', ' ', ' ' , ' ', ' ' , ' ', ' ', ' ' , ' ', ' ' , ' ', ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ', ' ' , ' ', ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ' , ' ', ' ', ' ' , ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', (char)186 };
	string bot = { (char)200, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)205, (char)205 , (char)205, (char)188 };
	string buffer = { ' ', ' ', ' ', ' ', ' ', ' ' , ' ', ' ', ' ', ' ' , ' ', ' ' , ' ', ' ' , ' ', ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ',  ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ' , ' ', ' ', ' ' , ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ', ' ' , ' ', ' ' };

	for (int i = 0; i < 17; i++) {
		GoToXY(55, 11 + i);
		if (i == 0) {
			cout << top;
		}
		else if (i == 16) {
			cout << bot;
		}
		else {
			cout << mid;
		}
	}

	for (int l = 0; l < numLogs; l++) {
		GoToXY(56, 12 + l);
		cout << gLog[l];
	}
}