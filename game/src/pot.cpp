#include "pot.hpp"

#define POT_IMAGE_URI "resources/pot.png"
#define POT_SOURCE_WIDTH 150
#define POT_SOURCE_HEIGHT 150
#define POT_SOURCE_RECTANGLE CLITERAL(Rectangle){0, 0, POT_SOURCE_WIDTH, POT_SOURCE_HEIGHT}

using namespace std;

Pot::Pot(){
    atlasPot = LoadTexture(POT_IMAGE_URI);
    Vector2 collPos = GetPosition();
    collision = { collPos.x, collPos.y, POT_SOURCE_WIDTH, POT_SOURCE_HEIGHT };
    SetPosition();
}

Pot::~Pot() {
    UnloadTexture(atlasPot);
}

void Pot::SetPosition(void) {
    float xPos = GetMousePosition().x;
    this->x = xPos;
    collision.x = xPos - atlasPot.width/2;
    
}

Rectangle Pot::GetCollision() {
    return collision;
}

Vector2 Pot::GetPosition(void) {
    return { x - atlasPot.width/2, SCREEN_HEIGHT - (POT_SOURCE_HEIGHT * 1.25) };
}

tuple<int, int> Pot::GetDimensions(void) {
    return make_tuple(atlasPot.width, atlasPot.height);
}

void Pot::Update(void) {
    SetPosition();
}

void Pot::Render(void) {
    Vector2 position = GetPosition();
    
    // position.x -= atlasPot.width/2;
    // position.y -= atlasPot.height/2;
    
    DrawTextureRec(atlasPot, POT_SOURCE_RECTANGLE, position, WHITE);
    // DrawRectangleRec(collision, GREEN);
}
