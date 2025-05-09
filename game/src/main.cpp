#include "game.hpp"

#define GAME_CONFIG_URI "game/jamslam.ini"

int main() {

  SetConfigFlags(FLAG_VSYNC_HINT);
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TEXT_GAME_TITLE);
  SetTargetFPS(120);
  InitAudioDevice();
  SetExitKey(KEY_NULL);
  
  Game JamSlam(GAME_CONFIG_URI);

  if(JamSlam.isDebug()){
    JamSlam.UpdateDebug();
    while (!WindowShouldClose() && JamSlam.isRunning()) {
      BeginDrawing();
      JamSlam.RenderDebug();
      EndDrawing();
    }
  } else {
    while (!WindowShouldClose() && JamSlam.isRunning()) {
      JamSlam.Update();
      BeginDrawing();
      JamSlam.Render();
      EndDrawing();
    }
  }

  CloseAudioDevice();
  CloseWindow();
  
  return 0;
}
