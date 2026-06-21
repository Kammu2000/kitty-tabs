#ifndef KITTY_H
#define KITTY_H

#include "types.h"

char *kitty_list_tabs(void);
int pick_tab(Tab *tabs, int count);
void focus_tab(int tab_id);

#endif
