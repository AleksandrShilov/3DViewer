#include "parse.h"

int my_create_matrix(int rows, int columns, matrix_t *result) {
  int res = 1;
  result->rows = 0;
  result->cols = 0;
  result->matrix = NULL;
  if (rows > 0 && columns > 0) {
    result->rows = rows;
    result->cols = columns;
    result->matrix = (double **)calloc(rows, sizeof(double *));
    for (int i = 0; i < rows; i++) {
      result->matrix[i] = (double *)calloc(columns, sizeof(double));
      for (int j = 0; j < columns; j++) {
        result->matrix[i][j] = 0;
      }
    }
    res = 0;
  }
  return res;
}

/* Создание матриц */
double **create_matrix(struct data *objetc) {
  objetc->matrix_3d.rows = objetc->count_of_vertexes;
  objetc->matrix_3d.cols = 3;
  objetc->matrix_3d.matrix = NULL;

  if ((objetc->matrix_3d.matrix =
           (double **)calloc(objetc->count_of_vertexes, sizeof(double *)))) {
    for (size_t i = 0; i < objetc->count_of_vertexes; i++) {
      objetc->matrix_3d.matrix[i] = (double *)calloc(4, sizeof(double));
    }
  }
  return objetc->matrix_3d.matrix;
}

/* Запись значений в матрицу */
void writing_to_matrix(struct data *objetc, char *line, int row_s) {
  if (objetc->matrix_3d.matrix) {
    for (int j = 0; *(line++) != '\n'; j++) {
      objetc->matrix_3d.matrix[row_s][j] = strtod(line, &line);
    }
  }
}

/* Очистка матриц */
void remove_matrix(matrix_t *A) {
  if (A != NULL) {
    if (A->matrix != NULL) {
      for (int i = 0; i < A->rows; i++) {
        if (A->matrix[i] != NULL) {
          free(A->matrix[i]);
        }
      }
      free(A->matrix);
      A->matrix = NULL;
    }
  }
}

void create_polygon(struct data *objetc) {
  objetc->polygons =
      (polygon_t *)calloc(objetc->count_of_facets, sizeof(polygon_t));
  objetc->polygons->numbers_of_vertexes_in_facets = 0;
  objetc->polygons->vertexes = NULL;
}

void remove_polygon(struct data *objetc) {
  for (unsigned int i = 0; i < objetc->count_of_facets; i++) {
    if (objetc->polygons[i].vertexes != NULL) {
      free(objetc->polygons[i].vertexes);
    }
  }
  free(objetc->polygons);
}

void score_polygon_vertexes(struct data *objetc, char *line, int row_s) {
  for (int i = 0; *(line++) != '\n'; i++) {
    if (*line == ' ') {
      line++;
      if (isdigit(*line) && *line != '0') {
        objetc->polygons[row_s].numbers_of_vertexes_in_facets++;
      }
    }
  }
  objetc->polygons[row_s].numbers_of_vertexes_in_facets *= 2;
}

void create_polygon_vertexes(struct data *objetc, char *line, int row_s) {
  objetc->polygons[row_s].vertexes = (int *)calloc(
      objetc->polygons[row_s].numbers_of_vertexes_in_facets + 1, sizeof(int));
  int i = 0;
  int tmp_val;
  int tmp_first_val = 0;
  int flag_first = 1;
  line++;
  while (*line != '\n') {
    if (*line == ' ') {
      line++;
      if (isdigit(*line)) {
        if (flag_first) {
          flag_first = 0;
          tmp_first_val = atof(line);
          objetc->polygons[row_s].vertexes[i] = tmp_first_val;
          i++;
        } else {
          tmp_val = atof(line);
          objetc->polygons[row_s].vertexes[i] = tmp_val;
          i++;
          objetc->polygons[row_s].vertexes[i] = tmp_val;
          i++;
        }
      }
    }
    line++;
  }
  if (!flag_first) {
    objetc->polygons[row_s].vertexes[i] = tmp_first_val;
  }
}

/* Чтение файла для записи в структуру */
void read_file_for_writing(char *name_image, struct data *objetc) {
  setlocale(LC_NUMERIC, "C");
  FILE *file = fopen(name_image, "r");
  if (file) {
    char *line = NULL;
    size_t len = 0;
    objetc->matrix_3d.rows = 0;
    objetc->matrix_3d.cols = 0;
    objetc->matrix_3d.matrix = create_matrix(objetc);
    create_polygon(objetc);
    int matr_row = 1;
    int polygon_row = 0;
    while (getline(&line, &len, file) != -1) {
      if (line[0] == 'v' && line[1] == ' ') {
        writing_to_matrix(objetc, line, matr_row);
        matr_row++;
      } else if (*line == 'f') {
        score_polygon_vertexes(objetc, line, polygon_row);
        create_polygon_vertexes(objetc, line, polygon_row);
        polygon_row++;
      }
    }
    if (line) {
      free(line);
    }
    fclose(file);
  }
}

/* Чтение файла для определения количества вершин и полигонов */
void read_file_for_counting(char *name_image, struct data *objetc) {
  setlocale(LC_NUMERIC, "C");
  FILE *file = fopen(name_image, "r");
  if (file) {
    char *line = NULL;
    size_t len = 0;
    objetc->count_of_facets = 0;
    objetc->count_of_vertexes = 1;
    while (getline(&line, &len, file) != -1) {
      if (line[0] == 'v' && line[1] == ' ') {
        objetc->count_of_vertexes++;
      } else if (*line == 'f') {
        objetc->count_of_facets++;
      }
    }
    if (line) {
      free(line);
    }
    fclose(file);
  }
}

void move_object(matrix_t matrix_3d, double value, int value_move) {
  for (int i = 0; i < matrix_3d.rows; ++i) {
    matrix_3d.matrix[i][value_move] += value;
  }
}

void change_scale(matrix_t matrix_3d, double value) {
  for (int i = 0; i < matrix_3d.rows; ++i) {
    for (int j = 0; j < matrix_3d.cols; ++j) {
      matrix_3d.matrix[i][j] = matrix_3d.matrix[i][j] * value;
    }
  }
}

void turn_axis_x(matrix_t matrix_3d, double value) {
  for (int i = 0; i < matrix_3d.rows; ++i) {
    double y = matrix_3d.matrix[i][1];
    double z = matrix_3d.matrix[i][2];
    matrix_3d.matrix[i][1] = cos(value) * y - sin(value) * z;
    matrix_3d.matrix[i][2] = sin(value) * y + cos(value) * z;
  }
}

void turn_axis_y(matrix_t matrix_3d, double value) {
  for (int i = 0; i < matrix_3d.rows; ++i) {
    double x = matrix_3d.matrix[i][0];
    double z = matrix_3d.matrix[i][2];
    matrix_3d.matrix[i][0] = cos(value) * x + sin(value) * z;
    matrix_3d.matrix[i][2] = -sin(value) * x + cos(value) * z;
  }
}

void turn_axis_z(matrix_t matrix_3d, double value) {
  for (int i = 0; i < matrix_3d.rows; ++i) {
    double x = matrix_3d.matrix[i][0];
    double y = matrix_3d.matrix[i][1];
    matrix_3d.matrix[i][0] = cos(value) * x - sin(value) * y;
    matrix_3d.matrix[i][1] = sin(value) * x + cos(value) * y;
  }
}
