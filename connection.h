#ifndef CONNECTION_CHTTP_SERVER_ANT_H
#define CONNECTION_CHTTP_SERVER_ANT_H
#include "errors.h"
#include <linux/in.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

typedef struct {
  uint32_t fd;
  struct sockaddr_in addr;
  socklen_t addr_len;
} SocketIPv4;

ErrCode createSocket(uint32_t addr, uint16_t port, SocketIPv4 *socket_s);
ErrCode bindSocket(const SocketIPv4 *socket_s);
ErrCode listenSocket(const SocketIPv4 *socket_s, size_t queue_size);
ErrCode acceptConnection(const SocketIPv4 *listen_s, SocketIPv4 *connection_s);
ErrCode initListenSocket(uint32_t addr, uint16_t port, size_t queue_size,
                         SocketIPv4 *socket);
void destroySocket(SocketIPv4 *s);
#endif // CONNECTION_CHTTP_SERVER_ANT_H
