#ifndef SRC_PARSE_H_
#define SRC_PARSE_H_

#define _GNU_SOURCE
#include <ctype.h>
#include <locale.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Matrix {
  int rows;
  int cols;
  double **matrix;
} matrix_t;

typedef struct facets {
  int *vertexes;
  int numbers_of_vertexes_in_facets;
} polygon_t;

typedef struct data {
  unsigned int count_of_vertexes;
  unsigned int count_of_facets;
  matrix_t matrix_3d;
  polygon_t *polygons;
} data;

int my_create_matrix(int rows, int columns, matrix_t *result);
double **create_matrix(struct data *objetc);
void writing_to_matrix(struct data *objetc, char *line, int row_s);
void remove_matrix(matrix_t *A);
void remove_polygon(struct data *objetc);
void read_file_for_writing(char *name_image, struct data *first);
void read_file_for_counting(char *name_image, struct data *first);

void move_object(matrix_t matrix_3d, double value, int type_move);
void change_scale(matrix_t matrix_3d, double value);
void turn_axis_x(matrix_t matrix_3d, double value);
void turn_axis_y(matrix_t matrix_3d, double value);
void turn_axis_z(matrix_t matrix_3d, double value);

#endif  // SRC_PARSE_H_
