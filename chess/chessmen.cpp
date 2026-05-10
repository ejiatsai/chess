#include <iostream>
#include "chessmen.h"

void chessmen::move(Chess(&chessboard)[8][8], int currentCol, int currentRow, int clickCol, int clickRow) {
	Chess Chessmen = chessboard[currentRow][currentCol];
	chessboard[currentRow][currentCol] = { -1,0,0 };
	chessboard[clickRow][clickCol] = Chessmen;
}