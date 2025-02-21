#include "fruits.hpp"

#define ATLAS_FRUIT_URI "resources/applesprites.png"
#define ATLAS_FRUIT_WIDTH 75
#define ATLAS_FRUIT_HEIGHT 75
#define ATLAS_FRUIT_TYPES 4
#define ATLAS_FRUIT_RECT(xPos) CLITERAL(Rectangle){xPos, 0, ATLAS_FRUIT_WIDTH, ATLAS_FRUIT_HEIGHT}

#define TIME_BETWEEN_APPLES 1.0f
#define FALL_SPEED_MIN 150
#define FALL_SPEED_MAX 250

using namespace std;

void Fruits::Init() {
    atlasFruit = LoadTexture(ATLAS_FRUIT_URI);
    timeNextFruit = TIME_BETWEEN_APPLES;
    for(int i=0; i < GAME_FRUITS_MAX; i++){
        Remove(&fruits[i]);
    }
}

void Fruits::Remove(Movable *fruit) {
    fruit->active = false;
}

void Fruits::Add(Movable *fruit, Vector2 position, int speed) {
    fruit->active = true;
    fruit->position = position;
    fruit->speed = speed;
    float fruitTexturePos = GetRandomValue(0, ATLAS_FRUIT_TYPES-1) * ATLAS_FRUIT_WIDTH;
    fruit->atlasPosition = fruitTexturePos;
}

void Fruits::Spawn(void) {
    int availableIndex = -1;
    for(int i=0; i<GAME_FRUITS_MAX; i++){
        if(!fruits[i].active){
           availableIndex = i;
           break;
        }
    }
    
    if(availableIndex == -1) {
        return;
    }
    
    int speed = GetRandomValue(FALL_SPEED_MIN, FALL_SPEED_MAX);
    int posX = GetRandomValue(ATLAS_FRUIT_WIDTH/2, SCREEN_WIDTH - (ATLAS_FRUIT_WIDTH/2));
    
    Add(&fruits[availableIndex], { float(posX), -ATLAS_FRUIT_HEIGHT}, speed);
}

tuple<int, int> Fruits::Update(Basket &basket) {
    int lives = 0;
    int score = 0;

    timeNextFruit -= GetFrameTime();
    if(timeNextFruit <= 0) {
        timeNextFruit = TIME_BETWEEN_APPLES;
        Spawn();
    }

    for(int i=0; i<GAME_FRUITS_MAX; i++){
       if(!fruits[i].active){
            continue;
       }
        
       if(fruits[i].position.y > SCREEN_HEIGHT) {
            lives--;
            Remove(&fruits[i]);
            continue;
       }
       
       Vector2 basketPosition = basket.GetPosition();
       tuple<int, int> basketDimensions = basket.GetDimensions();
       int basketWidth = get<0>(basketDimensions);
       int basketHeight = get<1>(basketDimensions);
       if(fabsf(fruits[i].position.x - basketPosition.x) < basketWidth && fabsf(fruits[i].position.y - basketPosition.y) < basketHeight/4) {
            score++;
            Remove(&fruits[i]);
            continue;
       }

       fruits[i].position.y += fruits[i].speed * GetFrameTime();
    }

    return make_tuple(lives, score);
}

void Fruits::Render(void) {
    for(int i=0; i<GAME_FRUITS_MAX; i++){
       if(!fruits[i].active){
           continue;
       }
       Vector2 position = fruits[i].position;
       position.x -= ATLAS_FRUIT_WIDTH/2;
       position.y -= ATLAS_FRUIT_HEIGHT/2;
       
       DrawTextureRec(atlasFruit, ATLAS_FRUIT_RECT(fruits[i].atlasPosition), position, WHITE);
    }
}
