#include "game.hpp"

#define GAME_CONFIG_URI "game/FruitCatcher.ini"


//DONE: make bucket into a transparent jar, every fruit turns into jam at the bottom, layers build up and clear when it gets to the top.
//DONE: add progressive difficulty, starting with small fruit and more fruits are added faster -- LEVELS
//DONE: Fix background scroll depending on FPS

//TODO: add WIN screen (based on game over screen)
//TODO: add LOSE screen (based on game over screen)
//TODO: add eggs that add one life point
//TODO: add spikes that stun for 1 second
//TODO: add bucket effects when hit by spike
//TODO: Add score number popup when fruit is caught
//TODO: add satisfying sound when fruit is caught
//TODO: add comic style impact lines when fruit is caught
//TODO: add more levels, and test for balance
//TODO: iterate until it's fun
//TODO: update game name
//TODO: Look into packaging for different platforms
//TODO: Look into adding this to itch.io
//TODO: Do Emscriptem build
//TODO: Look into creating a debug build instead of including debug functions always, use define macros to read compiler variables

int main() {

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TEXT_GAME_TITLE);
  SetTargetFPS(120);
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
