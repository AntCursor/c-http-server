#ifndef MESSAGE_CHTTP_SERVER_ANT_H
#define MESSAGE_CHTTP_SERVER_ANT_H

#include "connection.h"
#include "errors.h"
#include "vector.h"
#include <stddef.h>

#define DEFAULT_MSG_BUFFER_SIZE 512

typedef struct {
  SocketIPv4 socket;
  CharVec *last_data;
} ConHandler;

ConHandler *initConHandler();
void freeConHandler(ConHandler **handle);

ErrCode receive_bytes(size_t max_bytes, ConHandler *handle);

#endif // !MESSAGE_CHTTP_SERVER_ANT_H
