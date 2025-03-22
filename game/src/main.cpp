#include "game.hpp"

int main() {

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TEXT_GAME_TITLE);
  SetTargetFPS(60);
 
  Game Game;

  while (!WindowShouldClose() && Game.isRunning()) {
    Game.Update();
    BeginDrawing();
    Game.Render();
    EndDrawing();
  }

  CloseWindow();
  
  return 0;
}
