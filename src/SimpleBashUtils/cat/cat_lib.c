#include "cat_lib.h"

#include <ctype.h>
#include <stdio.h>

int number(int counter_check, int c, int* count) {
  if (counter_check == 0) {
    printf("%6d\t", ++*count);
    counter_check = 1;
  }
  if (c == '\n') {
    counter_check = 0;
  }
  return counter_check;
}

int number_nonblank(int counter_check, int c, int* count) {
  if ((c != '\n')) {
    if (counter_check == 0) {
      printf("%6d\t", ++*count);
      counter_check = 1;
    }
  } else {
    counter_check = 0;
  }
  return counter_check;
}

void vvvv(int* c) {
  if (!isgraph(*c) && (*c != '\n') && (*c != ' ') && (*c != '\t')) {
    int flag = 1;
    if (*c == 127) {
      *c = '?';
    } else if ((*c + 64) > 128) {
      *c = (*c + 64) % 128;
      printf("M-");
      flag = 0;
    } else
      *c = (*c + 64) % 128;
    switch (*c) {
      case '`':
        *c = ' ';
        break;
      case 'I':
        if (flag) {
          *c = '\t';
        }
        break;
      case 'J':
        if (flag) {
          *c = '\n';
        }
        break;
    }
    if (*c != ' ') {
      printf("^");
    }
  }
}
