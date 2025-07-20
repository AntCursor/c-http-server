#ifndef ERROR_CODES_ENUM_ANT_CHTTPSERVER_H
#define ERROR_CODES_ENUM_ANT_CHTTPSERVER_H

#define LOG_ERROR(msg, ...)                                                    \
  fprintf(stderr, "[ERROR] %s:%d: " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_WARNING(msg, ...)                                                  \
  fprintf(stderr, "[WARNING] %s:%d: " msg "\n", __FILE__, __LINE__,            \
          ##__VA_ARGS__)

#define CHECK_ERROR(condition, msg, cleanup_label, var, code)                  \
  if (condition) {                                                             \
    LOG_ERROR(msg);                                                            \
    var = code;                                                                \
    goto cleanup_label;                                                        \
  }

typedef enum {
  ERR_SUCCESS = 0,
  ERR_FAILURE = 1,
  ERR_INVALID_ARGS,
  ERR_MEMORY_ALLOC,
  ERR_BAD_FD,
  ERR_CREATE_SOCK,
  ERR_BIND_SOCK,
  ERR_MARK_LISTEN,
  ERR_ACPT_CON,
} ErrCode;

#endif // !ERROR_CODES_ENUM_ANT_CHTTPSERVER_H
