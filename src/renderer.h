#ifndef RENDERER_H
#define RENDERER_H

#include "../include/markdown_gen.h"

// Function declarations
char *render_html(PageData *page_data, const char *template_path);
int write_output(const char *filename, const char *html_content);
char *load_template(const char *template_path);

#endif // RENDERER_H