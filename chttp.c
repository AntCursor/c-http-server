#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "addrconv.h"
#include "connection.h"
#include "vector.h"

#define LOCAL_HOST 0x7f000001 // 127.0.0.1
// #define LOCAL_HOST 0xc0a80108 // 192.168.1.8
#define BACKLOG 20 // size of connection queue
#define DEFAULT_PORT (uint16_t)8080
#define MSG_BUFFER_SIZE 256

#define MAX_SIZE_ADDR 15
#define MAX_SIZE_PORT 5
#define MAX_SIZE_ADDRPORT                                                      \
  MAX_SIZE_ADDR + MAX_SIZE_PORT +                                              \
      2 // Ex: "192.168.150.255:65535\0" -> 22 characters long.

int main(int argc, char **argv) {
  uint32_t addr = LOCAL_HOST;
  uint16_t port = DEFAULT_PORT;

  if (argc == 2) {
    size_t argumentLenght = strlen(argv[1]);
    bool error = false;

    // counting the ':'
    if (argumentLenght > MAX_SIZE_ADDR + 1)
      error = strToAddrPort(argv[1], &addr, &port);
    else if (argumentLenght > MAX_SIZE_PORT)
      error = strToAddr(argv[1], &addr);
    else
      port = atoi(argv[1]);

    if (error) {
      fprintf(stderr, "Warning: invalid address/port, using default values.\n");
      addr = LOCAL_HOST;
      port = DEFAULT_PORT;
    }
  }

  SocketIPv4 *listen_socket = malloc(sizeof(SocketIPv4));
  initListenSocket(addr, port, BACKLOG, listen_socket);

  if (!listen_socket) {
    fprintf(stderr, "Error creating listening socket.");
    return EXIT_FAILURE;
  }

  printf("Listening for connections.\n");

  SocketIPv4 *connection = malloc(sizeof(SocketIPv4));
  if (acceptConnection(listen_socket, connection) == EXIT_FAILURE) {
    fprintf(stderr, "Error accepting connection.");
    return EXIT_FAILURE;
  }

  char addrPortString[MAX_SIZE_ADDRPORT];
  addrPortToStr(connection->addr.sin_addr.s_addr, connection->addr.sin_port,
                addrPortString, MAX_SIZE_ADDRPORT);

  printf("Accepted connection from: %s\n\n", addrPortString);

  char msgBuffer[MSG_BUFFER_SIZE];
  size_t bytesRead;

  CharVec *message = vector_init(MSG_BUFFER_SIZE);
  do {
    bytesRead = read(connection->fd, msgBuffer, MSG_BUFFER_SIZE);
    if (bytesRead > 0) {
      if (vector_vpush(msgBuffer, bytesRead, message) == EXIT_FAILURE) {
        fprintf(stderr, "Error expanding vector.");
        return EXIT_FAILURE;
      }
    }
  } while (bytesRead == MSG_BUFFER_SIZE);

  printf("%s", message->data);
  destroySocket(connection);
  vector_free(message);
  destroySocket(listen_socket);

  return EXIT_SUCCESS;
}
