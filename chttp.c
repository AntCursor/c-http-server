#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "addrconv.h"
#include "config.h"
#include "connection.h"
#include "errors.h"
#include "vector.h"

int exit_code = EXIT_SUCCESS;

int
main(int argc, char** argv)
{
  uint32_t addr = DEFAULT_LOCAL_HOST;
  uint16_t port = DEFAULT_PORT;

  if (argc == 2) {
    if (getAddrPort(argv[1], &addr, &port)) {
      fprintf(stderr, "Warning: invalid address/port, using default values.\n");
      addr = DEFAULT_LOCAL_HOST;
      port = DEFAULT_PORT;
    }
  }

  SocketIPv4* listen_socket = NULL;
  SocketIPv4* connection    = NULL;
  CharVec*    message       = NULL;

  listen_socket = malloc(sizeof(SocketIPv4));
  CHECK_ERROR(!listen_socket,
              "Error allocating memory for listen socket.",
              cleanup_and_exit,
              exit_code,
              EXIT_FAILURE);

  connection = malloc(sizeof(SocketIPv4));
  CHECK_ERROR(!connection,
              "Error allocating memory for connection socket.",
              cleanup_and_exit,
              exit_code,
              EXIT_FAILURE);

  message = vector_init(DEFAULT_MSG_BUFFER_SIZE);
  CHECK_ERROR(!message,
              "Error allocating memory for message buffer.",
              cleanup_and_exit,
              exit_code,
              EXIT_FAILURE);

  CHECK_ERROR(initListenSocket(addr, port, DEFAULT_BACKLOG, listen_socket),
              "Error initializing listen socket.",
              cleanup_and_exit,
              exit_code,
              EXIT_FAILURE);

  printf("Listening for connections.\n");

  CHECK_ERROR(acceptConnection(listen_socket, connection),
              "Error accepting connection.",
              cleanup_and_exit,
              exit_code,
              EXIT_FAILURE);

  printf("Accepted connection from: ");
  fprintAddrPort(
    stdout, connection->addr.sin_addr.s_addr, connection->addr.sin_port);
  putchar('\n');

  char   msgBuffer[DEFAULT_MSG_BUFFER_SIZE];
  size_t bytesRead;

  do {
    bytesRead = read(connection->fd, msgBuffer, DEFAULT_MSG_BUFFER_SIZE);
    if (bytesRead > 0) {
      CHECK_ERROR(vector_vpush(msgBuffer, bytesRead, message),
                  "Error expanding vector.",
                  cleanup_and_exit,
                  exit_code,
                  EXIT_FAILURE);
    }
  } while (bytesRead == DEFAULT_MSG_BUFFER_SIZE);

  CHECK_ERROR(vector_push('\0', message),
              "Error adding null terminator to message.",
              cleanup_and_exit,
              exit_code,
              EXIT_FAILURE);

  printf("%s", message->data);

cleanup_and_exit:
  closeSocket(connection);
  free(connection);

  closeSocket(listen_socket);
  free(listen_socket);

  vector_free(message);

  return exit_code;
}
