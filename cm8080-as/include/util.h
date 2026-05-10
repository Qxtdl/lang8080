#pragma once

#include <stddef.h>

// Strong malloc
void *smalloc(size_t size);
// Strong realloc
void *srealloc(void *ptr, size_t size);
// Strong calloc
void *scalloc(size_t nmemb, size_t size);
/**
 * @brief Read a text file
 * 
 * @param filename Path to the text file
 * @return char* Heap string
 */
char *read_file(const char *filename);
