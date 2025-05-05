#include "arena.hpp"

Arena arena_init(size_t capacity) {
    void *data = malloc(sizeof(uint8_t) *capacity);
    Arena arena {
        .capacity = capacity,
        .size = 0,
        .data = (uint8_t*) data,
    };
    return arena;
}

void *arena_alloc(Arena *arena, size_t size) {
    assert(arena->size + size < arena->capacity);

    uint8_t *data = &arena->data[arena->size];
    arena->size += size;
    return data;
}

void *arena_reset(Arena *arena) {
    arena->size = 0;
    return arena;
}

void arena_free(Arena *arena) {
    arena->capacity = 0;
    arena->size = 0;
    free(arena->data);
}