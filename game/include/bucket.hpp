#pragma once

#include <tuple>

#include "lib.h"

class Bucket {
    Texture2D texture;
    Rectangle collision;
    
    public:
        Bucket();
        ~Bucket();
        const Vector2 GetPosition() const;
        const Rectangle GetCollision() const;
        // const std::tuple<int, int> GetDimensions() const;
        void Update(Vector2 mousePosition);
        void Render() const;
};
