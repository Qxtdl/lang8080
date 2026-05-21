#pragma once

#include <stdio.h>
#include <stdlib.h>

#define app_error(...) \
   { \
      fprintf(stderr, "[ERROR]: "__VA_ARGS__); \
      exit(1); \
   }

#define app_warn(...) \
   { \
      fprintf(stderr, "[WARN]: " __VA_ARGS__); \
   }

#define app_log(...) \
   { \
      fprintf(stderr, "[LOG]: " __VA_ARGS__); \
   }
