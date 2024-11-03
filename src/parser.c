#include "parser.h"
#include "utils.h"
#include <cmark.h>
#include <string.h>
#include <stdlib.h>

void cleanup_page_data(PageData *page) {
    if (page) {
        if (page->content) {
            free(page->content);
        }
        if (page->html_content) {
            free(page->html_content);
        }
        free(page);
    }
}

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

    // Store original content
    page->content = content;

    // Parse metadata from content
    parse_metadata(page, content);

    // Find the actual markdown content after metadata
    const char *markdown_start = content;
    if (strncmp(content, "---\n", 4) == 0) {
        const char *end = strstr(content + 4, "\n---\n");
        if (end) {
            markdown_start = end + 5;  // Skip past the front matter
        }
    }

    // Convert markdown to HTML using cmark
    cmark_node *doc = cmark_parse_document(markdown_start, strlen(markdown_start), CMARK_OPT_DEFAULT);
    page->html_content = cmark_render_html(doc, CMARK_OPT_DEFAULT);
    cmark_node_free(doc);

    return page;
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
                char *saveptr;
                char *metadata_copy = strdup(metadata);
                char *line = strtok_r(metadata_copy, "\n", &saveptr);
                
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
                    line = strtok_r(NULL, "\n", &saveptr);
                }
                free(metadata_copy);
            }
        }
    }
}