#ifndef MESSAGE_CHTTP_SERVER_ANT_H
#define MESSAGE_CHTTP_SERVER_ANT_H

#include "errors.h"
#include "vector.h"
#include <stddef.h>

#define DEFAULT_MSG_BUFFER_SIZE 512

ErrCode receive_bytes(int fd, size_t max_bytes, CharVec *byte_vec);

#endif // !MESSAGE_CHTTP_SERVER_ANT_H
