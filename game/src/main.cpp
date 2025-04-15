#include "game.hpp"

#define GAME_CONFIG_URI "game/FruitCatcher.ini"

//TODO: update game name
//TODO: make bucket into a transparent jar, every fruit turns into jam at the bottom, layers build up and clear when it gets to the top.
//TODO: add progressive difficulty, starting with small fruit and more fruits are added faster -- LEVELS
//TODO: add timer (and LEVELS)
//TODO: spice up the game over screen
//TODO: add animations when scoring and losing lives in HUD
//TODO: add satisfying sound when fruit is caught, add some other secondary effects (cartoon lines)
int main() {

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TEXT_GAME_TITLE);
  SetTargetFPS(60);
  SetExitKey(KEY_NULL);
  
  Game FruitJam(GAME_CONFIG_URI);

  if(FruitJam.isDebug()){
    FruitJam.UpdateDebug();
    while (!WindowShouldClose() && FruitJam.isRunning()) {
      BeginDrawing();
      FruitJam.RenderDebug();
      EndDrawing();
    }
  } else {
    while (!WindowShouldClose() && FruitJam.isRunning()) {
      FruitJam.Update();
      BeginDrawing();
      FruitJam.Render();
      EndDrawing();
    }
  }

  CloseWindow();
  
  return 0;
}
