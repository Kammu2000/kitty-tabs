#include "kitty.h"
#include "parser.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  char *tabs_json = kitty_list_tabs();

  if (tabs_json == NULL) {
    return EXIT_FAILURE;
  }

  TabList tabs = parse_tabs(tabs_json);
  int tab_id = pick_tab(tabs.tabs, tabs.count);
  focus_tab(tab_id);

  free(tabs_json);
  free_tabs(tabs);
  return EXIT_SUCCESS;
}
