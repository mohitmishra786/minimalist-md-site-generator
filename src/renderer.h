#ifndef RENDERER_H
#define RENDERER_H
#include "../include/markdown_gen.h"
#include <stdbool.h>

typedef enum {
    CONTENT_TYPE_TEXT,
    CONTENT_TYPE_CODE,
    CONTENT_TYPE_TABLE,
    CONTENT_TYPE_FLOWCHART,
    CONTENT_TYPE_IMAGE
} ContentType;

// Function declarations
char *render_html(PageData *page_data, const char *template_path);
int write_output(const char *filename, const char *html_content);
char *load_template(const char *template_path);
char *process_special_blocks(const char *content);
char *process_mermaid_diagrams(const char *content);
char *process_code_blocks(const char *content);
char *process_tables(const char *content);
bool copy_assets(const char *src_dir, const char *dest_dir);

#endif // RENDERER_H