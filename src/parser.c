#include "parser.h"
#include "utils.h"
#include <cmark.h>

PageData *parse_markdown_file(const char *filename) {
    PageData *page = malloc(sizeof(PageData));
    if (!page) {
        log_error("Memory allocation failed");
        return NULL;
    }

    // Initialize page data
    memset(page, 0, sizeof(PageData));

    char *content = read_file_contents(filename);
    if (!content) {
        free(page);
        return NULL;
    }

    // Parse metadata from content
    parse_metadata(page, content);

    // Convert markdown to HTML using cmark
    cmark_node *doc = cmark_parse_document(content, strlen(content), CMARK_OPT_DEFAULT);
    page->html_content = cmark_render_html(doc, CMARK_OPT_DEFAULT);
    
    cmark_node_free(doc);
    free(content);

    return page;
}

char *read_file_contents(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        log_error("Failed to open file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *content = malloc(length + 1);
    if (!content) {
        fclose(file);
        return NULL;
    }

    fread(content, 1, length, file);
    content[length] = '\0';

    fclose(file);
    return content;
}

void parse_metadata(PageData *page, const char *content) {
    // Simple metadata parser (YAML-like front matter)
    if (strncmp(content, "---\n", 4) == 0) {
        const char *end = strstr(content + 4, "\n---\n");
        if (end) {
            char metadata[1024];
            size_t len = end - (content + 4);
            if (len < sizeof(metadata) - 1) {
                strncpy(metadata, content + 4, len);
                metadata[len] = '\0';
                
                // Parse each line
                char *line = strtok(metadata, "\n");
                while (line) {
                    char key[64], value[256];
                    if (sscanf(line, "%[^:]: %[^\n]", key, value) == 2) {
                        if (strcmp(key, "title") == 0)
                            strncpy(page->title, value, sizeof(page->title) - 1);
                        else if (strcmp(key, "date") == 0)
                            strncpy(page->date, value, sizeof(page->date) - 1);
                        else if (strcmp(key, "author") == 0)
                            strncpy(page->author, value, sizeof(page->author) - 1);
                    }
                    line = strtok(NULL, "\n");
                }
            }
        }
    }
}