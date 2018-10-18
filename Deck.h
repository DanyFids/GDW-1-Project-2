#pragma once
using namespace std;
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