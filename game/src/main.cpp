#include "arena.hpp"
#include "game.hpp"

int main() {

  // Fruits _Fruit;
  // Basket _Basket;
  // Display _Display;
  Game _Game;

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TEXT_GAME_TITLE);
  SetTargetFPS(60);

  _Game.Load();
  // int frames = 0;
  // Image bg = LoadImageAnim("resources/bg_anim.gif", &frames);
  // Texture2D bganim = LoadTextureFromImage(bg);
  // BeginDrawing();
  // DrawTextureRec(bg, {0, 0, 800, 600 }, {0, 0}, RAYWHITE);
  // EndDrawing();
  // unsigned int nextFrameDataOffset = 0;  // Current byte offset to next frame in image.data

  // int currentAnimFrame = 0;       // Current animation frame to load and draw
  // int frameDelay = 8;             // Frame delay to switch between animation frames
  // int frameCounter = 0;           // General frames counter

  while (!WindowShouldClose()) {
    _Game.Update();

    // frameCounter++;
    // if (frameCounter >= frameDelay)
    // {
    //     // Move to next frame
    //     // NOTE: If final frame is reached we return to first frame
    //     currentAnimFrame++;
    //     if (currentAnimFrame >= frames) currentAnimFrame = 0;

    //     // Get memory offset position for next frame data in image.data
    //     nextFrameDataOffset = bg.width*bg.height*4*currentAnimFrame;

    //     // Update GPU texture data with next frame image data
    //     // WARNING: Data size (frame size) and pixel format must match already created texture
    //     UpdateTexture(bganim, ((unsigned char *)bg.data) + nextFrameDataOffset);

    //     frameCounter = 0;
    // }

    BeginDrawing();
    // ClearBackground(RAYWHITE);
    // DrawTexturePro(bganim, {0,0, (float)bg.width, (float)bg.height}, {0,0, 800, 600}, {0,0}, 0, RAYWHITE);

    _Game.Render();
    
    EndDrawing();
  }

  _Game.Unload();
  // UnloadTexture(bganim);   // Unload texture
  // UnloadImage(bg);      // Unload image (contains all frames)

  CloseWindow();
  
  return 0;
}
