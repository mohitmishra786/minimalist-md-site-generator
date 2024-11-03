#include "../include/markdown_gen.h"
#include "utils.h"
#include "parser.h"
#include "renderer.h"
#include <dirent.h>

static int process_markdown_file(const char *input_path, const char *output_path, const char *template_path) {
    PageData *page = parse_markdown_file(input_path);
    if (!page) {
        log_error("Failed to parse markdown file");
        return 0;
    }

    // Create output path
    char *filename = get_filename_without_ext(strrchr(input_path, '/') ? strrchr(input_path, '/') + 1 : input_path);
    char output_file[512];
    snprintf(output_file, sizeof(output_file), "%s/%s.html", output_path, filename);
    free(filename);

    // Render HTML
    char template_file[512];
    snprintf(template_file, sizeof(template_file), "%s/default.html", template_path);
    
    char *html = render_html(page, template_file);
    if (!html) {
        cleanup_page_data(page);
        return 0;
    }

    // Write output
    int success = write_output(output_file, html);
    free(html);
    cleanup_page_data(page);

    return success;
}

void init_config(Config *config) {
    strncpy(config->input_dir, "./", sizeof(config->input_dir) - 1);
    strncpy(config->output_dir, "./output", sizeof(config->output_dir) - 1);
    strncpy(config->template_dir, "./templates", sizeof(config->template_dir) - 1);
    config->watch_mode = false;
    config->verbose = false;
}

int generate_site(const Config *config) {
    if (config->verbose) {
        log_info("Generating site...");
        log_info("Input directory: ");
        log_info(config->input_dir);
        log_info("Output directory: ");
        log_info(config->output_dir);
        log_info("Template directory: ");
        log_info(config->template_dir);
    }

    DIR *dir = opendir(config->input_dir);
    if (!dir) {
        log_error("Failed to open input directory");
        return 1;
    }

    // Ensure output directory exists
    if (!create_directory(config->output_dir)) {
        log_error("Failed to create output directory");
        closedir(dir);
        return 1;
    }

    struct dirent *entry;
    int success = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG && is_markdown_file(entry->d_name)) {
            char input_path[512];
            snprintf(input_path, sizeof(input_path), "%s/%s", config->input_dir, entry->d_name);
            
            if (config->verbose) {
                log_info("Processing file: ");
                log_info(input_path);
            }

            if (!process_markdown_file(input_path, config->output_dir, config->template_dir)) {
                log_error("Failed to process markdown file: ");
                log_error(input_path);
                success = 1;
            }
        }
    }

    closedir(dir);
    return success;
}