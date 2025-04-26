#include "bucket.hpp"

#define BUCKET_IMAGE_URI "resources/bucket.png"
#define BUCKET_JAM_TOP_URI "resources/jam-top.png"
#define BUCKET_JAM_MIDDLE_URI "resources/jam-middle.png"
#define BUCKET_JAM_BOTTOM_URI "resources/jam-bottom.png"
#define BUCKET_SOURCE_WIDTH 131
#define BUCKET_SOURCE_HEIGHT 160
#define BUCKET_COLLISION_SIZE 110
#define BUCKET_SOURCE_RECTANGLE CLITERAL(Rectangle){0, 0, BUCKET_SOURCE_WIDTH, BUCKET_SOURCE_HEIGHT}

using namespace std;

Bucket::Bucket(){
    texture = LoadTexture(BUCKET_IMAGE_URI);
    textureJamTop = LoadTexture(BUCKET_JAM_TOP_URI);
    textureJamMiddle = LoadTexture(BUCKET_JAM_MIDDLE_URI);
    textureJamBottom = LoadTexture(BUCKET_JAM_BOTTOM_URI);
    Vector2 mousePosition = GetMousePosition();
    position = { mousePosition.x - texture.width/2, SCREEN_HEIGHT - BUCKET_SOURCE_HEIGHT * 1.18 };
    jamTopPosition = { mousePosition.x - texture.width/2 + 12, SCREEN_HEIGHT - BUCKET_SOURCE_HEIGHT + 10 };
    jamMiddlePosition = { mousePosition.x - texture.width/2 + 12, SCREEN_HEIGHT - BUCKET_SOURCE_HEIGHT + 40 };
    jamBottomPosition = { mousePosition.x - texture.width/2 + 12, SCREEN_HEIGHT - BUCKET_SOURCE_HEIGHT + 90 };
    collision = { mousePosition.x - BUCKET_COLLISION_SIZE/2, SCREEN_HEIGHT - BUCKET_SOURCE_HEIGHT, BUCKET_COLLISION_SIZE, BUCKET_COLLISION_SIZE };

    UpdateColor(PINK);
}

Bucket::~Bucket() {
    UnloadTexture(texture);
    UnloadTexture(textureJamTop);
    UnloadTexture(textureJamMiddle);
    UnloadTexture(textureJamBottom);
}

const Rectangle Bucket::GetCollision() const {
    return collision;
}

void Bucket::UpdateColor(const Color color) {
    jamColor = color;
}

void Bucket::Update(const Vector2 mousePosition) {
    position.x = mousePosition.x - texture.width/2;
    jamTopPosition.x = mousePosition.x - texture.width/2 + 12;
    jamMiddlePosition.x = mousePosition.x - texture.width/2 + 12;
    jamBottomPosition.x = mousePosition.x - texture.width/2 + 12;
    collision.x = mousePosition.x - BUCKET_COLLISION_SIZE/2;
}

void Bucket::UpdateDebug() {
    position.x = SCREEN_WIDTH/2 - texture.width/2;
    jamTopPosition.x = SCREEN_WIDTH/2 - texture.width/2 + 12;
    jamMiddlePosition.x = SCREEN_WIDTH/2 - texture.width/2 + 12;
    jamBottomPosition.x = SCREEN_WIDTH/2 - texture.width/2 + 12;
    collision.x = SCREEN_WIDTH/2 - BUCKET_COLLISION_SIZE/2;
}

void Bucket::Render(void) const {
    DrawTextureRec(textureJamTop, {0, 0, 105, 30}, jamTopPosition, jamColor);
    DrawTextureRec(textureJamMiddle, {0, 0, 105, 50}, jamMiddlePosition, jamColor);
    DrawTextureRec(textureJamBottom, {0, 0, 105, 30}, jamBottomPosition, jamColor);
    DrawTextureRec(texture, BUCKET_SOURCE_RECTANGLE, position, WHITE);
}

void Bucket::RenderDebug(void) const {
    Render();

    DrawRectangleLinesEx(collision, 2, GREEN);
}
