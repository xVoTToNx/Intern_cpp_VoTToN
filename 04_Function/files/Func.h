#include <iostream>
#include <string>
#include <vector>

enum CellType {
	cross,
	circle,
	empty,
	full
};

union Move {
	int row;
	int column;
	CellType player;
};

struct Game {
	const std::vector<std::vector<CellType>> Field = {
		{circle, circle, cross},
		{cross, cross, circle},
		{circle, empty, cross}
	};

	const void operator()() {

	}

	const CellType analize();
};

std::ostream& operator<<(std::ostream&, const Game&);
