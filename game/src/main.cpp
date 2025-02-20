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
  Game FruitCatcher(_Fruit, _Basket);

  InitWindow(screenWidth, screenHeight, "Fruits Catcher");
  SetTargetFPS(60);

  FruitCatcher.Begin();

  while (!WindowShouldClose()) {
    FruitCatcher.Update();

    BeginDrawing();
    ClearBackground(RAYWHITE);

    FruitCatcher.Render();
    
    EndDrawing();
  }
  
  CloseWindow();
  
  return 0;
}
