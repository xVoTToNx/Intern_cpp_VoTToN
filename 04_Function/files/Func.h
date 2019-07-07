#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>

template<class T>
using matrix = std::vector<std::vector<T>>;

enum CellType {
	player,
	computer,
	empty,
	full
};

struct Move {
	Move(int r, int c, CellType p)
		: row(r)
		, column(c)
		, player(p)
	{}

	int row;
	int column;
	CellType player;
};

class Game {
public:
	const matrix<CellType> Field;

	Game(Game g, Move m) : Field{ g.ApplyMove(m) }
	{}

	Game() : Field{
		{empty, empty, empty},
		{empty, empty, empty},
		{empty, empty, empty}
	}
	{}

	const void operator()(Game, Move);
	const CellType Analize();
	const matrix<CellType> ApplyMove(Move);
	const Move LookingForTheBestMove(Game, Move);
private:
	const int MoveCalculation(Game, Move);
	const void MovePrepataion(Game, Move);
};

std::ostream& operator<<(std::ostream&, const Game&);
const Move GetPlayerMove();
