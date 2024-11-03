#ifndef PARSER_H
#define PARSER_H

#include "../include/markdown_gen.h"

// Parser states
typedef enum {
    NORMAL,
    HEADER,
    CODE_BLOCK,
    LIST,
    BLOCKQUOTE
} ParserState;

// Function declarations
PageData *parse_markdown_file(const char *filename);
void parse_metadata(PageData *page, const char *content);

#endif // PARSER_H