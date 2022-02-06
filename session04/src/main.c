#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

static void usage(void);

/**
 * This project defines pScheme: Pico Scheme.
 * 
 * Read from a file via terminal args.
 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
        usage();
        return 1;
    } else {
        // Load in the program and pass it off to the parser.
        ps_parser_init(argv[1]);
        ps_parser_cleanup();
        return 0;
    }
}

static void usage(void) {
    fprintf(stderr, "Usage: bin/picoscheme program_name.pscm\n");
}