#include "core/Game.h"
#include "states/MainMenuState.h"
#include <iostream>

int main() {
    Game game;

    if (!game.init()) {
        std::cerr << "Failed to initialize game!" << std::endl;
        return 1;
    }

    // 启动主菜单
    game.pushState(std::make_unique<MainMenuState>());

    game.run();

    return 0;
}
