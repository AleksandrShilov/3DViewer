#include "parse.h"

int main() {
  data objetc;
  read_file_for_counting("cube.obj", &objetc);
  read_file_for_writing("cube.obj", &objetc);
  return 0;
}
