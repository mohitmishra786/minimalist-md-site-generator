#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

// Function declarations
bool create_directory(const char *path);
bool is_markdown_file(const char *filename);
char *get_filename_without_ext(const char *filename);
void log_error(const char *message);
void log_info(const char *message);
char *read_file_contents(const char *filename);

#endif // UTILS_H