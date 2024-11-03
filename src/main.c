#include "../include/markdown_gen.h"
#include "parser.h"
#include "renderer.h"
#include "utils.h"
#include <dirent.h>
#include <getopt.h>

void print_usage() {
    printf("Usage: markdown-site-gen [OPTIONS]\n");
    printf("Options:\n");
    printf("  -i, --input DIR      Input directory (default: ./)\n");
    printf("  -o, --output DIR     Output directory (default: ./output)\n");
    printf("  -t, --template DIR   Template directory (default: ./templates)\n");
    printf("  -w, --watch         Watch mode for file changes\n");
    printf("  -v, --verbose       Verbose output\n");
    printf("  -h, --help          Show this help message\n");
}

int process_arguments(int argc, char *argv[], Config *config) {
    static struct option long_options[] = {
        {"input", required_argument, 0, 'i'},
        {"output", required_argument, 0, 'o'},
        {"template", required_argument, 0, 't'},
        {"watch", no_argument, 0, 'w'},
        {"verbose", no_argument, 0, 'v'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    int option_index = 0;
    int c;

    while ((c = getopt_long(argc, argv, "i:o:t:wvh", long_options, &option_index)) != -1) {
        switch (c) {
            case 'i':
                strncpy(config->input_dir, optarg, sizeof(config->input_dir) - 1);
                break;
            case 'o':
                strncpy(config->output_dir, optarg, sizeof(config->output_dir) - 1);
                break;
            case 't':
                strncpy(config->template_dir, optarg, sizeof(config->template_dir) - 1);
                break;
            case 'w':
                config->watch_mode = true;
                break;
            case 'v':
                config->verbose = true;
                break;
            case 'h':
                print_usage();
                return 0;
            case '?':
                return 1;
        }
    }

    return -1;
}

int main(int argc, char *argv[]) {
    Config config;
    init_config(&config);

    int result = process_arguments(argc, argv, &config);
    if (result >= 0) return result;

    if (!create_directory(config.output_dir)) {
        log_error("Failed to create output directory");
        return 1;
    }

    return generate_site(&config);
}