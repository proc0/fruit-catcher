#include "basket.hpp"

#define BASKET_SOURCE_WIDTH 100
#define BASKET_SOURCE_HEIGHT 100
#define BASKET_SOURCE_RECTANGLE CLITERAL(Rectangle){0, 0, BASKET_SOURCE_WIDTH, BASKET_SOURCE_HEIGHT}

using namespace std;

void Basket::Init(){
    _atlasBasket = LoadTexture("resources/basket.png");
}

void Basket::SetPosition(void) {
    _x = GetMousePosition().x;
}

Vector2 Basket::GetPosition(void) {
    return { _x, screenHeight - (BASKET_SOURCE_HEIGHT * 0.75) };
}

tuple<int, int> Basket::GetDimensions(void) {
    return make_tuple(_atlasBasket.width, _atlasBasket.height);
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