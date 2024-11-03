#include "../include/markdown_gen.h"
#include "utils.h"

void init_config(Config *config) {
    // Set default values
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
    
    // TODO: Add site generation logic
    return 0;
}