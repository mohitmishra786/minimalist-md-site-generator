#include "renderer.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <cmark.h>

#define MAX_TEMPLATE_SIZE 16384
#define MAX_PATH 1024

char *process_mermaid_diagrams(const char *content) {
    const char *mermaid_start = "```mermaid";
    const char *code_end = "```";
    char *result = malloc(strlen(content) * 2); // Allocate extra space for HTML
    char *current = result;
    const char *pos = content;

    while ((pos = strstr(pos, mermaid_start)) != NULL) {
        // Copy everything up to the mermaid block
        size_t prefix_len = pos - content;
        strncpy(current, content, prefix_len);
        current += prefix_len;
        
        // Move past the mermaid marker
        pos += strlen(mermaid_start);
        
        // Find the end of the mermaid block
        const char *end = strstr(pos, code_end);
        if (!end) break;

        // Extract mermaid content
        size_t diagram_len = end - pos;
        char *diagram = malloc(diagram_len + 1);
        strncpy(diagram, pos, diagram_len);
        diagram[diagram_len] = '\0';

        // Create mermaid div
        current += sprintf(current, 
            "<div class=\"mermaid\">\n%s\n</div>\n", 
            diagram);

        free(diagram);
        pos = end + strlen(code_end);
        content = pos;
    }

    // Copy remaining content
    strcpy(current, content);
    return result;
}

char *process_code_blocks(const char *content) {
    const char *code_start = "```";
    char *result = malloc(strlen(content) * 2); // Allocate extra space for HTML
    char *current = result;
    const char *pos = content;

    while ((pos = strstr(pos, code_start)) != NULL) {
        // Copy everything up to the code block
        size_t prefix_len = pos - content;
        strncpy(current, content, prefix_len);
        current += prefix_len;
        
        // Move past the code marker
        pos += strlen(code_start);
        
        // Get language (if specified)
        char language[32] = "";
        const char *newline = strchr(pos, '\n');
        if (newline) {
            size_t lang_len = newline - pos;
            if (lang_len < sizeof(language)) {
                strncpy(language, pos, lang_len);
                language[lang_len] = '\0';
                pos = newline + 1;
            }
        }

        // Find the end of the code block
        const char *end = strstr(pos, code_start);
        if (!end) break;

        // Extract code content
        size_t code_len = end - pos;
        char *code = malloc(code_len + 1);
        strncpy(code, pos, code_len);
        code[code_len] = '\0';

        // Create code block with syntax highlighting
        current += sprintf(current, 
            "<pre><code class=\"language-%s\">%s</code></pre>\n", 
            language, code);

        free(code);
        pos = end + strlen(code_start);
        content = pos;
    }

    // Copy remaining content
    strcpy(current, content);
    return result;
}

char *process_tables(const char *content) {
    // Simple table processing - assumes GitHub-style markdown tables
    const char *table_start = "\n|";
    char *result = malloc(strlen(content) * 2);
    char *current = result;
    const char *pos = content;

    while ((pos = strstr(pos, table_start)) != NULL) {
        // Copy everything up to the table
        size_t prefix_len = pos - content;
        strncpy(current, content, prefix_len);
        current += prefix_len;
        
        // Start table HTML
        current += sprintf(current, "<table class=\"markdown-table\">\n<thead>\n<tr>\n");
        
        // Process header row
        pos++; // Skip newline
        char *header = strdup(pos);
        char *header_end = strchr(header, '\n');
        if (header_end) *header_end = '\0';
        
        // Split header cells
        char *cell = strtok(header, "|");
        while (cell) {
            while (isspace(*cell)) cell++;
            current += sprintf(current, "<th>%s</th>\n", cell);
            cell = strtok(NULL, "|");
        }
        free(header);
        
        current += sprintf(current, "</tr>\n</thead>\n<tbody>\n");
        
        // Find alignment row and skip it
        pos = strchr(pos, '\n');
        if (!pos) break;
        pos = strchr(pos + 1, '\n');
        if (!pos) break;
        pos++;

        // Process data rows
        while (*pos == '|') {
            current += sprintf(current, "<tr>\n");
            char *row = strdup(pos);
            char *row_end = strchr(row, '\n');
            if (row_end) *row_end = '\0';
            
            cell = strtok(row, "|");
            while (cell) {
                while (isspace(*cell)) cell++;
                current += sprintf(current, "<td>%s</td>\n", cell);
                cell = strtok(NULL, "|");
            }
            
            current += sprintf(current, "</tr>\n");
            free(row);
            
            pos = strchr(pos, '\n');
            if (!pos) break;
            pos++;
        }
        
        current += sprintf(current, "</tbody>\n</table>\n");
        content = pos;
    }

    // Copy remaining content
    strcpy(current, content);
    return result;
}

char *process_special_blocks(const char *content) {
    char *with_mermaid = process_mermaid_diagrams(content);
    char *with_code = process_code_blocks(with_mermaid);
    char *with_tables = process_tables(with_code);
    
    free(with_mermaid);
    free(with_code);
    return with_tables;
}

char *render_html(PageData *page_data, const char *template_path) {
    // Load template
    char *template = read_file_contents(template_path);
    if (!template) {
        log_error("Failed to load template");
        return NULL;
    }

    // Process special content blocks
    char *processed_content = process_special_blocks(page_data->html_content);
    
    // Calculate required buffer size
    size_t total_size = strlen(template) + 1;
    
    if (strstr(template, "{{content}}"))
        total_size += strlen(processed_content) - strlen("{{content}}");
    if (strstr(template, "{{title}}"))
        total_size += strlen(page_data->title) - strlen("{{title}}");
    if (strstr(template, "{{date}}"))
        total_size += strlen(page_data->date) - strlen("{{date}}");
    if (strstr(template, "{{author}}"))
        total_size += strlen(page_data->author) - strlen("{{author}}");

    // Add CSS and JavaScript for special features
    const char *header_inject = "\n<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.9.0/styles/default.min.css\">\n"
                               "<script src=\"https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.9.0/highlight.min.js\"></script>\n"
                               "<script src=\"https://cdn.jsdelivr.net/npm/mermaid/dist/mermaid.min.js\"></script>\n"
                               "<script>hljs.highlightAll(); mermaid.initialize({startOnLoad:true});</script>\n"
                               "<style>\n"
                               ".markdown-table { border-collapse: collapse; width: 100%; margin: 1em 0; }\n"
                               ".markdown-table th, .markdown-table td { border: 1px solid #ddd; padding: 8px; text-align: left; }\n"
                               ".markdown-table th { background-color: #f5f5f5; }\n"
                               "pre { background-color: #f6f8fa; padding: 16px; border-radius: 6px; overflow: auto; }\n"
                               "code { font-family: 'SFMono-Regular', Consolas, 'Liberation Mono', Menlo, Courier, monospace; }\n"
                               "</style>\n</head>";
    
    const char *head_close_tag = "</head>";
    char *head_pos = strstr(template, head_close_tag);
    if (head_pos) {
        total_size += strlen(header_inject);
    }

    // Allocate result buffer
    char *result = malloc(total_size);
    if (!result) {
        free(template);
        free(processed_content);
        return NULL;
    }

    char *current = result;
    const char *template_ptr = template;

    // Insert CSS and JavaScript before </head>
    if (head_pos) {
        size_t prefix_len = head_pos - template;
        strncpy(current, template, prefix_len);
        current += prefix_len;
        strcpy(current, header_inject);
        current += strlen(header_inject);
        template_ptr = head_pos;
    }

    // Replace template variables
    while (*template_ptr) {
        if (template_ptr[0] == '{' && template_ptr[1] == '{') {
            if (strncmp(template_ptr, "{{content}}", 10) == 0) {
                strcpy(current, processed_content);
                current += strlen(processed_content);
                template_ptr += 10;
            } else if (strncmp(template_ptr, "{{title}}", 8) == 0) {
                strcpy(current, page_data->title);
                current += strlen(page_data->title);
                template_ptr += 8;
            } else if (strncmp(template_ptr, "{{date}}", 8) == 0) {
                strcpy(current, page_data->date);
                current += strlen(page_data->date);
                template_ptr += 8;
            } else if (strncmp(template_ptr, "{{author}}", 10) == 0) {
                strcpy(current, page_data->author);
                current += strlen(page_data->author);
                template_ptr += 10;
            } else {
                *current++ = *template_ptr++;
            }
        } else {
            *current++ = *template_ptr++;
        }
    }
    *current = '\0';

    free(template);
    free(processed_content);
    return result;
}

int write_output(const char *filename, const char *html_content) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        log_error("Failed to open output file for writing");
        return 0;
    }

    size_t written = fwrite(html_content, 1, strlen(html_content), fp);
    fclose(fp);
    return written == strlen(html_content);
}