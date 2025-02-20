#include <stdlib.h>
#include <time.h>

#include "raylib.h"
#include "raymath.h"

#include "lib.h"
#include "arena.hpp"

#include "game.hpp"
#include "fruit.hpp"
#include "basket.hpp"

int main() {

  Fruit _Fruit;
  Basket _Basket;
  FruitCatcher _Game(_Fruit, _Basket);

  InitWindow(screenWidth, screenHeight, "Fruit Catcher");
  SetTargetFPS(60);

  _Game.InitGame();

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
