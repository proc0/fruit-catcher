#include "stage.hpp"

void Stage::Load() {
    cloudsBackground = LoadTexture("resources/cloud_bg.png");
    cloudsForeground = LoadTexture("resources/cloud_fg.png");
    mountains = LoadTexture("resources/mountains.png");
    sun = LoadTexture("resources/sun.png");
    SetTextureWrap(cloudsBackground, TEXTURE_WRAP_MIRROR_REPEAT);
    SetTextureWrap(cloudsForeground, TEXTURE_WRAP_MIRROR_REPEAT);

    cloudsBackgroundRect = { 0, 0, SCREEN_WIDTH, 596 };
    cloudsForegroundRect = { 0, 0, SCREEN_WIDTH, 296 };
    cloudsTextureRect = { 0, 0, SCREEN_WIDTH, -596 };
    cloudTexture = LoadRenderTexture(SCREEN_WIDTH, 596);
}

void Stage::Unload() {
    UnloadTexture(cloudsBackground);
    UnloadTexture(cloudsForeground);
    UnloadTexture(mountains);
    UnloadTexture(sun);
    UnloadRenderTexture(cloudTexture);
}

void Stage::Update() {
    cloudsBackgroundRect.x += 2.0f * GetFrameTime();
    if(cloudsBackgroundRect.x > SCREEN_WIDTH){
        cloudsBackgroundRect.x = 0.0f;
    }

    cloudsForegroundRect.x += 4.0f * GetFrameTime();
    if(cloudsForegroundRect.x > SCREEN_WIDTH){
        cloudsForegroundRect.x = 0.0f;
    }
}

void Stage::Render() const {
    DrawRectangleGradientV(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Color {80, 123, 179, 255}, Color {213, 246, 255, 255});
    DrawTexture(sun, 600, 300, WHITE);

    BeginTextureMode(cloudTexture);
    ClearBackground(BLANK);
    DrawTextureRec(cloudsBackground, cloudsBackgroundRect, {0, 241}, WHITE); 
    DrawTextureRec(cloudsForeground, cloudsForegroundRect, {0, 424}, WHITE); 
    EndTextureMode();

    DrawTextureRec(cloudTexture.texture, cloudsTextureRect, {0, 0}, { 255, 255, 255, 150 });
    DrawTexture(mountains, 0, 500, WHITE);
}
