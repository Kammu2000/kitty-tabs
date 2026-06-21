#include "kitty.h"
#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *build_fzf_input(Tab *tabs, int count) {
  size_t size = 0;

  for (int i = 0; i < count; i++) {
    size += snprintf(NULL, 0, "%d:%s\n", tabs[i].id, tabs[i].title);
  }

  char *buf = malloc(size + 1);
  if (!buf)
    return NULL;

  char *p = buf;

  for (int i = 0; i < count; i++) {
    p += sprintf(p, "%d:%s\n", tabs[i].id, tabs[i].title);
  }

  return buf;
}

char *kitty_list_tabs(void) {
  char *const argv[] = {"kitty", "@", "ls", NULL};

  return capture_command_output(argv);
}

int pick_tab(Tab *tabs, int count) {
  if (!tabs || count <= 0) {
    return -1;
  }

  char *input = build_fzf_input(tabs, count);
  if (!input) {
    return -1;
  }

  char *const argv[] = {"/opt/homebrew/bin/fzf",
                        "--height=50%",
                        "--reverse",
                        "--border",
                        "--prompt=Tabs> ",
                        NULL};

  char *output = run_command_with_input(argv, input);

  free(input);

  if (!output || strlen(output) == 0) {
    free(output);
    return -1;
  }

  // output format: "id:title"
  char *colon = strchr(output, ':');
  if (!colon) {
    free(output);
    return -1;
  }

  *colon = '\0';

  int id = atoi(output);

  free(output);
  return id;
}

void focus_tab(int tab_id) {
  char match_arg[64];
  snprintf(match_arg, sizeof(match_arg), "id:%d", tab_id);

  char *argv[] = {"kitty", "@", "focus-tab", "--match", match_arg, NULL};

  run_command(argv);
}
