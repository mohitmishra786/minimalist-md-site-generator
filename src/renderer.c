#include "renderer.h"
#include "utils.h"
#include <ctype.h>

char *render_html(PageData *page_data, const char *template_path) {
    char *template = load_template(template_path);
    if (!template) return NULL;

    // Calculate required buffer size
    size_t total_size = strlen(template) + strlen(page_data->html_content) + 
                       strlen(page_data->title) + strlen(page_data->date) + 
                       strlen(page_data->author) + 1000; // Extra space for formatting

    char *result = malloc(total_size);
    if (!result) {
        free(template);
        return NULL;
    }

    // Replace template placeholders
    snprintf(result, total_size, template,
             page_data->title,
             page_data->date,
             page_data->author,
             page_data->html_content);

    free(template);
    return result;
}

int write_output(const char *filename, const char *html_content) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        log_error("Failed to open output file");
        return 0;
    }

    size_t written = fwrite(html_content, 1, strlen(html_content), fp);
    fclose(fp);

    return written == strlen(html_content);
}

char *load_template(const char *template_path) {
    return read_file_contents(template_path);
}