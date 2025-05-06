#include "fruits.hpp"

#define FRUIT_SOUND_SPLAT(buf, idx) sprintf(buf, "resources/splat%d.wav", idx)

#define FRUIT_MAX_SIZE 150.0f
#define FRUIT_COLLISION_RADIUS 42.0f
#define FRUIT_TIME_INTERVAL 1.0f
#define GRAVITY 982.0f

Fruits::Fruits(const ConfigData& configData, const FruitLevels& levelData): 
fruitLevels(levelData),
displayDebug(configData.debug.displayDebug),
showCollisions(configData.debug.showCollisions) {
    for (const auto& fruitData : static_FruitDataMap) {
        try {
            const int spriteKey = static_cast<int>(fruitData.first);
            const char* spriteUri = fruitData.second.uri.c_str();
            sprites[spriteKey] = LoadTexture(spriteUri);
        } catch (const std::exception& error) {
            std::cerr << error.what() << '\n';
            CloseWindow();
        }
    }
    for(int i = 0; i < SOUND_SPLAT_LENGTH; i++){
        const int idx = i + 1;
        char uri[20];
        FRUIT_SOUND_SPLAT(uri, idx);
        soundSplat[i] = LoadSound(uri);
    }
}

Fruits::~Fruits() {
    for (const auto& sprite : sprites) {
        UnloadTexture(sprite);
    }
    for(int i = 0; i < SOUND_SPLAT_LENGTH; i++){
        UnloadSound(soundSplat[i]);
    }
}

void Fruits::Reset(void){
    for(auto& fruit : fruits){
        fruit.active = false;
    }
    currentFruits = 0;
}

void Fruits::SetLevel(int level){
    if(level > fruitLevels.size()) {
        std::cout << "Fruit.SetLevel Index Error" << std::endl;
        return;
    }
    currentLevel = level;
}

void Fruits::CreateFruit(Fruit &fruit, int index) {
    fruit.type = (FruitType)index;

    Texture2D fruitSprite = sprites[index];
    const float fruitWidth = (float)fruitSprite.width;
    const float fruitHeight = (float)fruitSprite.height;
    fruit.origin = { fruitWidth/2, fruitHeight/2 };
    fruit.width = fruitWidth;
    fruit.height = fruitHeight;
    
    FruitData fruitData = static_FruitDataMap.at(fruit.type);
    const int longSide = fruitWidth > fruitHeight ? fruitWidth : fruitHeight;
    const float ratio = longSide / FRUIT_MAX_SIZE;
    
    fruit.collision = { 
        offset: fruitData.offset,
        radius: FRUIT_COLLISION_RADIUS * ratio 
    };

    fruit.mass = ratio;
    fruit.created = true;
}

void Fruits::SpawnFruit(Fruit &fruit) {
    fruit.active = true;
    currentFruits++;
    const float posX = GetRandomValue(fruit.width, SCREEN_WIDTH - fruit.height);
    fruit.position = { posX, -fruit.height };
    
    float forceX = 0.0f;
    if(fruit.position.x > SCREEN_WIDTH/2) {
        forceX = GetRandomValue(-500, 0);
    } else {
        forceX = GetRandomValue(0, 500);
    }
    fruit.force = { forceX, GRAVITY };

    fruit.velocity = { 0.0f, 0.0f };
    fruit.rotation = 0.0f;
    fruit.collided = false;
    fruit.debounce = false;
}

void Fruits::Spawn(void) {
    const FruitSample &fruitSample = fruitLevels[currentLevel].fruitSample;
    const int index = GetRandomValue(0, fruitSample.size()-1);
    const int fruitIndex = static_cast<int>(fruitSample[index]);

    int availableIndex = -1;
    bool skipCreate = false;
    for(int i=0; i<FRUIT_TYPE_COUNT; i++){
        if(fruits[i].active) {
            continue;
        }
        
        if(fruits[i].type == fruitSample[index]){
            skipCreate = true;
            availableIndex = i;
            break;
        } else if(availableIndex == -1){
            availableIndex = i;
        }
    }
    
    if(availableIndex == -1) {
        return;
    }

    Fruit &fruit = fruits[availableIndex];
    if(!fruit.created || !skipCreate) {
        CreateFruit(fruit, fruitIndex);
    }

    SpawnFruit(fruit);
}

void Fruits::UpdateMovementFruit(Fruit &fruit) {
    // Velocity Verlet Integration
    // (i)   x(t+Δt) = x(t) + v(t)Δt + 1/2a(t)Δt^2
    // (ii)  a(t+Δt) = f(x(t+Δt))
    // (iii) v(t+Δt) = v(t) + 1/2(a(t)+a(t+Δt))Δt

    // Δt and Δt * 1/2
    const float deltaTime = GetFrameTime();
    const float halfTime = deltaTime * 0.5f;
    // [acceleration] using F=ma
    const Vector2 acceleration = fruit.force/fruit.mass;
    // (i) [next position] expanding first equation to match order
    // x(t+Δt) = x(t) + v(t) * Δt + a(t) * Δt * (Δt * 1/2)
    fruit.position = fruit.position + fruit.velocity * deltaTime + acceleration * deltaTime * halfTime;
    // (iii) [next velocity] without force change (skips (ii))
    // v(t+Δt) = v(t) + a(t)Δt
    fruit.velocity = fruit.velocity + acceleration * deltaTime;

    // [rotation] basic simple rotation effect
    fruit.rotation += acceleration.x * halfTime;
    
    // on collision
    if(fruit.collided && !fruit.debounce) {
        // reverse direction
        fruit.velocity *= -1;
        fruit.collided = false;
        fruit.debounce = true;
    }

    if(fruit.velocity.y < 0) {
        // add gravity on bounce
        fruit.velocity.y += GRAVITY * deltaTime;
    } else {
        // reset debounce on fall
        fruit.debounce = false;
    }
}

const FruitResult Fruits::Update(Rectangle bucketCollision) {
    FruitResult result = {
        .location = { 0.0f, 0.0f },
        .color = { 0, 0, 0, 0 },
        .score = 0,
        .lives = 0,
        .isCatch = false,
        .bounced = false,
        .isSpike = false,
    };

    fruitTimeInterval -= GetFrameTime();

    if(fruitTimeInterval <= 0) {
        const FruitLevelData level = fruitLevels[currentLevel];
        fruitTimeInterval = GetRandomValue(level.dropFrequencyMin, level.dropFrequencyMax)/1000.0f;

        if(currentFruits < fruitLevels[currentLevel].density) {
            Spawn();
        }
    } else {
        fruitTimeInterval -= GetFrameTime();
    }

    for(int i=0; i<FRUIT_TYPE_COUNT; i++){
        Fruit &fruit = fruits[i];

        if(!fruit.active){
            continue;
        }

        const Vector2 fruitCenter = { fruit.position.x + fruit.collision.offset.x, fruit.position.y + fruit.collision.offset.y };
        // fruit hits bottom
        if(fruit.position.y > SCREEN_HEIGHT) {
            fruit.active = false;
            currentFruits--;
            result = {
                .location = fruitCenter,
                .color = static_FruitDataMap.at(fruit.type).color,
                .score = -1,
                .lives = -1,
                .isCatch = false,
                .bounced = false,
            };
            const int splatIdx = GetRandomValue(0, SOUND_SPLAT_LENGTH-1);
            PlaySound(soundSplat[splatIdx]);
            continue;
        }
        // fruit hits bucket
        if(!fruit.debounce && CheckCollisionCircleRec(fruitCenter, fruit.collision.radius, bucketCollision)) {
            fruit.collided = true;

            if(fruit.type == FruitType::SPIKE){
                result = {
                    .location = fruitCenter,
                    .color = static_FruitDataMap.at(fruit.type).color,
                    .score = 0,
                    .lives = 0,
                    .isCatch = true,
                    .bounced = false,
                    .isSpike = true,
                };
                continue;
            }
            // when fruit is above bucket, and fruit is centered with bucket
            if(fruitCenter.y - fruit.collision.radius < bucketCollision.y && fruitCenter.x - fruit.collision.radius > bucketCollision.x && fruitCenter.x + fruit.collision.radius < bucketCollision.x + bucketCollision.width){
                currentFruits--;
                fruit.active = false;

                result = {
                    .location = fruitCenter,
                    .color = static_FruitDataMap.at(fruit.type).color,
                    .score = (int)(static_FruitDataMap.at(fruit.type).rating * 100.0f),
                    .lives = (int)(fruit.type == FruitType::EGG),
                    .isCatch = true,
                    .bounced = false,
                    .isSpike = false,
                };
                continue;
            }

            result.bounced = true;
        }

        UpdateMovementFruit(fruit);
    }

    return result;
}

void Fruits::UpdateWin(){
    Spawn();
    for(int i=0; i<FRUIT_TYPE_COUNT; i++){
        if(fruits[i].position.y > SCREEN_HEIGHT) {
            fruits[i].active = false;
            continue;
        }
        UpdateMovementFruit(fruits[i]);
    }
}

void Fruits::UpdateDebug(void){
    int colNum = 0;
    int rowNum = 0;
    for(int i=0; i<FRUIT_TYPE_COUNT; i++){
        Fruit& fruit = fruitsDebug[i];

        CreateFruit(fruit, i);
    
        fruit.position.x = 150*colNum + 100;
        fruit.position.y = 150*rowNum + 100;
        if(colNum == 4){
            colNum = 0;
            rowNum++;
        } else {
            colNum++; 
        }
    }
}

void Fruits::RenderFruit(const Fruit& fruit) const {
    int spriteKey = static_cast<int>(fruit.type);
    Texture2D fruitSprite = sprites[spriteKey];
    DrawTexturePro(fruitSprite, {0, 0, fruit.width, fruit.height}, {fruit.position.x, fruit.position.y, fruit.width, fruit.height}, fruit.origin, fruit.rotation, WHITE);
}

void Fruits::RenderDebug(void) const {
    for (const Fruit& fruit : fruitsDebug) {
        RenderFruit(fruit);

        if(showCollisions){
            const Vector2 fruitCenter = { fruit.position.x + fruit.collision.offset.x, fruit.position.y + fruit.collision.offset.y };
            DrawCircleV(fruitCenter, fruit.collision.radius, BLUE);
        }

        DrawRectangleLines(fruit.position.x - fruit.origin.x, fruit.position.y - fruit.origin.y, fruit.width, fruit.height, RED);
    }
}

void Fruits::Render(void) const {
    for (const Fruit& fruit : fruits) {
        if(!fruit.active) continue;

        RenderFruit(fruit);
    }
}
