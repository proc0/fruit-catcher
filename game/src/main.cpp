#include "game.hpp"

#define GAME_CONFIG_URI "game/FruitCatcher.ini"

int main() {

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TEXT_GAME_TITLE);
  SetTargetFPS(60);
 
  Game Game(GAME_CONFIG_URI);

  if(Game.isDebug()){
    Game.UpdateDebug();
    while (!WindowShouldClose() && Game.isRunning()) {
      BeginDrawing();
      Game.RenderDebug();
      EndDrawing();
    }
  } else {
    while (!WindowShouldClose() && Game.isRunning()) {
      Game.Update();
      BeginDrawing();
      Game.Render();
      EndDrawing();
    }
  }

  CloseWindow();
  
  return 0;
}
