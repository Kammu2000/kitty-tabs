#include <vector>

#include "kitty.hpp"
#include "parser.hpp"

int main() {
  std::string window_list_json = kitty_list_tabs();

  std::vector<Tab> tabs = parse_tabs(window_list_json);
  int tab_id = pick_tab(tabs);

  if (tab_id != -1) {
    focus_tab(tab_id);
  }

  return 0;
}
