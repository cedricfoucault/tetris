#include "state.h"
#include <stdlib.h>

double delta = 0.1;

int main (int argc, char const *argv[]) {
    bool gameover = false;
    GameState game;
    
    catalog_init();
    GameState_init(&game);

    while (!gameover) {
        GameState_updateCountdown(&game);

        if (GameState_getCountdown(&game) <= 0.) {
            GameState_resetCountdown(&game);

            if (GameState_pieceIsActive(&game)) {
                if (GameState_pieceCanFall(&game)) {
                    GameState_moveDownPiece(&game);
                } else {
                    GameState_mergePiece(&game);
                }
            } else if (GameState_canSpawnPiece(&game)) {
                GameState_spawnPiece(&game);
            } else {
                gameover = true;
            }
        }
    }
    return 0;
}
