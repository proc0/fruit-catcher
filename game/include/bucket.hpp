#pragma once

#include <tuple>

#include "lib.h"

class Bucket {
    Texture2D atlasBucket;
    Rectangle collision;
    float x;
    
    public:
        Bucket();
        ~Bucket();
        Vector2 GetPosition();
        Rectangle GetCollision();
        std::tuple<int, int> GetDimensions();
        void SetPosition();
        void Update();
        void Render();
};
