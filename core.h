/*******************************************************************************
 * name: core.h
 * author: godynezz
 * description: core definitions
 ******************************************************************************/

#ifndef CORE_H
#define CORE_H

#define _GNU_SOURCE

#define VERSION "I lost count"
#define AUTHOR "godynezz"
#define TK_ARR_INIT_SIZE 2
#define TK_MAX_SIZE 12

#define DF_PROMPT "~(8:>"

#include <stdio.h>
#include <sys/types.h>

typedef struct input_buffer {
  char *buffer;
  size_t buffer_size;
  size_t nread;
} input_buffer_t;

typedef struct shell_state {
  FILE *input_stream;
  char *prompt;
  int inputfd;
  input_buffer_t iobuff;
  u_int8_t last_exit_status;
} shell_state_t;

typedef struct builtin_cmd {
  char *name;
  void (*function)(char **);
} builtin_cmd_t;

extern shell_state_t context;
extern char *default_prompt;
// core
int exec_buildin(char **tokens);
int exec_program(char **tokens);

char *get_program_path(char *command);
int is_builtin(char *command);

char **parse_input(char *input);
void free_tokens(char **args);
int is_sig_atoi_able(char *str);
int is_unsig_atoi_able(char *str);

// debug
void TODO(void);
void print_arguments(char **tokens);

// builtins commands
void shell_exit(char **tokens);
void type(char **tokens);
void echo(char **tokens);
void cls(char **tokens);
void help(char **tokens);
void wdir(char **tokens);
void cd(char **tokens);
void last_status(char **tokens);
void set_prompt(char **tokens);

#endif
