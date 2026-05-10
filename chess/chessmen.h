#pragma once

typedef struct Chess {
	int type;
	int color;
	int ismoved;
}Chess;

class chessmen {
	public:
		void move(Chess(&chessboard)[8][8], int currentCol, int currentRow, int clickCol, int clickRow);
};