#include "file.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *read_file(const char *path) {
  FILE *fp = fopen(path, "rb");
  if (!fp) {
    return NULL;
  }
  fseek(fp, 0, SEEK_END);
  long fsize = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  char *buf = (char *)malloc(fsize + 1);
  if (!buf) {
    fclose(fp);
    return NULL;
  }

  fread(buf, 1, fsize, fp);
  fclose(fp);

  return buf;
}

void write_file(const char *path, const char *contents, const char *mode) {
  FILE *fp = fopen(path, mode);
  if (!fp) {
    return;
  }
  fwrite(contents, 1, strlen(contents), fp);
  fclose(fp);
}