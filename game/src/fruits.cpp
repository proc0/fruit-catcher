#include "fruits.hpp"

void Fruits::Load(const ConfigData& config, const FruitLevels& levels) {
    fruitLevels = levels;
    displayDebug = config.debug.modeDebug;
    showCollisions = config.debug.showCages;

    // textures
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

    // sounds - todo try template function here
    for(int i = 0; i < LENGTH_SOUND_SPLATS; i++){
        char uriSplat[22];
        URI_SOUND_SPLAT(uriSplat, i + 1);
        soundSplat[i] = LoadSound(uriSplat);
    }

    for(int i = 0; i < LENGTH_SOUND_THUMPS; i++){
        char uriThump[22];
        URI_SOUND_THUMP(uriThump, i + 1);
        soundThump[i] = LoadSound(uriThump);
    }

    soundSpike = LoadSound(URI_SOUND_SPIKE_HIT);
    soundEgg = LoadSound(URI_SOUND_EGG_CATCH);

    // start menu fruits
    currentLevel = GetRandomValue(0, LEVEL_COUNT-1);
    isMute = true;
}

void Fruits::Unload() {
    for (const auto& sprite : sprites) {
        UnloadTexture(sprite);
    }
    for(int i = 0; i < LENGTH_SOUND_SPLATS; i++){
        UnloadSound(soundSplat[i]);
    }
    for(int i = 0; i < LENGTH_SOUND_THUMPS; i++){
        UnloadSound(soundThump[i]);
    }
    UnloadSound(soundSpike);
    UnloadSound(soundEgg);
}

void Fruits::Reset(void){
    for(auto& fruit : fruits){
        fruit.active = false;
    }
    currentFruits = 0;
}

void Fruits::SetLevel(int level){
    assert(level < fruitLevels.size());
    currentLevel = level;
}

void Fruits::CreateFruit(Fruit &fruit, int index) {
    fruit.type = (FruitType)index;
    fruit.id = GetTime();

    Texture2D fruitSprite = sprites[index];
    const float fruitWidth = (float)fruitSprite.width;
    const float fruitHeight = (float)fruitSprite.height;
    fruit.origin = { fruitWidth/2, fruitHeight/2 };
    fruit.width = fruitWidth;
    fruit.height = fruitHeight;
    
    FruitData fruitData = static_FruitDataMap.at(fruit.type);
    const int longSide = fruitWidth > fruitHeight ? fruitWidth : fruitHeight;
    const float ratio = longSide / SIZE_FRUIT_TEXTURE;
    
    fruit.collision = { 
        .offset = fruitData.offset,
        .radius = SIZE_FRUIT_COLLISION * ratio 
    };
    fruit.bounces = 1;
    fruit.mass = ratio;
    fruit.created = true;
}

void Fruits::SpawnFruit(Fruit &fruit) {
    fruit.active = true;
    fruit.id = GetTime();
    // fruits on screen
    currentFruits++;
    // position
    const float posX = GetRandomValue(fruit.width, SCREEN_WIDTH - fruit.height);
    fruit.position = { posX, -fruit.height };
    // initial forces
    float forceX = 0.0f;
    if(fruit.position.x > SCREEN_WIDTH/2) {
        forceX = GetRandomValue(-500, 0);
    } else {
        forceX = GetRandomValue(0, 500);
    }
    fruit.force = { forceX, GRAVITY };
    // momentum
    fruit.velocity = { 0.0f, 0.0f };
    fruit.rotation = 0.0f;
    // metadata
    fruit.bounces = 1;
    fruit.collided = false;
    fruit.debounce = false;
}

void Fruits::Spawn(void) {
    const LevelSample &sample = fruitLevels[currentLevel].levelSample;
    const int index = GetRandomValue(0, sample.size()-1);
    const int fruitIndex = static_cast<int>(sample[index]);

    int availableIndex = -1;
    bool skipCreate = false;
    for(int i=0; i<LENGTH_FRUIT_TYPES; i++){
        if(fruits[i].active) {
            continue;
        }
        
        if(fruits[i].type == sample[index]){
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

void Fruits::Mute(){
    isMute = true;
}

void Fruits::Unmute(){
    isMute = false;
}

const bool Fruits::IsMute() const {
    return isMute;
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
    if(fruit.collided && !fruit.debounce && fruit.bounces < MAX_FRUIT_BOUNCES) {
        // reverse direction
        fruit.velocity *= -1;
        fruit.collided = false;
        fruit.debounce = true;
    }

    if(fruit.velocity.y < 0 || fruit.bounces > MAX_FRUIT_BOUNCES) {
        // add gravity on bounce
        fruit.velocity.y += GRAVITY * deltaTime;
    } else {
        // reset debounce on fall
        fruit.debounce = false;
    }

}

const FruitResults Fruits::Update(Rectangle bucketCollision, std::vector<Vector2> projectiles) {

    FruitResults results = {};

    fruitTimeInterval -= GetFrameTime();

    if(fruitTimeInterval <= 0) {
        const FruitLevel level = fruitLevels[currentLevel];
        fruitTimeInterval = GetRandomValue(level.minDropTime, level.maxDropTime)/1000.0f;

        if(currentFruits < fruitLevels[currentLevel].density) {
            Spawn();
        }
    } else {
        fruitTimeInterval -= GetFrameTime();
    }

    for(int i=0; i<LENGTH_FRUIT_TYPES; i++){
        Fruit &fruit = fruits[i];

        if(!fruit.active){
            continue;
        }

        const Vector2 fruitCenter = { fruit.position.x + fruit.collision.offset.x, fruit.position.y + fruit.collision.offset.y };
        const Color fruitColor = static_FruitDataMap.at(fruit.type).color;


        for( auto &projectile : projectiles ){
            if(CheckCollisionCircles(fruitCenter, fruit.collision.radius, projectile, 50)){
                fruit.active = false;
                currentFruits--;
                const int splatIdx = GetRandomValue(0, LENGTH_SOUND_SPLATS-1);
                PlaySound(soundSplat[splatIdx]);
                continue;
            }
        }

        // fruit hits bottom
        if(fruit.position.y > SCREEN_HEIGHT) {
            fruit.active = false;
            currentFruits--;
            
            if(fruit.type == FruitType::SPIKE || fruit.type == FruitType::EGG){ 
                continue;
            }

            results.push_back({
                .color = fruitColor,
                .location = fruitCenter,
                .id = fruit.id,
                .score = -1,
                .lives = -1,
                .bounces = fruit.bounces,
                .isCatch = false,
                .bounced = false,
                .isSpike = false,
            });

            if(!isMute){
                const int splatIdx = GetRandomValue(0, LENGTH_SOUND_SPLATS-1);
                PlaySound(soundSplat[splatIdx]);
            }
            continue;
        }
        // fruit hits bucket
        if(!fruit.debounce && CheckCollisionCircleRec(fruitCenter, fruit.collision.radius, bucketCollision)) {
            fruit.collided = true;

            if(fruit.type == FruitType::SPIKE){
                results.push_back({
                    .color = fruitColor,
                    .location = fruitCenter,
                    .id = fruit.id,
                    .score = -1,
                    .lives = -1,
                    .bounces = fruit.bounces,
                    .isCatch = false,
                    .bounced = true,
                    .isSpike = true,
                });

                PlaySound(soundSpike);
                UpdateMovementFruit(fruit);
                continue;
            }
            
            // when fruit is above bucket, and fruit is centered with bucket
            if(fruitCenter.y - fruit.collision.radius < bucketCollision.y && fruitCenter.x - fruit.collision.radius > bucketCollision.x && fruitCenter.x + fruit.collision.radius < bucketCollision.x + bucketCollision.width){
                fruit.active = false;
                currentFruits--;

                const int multiplier = fruit.bounces > 0 ? fruit.bounces : 1;
                results.push_back({
                    .color = fruitColor,
                    .location = fruitCenter,
                    .id = fruit.id,
                    .score = (int)(static_FruitDataMap.at(fruit.type).rating * 100.0f * multiplier),
                    .lives = (int)(fruit.type == FruitType::EGG),
                    .bounces = fruit.bounces,
                    .isCatch = true,
                    .bounced = false,
                    .isSpike = false,
                });

                if(fruit.type == FruitType::EGG){
                    PlaySound(soundEgg);
                } else {
                    const int splatIdx = GetRandomValue(0, LENGTH_SOUND_THUMPS-1);
                    PlaySound(soundThump[splatIdx]);
                }

                UpdateMovementFruit(fruit);
                continue;
            }

            fruit.bounces++;
            results.push_back({
                .color = fruitColor,
                .location = fruitCenter,
                .id = fruit.id,
                .score = 0,
                .lives = 0,
                .bounces = fruit.bounces,
                .isCatch = false,
                .bounced = true,
                .isSpike = false,
            });
        }

        UpdateMovementFruit(fruit);
    }

    return results;
}

void Fruits::UpdateWin(){
    Spawn();
    for(int i=0; i<LENGTH_FRUIT_TYPES; i++){
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
    for(int i=0; i<LENGTH_FRUIT_TYPES; i++){
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
