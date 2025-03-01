#include "basket.hpp"

#define ATLAS_BASKET_URI "resources/pot.png"
#define BASKET_SOURCE_WIDTH 150
#define BASKET_SOURCE_HEIGHT 150
#define BASKET_SOURCE_RECTANGLE CLITERAL(Rectangle){0, 0, BASKET_SOURCE_WIDTH, BASKET_SOURCE_HEIGHT}

using namespace std;

Basket::Basket(){
    atlasBasket = LoadTexture(ATLAS_BASKET_URI);
    Vector2 collPos = GetPosition();
    collision = { collPos.x, collPos.y, BASKET_SOURCE_WIDTH, BASKET_SOURCE_HEIGHT };
    SetPosition();
}

Basket::~Basket() {
    UnloadTexture(atlasBasket);
}

void Basket::SetPosition(void) {
    float xPos = GetMousePosition().x;
    this->x = xPos;
    collision.x = xPos - atlasBasket.width/2;
    
}

Rectangle Basket::GetCollision() {
    return collision;
}

Vector2 Basket::GetPosition(void) {
    return { x - atlasBasket.width/2, SCREEN_HEIGHT - (BASKET_SOURCE_HEIGHT * 1.25) };
}

tuple<int, int> Basket::GetDimensions(void) {
    return make_tuple(atlasBasket.width, atlasBasket.height);
}

void Basket::Update(void) {
    SetPosition();
}

void Basket::Render(void) {
    Vector2 position = GetPosition();
    
    // position.x -= atlasBasket.width/2;
    // position.y -= atlasBasket.height/2;
    
    DrawTextureRec(atlasBasket, BASKET_SOURCE_RECTANGLE, position, WHITE);
    // DrawRectangleRec(collision, GREEN);
}
