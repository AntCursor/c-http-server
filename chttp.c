#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "addrconv.h"
#include "connection.h"
#include "errors.h"
#include "vector.h"

#define LOCAL_HOST 0x7f000001 // 127.0.0.1
// #define LOCAL_HOST 0xc0a80108 // 192.168.1.8
#define BACKLOG 20 // size of connection queue
#define DEFAULT_PORT (uint16_t)8080
#define MSG_BUFFER_SIZE 256

int main(int argc, char **argv) {
  uint32_t addr = LOCAL_HOST;
  uint16_t port = DEFAULT_PORT;

  if (argc == 2) {
    if (getAddrPort(argv[1], &addr, &port)) {
      fprintf(stderr, "Warning: invalid address/port, using default values.\n");
      addr = LOCAL_HOST;
      port = DEFAULT_PORT;
    }
  }

  SocketIPv4 *listen_socket = malloc(sizeof(SocketIPv4));
  if (!listen_socket) {
    fprintf(stderr, "Error allocating memory for socket.\n");
    return EXIT_FAILURE;
  }
  if (initListenSocket(addr, port, BACKLOG, listen_socket)) {
    fprintf(stderr, "Error creating listening socket.\n");
    return EXIT_FAILURE;
  }

  printf("Listening for connections.\n");

  SocketIPv4 *connection = malloc(sizeof(SocketIPv4));
  if (acceptConnection(listen_socket, connection)) {
    fprintf(stderr, "Error accepting connection.");
    return EXIT_FAILURE;
  }

  printf("Accepted connection from: ");
  fprintAddrPort(stdout, connection->addr.sin_addr.s_addr,
                 connection->addr.sin_port);
  putchar('\n');

  char msgBuffer[MSG_BUFFER_SIZE];
  size_t bytesRead;

  CharVec *message = vector_init(MSG_BUFFER_SIZE);
  do {
    bytesRead = read(connection->fd, msgBuffer, MSG_BUFFER_SIZE);
    if (bytesRead > 0) {
      if (vector_vpush(msgBuffer, bytesRead, message)) {
        fprintf(stderr, "Error expanding vector.");
        return EXIT_FAILURE;
      }
    }
  } while (bytesRead == MSG_BUFFER_SIZE);
  if (vector_push('\0', message)) {
    fprintf(stderr, "Error adding null terminator to message.\n");
    return EXIT_FAILURE;
  }

  printf("%s", message->data);
  destroySocket(connection);
  vector_free(message);
  destroySocket(listen_socket);

  return ERR_SUCCESS;
}
