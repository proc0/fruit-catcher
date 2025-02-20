#include <stdlib.h>
#include <time.h>

#include "raylib.h"
#include "raymath.h"

#include "lib.h"
#include "arena.hpp"

#include "game.hpp"
#include "fruits.hpp"
#include "basket.hpp"

int main() {

  Fruits _Fruit;
  Basket _Basket;
  FruitCatcher Game(_Fruit, _Basket);

  InitWindow(screenWidth, screenHeight, "Fruits Catcher");
  SetTargetFPS(60);

  Game.Begin();

  while (!WindowShouldClose()) {
    Game.Update();

    BeginDrawing();
    ClearBackground(RAYWHITE);

    Game.Render();
    
    EndDrawing();
  }
  
  CloseWindow();
  
  return 0;
}
