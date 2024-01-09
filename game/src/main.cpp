#include <stdio.h>

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

#include "core/game.hpp"
#include "scenes/scene_menu.hpp"

int main()
{
    Game* game = new Game();
    game->ChangeScene("test", std::make_shared<SceneMenu>(game));
    while (!game->ShouldClose()) {
        game->Tick();
    }
}