#ifndef PARSER_H
#define PARSER_H

#include "types.h"

TabList parse_tabs(char *tabs_json);
void free_tabs(TabList list);

#endif
