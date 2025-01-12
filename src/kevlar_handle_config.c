#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/utils.h"
#include "kevlar_handle_config.h"
#include "kevlar_new.h"

void kevlar_load_config(char file_path[CONFIG_MAX_PATH_SIZE], KevlarConfig *kev_config) {
  FILE *file_buf;

  if (!(file_buf = fopen(file_path, "r"))) {
    fprintf(stderr, "[kevlar] something went wrong while opening %s\n", file_path);
    exit(1);
  }

  int line_count = 0;
  while (!feof(file_buf)) {
    char cur_line[CONFIG_MAX_FILE_SIZE];
    char *target;
    fgets(cur_line, CONFIG_MAX_FILE_SIZE, file_buf);
    target = strtok(cur_line, "=");
    char command[2][CONFIG_MAX_OPT_SIZE] = {};

    // Only line comments for now
    if (cur_line[0] == '#')
      continue;

    int i = 0;
    while (target != NULL) {
      if (i > 1 || strlen(target) == 1) {
        fprintf(stderr, "[kevlar] invalid config at %s at line %d\n", file_path, line_count + 1);
        exit(1);
      }
      strcpy(command[i], target);
      target = strtok(NULL, "=");
      i++;
    }

    utl_truncateLast(command[1]);
    // technically this should never happen
    if (strlen(command[1]) == 0) {
      continue;
    }

    if (strcmp(command[0], "author") == 0) {
      strcpy(kev_config->configAuthor, command[1]);
    } else if (strcmp(command[0], "title") == 0) {
      strcpy(kev_config->configTitle, command[1]);
    } else if (strcmp(command[0], "theme") == 0) {
      strcpy(kev_config->configTheme, command[1]);
    } else if (strcmp(command[0], "rst_loader") == 0) {
      strcpy(kev_config->configRstLoader, command[1]);
    } else if (strcmp(command[0], "md_loader") == 0) {
      strcpy(kev_config->configMarkdownLoader, command[1]);
    }
    line_count++;
  }

  sprintf(kev_config->configFooterPath, "./%s/%s/%s", "templates", kev_config->configTheme,
          "footer.html");
  sprintf(kev_config->configHeaderPath, "./%s/%s/%s", "templates", kev_config->configTheme,
          "header.html");
  sprintf(kev_config->configIndexPath, "./%s/%s/%s", "templates", kev_config->configTheme,
          "index.html");
  sprintf(kev_config->configPostPath, "./%s/%s/%s", "templates", kev_config->configTheme,
          "post.html");
};

void kevlar_generate_skeleton_config(char file_path[CONFIG_MAX_PATH_SIZE]) {
  FILE *file_buf = fopen(file_path, "w");
  if (file_buf == NULL) {
    fprintf(stderr, "[kevlar] something went wrong while generating sample config\n");
    exit(1);
  };

  // This works for now since the file we will be generating won't be that big
  fprintf(file_buf, "author=John\ntitle=My example site\ntheme=kyudo\n");
  fclose(file_buf);
}
