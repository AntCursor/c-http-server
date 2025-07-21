#include <stdbool.h>
#include <stdint.h>

#include "addrconv.h"
#include "con_handler.h"
#include "config.h"
#include "connection.h"
#include "errors.h"

ErrCode exit_code = ERR_SUCCESS;

int
main(int argc, char** argv)
{
  uint32_t addr = DEFAULT_LOCAL_HOST;
  uint16_t port = DEFAULT_PORT;

  if (argc == 2) {
    if (getAddrPort(argv[1], &addr, &port)) {
      LOG_WARNING("Invalid addr/port pair, using default values.");
      addr = DEFAULT_LOCAL_HOST;
      port = DEFAULT_PORT;
    }
  }

  SocketIPv4 listen_socket;
  CHECK_ERROR(initListenSocket(addr, port, DEFAULT_BACKLOG, &listen_socket),
              "Error initializing listen socket.",
              cleanup_and_exit,
              exit_code,
              ERR_FAILURE);

  LOG_INFO("listening for connections.");

  ConHandler* con_handle = NULL;
  con_handle             = initConHandler();
  CHECK_ERROR(!con_handle,
              "Error allocating memory for message buffer.",
              cleanup_and_exit,
              exit_code,
              ERR_FAILURE);

  CHECK_ERROR(acceptConnection(&listen_socket, &con_handle->socket),
              "Error accepting connection.",
              cleanup_and_exit,
              exit_code,
              ERR_FAILURE);

  LOG_INFO("accepted connection from: ");
  printAddrPortln(stderr, &con_handle->socket);

  CHECK_ERROR(receive_bytes(MAX_REQUEST_SIZE, con_handle),
              "Error receiving message.",
              cleanup_and_exit,
              exit_code,
              ERR_FAILURE)

  LOG_INFO("\n%s", con_handle->last_data->data);

cleanup_and_exit:
  closeSocket(&listen_socket);

  freeConHandler(&con_handle);

  return exit_code;
}
