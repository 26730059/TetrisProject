#include "GameManager.h"

int main() {
    GameManager game;
    game.Init();
    game.Run();
    game.Cleanup();
    return 0;
}
