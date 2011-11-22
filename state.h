#define BOARD_WIDTH 10
#define BOARD_HEIGHT 10
#define PIECE_SIZE 4

#include <stdbool.h>

extern double delta;

/* Catalog of Tetris's pieces */

typedef bool PieceData[PIECE_SIZE][PIECE_SIZE];

PieceData catalog[7][4];
// enum PieceShape {O, I, S, Z, L, J, T};

void catalog_init (void);

PieceData *catalog_getPiece (int shape, int orientation);

/* Active Piece */

typedef struct ActivePiece_ {
    PieceData *data;
    int x;
    int y;
} ActivePiece;

/* Board */

typedef bool Board[BOARD_HEIGHT][BOARD_WIDTH];

void Board_init (Board board);

/* Game State */

typedef struct GameState_ {
    Board board;
    ActivePiece piece;
    bool isActivePiece;
    double countdown;
} GameState;

void GameState_init (GameState *game);

void GameState_resetCountdown (GameState *game);

void GameState_updateCountdown (GameState *game);

bool GameState_canFall (GameState *game);

void GameState_moveDownPiece (GameState *game);

void GameState_mergePiece (GameState *game);

bool GameState_canContinue (GameState *game);

void GameState_spawnPiece(GameState *game);
