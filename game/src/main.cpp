#include "arena.hpp"
#include "game.hpp"

int main() {

  Game Game;

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TEXT_GAME_TITLE);
  SetTargetFPS(60);

  Game.Load();

  while (!WindowShouldClose()) {
    Game.Update();

    BeginDrawing();

    Game.Render();
    
    EndDrawing();
  }

  Game.Unload();

  CloseWindow();
  
  return 0;
}
