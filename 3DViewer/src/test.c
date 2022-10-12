#include <check.h>

#include "parse.h"

START_TEST(my_3dviewer_1) {
  data object;
  read_file_for_counting("cube.obj", &object);
  read_file_for_writing("cube.obj", &object);
  double matrix_test[9][3] = {{0, 0, 0},
                              {-0.500000, -0.500000, 0.500000},
                              {0.500000, -0.500000, 0.500000},
                              {-0.500000, 0.500000, 0.500000},
                              {0.500000, 0.500000, 0.500000},
                              {-0.500000, 0.500000, -0.500000},
                              {0.500000, 0.500000, -0.500000},
                              {-0.500000, -0.500000, -0.500000},
                              {0.500000, -0.500000, -0.500000}};
  int matrix_test2[12][6] = {
      {1, 2, 2, 3, 3, 1}, {3, 2, 2, 4, 4, 3}, {3, 4, 4, 5, 5, 3},
      {5, 4, 4, 6, 6, 5}, {5, 6, 6, 7, 7, 5}, {7, 6, 6, 8, 8, 7},
      {7, 8, 8, 1, 1, 7}, {1, 8, 8, 2, 2, 1}, {2, 8, 8, 4, 4, 2},
      {4, 8, 8, 6, 6, 4}, {7, 1, 1, 5, 5, 7}, {5, 1, 1, 3, 3, 5}};
  for (unsigned int i = 1; i < object.count_of_vertexes; ++i) {
    ck_assert_int_eq(matrix_test[i][0], object.matrix_3d.matrix[i][0]);
    ck_assert_int_eq(matrix_test[i][1], object.matrix_3d.matrix[i][1]);
    ck_assert_int_eq(matrix_test[i][2], object.matrix_3d.matrix[i][2]);
  }
  for (unsigned int j = 0; j < object.count_of_facets; ++j) {
    for (int i = 0; i < object.polygons[j].numbers_of_vertexes_in_facets; ++i) {
      ck_assert_int_eq(matrix_test2[j][i], object.polygons[j].vertexes[i]);
    }
  }
  remove_matrix(&object.matrix_3d);
  remove_polygon(&object);
}
END_TEST

START_TEST(my_move_test) {
  matrix_t matrix;
  my_create_matrix(8, 3, &matrix);
  move_object(matrix, 4, 0);
  for (int i = 0; i < matrix.rows; ++i) {
    ck_assert_int_eq(matrix.matrix[i][0], 4);
  }
  move_object(matrix, 2, 1);
  for (int i = 0; i < matrix.rows; ++i) {
    ck_assert_int_eq(matrix.matrix[i][1], 2);
  }
  move_object(matrix, 6, 2);
  for (int i = 0; i < matrix.rows; ++i) {
    ck_assert_int_eq(matrix.matrix[i][2], 6);
  }
  remove_matrix(&matrix);
}
END_TEST

START_TEST(my_turn_test) {
  matrix_t matrix;
  matrix.rows = 0;
  matrix.cols = 0;
  matrix_t matrix_2;
  my_create_matrix(8, 3, &matrix_2);
  turn_axis_x(matrix_2, 2);
  turn_axis_y(matrix_2, 2);
  turn_axis_z(matrix_2, 2);
  matrix.matrix = calloc(3, sizeof(double *));
  if (matrix.matrix) {
    for (int m = 0; m < 3; m++) {
      matrix.matrix[m] = calloc(3, sizeof(double));
    }
    matrix.rows = 3;
    matrix.cols = 3;
  }
  change_scale(matrix, 4);
  for (int i = 0; i < matrix.rows; ++i) {
    for (int j = 0; j < matrix.cols; ++j) {
      ck_assert_int_eq(matrix.matrix[i][j], 0);
    }
  }
  remove_matrix(&matrix_2);
  remove_matrix(&matrix);
}
END_TEST

START_TEST(my_mashtab_test) {
  matrix_t matrix;
  matrix.rows = 0;
  matrix.cols = 0;
  matrix.matrix = calloc(3, sizeof(double *));
  if (matrix.matrix) {
    for (int m = 0; m < 3; m++) {
      matrix.matrix[m] = calloc(3, sizeof(double));
    }
    matrix.rows = 3;
    matrix.cols = 3;
  }
  change_scale(matrix, 4);
  for (int i = 0; i < matrix.rows; ++i) {
    for (int j = 0; j < matrix.cols; ++j) {
      ck_assert_int_eq(matrix.matrix[i][j], 0);
    }
  }
  remove_matrix(&matrix);
}
END_TEST

int main(void) {
  Suite *s1 = suite_create("matr");
  SRunner *sr = srunner_create(s1);
  int nf;

  TCase *tc1_1 = tcase_create("matr");
  suite_add_tcase(s1, tc1_1);
  tcase_add_test(tc1_1, my_3dviewer_1);
  tcase_add_test(tc1_1, my_move_test);
  tcase_add_test(tc1_1, my_turn_test);
  tcase_add_test(tc1_1, my_mashtab_test);

  srunner_run_all(sr, CK_ENV);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nf == 0 ? 0 : 1;
}
