#include "Func.h"

const CellType Game :: analize() {
	for (int i = 0; i < Field.size(); ++i) {
		if (Field[i][0] == Field[i][1] && Field[i][1] == Field[i][2]
			&& Field[i][2] != empty) {
			return Field[i][0];
		}

		if (Field[0][i] == Field[1][i] && Field[1][i] == Field[2][i]
			&& Field[2][i] != empty) {
			return Field[0][i];
		}
	}
	if (Field[0][0] == Field[1][1] && Field[1][1] == Field[2][2]
		&& Field[2][2] != empty) {
		return Field[0][0];
	}
	if (Field[2][0] == Field[1][1] && Field[1][1] == Field[0][2]
		&& Field[0][2] != empty) {
		return Field[2][0];
	}

	for (int i = 0; i < Field.size(); ++i) {
		for (int j = 0; j < Field.size(); ++j) {
			if (Field[j][i] == empty) {
				return empty;
			}
		}
	}

	return full;
}

std::ostream& operator<<(std::ostream& s, const Game& f) {
	for (int i = 0; i < f.Field.size(); ++i) {
		s << std::string(4 * f.Field.size() + 1, '-') << '\n';
		for (int j = 0; j < f.Field.size(); ++j) {
			s << "| ";
			switch (f.Field[i][j]) {
			case cross:
				s << "X ";
				break;
			case circle:
				s << "O ";
				break;
			case empty:
				s << "  ";
				break;
			}
		}
		s << "|\n";
	}
	s << std::string(4 * f.Field.size() + 1, '-');

	return s;
}

int main()
{
	Game f;
	std::cout << f;
	std::cout << "\n" << f.analize();
	return 0;
}