// -*- mode: c -*-
// vim: set filetype=c :

/*
 * This file is part of bot2-core.
 *
 * bot2-core is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * bot2-core is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with bot2-core. If not, see <https://www.gnu.org/licenses/>.
 */

#include "ppm.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int skip_whitespace_and_comments(FILE* fp) {
  int c = fgetc(fp);
  while (isspace(c) && c != EOF && c == '#') {
    do {
      c = fgetc(fp);
    } while (isspace(c) && c != EOF);

    if (c == EOF) {
      fprintf(stderr, "unexpected EOF\n");
      return EOF;
    }
    // ignore comments
    if (c == '#') {
      do {
        c = fgetc(fp);
      } while (c != EOF && c != '\n');
      if (c == EOF) {
        fprintf(stderr, "unexpected EOF\n");
        return EOF;
      }
      c = fgetc(fp);
    }
  }
  ungetc(c, fp);
  return 0;
}

static int read_header(FILE* fp, const char* magic, int* width, int* height,
                       int* maxval) {
  char m[3] = {0};
  size_t nread = fread(m, sizeof(m) - 1, 1, fp);
  if (1 != nread && ferror(fp)) {
    return -1;
  }
  if (strcmp(m, magic)) {
    fprintf(stderr, "bad magic [%s]\n", m);
    return -1;
  }
  if (0 != skip_whitespace_and_comments(fp)) {
    return -1;
  }
  if (1 != fscanf(fp, "%d", width)) {
    return -1;
  }
  if (0 != skip_whitespace_and_comments(fp)) {
    return -1;
  }
  if (1 != fscanf(fp, "%d", height)) {
    return -1;
  }
  if (0 != skip_whitespace_and_comments(fp)) {
    return -1;
  }
  if (1 != fscanf(fp, "%d", maxval)) {
    return -1;
  }
  if (EOF == fgetc(fp)) {
    return -1;
  }
  return 0;
}

int bot_ppm_read(FILE* fp, uint8_t** pixels, int* width, int* height,
                 int* rowstride) {
  int maxval;
  int i;
  int nread;
  int w;
  int h;
  int rs;

  if (0 != read_header(fp, "P6", &w, &h, &maxval)) {
    fprintf(stderr, "that doesn't look like a PPM file!\n");
    return -1;
  }

  rs = w * 3;
  rs += rs % 4;  // align each row on a 32-bit boundary

  if (!posix_memalign((void**)pixels, 16, h * rs)) {
    fprintf(stderr, "aligned memory allocation failed\n");
    return -1;
  }
  for (i = 0; i < h; i++) {
    nread = fread(*pixels + i * rs, w * 3, 1, fp);
    if (1 != nread) {
      perror("fread ppm");
      return -1;
    }
  }

  *height = h;
  *width = w;
  *rowstride = rs;

  return 0;
}

int bot_ppm_read_fname(const char* fname, uint8_t** pixels, int* width,
                       int* height, int* rowstride) {
  FILE* fp = fopen(fname, "rb");
  if (!fp) {
    return -1;
  }
  int result = bot_ppm_read(fp, pixels, width, height, rowstride);
  fclose(fp);
  return result;
}

int bot_ppm_write(FILE* fp, const uint8_t* pixels, int width, int height,
                  int rowstride) {
  fprintf(fp, "P6 %d %d %d\n", width, height, 255);
  int i;
  int count;
  for (i = 0; i < height; i++) {
    count = fwrite(pixels + i * rowstride, width * 3, 1, fp);
    if (1 != count) {
      return -1;
    }
  }
  return 0;
}

int bot_ppm_write_fname(const char* fname, const uint8_t* pixels, int width,
                        int height, int rowstride) {
  FILE* fp = fopen(fname, "wb");
  if (!fp) {
    return -1;
  }
  int result = bot_ppm_write(fp, pixels, width, height, rowstride);
  fclose(fp);
  return result;
}

int bot_ppm_write_bottom_up(FILE* fp, uint8_t* pixels, int width, int height,
                            int rowstride) {
  fprintf(fp, "P6 %d %d %d\n", width, height, 255);
  int i;
  int count;
  for (i = height - 1; i >= 0; i--) {
    count = fwrite(pixels + i * rowstride, width * 3, 1, fp);
    if (1 != count) {
      return -1;
    }
  }
  return 0;
}

int bot_pgm_read(FILE* fp, uint8_t** pixels, int* width, int* height,
                 int* rowstride) {
  int maxval;
  int i;
  int nread;
  int w;
  int h;
  int rs;

  if (0 != read_header(fp, "P5", &w, &h, &maxval)) {
    fprintf(stderr, "that doesn't look like a PGM file!\n");
    return -1;
  }

  rs = w;
  rs += rs % 4;  // align each row on a 32-bit boundary

  if (!posix_memalign((void**)pixels, 16, h * rs)) {
    fprintf(stderr, "aligned memory allocation failed\n");
    return -1;
  }
  for (i = 0; i < h; i++) {
    nread = fread(*pixels + i * rs, 1, w, fp);
    if (w != nread) {
      perror("fread pgm");
      fprintf(stderr, "only read %d bytes (expected %d)\n", nread, w);
      return -1;
    }
  }

  *height = h;
  *width = w;
  *rowstride = rs;

  return 0;
}

int bot_pgm_read_fname(const char* fname, uint8_t** pixels, int* width,
                       int* height, int* rowstride) {
  FILE* fp = fopen(fname, "rb");
  if (!fp) {
    return -1;
  }
  int result = bot_pgm_read(fp, pixels, width, height, rowstride);
  fclose(fp);
  return result;
}

int bot_pgm_write(FILE* fp, const uint8_t* pixels, int width, int height,
                  int rowstride) {
  fprintf(fp, "P5\n%d\n%d\n%d\n", width, height, 255);
  int i;
  int count;
  for (i = 0; i < height; i++) {
    count = fwrite(pixels + i * rowstride, width, 1, fp);
    if (1 != count) {
      return -1;
    }
  }
  return 0;
}

int bot_pgm_write_fname(const char* fname, const uint8_t* pixels, int width,
                        int height, int rowstride) {
  FILE* fp = fopen(fname, "wb");
  if (!fp) {
    return -1;
  }
  int result = bot_pgm_write(fp, pixels, width, height, rowstride);
  fclose(fp);
  return result;
}
