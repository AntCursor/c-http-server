#include "connection.h"

#include <netinet/in.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "errors.h"

ErrCode createSocket(uint32_t addr, uint16_t port, SocketIPv4 *socket_s) {
  memset(socket_s, 0, sizeof(SocketIPv4));
  /* socket() creates an endpoint for communication and returns a file
       descriptor that refers to that endpoint. */
  socket_s->fd =
      socket(AF_INET,     // domain: IPv4 internet protocols.
             SOCK_STREAM, // type: Provides sequenced, reliable, two-way,
                          // connection-based byte streams.
             0 // protocol: TCP, the default for this type and domain.
      );
  if (socket_s->fd < 0) {
    return ERR_CREATE_SOCK;
  }

  /*
  struct sockaddr_in {


    sa_family_t    sin_family; // address family: AF_INET


    in_port_t      sin_port;   // port in network byte order


    struct in_addr sin_addr;   // internet address
  };
// Internet address
  struct in_addr {
    uint32_t       s_addr;     //address in network byte order
  };
  */
  struct in_addr s_addr;
  s_addr.s_addr = htonl(addr); // htonl() converts from host byte order to
                               // network byte order(big endian)

  socket_s->addr.sin_family = AF_INET;
  socket_s->addr.sin_port = htons(port);
  socket_s->addr.sin_addr = s_addr;
  socket_s->addr_len = sizeof(socket_s->addr);
  return ERR_SUCCESS;
}
ErrCode bindSocket(const SocketIPv4 *socket_s) {
  // int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
  // binds a socket created with socket() to an address/port pair.
  if (bind(socket_s->fd, (struct sockaddr *)&socket_s->addr,
           socket_s->addr_len) < 0) {
    return ERR_BIND_SOCK;
  }
  return ERR_SUCCESS;
}
ErrCode listenSocket(const SocketIPv4 *socket_s, size_t queue_size) {
  // int listen(int sockfd, int backlog);
  /*listen() marks the socket referred to by sockfd as a passive socket,
       that is, as a socket that will be used to accept incoming connection
       requests using accept(2).*/
  if (listen(socket_s->fd, queue_size) < 0) {
    return ERR_MARK_LISTEN;
  }
  return ERR_SUCCESS;
}
ErrCode acceptConnection(const SocketIPv4 *listen_s, SocketIPv4 *connection_s) {
  memset(connection_s, 0, sizeof(SocketIPv4));
  connection_s->addr_len = sizeof(connection_s->addr);

  // int accept(int sockfd, struct sockaddr *_Nullable restrict addr, socklen_t
  // *_Nullable restrict addrlen);
  /*It extracts the first connection
       request on the queue of pending connections for the listening socket,
       sockfd, creates a new connected socket, and returns a new file
       descriptor referring to that socket.*/
  connection_s->fd =
      accept(listen_s->fd, (struct sockaddr *)&connection_s->addr,
             &connection_s->addr_len);
  if (connection_s->fd < 0) {
    return ERR_ACPT_CON;
  }
  return ERR_SUCCESS;
}
ErrCode initListenSocket(uint32_t addr, uint16_t port, size_t queue_size,
                         SocketIPv4 *socket) {
  if (createSocket(addr, port, socket) != ERR_SUCCESS)
    return ERR_CREATE_SOCK;
  if (bindSocket(socket) != ERR_SUCCESS)
    return ERR_BIND_SOCK;
  if (listenSocket(socket, queue_size) != ERR_SUCCESS)
    return ERR_MARK_LISTEN;

  return ERR_SUCCESS;
}

void destroySocket(SocketIPv4 *s) {
  if (s) {
    close(s->fd);
    free(s);
    s = NULL;
  }
}
