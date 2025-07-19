#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "addrconv.h"
#include "con_handler.h"
#include "config.h"
#include "connection.h"
#include "errors.h"
#include "vector.h"

ErrCode exit_code = ERR_SUCCESS;

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

  CharVec* message = NULL;

  message = vector_init(DEFAULT_MSG_BUFFER_SIZE);
  CHECK_ERROR(!message,
              "Error allocating memory for message buffer.",
              cleanup_and_exit,
              exit_code,
              ERR_FAILURE);

  SocketIPv4 listen_socket;
  CHECK_ERROR(initListenSocket(addr, port, DEFAULT_BACKLOG, &listen_socket),
              "Error initializing listen socket.",
              cleanup_and_exit,
              exit_code,
              ERR_FAILURE);

  printf("Listening for connections.\n");

  SocketIPv4 connection;
  CHECK_ERROR(acceptConnection(&listen_socket, &connection),
              "Error accepting connection.",
              cleanup_and_exit,
              exit_code,
              ERR_FAILURE);

  printf("Accepted connection from: ");
  fprintAddrPort(
    stdout, connection.addr.sin_addr.s_addr, connection.addr.sin_port);
  putchar('\n');

  CHECK_ERROR(receive_bytes(connection.fd, MAX_REQUEST_SIZE, message),
              "Error receiving message.",
              cleanup_and_exit,
              exit_code,
              ERR_FAILURE)

  printf("%s", message->data);

cleanup_and_exit:
  closeSocket(&connection);

  closeSocket(&listen_socket);

  vector_free(message);

  return exit_code;
}
