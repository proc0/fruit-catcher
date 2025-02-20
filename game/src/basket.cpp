#include "raylib.h"
#include "lib.h"

#include "basket.hpp"

void Basket::Init(){
    this->_atlasBasket = LoadTexture("resources/basket.png");
}

void Basket::SetPosition(void) {
    this->_x = GetMousePosition().x;
}

Vector2 Basket::GetPosition(void) {
    return { this->_x, screenHeight - (BASKET_SOURCE_HEIGHT * 0.75) };
}

void Basket::Update(void) {
    this->SetPosition();
}

void Basket::Render(void) {
    Vector2 position = this->GetPosition();
    
    position.x -= this->_atlasBasket.width / 2;
    position.y -= this->_atlasBasket.height / 2;
    
    DrawTextureRec(this->_atlasBasket, BASKET_SOURCE_RECTANGLE, position, WHITE);
}