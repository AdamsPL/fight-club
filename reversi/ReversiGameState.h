#include "GameState.h"

class ReversiGameState : public GameState
{
public:
	ReversiGameState(int player, int boardSize);
	virtual ~ReversiGameState();

	virtual bool makeMove(QString move);
	virtual int getPoints(int player);
	virtual int getNextPlayer();

	int getStatus(int x, int y) const;

	void print();

	int getEmpty();
	inline int getBoardSize() const { return boardSize; }

	static const int BLACK_PLAYER = 0;
	static const int WHITE_PLAYER = 1;
	static const int EMPTY = 2;
	static const int INVALID = 3;
	static const QString PASS;

	int flip(int x, int y, bool dryRun = false);
	static QString convertToMove(int x, int y);

protected:
	void setStatus(int x, int y, int status);

private:
	inline int getIndex(int x, int y) const { return y * boardSize + x; }
	int flipInDir(int x, int y, char dx, char dy, int length, bool dryRun = false);
	bool hasPossibleMoves();

	const int boardSize;
	int *board;
};
