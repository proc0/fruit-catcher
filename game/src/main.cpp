// #ifdef __EMSCRIPTEN__
// #include <emscripten.h>
// #endif

#include "game.hpp"

#define GAME_CONFIG_URI "game/jamslam.ini"

// Game JamSlam(GAME_CONFIG_URI);

// void main_loop();

// void main_loop(){
//   JamSlam.Loop();
// };


int main() {

  SetConfigFlags(FLAG_VSYNC_HINT);
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TEXT_GAME_TITLE);
  InitAudioDevice();
  SetExitKey(KEY_NULL);
  
  Game JamSlam(GAME_CONFIG_URI);

  JamSlam.Run([&](){
    JamSlam.Loop();
  });
  // #ifdef __EMSCRIPTEN__
  //   emscripten_set_main_loop(main_loop, 0, 1);
  // #else
  //   SetTargetFPS(60);
  //   while (!WindowShouldClose() && JamSlam.isRunning()) {
  //       JamSlam.Loop();
  //   }
  // #endif

  // if(JamSlam.isDebug()){
  //   while (!WindowShouldClose() && JamSlam.isRunning()) {
  //     JamSlam.UpdateDebug();
  //     BeginDrawing();
  //     JamSlam.RenderDebug();
  //     EndDrawing();
  //   }
  // } else {
  //   while (!WindowShouldClose() && JamSlam.isRunning()) {
  //     JamSlam.Update();
  //     PollInputEvents();
  //     BeginDrawing();
  //     JamSlam.Render();
  //     EndDrawing();
  //   }
  // }

  CloseAudioDevice();
  CloseWindow();
  
  return 0;
}
