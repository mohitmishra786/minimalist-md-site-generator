#include "utils.h"
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#ifdef _WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif

bool create_directory(const char *path) {
    struct stat st = {0};
    if (stat(path, &st) == -1) {
#ifdef _WIN32
        return _mkdir(path) == 0;
#else
        return mkdir(path, 0755) == 0;
#endif
    }
    return true;
}

bool is_markdown_file(const char *filename) {
    const char *ext = strrchr(filename, '.');
    return ext && (strcmp(ext, ".md") == 0 || strcmp(ext, ".markdown") == 0);
}

char *get_filename_without_ext(const char *filename) {
    char *result = strdup(filename);
    char *dot = strrchr(result, '.');
    if (dot) *dot = '\0';
    return result;
}

void log_error(const char *message) {
    fprintf(stderr, "Error: %s\n", message);
}

void log_info(const char *message) {
    printf("Info: %s\n", message);
}

char *read_file_contents(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        log_error("Failed to open file");
        return NULL;
    }

    // Get file size
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // Allocate buffer
    char *buffer = malloc(size + 1);
    if (!buffer) {
        log_error("Failed to allocate memory");
        fclose(fp);
        return NULL;
    }

    // Read file contents
    size_t read = fread(buffer, 1, size, fp);
    buffer[read] = '\0';
    fclose(fp);

    return buffer;
}