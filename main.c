#define BOARD_WIDTH 10;
#define BOARD_HEIGHT 10;
#define PIECE_SIZE 4;

typedef bool[PIECE_SIZE][PIECE_SIZE] PieceData;

typedef struct ActivePiece_ {
	PieceData *pieceData;
	bool x;
	bool y;
} ActivePiece;

typedef bool[BOARD_WIDTH][BOARD_HEIGHT] Board;

// enum PieceShape {O, I, S, Z, L, J, T};

typedef struct GameState_ {
	Board board;
	ActivePiece piece;
	bool isActivePiece;
	double countdown;
} GameState;


PieceData[7][4] catalog;

void catalog_init (void);

PieceData* catalog_getPiece (int shape, int orientation);

// turnPiece ()

