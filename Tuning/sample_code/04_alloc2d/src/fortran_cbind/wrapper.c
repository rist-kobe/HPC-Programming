/* Copyright 2024 Research Organization for Information Science and Technology */
#include <stdio.h>

void printf_wrapper(int i, int j, double x, void *ptr) {
  printf("%5d %5d %11.4f   %p\n", i, j, x, ptr) ;
}

