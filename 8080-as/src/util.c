#include <stdlib.h>
#include <string.h>

#include <util.h>
#include <global.h>

// Strong malloc
void *smalloc(size_t size) {
    void *alloc = malloc(size);
    if (!alloc)
        app_error("Failed to allocate memory\n")
    return alloc;
}

// Strong realloc
void *srealloc(void *ptr, size_t size) {
    void *alloc = realloc(ptr, size);
    if (!alloc)
        app_error("Failed to allocate memory\n")
    return alloc;    
}

// Strong calloc
void *scalloc(size_t nmemb, size_t size) {
    void *alloc = calloc(nmemb, size);
    if (!alloc)
        app_error("Failed to allocate memory\n")
    return alloc;
}

char *read_file(const char *filename) {
    int padding = 1024;
    FILE *fptr = fopen(filename, "rb");
    if (!fptr) {
        app_error("Failed to open the file %s\n", filename)
    }
    fseek(fptr, 0, SEEK_END);
    size_t size = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);
    char *bin = smalloc(size + padding);
    fread(bin, 1, size, fptr);
    fclose(fptr);
    bin[size] = '\0';
    memset(bin + size + 1, 0, padding - 1);
    return bin;
}
