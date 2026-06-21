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

  for (int i = 0; i < window_count; i++) {
    cJSON *window = cJSON_GetArrayItem(window_list, i);

    cJSON *is_active = cJSON_GetObjectItem(window, "is_active");

    if (!cJSON_IsBool(is_active) || !cJSON_IsTrue(is_active)) {
      continue;
    }

    cJSON *tabs = cJSON_GetObjectItem(window, "tabs");
    int tab_count = cJSON_GetArraySize(tabs);

    result.tabs = malloc(sizeof(Tab) * tab_count);
    result.count = tab_count;

    for (int j = 0; j < tab_count; j++) {
      cJSON *tab = cJSON_GetArrayItem(tabs, j);

      cJSON *id = cJSON_GetObjectItem(tab, "id");
      cJSON *title = cJSON_GetObjectItem(tab, "title");

      result.tabs[j].id = id ? id->valueint : -1;

      if (title && cJSON_IsString(title)) {
        result.tabs[j].title = strdup(title->valuestring);
      } else {
        result.tabs[j].title = strdup("");
      }
    }

    break;
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
