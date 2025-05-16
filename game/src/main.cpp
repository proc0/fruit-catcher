#include "game.hpp"

#define GAME_CONFIG_URI "game/jamslam.ini"

Game JamSlam(GAME_CONFIG_URI);

void main_loop() {
  JamSlam.Loop();
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
EM_BOOL key_callback(int eventType, const EmscriptenKeyboardEvent *keyEvent, void *userData) {
    if (eventType == EMSCRIPTEN_EVENT_KEYDOWN) {
        if (keyEvent->keyCode == 27) { // Escape key
            JamSlam.Pause();
            return true; // Consume the event
         }
         return true;
    }
    return false; // Don't consume the event
}
#endif

int main() {

  SetConfigFlags(FLAG_VSYNC_HINT);
  SetConfigFlags(FLAG_MSAA_4X_HINT);
  
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TEXT_GAME_TITLE);
  InitAudioDevice();
  SetExitKey(KEY_NULL);
  
  JamSlam.Load();

  #ifdef __EMSCRIPTEN__
    emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, true, key_callback);
    emscripten_set_main_loop(main_loop, 0, 1);
  #else
    if(JamSlam.isDebug()){
      while (!WindowShouldClose() && JamSlam.isRunning()) {
        JamSlam.UpdateDebug();
        BeginDrawing();
        JamSlam.RenderDebug();
        EndDrawing();
      }
    } else {
      SetTargetFPS(120);
      while (!WindowShouldClose() && JamSlam.isRunning()) {
          JamSlam.Loop();
      }
    }
  #endif

  JamSlam.Unload();
  CloseAudioDevice();
  CloseWindow();
  
  return 0;
}
