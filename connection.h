#ifndef CONNECTION_CHTTP_SERVER_ANT_H
#define CONNECTION_CHTTP_SERVER_ANT_H
#include <netinet/in.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/socket.h>

#include "errors.h"

typedef struct {
  int fd;
  struct sockaddr_in addr;
  socklen_t addr_len;
} SocketIPv4;

ErrCode acceptConnection(const SocketIPv4 *listen_s, SocketIPv4 *connection_s);
ErrCode initListenSocket(uint32_t addr, uint16_t port, size_t queue_size,
                         SocketIPv4 *socket);
void closeSocket(SocketIPv4 *s);
#endif // CONNECTION_CHTTP_SERVER_ANT_H
