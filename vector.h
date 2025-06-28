#ifndef VECTOR_STRUCT_ANT_H
#define VECTOR_STRUCT_ANT_H
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char *data;
  size_t size;
  size_t capacity;
} CharVec;

typedef uint32_t VecErr;

CharVec *vector_init(size_t initial_capacity);
void vector_free(CharVec *v);
VecErr vector_resize(size_t new_capacity, CharVec *v);
VecErr vector_push(char item, CharVec *v);
char vector_pop(CharVec *v);
VecErr vector_vpush(const char arr[], size_t n, CharVec *v);
#endif // !VECTOR_STRUCT_ANT_H
