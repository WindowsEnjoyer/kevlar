#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/utils.h"

#include "kevlar_templating.h"
#include "kevlar_handle_config.h"

void kevlar_parse_template_token(char line[], char substr[], char content[], FILE * out_file_buffer) {
  char tail[TEMPLATE_MAX_TAG_SIZE];
  strcpy(tail, utl_strchrev(strstr(line, substr), '-')-1);
  *(strchr(line, '-')) = '\0';

  line[strlen(line)-1] == ' ' 
    ? line[strlen(line)-1] = '\0' 
    : line[strlen(line-1)];

  fprintf(out_file_buffer, "%s %s %s", line, content, tail);
};

void kevlar_parse_stylesheet(char *stylesheet_path, char *output) {
  FILE * file_buffer;
  char style_line[TEMPLATE_MAX_LINE_SIZE];
  char file[TEMPLATE_MAX_FILE_SIZE];

  if ((file_buffer = fopen(stylesheet_path, "r")) == NULL) {
    fprintf(stderr, "[kevlar] couldn't open stylesheet! Perhaps your theme is invalid?\n");
    exit(1);
  }

  strcat(output, "<style>\n");
  while ((fgets(style_line, TEMPLATE_MAX_LINE_SIZE, file_buffer)) != NULL) {
    strcat(output, style_line);
  } 
  strcat(output, "</style>\n");
};

void kevlar_parse_template(FILE * in_file_buffer, FILE * out_file_buffer, KevlarConfig *kev_config) {
  char file[TEMPLATE_MAX_FILE_SIZE];
  char line[TEMPLATE_MAX_LINE_SIZE];


  while ((fgets(line, TEMPLATE_MAX_LINE_SIZE, in_file_buffer)) != NULL) {

    if (strstr(line, "--TITLE--")) { 
      kevlar_parse_template_token(line, "--TITLE--", kev_config->configTitle, out_file_buffer);
      continue;
    } else if (strstr(line, "--AUTHOR--")) {
      kevlar_parse_template_token(line, "--AUTHOR--", kev_config->configAuthor, out_file_buffer);
      continue;
    } else if (strstr(line, "--LISTING--")) {
      kevlar_parse_template_token(line, "--LISTING--", kev_config->configListing, out_file_buffer);
      continue;
    } else if (strstr(line, "--STYLES--") || strstr(line, "--STYLE--")) {
      char stylesheet[TEMPLATE_MAX_FILE_SIZE];
      kevlar_parse_stylesheet(kev_config->configCSSPath, stylesheet);
      kevlar_parse_template_token(line, "--STYLE--", stylesheet, out_file_buffer);
      continue;

    } else if (strstr(line, "--HEADER--")) {
      FILE * header_file = fopen(kev_config->configHeaderPath, "r");
      kevlar_parse_template(header_file, out_file_buffer, kev_config);
      fclose(header_file);
      continue;
    } else if (strstr(line, "--FOOTER--")) {

      FILE * footer_file = fopen(kev_config->configFooterPath, "r");
      kevlar_parse_template(footer_file, out_file_buffer, kev_config);
      fclose(footer_file);

      continue;
    } else if (strstr(line, "--CONTENT--")) {
      kevlar_parse_template_token(line, "--CONTENT--", kev_config->configHtmlContents, out_file_buffer);
      continue;
    } else {
      fprintf(out_file_buffer, "%s", line);
    }
  }
};

void kevlar_build_template(char *in_file_path, char *out_file_path, KevlarConfig * kev_config) {
  FILE * infile = fopen(in_file_path, "r");
  FILE * outfile = fopen(out_file_path, "w");

  if (infile == NULL) {
    fprintf(stderr, "[kevlar] couldn't find %s while building the template\n", in_file_path);
    exit(1);
  }

  kevlar_parse_template(infile, outfile, kev_config);

  fclose(infile);
  fclose(outfile);
}