#include "apple.h"

void Fruit::Init() {
    this->_atlasFruits = LoadTexture("resources/applesprites.png");
    this->_nextFruitTimer = TIME_BETWEEN_APPLES;
    for(int i=0; i < APPLE_MAX_COUNT; i++){
        this->RemoveFruit(&this->_fruits[i]);
    }
}

void Fruit::RemoveFruit(Apple* fruit) {
    fruit->_active = false;
}

void Fruit::AddFruit(Apple* fruit, Vector2 position, int fallSpeed) {
    fruit->position = position;
    fruit->fallSpeed = fallSpeed;
    fruit->_active = true;
}

void Fruit::Spawn(void) {
    int availableIndex = -1;
    for(int i=0; i < APPLE_MAX_COUNT; i++){
        if(!this->_fruits[i]._active){
           availableIndex = i;
           break;
        }
    }
    
    if(availableIndex == -1) {
        return;
    }
    
    int fallSpeed = GetRandomValue(FALL_SPEED_MIN, FALL_SPEED_MAX);
    int posX = GetRandomValue(APPLE_SOURCE_WIDTH / 2, screenWidth - (APPLE_SOURCE_WIDTH / 2));
    
    this->AddFruit(&this->_fruits[availableIndex], { float(posX), -APPLE_SOURCE_HEIGHT}, fallSpeed);
}

std::tuple<int, int> Fruit::Update(Basket& basket) {
    int lives = 0;
    int score = 0;

    this->_nextFruitTimer -= GetFrameTime();
    if(this->_nextFruitTimer <= 0) {
        this->_nextFruitTimer = TIME_BETWEEN_APPLES;
        this->Spawn();
    }

    for(int i=0; i < APPLE_MAX_COUNT; i++){
       if(!this->_fruits[i]._active){
            continue;
       }
        
       if(this->_fruits[i].position.y > screenHeight) {
            lives--;
            this->RemoveFruit(&this->_fruits[i]);
            continue;
       }
       
       Vector2 basketPosition = basket.GetPosition();
       if(fabsf(this->_fruits[i].position.x - basketPosition.x) < BASKET_SOURCE_WIDTH && fabsf(this->_fruits[i].position.y - basketPosition.y) < BASKET_SOURCE_HEIGHT / 4 ) {
            score++;
            this->RemoveFruit(&this->_fruits[i]);
            continue;
       }
       
       float offset = this->_fruits[i].fallSpeed * GetFrameTime();
       this->_fruits[i].position.y += offset;
    }
    return std::make_tuple(lives, score);
}

void Fruit::Render(void) {
    for(int i=0; i < APPLE_MAX_COUNT; i++){
       if(!this->_fruits[i]._active){
           continue;
       }
       Vector2 position = this->_fruits[i].position;
       position.x -= APPLE_SOURCE_WIDTH / 2;
       position.y -= APPLE_SOURCE_HEIGHT / 2;
       DrawTextureRec(this->_atlasFruits, APPLE_SOURCE_RECTANGLE, position, WHITE);
    }
}
