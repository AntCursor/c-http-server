#include "con_handler.h"
#include "connection.h"
#include "errors.h"
#include "vector.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

ErrCode
receive_bytes(size_t max_bytes, ConHandler* handle)
{
  char   msgBuffer[DEFAULT_MSG_BUFFER_SIZE];
  size_t bytesRead      = 0;
  size_t totalBytesRead = 0;

  // acount for null termination
  --max_bytes;

  do {
    bytesRead = read(handle->socket.fd, msgBuffer, DEFAULT_MSG_BUFFER_SIZE);

    if (bytesRead == 0)
      continue;

    if (bytesRead + totalBytesRead < max_bytes) {
      if (vector_vpush(msgBuffer, bytesRead, handle->last_data))
        return ERR_MEMORY_ALLOC;

    } else {
      if (vector_vpush(
            msgBuffer, max_bytes - totalBytesRead, handle->last_data))
        return ERR_MEMORY_ALLOC;
    }

    totalBytesRead += bytesRead;
  } while (bytesRead == DEFAULT_MSG_BUFFER_SIZE);

  if (vector_push('\0', handle->last_data))
    return ERR_MEMORY_ALLOC;

  return ERR_SUCCESS;
}

ConHandler*
initConHandler()
{
  ConHandler* handler = (ConHandler*)malloc(sizeof(ConHandler));
  if (!handler)
    return NULL;
  memset(handler, 0, sizeof(ConHandler));

  handler->last_data = vector_init(DEFAULT_MSG_BUFFER_SIZE);

  return handler;
}

void
freeConHandler(ConHandler** handle)
{
  if (*handle) {
    closeSocket(&(*handle)->socket);
    vector_free(&(*handle)->last_data);
    free(*handle);
    *handle = NULL;
  }
}
