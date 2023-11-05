#include <stdio.h>

#include <string.h>

#include <stdint.h>

#include <stdlib.h>

#include "bmp_header.h"

#define dim 50

#define formula 31

typedef struct {
  uint8_t blue;
  uint8_t green;
  uint8_t red;
}
pixel;

void line(int y1, int x1, int y2, int x2, int width, pixel color, pixel ** m, bmp_infoheader info) {
  int max_x = 0, min_x = 0, max_y = 0, min_y = 0, i = 0, j = 0, g = 0, v = 0;
  if (x2 > x1) {
    max_x = x2;
    min_x = x1;
  } else {
    max_x = x1;
    min_x = x2;
  }
  if (y2 > y1) {
    max_y = y2;
    min_y = y1;
  } else {
    max_y = y1;
    min_y = y2;
  }
  if (max_x - min_x >= max_y - min_y) {
    for (i = min_x; i <= max_x; i++) {
      j = (i * y2 - i * y1 - x1 * y2 + x2 * y1) / (x2 - x1);
      for (g = i - width / 2; (g <= (i + width / 2)) && (g < info.height); g++)
        for (v = j - width / 2; (v <= (j + width / 2)) && (v < info.width); v++)
          if (g >= 0 && v >= 0)
            m[g][v] = color;
    }
  } else {
    for (j = min_y; j <= max_y; j++) {
      i = (j * x2 - j * x1 - x2 * y1 + x1 * y2) / (y2 - y1);
      for (g = i - width / 2; (g <= (i + width / 2)) && (g < info.height); g++)
        for (v = j - width / 2; (v <= (j + width / 2)) && (v < info.width); v++)
          if (g >= 0 && v >= 0)
            m[g][v] = color;
    }
  }
}
int cmp_pixel(pixel pixel_1, pixel pixel_2) {
  if ((pixel_1.blue == pixel_2.blue) && (pixel_1.green == pixel_2.green) && (pixel_1.red == pixel_2.red))
    return 0;
  else
    return 1;
}
void fill(int y, int x, pixel ** m, pixel * color, bmp_infoheader * info, pixel aux) {
  m[x][y] = * color;
  if (x > 0)
    if ((cmp_pixel(aux, m[x - 1][y]) == 0))
      fill(y, x - 1, m, color, info, aux);
  if (x + 1 < info -> height)
    if ((cmp_pixel(aux, m[x + 1][y]) == 0))
      fill(y, x + 1, m, color, info, aux);
  if (y - 1 >= 0)
    if ((cmp_pixel(aux, m[x][y - 1]) == 0))
      fill(y - 1, x, m, color, info, aux);
  if (y + 1 < info -> width)
    if ((cmp_pixel(aux, m[x][y + 1]) == 0))
      fill(y + 1, x, m, color, info, aux);
}

int main() {
  int bi_num1 = 0, pix_pad1 = 0, bi_num2 = 0, pix_pad2 = 0, j = 0, i = 0, k = 0, l = 0;
  int x1 = 0, x2 = 0, y1 = 0, y2 = 0, y3 = 0, x3 = 0, x = 0, y = 0, width = 0, line_width = 0, height = 0;
  FILE * in = NULL, * out = NULL;
  char * action = NULL, * input_filename = NULL, * output_filename = NULL, * extra = NULL;
  bmp_fileheader fileheader_1, fileheader_2;
  bmp_infoheader infoheader_1, infoheader_2;
  pixel ** pixel_matrix_1 = NULL, ** pixel_matrix_2 = NULL;
  pixel color;

  action = malloc(dim * sizeof(char));
  extra = malloc(dim * sizeof(char));
  input_filename = malloc(dim * sizeof(char));
  output_filename = malloc(dim * sizeof(char));

  if (action == NULL) {
    free(action);
    printf("Nu s-a alocat suficienta memorie pentru action\n");
    return -1;
  }

  while (scanf("%s", action)) {
    if (strcmp(action, "edit") == 0) {
      scanf("%s", input_filename);
      in = fopen(input_filename, "rb");
      if (in == NULL) {
        printf("Nu s-a putut deschide fisierul1\n");
        return -1;
      }
      fread( & fileheader_1, sizeof(bmp_fileheader), 1, in );
      fread( & infoheader_1, sizeof(bmp_infoheader), 1, in );
      bi_num1 = ((formula + infoheader_1.bitPix * infoheader_1.width) / (formula + 1)) * 4;
      pix_pad1 = (int)(bi_num1 / sizeof(pixel) + 1);
      fseek(in , fileheader_1.imageDataOffset, SEEK_SET);
      pixel_matrix_1 = malloc(infoheader_1.height * sizeof(void *));
      if (pixel_matrix_1 == NULL) {
        free(pixel_matrix_1);
        printf("Nu s-a alocat suficienta memorie pt liniile matricei\n");
        return -1;
      }
      for (i = 0; i < infoheader_1.height; i++) {
        pixel_matrix_1[i] = malloc(pix_pad1 * sizeof(pixel));
        if (pixel_matrix_1[i] == NULL) {
          free(pixel_matrix_1[i]);
          printf("Nu s-a alocat suficienta memorie pt coloanele matricei\n");
          return -1;
        }
        fread(pixel_matrix_1[i], bi_num1, 1, in);
      }
    }
    if (strcmp(action, "save") == 0) {
      scanf("%s", output_filename);
      out = fopen(output_filename, "wb");
      if (out == NULL) {
        printf("Nu s-a putut crea fisierul\n");
        return -1;
      }
      fwrite( & fileheader_1, sizeof(bmp_fileheader), 1, out);
      fwrite( & infoheader_1, sizeof(bmp_infoheader), 1, out);
      fseek(out, fileheader_1.imageDataOffset, SEEK_SET);
      for (i = 0; i < infoheader_1.height; i++)
        fwrite(pixel_matrix_1[i], bi_num1, 1, out);
    }
    if (strcmp(action, "quit") == 0) {
      fclose(out);
      free(action);
      for (i = 0; i < infoheader_1.height; i++) {
        free(pixel_matrix_1[i]);
      }
      free(pixel_matrix_1);
      fclose(in);
      return 0;
    }
    if (strcmp(action, "insert") == 0) {
      fclose(in);
      scanf("%s", input_filename);
      scanf("%d%d", & y, & x); in = fopen(input_filename, "rb");
      if (in == NULL) {
        printf("Nu s-a putut deschide fisierul\n");
        return -1;
      }
      fread(& fileheader_2, sizeof(bmp_fileheader), 1, in );
      fread(& infoheader_2, sizeof(bmp_infoheader), 1, in );
      bi_num2 = (formula + infoheader_2.bitPix * infoheader_2.width) / (formula + 1) * 4;
      pix_pad2 = (int)(bi_num2 / sizeof(pixel) + 1);
      fseek(in , fileheader_2.imageDataOffset, SEEK_SET);
      pixel_matrix_2 = malloc(infoheader_2.height * sizeof(void *));
      if (pixel_matrix_2 == NULL) {
        free(pixel_matrix_2);
        printf("Nu s-a alocat suficienta memorie pt liniile matricei\n");
        return -1;
      }
      for (i = 0; i < infoheader_2.height; i++) {
        pixel_matrix_2[i] = malloc(pix_pad2 * sizeof(pixel));
        if (pixel_matrix_2[i] == NULL) {
          free(pixel_matrix_2[i]);
          printf("Nu s-a alocat suficienta memorie pt coloanele matricei\n");
          return -1;
        }
        fread(pixel_matrix_2[i], bi_num2, 1, in);
      }
      l = 0;
      k = 0;
      for (i = x; i < infoheader_1.height && k < infoheader_2.height; i++) {
        for (j = y; j < infoheader_1.width && l < infoheader_2.width; j++) {
          pixel_matrix_1[i][j] = pixel_matrix_2[k][l];
          l++;
        }
        k++;
        l = 0;
      }
    }
    if (strcmp(action, "set") == 0) {
      scanf("%s", extra);
      if (strcmp(extra, "draw_color") == 0) {
        scanf("%hhd", & color.red);
        scanf("%hhd", & color.green);
        scanf("%hhd", & color.blue);
      }
      if (strcmp(extra, "line_width") == 0) {
        scanf("%d", & line_width);
      }
    }
    if (strcmp(action, "draw") == 0) {
      scanf("%s", extra);
      if (strcmp(extra, "line") == 0) {
        scanf("%d%d%d%d", & y1, & x1, & y2, & x2);
        line(y1, x1, y2, x2, line_width, color, pixel_matrix_1, infoheader_1);
      }
      if (strcmp(extra, "rectangle") == 0) {
        scanf("%d%d%d%d", & y1, & x1, & width, & height);
        line(y1, x1, y1, x1 + height, line_width, color, pixel_matrix_1, infoheader_1);
        line(y1, x1 + height, y1 + width, x1 + height, line_width, color, pixel_matrix_1, infoheader_1);
        line(y1 + width, x1 + height, y1 + width, x1, line_width, color, pixel_matrix_1, infoheader_1);
        line(y1 + width, x1, y1, x1, line_width, color, pixel_matrix_1, infoheader_1);
      }
      if (strcmp(extra, "triangle") == 0) {
        scanf("%d%d%d%d%d%d", & y1, & x1, & y2, & x2, & y3, & x3);
        line(y1, x1, y2, x2, line_width, color, pixel_matrix_1, infoheader_1);
        line(y2, x2, y3, x3, line_width, color, pixel_matrix_1, infoheader_1);
        line(y3, x3, y1, x1, line_width, color, pixel_matrix_1, infoheader_1);
      }
    }
    if (strcmp(action, "fill") == 0) {
      scanf("%d%d", & y, & x);
      fill(y, x, pixel_matrix_1, & color, & infoheader_1, pixel_matrix_1[x][y]);
    }
  }
  return 0;
}
