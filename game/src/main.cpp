#include "game.hpp"

#define GAME_CONFIG_URI "game/jamslam.ini"

//HISTORY
//DONE: make bucket into a transparent jar, every fruit turns into jam at the bottom, layers build up and clear when it gets to the top.
//DONE: add progressive difficulty, starting with small fruit and more fruits are added faster -- LEVELS
//DONE: Fix background scroll depending on FPS
//DONE: add WIN screen (based on game over screen)
//DONE: add LOSE screen (based on game over screen)
//DONE: add better fonts
//DONE: Add score number popup when fruit is caught
//DONE: add comic style impact lines when fruit is caught
//DONE: add satisfying sound when fruit is caught
//DONE: add sound for bounce, and fruit fall
//DONE: add BGM
//DONE: update game name
//DONE: add eggs that add one life point
//DONE: add spikes that stun for 1 second... after three spikes jar shatters?
//DONE: add bucket effects when hit by spike
//DONE: add effect for catching an egg
//DONE: Add "level #" to HUD

//BACKLOG
//WIP: add more levels, and test for balance
//WIP: iterate until it's fun
    //make bananas one of the harder fruits?
//NEXT: make life into hearts
//TODO: rearrange HUD, place elements in bottom?
//NEXT: add jar bounce combo mechanic
//NEXT: add fruit catch combo mechanic
//TODO: add effect for filling jam to the top 
//TODO: add jar ability - grows bigger after jam gets to top
//TODO: Add bg elements to show max vertical movement (i.e. a vineyard with wooden frames, and one of the wood planks extends horizontally where max Y is)

//TODO: Look into packaging for different platforms
//TODO: Update Readme
//TODO: Look into adding this to itch.io
//TODO: Do Emscriptem build

//OPTIONAL
//TODO: Look into creating a debug build instead of including debug functions always, use define macros to read compiler variables
//TODO: create reward mechanic for each level?
//TODO: make egg rotate
//TODO: update Start menu layout, make game name have transparent background (only start, quit buttons have panel)

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
