#include "state.h"
#include <stdlib.h>

double delta = 1.;

int main (int argc, char const *argv[]) {
    bool gameover = false;
    GameState game;
    GameState_init(&game);
    while(!gameover) {
        GameState_updateCountdown(&game);
        if (game.countdown <= 0) {
            GameState_resetCountdown(&game);
            if (game.isActivePiece) {
                if (GameState_canFall(&game)) {
                    GameState_moveDownPiece(&game);
                } else {
                    GameState_mergePiece(&game);
                }
            } else {
                if (GameState_canContinue(&game)) {
                    GameState_spawnPiece(&game);
                } else {
                    gameover = true;
                }
            }
        }
    }
    return 0;
}
