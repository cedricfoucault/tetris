#define PIECE_STARTX 5
#define PIECE_STARTY 19
#define LEVEL 1

#include "state.h"
#include <stdlib.h>

/* Piece */
void catalog_init (void) {
    
    // O shape
    PieceData O[] = catalog[0];
    "    "
    " ## "
    " ## "
    "    "
    
    // I shape
    PieceData I[] = catalog[1];
    "    "      "  # "
    "####"      "  # "
    "    "      "  # "
    "    "      "  # "
    
    // S shape
    PieceData S[] = catalog[2];
    "    "      "  # "
    "  ##"      "  ##"
    " ## "      "   #"
    "    "      "    "
    
    // Z shape
    PieceData Z[] = catalog[3];
    "    "      "   #"
    " ## "      "  ##"
    "  ##"      "  # "
    "    "      "    "
    
    // L shape
    PieceData L[] = catalog[4];
    catalog_affect(L,
    "    ",      "  # ",      "   #",      " ## ",
    " ###",      "  # ",      " ###",      "  # ",
    " #  ",      "  ##",      "    ",      "  # ",
    "    ",      "    ",      "    ",      "  # ")
    
    // J shape
    PieceData J[] = catalog[5];
    "    "      "  ##"      " #  "      "  # "
    " ###"      "  # "      " ###"      "  # "
    "   #"      "  # "      "    "      " ## "
    "    "      "    "      "    "      "    "
    
    // T shape
    PieceData T[] = catalog[6];
    "    "      "  # "      "  # "      "  # "
    " ###"      "  ##"      " ###"      " ## "
    "  # "      "  # "      "    "      "  # "
    "    "      "    "      "    "      "    "
}

void PieceData_select (PieceData **piece) {
    int type;
    while (type = rand() / (RAND_MAX / 7) >= 7);
    *piece = catalog_getPiece (type, 0);
}

PieceData *catalog_getPiece (int shape, int orientation) {
    return &catalog[shape][orientation];
}

/* Board */

void Board_init (Board board) {
    int i, j;

    // empty the board
    for (i = 0; i < BOARD_HEIGHT; i++) {
        for (j = 0; j < BOARD_WIDTH; j++) {
            board[i][j] = false;
        }
    }
}

// // beware ! unefficient implementation of Board_clean
// void Board_clean (Board board) {
//     int i, j;
//     bool isFull;
// 
//     for (i = 0; i < BOARD_HEIGHT; i++) {
//         isFull = true;
//         // check each row to see if it is complete
//         for (j = 0; j < BOARD_WIDTH; j++) {
//             if (!board[i][j]) {
//                 isFull = false;
//                 break; 
//             }
//         }
//         // supress the row and rearrange the board accordingly
//         if (isFull) {
//             int k;
//             // if it is the last row that is full, just clean it up
//             if (i == BOARD_HEIGHT - 1) {
//                 for (j = 0; k < BOARD_WIDTH; j++) {
//                     board[i][j] = false;
//                 }
//             }
//             // otherwise, move down the rows above the one that is full
//             for (k = i; k < BOARD_HEIGHT - 1; k++) {
//                 for (j= 0; j < BOARD_WIDTH; j++) {
//                     board[k][j] = board[k+1][j];
//                 }   
//             }
//             // after the rearrangement, we have to check the board again
//             // from the beginning, since new full rows may appear
//             i = - 1; // possible optimization : max(-1, i - 4) ?
//         }
//     }
// }

void Board_clean (Board board) {
    int i, j, gap;
    bool isFull;
    
    gap = 0;
    for (i = 0; i < BOARD_HEIGHT - gap; i++) {
        // loop invariant : gap == the number of full rows between 0 and i
        isFull = true;
        for (j = 0; j < BOARD_WIDTH; j++) {
            // loop invariant : 
            // isFull == "every box between 0 and j in the ith row is full"
            if (!board[i][j]) {
                isFull = false;
            }
            if (gap > 0) {
                board[i - gap][j] = board[i][j];
            }
        }
        if (isFull) {
            gap++;
        }
    }
    
    // end of loop : i == BOARD_HEIGHT - gap
    // each full row leads a row at the top of the board to empty itself
    for (; i < BOARD_HEIGHT; i++) {
        for (j = 0; j < BOARD_WIDTH; j++) {
            board[i][j] = false;
        }
    }
}

void Board_commitPiece (Board board, ActivePiece piece) {
    int x = piece.x, y = piece.y;
    // i, j: indexes relative to the piece
    // iB, jB: indexes relative to the board
    int i, j, iB, jB;
    
    iB = y - 2; 
    jB = x - 2;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            board[iB][jB] = piece.data[i][j];
            jB++;
        }
        iB++;
    }
}

/* Game State */

void GameState_init (GameState *game) {
    Board_init(game->board);
    game->pieceIsActive = false;
    game->countdown = 0.;
}

bool GameState_pieceIsActive (GameState *game) {
    return game->pieceIsActive;
}

double GameState_getCountdown(GameState *game) {
    return game->countdown;
}

void GameState_resetCountdown (GameState *game) {
    game->countdown = 0.05 * (11. - LEVEL);
}

void GameState_updateCountdown (GameState *game) {
    game->countdown -= delta;
}

bool GameState_pieceCanFall (GameState *game) {
    return !(game->board[game->piece.y - 1][game->piece.x]);
}

void GameState_moveDownPiece (GameState *game) {
    game->piece.y--;
}

void GameState_mergePiece (GameState *game) {   
    // commit the piece to the board
    Board_commitPiece(game->board, game->piece);
    // clean the full rows and collapse pile accordingly
    Board_clean(game->board);  
    // remove the active piece
    game->pieceIsActive = false;
}

bool GameState_canSpawnPiece (GameState *game) {
    return !(game->board[PIECE_STARTY][PIECE_STARTX]);
}

void GameState_spawnPiece (GameState *game) {
    PieceData *pieceData;
    // select a piece to be spawn
    PieceData_select(&pieceData);   
    // put the piece in the game
    game->piece.data = pieceData;
    game->piece.x = PIECE_STARTX;
    game->piece.y = PIECE_STARTY;
    game->pieceIsActive = true;
}
