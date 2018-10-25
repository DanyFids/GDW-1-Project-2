#pragma once
#include<ctime>

using namespace std;
// Deck
template <class T, int Size>
class Deck {
private:
	T cards[Size];
	int topId = 0;
	int deckSize = Size;
public:
	Deck() {
	}

	Deck(T c[]) {
		for (int i = 0; i < deckSize; i++) {
			cards[i] = c[i];
		}

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
		topId = 0;

		T newDeck[Size];
		T oldDeck[Size];
		//init dummy deck;
		for (int i = 0; i < deckSize; i++) {
			oldDeck[i] = cards[i];
		}

		int oldDeckSize = deckSize;

		for (int c = 0; c < deckSize; c++) {
			uniform_int_distribution<int> range(0, oldDeckSize - 1);
			int d = range(gen);
			newDeck[c] = oldDeck[d];

			for (int o = d; o < oldDeckSize - 1; o++) {
				oldDeck[o] = oldDeck[o + 1];
			}

			oldDeckSize--;
		}

		for (int t = 0; t < deckSize; t++) {
			cards[t] = newDeck[t];
		}
	}
};