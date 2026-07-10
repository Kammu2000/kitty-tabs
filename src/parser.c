#include "parser.h"
#include <cjson/cJSON.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TabList parse_tabs(char *tabs_json) {
  TabList result = {0};

  cJSON *window_list = cJSON_Parse(tabs_json);
  if (!window_list) {
    return result;
  }

  int window_count = cJSON_GetArraySize(window_list);

  // Pass 1: Count total tabs
  int total_tabs = 0;
  for (int i = 0; i < window_count; i++) {
    cJSON *window = cJSON_GetArrayItem(window_list, i);
    cJSON *tabs = cJSON_GetObjectItem(window, "tabs");

    if (cJSON_IsArray(tabs)) {
      total_tabs += cJSON_GetArraySize(tabs);
    }
  }

  result.tabs = malloc(sizeof(Tab) * total_tabs);
  result.count = total_tabs;

  // Pass 2: Copy tabs
  int index = 0;

  for (int i = 0; i < window_count; i++) {
    cJSON *window = cJSON_GetArrayItem(window_list, i);
    cJSON *tabs = cJSON_GetObjectItem(window, "tabs");

    if (!cJSON_IsArray(tabs))
      continue;

    int tab_count = cJSON_GetArraySize(tabs);

    for (int j = 0; j < tab_count; j++) {
      cJSON *tab = cJSON_GetArrayItem(tabs, j);

      cJSON *id = cJSON_GetObjectItem(tab, "id");
      cJSON *title = cJSON_GetObjectItem(tab, "title");

      result.tabs[index].id = id ? id->valueint : -1;
      result.tabs[index].title = (title && cJSON_IsString(title))
                                     ? strdup(title->valuestring)
                                     : strdup("");

      index++;
    }
  }

  cJSON_Delete(window_list);
  return result;
}

void free_tabs(TabList list) {
  for (size_t i = 0; i < list.count; i++) {
    free(list.tabs[i].title);
  }
  free(list.tabs);
}
