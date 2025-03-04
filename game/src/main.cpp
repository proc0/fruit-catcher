#include "arena.hpp"
#include "game.hpp"

int main() {

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TEXT_GAME_TITLE);
  SetTargetFPS(60);
  
  Fruits _fruits;
  Basket _basket;
  Display _display;
  Stage _stage;
  Game Game(_fruits, _basket, _display, _stage);

  while (!WindowShouldClose()) {
    Game.Update();
    BeginDrawing();
    Game.Render();
    EndDrawing();
  }

  CloseWindow();
  
  return 0;
}
