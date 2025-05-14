#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include "game.hpp"

#define GAME_CONFIG_URI "game/jamslam.ini"

Game JamSlam(GAME_CONFIG_URI);

void main_loop() {
  JamSlam.Loop();
}

int main() {

  SetConfigFlags(FLAG_VSYNC_HINT);
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TEXT_GAME_TITLE);
  InitAudioDevice();
  SetExitKey(KEY_NULL);
  
  JamSlam.Load();

  if(JamSlam.isDebug()){
    while (!WindowShouldClose() && JamSlam.isRunning()) {
      JamSlam.UpdateDebug();
      BeginDrawing();
      JamSlam.RenderDebug();
      EndDrawing();
    }
  } else {
    while (!WindowShouldClose() && JamSlam.isRunning()) {
      #ifdef __EMSCRIPTEN__
        emscripten_set_main_loop(main_loop, 0, 1);
      #else
        SetTargetFPS(120);
        while (!WindowShouldClose() && JamSlam.isRunning()) {
            JamSlam.Loop();
        }
      #endif
    }
  }

  JamSlam.Unload();
  CloseAudioDevice();
  CloseWindow();
  
  return 0;
}
