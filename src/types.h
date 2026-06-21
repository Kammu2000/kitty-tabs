#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>

typedef struct {
  int id;
  char *title;
} Tab;

typedef struct {
  Tab *tabs;
  size_t count;
} TabList;

#endif
