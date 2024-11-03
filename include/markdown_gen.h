#ifndef MARKDOWN_GEN_H
#define MARKDOWN_GEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Configuration structure
typedef struct {
    char input_dir[256];
    char output_dir[256];
    char template_dir[256];
    bool watch_mode;
    bool verbose;
} Config;

// Page metadata structure
typedef struct {
    char title[256];
    char date[64];
    char author[128];
    char *content;
    char *html_content;
} PageData;

// Function declarations
void init_config(Config *config);
int generate_site(const Config *config);
void cleanup_page_data(PageData *page_data);

#endif // MARKDOWN_GEN_H