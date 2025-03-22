#pragma once

#include <tuple>

#include "lib.h"

using namespace std;

class Pot {
    Texture2D atlasPot;
    Rectangle collision;
    float x;
    
    public:
        Pot();
        ~Pot();
        Vector2 GetPosition();
        Rectangle GetCollision();
        tuple<int, int> GetDimensions();
        void SetPosition();
        void Update();
        void Render();
};
