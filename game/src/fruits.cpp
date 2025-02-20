#include <tuple>
#include "fruits.hpp"

#define APPLE_SOURCE_WIDTH 75
#define APPLE_SOURCE_HEIGHT 75
#define APPLE_SOURCE_RECTANGLE CLITERAL(Rectangle){0, 0, APPLE_SOURCE_WIDTH, APPLE_SOURCE_HEIGHT}

#define TIME_BETWEEN_APPLES 1.0f
#define FALL_SPEED_MIN 150
#define FALL_SPEED_MAX 250

using namespace std;

void Fruits::Init() {
    _atlasFruits = LoadTexture("resources/applesprites.png");
    _nextFruitTimer = TIME_BETWEEN_APPLES;
    for(int i=0; i < APPLE_MAX_COUNT; i++){
        RemoveFruit(&fruits[i]);
    }
}

void Fruits::RemoveFruit(Apple *fruit) {
    fruit->_active = false;
}

void Fruits::AddFruit(Apple *fruit, Vector2 position, int fallSpeed) {
    fruit->position = position;
    fruit->fallSpeed = fallSpeed;
    fruit->_active = true;
}

void Fruits::Spawn(void) {
    int availableIndex = -1;
    for(int i=0; i < APPLE_MAX_COUNT; i++){
        if(!fruits[i]._active){
           availableIndex = i;
           break;
        }
    }
    
    if(availableIndex == -1) {
        return;
    }
    
    int fallSpeed = GetRandomValue(FALL_SPEED_MIN, FALL_SPEED_MAX);
    int posX = GetRandomValue(APPLE_SOURCE_WIDTH / 2, screenWidth - (APPLE_SOURCE_WIDTH / 2));
    
    AddFruit(&fruits[availableIndex], { float(posX), -APPLE_SOURCE_HEIGHT}, fallSpeed);
}

tuple<int, int> Fruits::Update(Basket &basket) {
    int lives = 0;
    int score = 0;

    _nextFruitTimer -= GetFrameTime();
    if(_nextFruitTimer <= 0) {
        _nextFruitTimer = TIME_BETWEEN_APPLES;
        Spawn();
    }

    for(int i=0; i < APPLE_MAX_COUNT; i++){
       if(!fruits[i]._active){
            continue;
       }
        
       if(fruits[i].position.y > screenHeight) {
            lives--;
            RemoveFruit(&fruits[i]);
            continue;
       }
       
       Vector2 basketPosition = basket.GetPosition();
       tuple<int, int> basketDimensions = basket.GetDimensions();
       int basketWidth = get<0>(basketDimensions);
       int basketHeight = get<1>(basketDimensions);
       if(fabsf(fruits[i].position.x - basketPosition.x) < basketWidth && fabsf(fruits[i].position.y - basketPosition.y) < basketHeight / 4 ) {
            score++;
            RemoveFruit(&fruits[i]);
            continue;
       }
       
       float offset = fruits[i].fallSpeed * GetFrameTime();
       fruits[i].position.y += offset;
    }
    return make_tuple(lives, score);
}

void Fruits::Render(void) {
    for(int i=0; i < APPLE_MAX_COUNT; i++){
       if(!fruits[i]._active){
           continue;
       }
       Vector2 position = fruits[i].position;
       position.x -= APPLE_SOURCE_WIDTH / 2;
       position.y -= APPLE_SOURCE_HEIGHT / 2;
       DrawTextureRec(_atlasFruits, APPLE_SOURCE_RECTANGLE, position, WHITE);
    }
}
