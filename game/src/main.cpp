#include "arena.hpp"
#include "game.hpp"

int main() {

  Fruits _Fruit;
  Basket _Basket;
  Display _Display;
  Game _Game(_Fruit, _Basket, _Display);

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TEXT_GAME_TITLE);
  SetTargetFPS(60);

  _Game.Begin();

  while (!WindowShouldClose()) {
    _Game.Update();

    BeginDrawing();
    ClearBackground(RAYWHITE);

    _Game.Render();
    
    EndDrawing();
  }
  
  CloseWindow();
  
  return 0;
}
