#include "bucket.hpp"

#define POT_IMAGE_URI "resources/pot.png"
#define POT_SOURCE_WIDTH 150
#define POT_SOURCE_HEIGHT 150
#define POT_SOURCE_RECTANGLE CLITERAL(Rectangle){0, 0, POT_SOURCE_WIDTH, POT_SOURCE_HEIGHT}

using namespace std;

Bucket::Bucket(){
    atlasBucket = LoadTexture(POT_IMAGE_URI);
    Vector2 collPos = GetPosition();
    collision = { collPos.x, collPos.y, POT_SOURCE_WIDTH, POT_SOURCE_HEIGHT };
    SetPosition();
}

Bucket::~Bucket() {
    UnloadTexture(atlasBucket);
}

void Bucket::SetPosition(void) {
    float xPos = GetMousePosition().x;
    this->x = xPos;
    collision.x = xPos - atlasBucket.width/2;
    
}

Rectangle Bucket::GetCollision() {
    return collision;
}

Vector2 Bucket::GetPosition(void) {
    return { x - atlasBucket.width/2, SCREEN_HEIGHT - (POT_SOURCE_HEIGHT * 1.25) };
}

tuple<int, int> Bucket::GetDimensions(void) {
    return make_tuple(atlasBucket.width, atlasBucket.height);
}

void Bucket::Update(void) {
    SetPosition();
}

void Bucket::Render(void) {
    Vector2 position = GetPosition();
    
    // position.x -= atlasBucket.width/2;
    // position.y -= atlasBucket.height/2;
    
    DrawTextureRec(atlasBucket, POT_SOURCE_RECTANGLE, position, WHITE);
    // DrawRectangleRec(collision, GREEN);
}
