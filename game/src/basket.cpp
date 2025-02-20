#include "raylib.h"
#include "lib.h"

#include "basket.hpp"

void Basket::Init(){
    _atlasBasket = LoadTexture("resources/basket.png");
}

void Basket::SetPosition(void) {
    _x = GetMousePosition().x;
}

Vector2 Basket::GetPosition(void) {
    return { _x, screenHeight - (BASKET_SOURCE_HEIGHT * 0.75) };
}

void Basket::Update(void) {
    SetPosition();
}

void Basket::Render(void) {
    Vector2 position = GetPosition();
    
    position.x -= _atlasBasket.width / 2;
    position.y -= _atlasBasket.height / 2;
    
    DrawTextureRec(_atlasBasket, BASKET_SOURCE_RECTANGLE, position, WHITE);
}