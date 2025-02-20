#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

typedef struct {
    size_t capacity;
    size_t size;
    uint8_t *data;
} Arena;

Arena arena_init(size_t);

void *arena_alloc(Arena*, size_t);
void *arena_reset(Arena*);
void arena_free(Arena*);
