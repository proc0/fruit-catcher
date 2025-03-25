#pragma once

#include <tuple>

#include "lib.h"

class Bucket {
    Texture2D texture;
    Rectangle collision;
    Vector2 position;

    public:
        Bucket();
        ~Bucket();
        const Rectangle GetCollision() const;
        void Update(Vector2 mousePosition);
        void Render() const;
};
