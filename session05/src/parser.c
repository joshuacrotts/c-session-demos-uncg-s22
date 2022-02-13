#include "parser.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "eval.h"
#include "mpc.h"

/* Function prototypes. */
static void ps_parser_initialize_rules(void);
static void ps_parser_readfile(const char *file);

/* Global/static parser rules. */
static mpc_parser_t *number_rule;
static mpc_parser_t *expr_rule;
static mpc_parser_t *symbol_rule;
static mpc_parser_t *application_rule;
static mpc_parser_t *define_var_rule;
static mpc_parser_t *define_proc_rule;
static mpc_parser_t *program_rule;
static mpc_parser_t *picoscheme_rule;

/**
 * Initializes the MPC parser rules and reads in from the file parameter.
 *  
 * @param const char * filename.
 *
 * @return void.
 */
void ps_parser_execute(const char *file) {
    ps_parser_initialize_rules();
    ps_parser_readfile(file);
}

/**
 * Removes/deallocates the MPC parser rules.
 * 
 * @param void.
 *
 * @return void.
 */
void ps_parser_cleanup(void) {
    mpc_cleanup(8, number_rule, symbol_rule, expr_rule, application_rule, define_var_rule, define_proc_rule, program_rule, picoscheme_rule);
}

/**
 * Opens a file and parses its contents. After this, it is handed to the semantic 
 * analyzer. If an error occurs, it is printed.
 * 
 * @param const char *filename.
 *
 * @return void.
 */
static void ps_parser_readfile(const char *file) {
    FILE *fp;
    if ((fp = fopen(file, "r")) == NULL) {
        fprintf(stderr, "ERR cannot open file %s\n", file);
        exit(EXIT_FAILURE);
    }

    mpc_result_t res;
    if (mpc_parse_file(file, fp, picoscheme_rule, &res)) {
        eval_ast(res.output);
        mpc_ast_delete(res.output);
    } else {
        mpc_err_print(res.error);
        mpc_err_delete(res.error);
    }

    fclose(fp);
}

/**
 * Initializes all static parser rules and creates the ATL grammar.
 * 
 * @param void.
 *
 * @return void.
 */
static void ps_parser_initialize_rules(void) {
    /* Define the parser rules. */
    number_rule = mpc_new("number");
    symbol_rule = mpc_new("symbol");
    expr_rule = mpc_new("expr");
    application_rule = mpc_new("application");
    define_var_rule = mpc_new("define_var");
    define_proc_rule = mpc_new("define_proc");
    program_rule = mpc_new("program");
    picoscheme_rule = mpc_new("picoscheme");

    /* Now define the grammar itself. */
    mpc_err_t *error = mpca_lang(MPCA_LANG_DEFAULT,
                                 "    symbol            : /[a-zA-Z_+\\-*\\/\\\\=<>!&]+[a-zA-Z0-9_+\\-*\\/\\\\=<>!&]*/   ; \n"  // An l-value is just a variable.
                                 "    number            : /[+-]?([0-9]*[.])?[0-9]+/                                     ; \n"                // Floating point number.
                                 "    define_var        : '(' \"define\" <symbol> <expr> ')'                            ; \n"
                                 "    define_proc       : '(' \"define\" '(' <symbol> <expr>* ')' <expr> ')'            ; \n"    
                                 "    application       : '('<symbol> <expr>+')'                                        ; \n"
                                 "    expr              : <application>                                                   \n"
                                 "                          | <number>                                                    \n"
                                 "                          | <symbol>                                                  ; \n"
                                 "    program           : (<define_var> | <define_proc> | <expr>)                                           ; \n"
                                 "    picoscheme        : /^/ <program>* /$/                                            ; \n"  // Root rule.
                                 ,
                                 number_rule, symbol_rule, application_rule, define_var_rule, define_proc_rule, expr_rule, program_rule, picoscheme_rule, NULL);

    // If we find an error, print it here and just bail out.
    if (error != NULL) {
        mpc_err_print(error);
        exit(EXIT_FAILURE);
    }
}