#include "eval.h"
#include "apply.h"
#include "lval.h"
#include "utils.h"
#include "env.h"

#define PROC_ID 3
#define PROC_PARAM_OFFSET 4
#define PROC_PARAM_NUM_OFFSET 7
#define PROC_BODY_OFFSET 2
#define APP_ARG_NUM_OFFSET 3
#define APP_ARG_OFFSET 2
#define APP_OP 1
#define VAR_ID 2
#define VAR_EXPR 3

static lval *eval(mpc_ast_t *ast, environment *env);
static lval *eval_number(mpc_ast_t *ast, environment *env);
static lval *eval_symbol(mpc_ast_t *ast, environment *env);
static lval *eval_define_var(mpc_ast_t *ast, environment *env);
static lval *eval_define_proc(mpc_ast_t *ast, environment *env);
static lval *eval_application(mpc_ast_t *ast, environment *env);

void eval_ast(mpc_ast_t *ast) {
    environment *global = environment_init(NULL);
    for (int i = 0; i < ast->children_num; i++) {
        lval *val = eval(ast->children[i], global);
        if (val != NULL) {
            lval_print(val);
        }
    }

    environment_delete(global);
}

/**
 * Evaluates an abstract syntax tree node based on its tag.
 */
static lval *eval(mpc_ast_t *ast, environment *env) {
    char *tag = ast->tag;
    if (strstr(tag, "number")) {
        return eval_number(ast, env);
    } else if (strstr(tag, "symbol")) {
        return eval_symbol(ast, env);
    } else if (strstr(tag, "define_var")) {
        return eval_define_var(ast, env);
    } else if (strstr(tag, "define_proc")) {
        return eval_define_proc(ast, env);
    } else if (strstr(tag, "application")) {
        return eval_application(ast, env);
    }

    return NULL;
}

/**
 * Evaluates a number. If the parse fails, we throw an error. 
 */
static lval *eval_number(mpc_ast_t *ast, environment *env) {
    long double number;
    int ret = sscanf(ast->contents, "%Le", &number);
    if (ret == 0 || ret == EOF) {
        fprintf(stderr, "Failed to scan number %s\n", ast->contents);
        exit(1);
    }
    return lval_init_number(number);
}

/**
 * Evaluates a symbol by converting it into its corresponding lvalue.
 */
static lval *eval_symbol(mpc_ast_t *ast, environment *env) {
    lval *lv = environment_lookup(env, ast->contents);
    if (lv != NULL) { return lv; }
    return lval_init_symbol(ast->contents);
}

/**
 * Evaluates a definition of a symbol or variable.
 */
static lval *eval_define_var(mpc_ast_t *ast, environment *env) {
    char *key = ast->children[VAR_ID]->contents;
    lval *val = eval(ast->children[VAR_EXPR], env);
    environment_bind(env, key, val);
    return NULL;
}

/**
 * Evaluates a definition of a procedure
 */
static lval *eval_define_proc(mpc_ast_t *ast, environment *env) {
    // Identifier of procedure.
    lval *identifier = eval(ast->children[PROC_ID], env);

    // Create an lval for each parameter.
    size_t num_parameters = ast->children_num - PROC_PARAM_NUM_OFFSET;
    lval **parameters = s_malloc(sizeof(lval *) * num_parameters);
    for (int i = 0; i < num_parameters; i++) {
        parameters[i] = lval_init_symbol(ast->children[i + PROC_PARAM_OFFSET]->contents);
    }

    // Finally retrieve the body.
    mpc_ast_t *body = ast->children[ast->children_num - PROC_BODY_OFFSET];
    lval *procedure = lval_init_procedure(identifier, parameters, body, num_parameters, env);
    environment_bind(env, identifier->data.sym_value, procedure);
    return NULL;
}

/**
 * Evaluates an application. An application is (op ...) followed by operands
 * where ... is.
 */
static lval *eval_application(mpc_ast_t *ast, environment *env) {
    // Evaluate the arguments first.
    size_t num_args = ast->children_num - APP_ARG_NUM_OFFSET;
    lval **evaluated_args = s_malloc(sizeof(lval *) * num_args);
    for (int i = 0; i < num_args; i++) {
        mpc_ast_t *curr = ast->children[i + APP_ARG_OFFSET];
        evaluated_args[i] = eval(curr, env);
    }

    // Evaluate the operator.
    lval *op = eval(ast->children[APP_OP], env);

    // Apply the operator to its arguments. If the lval is NULL, it's not a builtin procedure.
    lval *ret_val = apply(op, evaluated_args, num_args);
    if (!ret_val) {
        // Create a new environment from the procedure.
        char *proc_name = op->data.proc_val.proc_name->data.sym_value;
        lval *proc_def = environment_lookup(env, proc_name);
        if (proc_def == NULL || proc_def->type != LVAL_PROCEDURE) {
            fprintf(stderr, "cannot apply on non-procedure %s\n", proc_name);
            exit(1);
        }

        // Create a new environment by binding the arguments to the parameters.
        environment *new_env = environment_perpend(env, proc_def->data.proc_val.proc_parameters, evaluated_args, proc_def->data.proc_val.proc_num_parameters);
        ret_val = eval(proc_def->data.proc_val.proc_body, new_env);
    }
    return ret_val;
}