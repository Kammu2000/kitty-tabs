#include "process.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void run_command(char *const *argv) {
  pid_t pid = fork();
  if (pid == 0) {
    execvp(argv[0], argv);

    perror("error running command");
    exit(EXIT_FAILURE);
  } else {
    waitpid(pid, NULL, 0);
  }
}

char *run_command_with_input(char *const argv[], const char *input) {
  int in_pipe[2];
  int out_pipe[2];

  if (pipe(in_pipe) == -1 || pipe(out_pipe) == -1) {
    perror("pipe");
    return NULL;
  }

  pid_t pid = fork();

  if (pid == -1) {
    perror("fork");
    return NULL;
  }

  if (pid == 0) {
    // CHILD → fzf (or any command)

    dup2(in_pipe[0], STDIN_FILENO);
    dup2(out_pipe[1], STDOUT_FILENO);

    close(in_pipe[1]);
    close(out_pipe[0]);

    execvp(argv[0], argv);

    // if exec fails
    perror("execvp");
    exit(1);
  }

  // PARENT

  close(in_pipe[0]);
  close(out_pipe[1]);

  // write input
  if (input) {
    write(in_pipe[1], input, strlen(input));
  }
  close(in_pipe[1]);

  // read output dynamically
  size_t cap = 1024;
  size_t len = 0;
  char *buffer = malloc(cap);
  if (!buffer)
    return NULL;

  ssize_t n;
  while ((n = read(out_pipe[0], buffer + len, cap - len)) > 0) {
    len += n;

    if (len >= cap) {
      cap *= 2;
      buffer = realloc(buffer, cap);
      if (!buffer)
        return NULL;
    }
  }

  close(out_pipe[0]);

  waitpid(pid, NULL, 0);

  buffer[len] = '\0';
  return buffer;
}

char *capture_command_output(char *const argv[]) {
  int pipefd[2];

  if (pipe(pipefd) == -1) {
    perror("pipe");
    return NULL;
  }

  pid_t pid = fork();

  if (pid == -1) {
    perror("fork");
    close(pipefd[0]);
    close(pipefd[1]);
    return NULL;
  }

  // ----------------------
  // Child
  // ----------------------
  if (pid == 0) {

    // stdout -> pipe
    if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
      perror("dup2");
      exit(EXIT_FAILURE);
    }

    // We don't need these anymore
    close(pipefd[0]);
    close(pipefd[1]);

    execvp(argv[0], argv);

    // Only reached if execvp fails
    fprintf(stderr, "Failed to execute '%s': %s\n", argv[0], strerror(errno));
    exit(EXIT_FAILURE);
  }

  // ----------------------
  // Parent
  // ----------------------

  close(pipefd[1]); // parent only reads

  size_t capacity = 4096;
  size_t length = 0;

  char *result = malloc(capacity);

  if (!result) {
    perror("malloc");
    close(pipefd[0]);
    waitpid(pid, NULL, 0);
    return NULL;
  }

  while (1) {

    // Grow buffer if necessary
    if (length == capacity) {
      capacity *= 2;

      char *tmp = realloc(result, capacity);

      if (!tmp) {
        free(result);
        close(pipefd[0]);
        waitpid(pid, NULL, 0);
        return NULL;
      }

      result = tmp;
    }

    ssize_t bytes = read(pipefd[0], result + length, capacity - length);

    if (bytes == -1) {
      perror("read");
      free(result);
      close(pipefd[0]);
      waitpid(pid, NULL, 0);
      return NULL;
    }

    if (bytes == 0)
      break; // EOF

    length += bytes;
  }

  close(pipefd[0]);

  waitpid(pid, NULL, 0);

  // Make room for '\0'
  char *tmp = realloc(result, length + 1);
  if (tmp)
    result = tmp;

  result[length] = '\0';

  return result;
}
