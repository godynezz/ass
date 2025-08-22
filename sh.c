/*******************************************************************************
 * name: sh.c
 * author: godynezz
 * description: shell main function
 ******************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "core.h"
#include "errno.h"
#include "signal.h"

void handle_sig(int sig);

char *default_prompt = DF_PROMPT;

shell_state_t context = {
    .input_stream = NULL,
    .prompt = "",
    .last_exit_status = 0,
    .inputfd = -1,
    .iobuff.buffer = NULL,
    .iobuff.buffer_size = 0,
    .iobuff.nread = 0,
};

int shell_getline(shell_state_t *state) {
  int status = 0;  // OK
  state->iobuff.nread = getline(
      &state->iobuff.buffer, &state->iobuff.buffer_size, state->input_stream);

  if (state->iobuff.nread == (size_t)-1) {
    if (errno == EINVAL || errno == ENOMEM) {
      perror("getline");
      status = 1;  // error
    } else {
      status = -1;  // EOF
    }
  }

  // remove new line
  if (state->iobuff.nread > 0 &&
      state->iobuff.buffer[state->iobuff.nread - 1] == '\n')
    state->iobuff.buffer[state->iobuff.nread - 1] = '\0';

  return returnv;
}

int main(int argc, const char **argv) {
  char **tokens;

  context.prompt = default_prompt;
  if (argc == 2) {
    context.input_stream = fopen(argv[1], "r+");
    if (!context.input_stream) {
      printf("couldn't open file");
      return 1;
    }
  } else if (argc > 2) {
    printf("usage: yassh <script path>");
    return 1;
  } else {
    context.input_stream = stdin;
  }

  context.inputfd = fileno(context.input_stream);
  if (context.inputfd == -1) {
    perror("fileno");
    return 1;
  }

  signal(SIGINT, handle_sig);
  setbuf(stdout, NULL);

  while (1) {
    if (isatty(context.inputfd)) {
      printf("%s ", context.prompt);
    }

    switch (shell_getline(&context)) {
      case 1:
        exit(1);
      case -1:
        putchar('\n');
        exit(0);
    }

    tokens = parse_input(context.iobuff.buffer);

    if (!tokens || !tokens[0]) goto clean;
    if (exec_buildin(tokens)) goto clean;
    if (exec_program(tokens)) goto clean;
    printf("%s: command not found\n", tokens[0]);

  clean:
    free_tokens(tokens);
    free(context.iobuff.buffer);
    tokens = NULL;
    context.iobuff.buffer = NULL;
  }

  if (context.input_stream != stdin) {
    fclose(context.input_stream);
  }
  putchar('\n');
  return 0;
}

void handle_sig(int sig) {
  if (sig == SIGINT) {
    printf("\n");
    if (isatty(context.inputfd)) printf("%s ", context.prompt);
  }
}
