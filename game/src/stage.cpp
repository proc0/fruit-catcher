#include "stage.hpp"

Stage::Stage() {
    cloudsBackground = LoadTexture("resources/cloud_bg.png");
    cloudsForeground = LoadTexture("resources/cloud_fg.png");
    mountains = LoadTexture("resources/mountains.png");
    sun = LoadTexture("resources/sun.png");
    SetTextureWrap(cloudsBackground, TEXTURE_WRAP_REPEAT);
    SetTextureWrap(cloudsForeground, TEXTURE_WRAP_REPEAT);
    cloudsBackgroundRect = { 0, 0, SCREEN_WIDTH, 596 };
    cloudsForegroundRect = { 0, 0, SCREEN_WIDTH, 296 };
}

Stage::~Stage() {
    UnloadTexture(cloudsBackground);
    UnloadTexture(cloudsForeground);
    UnloadTexture(mountains);
    UnloadTexture(sun);
}

void Stage::Update() {
    cloudsBackgroundRect.x += 0.005;
    if(cloudsBackgroundRect.x > SCREEN_WIDTH){
        cloudsBackgroundRect.x = 0;
    }

    cloudsForegroundRect.x += 0.01;
    if(cloudsForegroundRect.x > SCREEN_WIDTH){
        cloudsForegroundRect.x = 0;
    }
}

void Stage::Render() {
    DrawRectangleGradientV(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Color {80, 123, 179, 255}, Color {213, 246, 255, 255});
    DrawTexture(sun, 600, 300, WHITE);
    DrawTextureRec(cloudsBackground, cloudsBackgroundRect, {0, 241}, { 255, 255, 255, 150 }); 
    DrawTextureRec(cloudsForeground, cloudsForegroundRect, {0, 424}, WHITE); 
    DrawTexture(mountains, 0, 500, WHITE);
}
