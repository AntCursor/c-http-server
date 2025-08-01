#ifndef VECTOR_STRUCT_ANT_H
#define VECTOR_STRUCT_ANT_H
#include <stddef.h>
#include <stdint.h>

typedef struct {
  char *data;
  size_t size;
  size_t capacity;
} CharVec;

typedef enum {
  ESUCCESS = 0,
  EMEMORY_ALLOC,
} VecErr;

CharVec *vector_init(size_t initial_capacity);
void vector_free(CharVec **v);

VecErr vector_resize(size_t new_capacity, CharVec *v);

VecErr vector_push(char item, CharVec *v);
VecErr vector_vpush(const char arr[], size_t n, CharVec *v);
char vector_pop(CharVec *v);
#endif // !VECTOR_STRUCT_ANT_H
