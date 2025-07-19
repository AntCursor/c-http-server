#include "vector.h"

#include <stdlib.h>
#include <string.h>

CharVec*
vector_init(size_t initial_capacity)
{
  CharVec* v = (CharVec*)malloc(sizeof(CharVec));
  if (v == NULL)
    return NULL;

  v->data = (char*)malloc(sizeof(char) * initial_capacity);
  if (v->data == NULL) {
    free(v);
    return NULL;
  };

  v->size     = 0;
  v->capacity = initial_capacity;

  return v;
}

void
vector_free(CharVec** v)
{
  if (*v) {
    free((*v)->data);
    free(*v);
    *v = NULL;
  }
}

VecErr
vector_resize(size_t new_capacity, CharVec* v)
{
  char* new_place = (char*)realloc(v->data, new_capacity);
  if (!new_place) {
    return EMEMORY_ALLOC;
  }

  v->data     = new_place;
  v->capacity = new_capacity;
  return ESUCCESS;
}

VecErr
vector_push(char item, CharVec* v)
{
  if (v->size == v->capacity) {
    if (vector_resize(v->capacity << 1, v))
      return EMEMORY_ALLOC;
  }
  v->data[v->size] = item;
  ++v->size;
  return ESUCCESS;
}

char
vector_pop(CharVec* v)
{
  if (v->size == 0)
    return '\0';
  char item = v->data[v->size - 1];
  --v->size;
  return item;
}

VecErr
vector_vpush(const char arr[], size_t n, CharVec* v)
{
  if ((v->capacity - v->size) < n) {
    // Increase capacity to the next power of 2
    size_t new_capacity = v->capacity << 1;
    for (; new_capacity < v->size + n;) {
      new_capacity <<= 1;
    }
    if (vector_resize(new_capacity, v))
      return EMEMORY_ALLOC;
  }

  memcpy(v->data + v->size, arr, n);
  v->size += n;

  return ESUCCESS;
}
