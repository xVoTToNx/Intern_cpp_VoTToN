#include "Func.h"

const CellType Game :: Analize() {
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
	system("CLS");
	s << "    0   1   2" << '\n';
	for (int i = 0; i < f.Field.size(); ++i) {
		s << "  " << std::string(4 * f.Field.size(), '-') << '\n';
		for (int j = 0; j < f.Field.size(); ++j) {
			if (j == 0)
				s << i;
			s <<"| ";
			switch (f.Field[i][j]) {
			case player:
				s << "X ";
				break;
			case computer:
				s << "O ";
				break;
			case empty:
				s << "  ";
				break;
			}
		}
		s << "|\n";
	}
	s << "  " << std::string(4 * f.Field.size(), '-');
	s << '\n';

	return s;
}

const void Game::operator()(Game g, Move m) {
	Game newGame(g, m);
	std::cout << newGame; // Print field;
	switch (newGame.Analize()) { // Returns game result;
	case full:
		std::cout << "\nGame Over! \nNo one has won.";
		return;
	case player:
		std::cout << "\nYou've won!";
		return;
	case computer:
		std::cout << "Humanity : SkyNet\n0        : 1";
		return;
	case empty:
		if (m.player == player) {
			Move move = newGame.LookingForTheBestMove(newGame, m);
			if (move.player == empty) {
				std::cout << "\nPlease wait...";
				MovePrepataion(newGame, m);
			}
			else {
				newGame(newGame, Move(move.row, move.column, computer));
			}
		}
		if (m.player == computer)
			newGame(newGame, GetPlayerMove());
		return;
	}
}

const void Game::MovePrepataion(Game g, Move m) { // Decides which move should the computer do;
	int maxRow = 0;
	int maxCol = 0;
	int maxVictory = INT_MIN;

	for (int i = 0; i < g.Field.size(); ++i) {
		for (int j = 0; j < g.Field.size(); ++j) {
			if (g.Field[i][j] == empty) {
				int result = MoveCalculation(g, Move(i, j, computer));
				if (result > maxVictory) {
					maxRow = i;
					maxCol = j;
					maxVictory = result;
				}
			}
		}
	}
	g(g, Move(maxRow, maxCol, computer));
	return;
}

const int Game::MoveCalculation(Game g, Move m) { // Calculates efficiency of each possible move;
	int result = 0;
	Game newGame(g, m);

	switch (newGame.Analize()) {
	case full:
		return result;
	case player:
		return result - 1;
	case computer:
		return result + 1;
	case empty:
		for (int i = 0; i < newGame.Field.size(); ++i) {
			for (int j = 0; j < newGame.Field.size(); ++j) {
				if (newGame.Field[i][j] == empty) {
					CellType type = (m.player == player) ? computer : player;
					result += MoveCalculation(newGame, Move(i, j, type));
				}
			}
		}
	}
	return result;
}

const Move Game::LookingForTheBestMove(Game g, Move m) {
	int row = 0;
	int column = 0;
	CellType type = empty;
	for (int i = 0; i < g.Field.size(); ++i) {
		for (int j = 0; j < g.Field.size(); ++j) {
			if (g.Field[i][j] == empty) {
				CellType tType = Game(g, Move(i, j, computer)).Analize();
				if (tType == computer) {
					row = i;
					column = j;
					type = computer;
				}
				tType = Game(g, Move(i, j, player)).Analize();
				if (tType == player && type != computer) {
					row = i;
					column = j;
					type = player;
				}
			}
		}
	}
	return Move(row, column, type);
}

const matrix<CellType> Game::ApplyMove(Move m) {
	matrix<CellType> newField = Field;
	newField[m.row][m.column] = m.player;
	return newField;
}

const Move GetPlayerMove() {
	std::string answer;
	std::cout << "Write your choice:\nFor example: \"0 0\"\n";
	while (true) {
		std::getline(std::cin, answer);
		std::cout << '\n';
		try {
			if (answer[0] - '0' > -1 && answer[0] - '0' < 3)
				if (answer[2] - '0' > -1 && answer[2] - '0' < 3)
					return Move(answer[0] - '0', answer[2] - '0', player);
			std::cout << "Wrong numbers!\nTry one more time!\n";
		}
		catch(const std::exception&){
			std::cout << "Wrong numbers!\nTry one more time!\n";
		}
	}
}

int main()
{
	while (true) {
		Game g;
		std::cout << g;

		g(g, GetPlayerMove());
		std::cout << "\nPress any button to continue playing.";
		std::cin.get();
	}
	return 0;
}