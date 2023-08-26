#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "cat_lib.h"

void parser();
void cat();

int main(int argc, char* argv[]) {
  opt opt = {0};  // Инициализация входных опций

  parser(argc, argv, &opt);

  for (int i = optind; i < argc; i++) {
    cat(i, argv, opt);
  }
  if (optind == argc) {
#ifdef _WIN32_
    argv[1] = "con";
#else
    argv[1] = "/dev/tty";
#endif

    cat(1, argv, opt);
  }
  return 0;
}

void parser(int argc, char* argv[], opt* opt) {
  int res = 0;
  const char* short_options = "+benstvTE";  // Короткие опции
  const struct option long_options[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {NULL, no_argument, NULL, 0}};  // Массив структур с длинными опциями и
                                      // соответствующие им короткие
  int option_index;
  while ((res = getopt_long(argc, argv, short_options, long_options,
                            &option_index)) != -1) {
    switch (res) {
      case 'b':
        opt->b = 1;
        break;
      case 'e':
        opt->e = 1;
        opt->v = 1;
        break;
      case 'n':
        opt->n = 1;
        break;
      case 's':
        opt->s = 1;
        break;
      case 't':
        opt->t = 1;
        opt->v = 1;
        break;
      case 'v':
        opt->v = 1;
        break;
      case 'T':
        opt->T = 1;
        break;
      case 'E':
        opt->E = 1;
        break;
      default:
        fprintf(stderr, "cat: %c: No such file or directory", res);
        break;
    }
  }
}

void cat(int i, char* argv[], opt opt) {
  FILE* f;
  f = fopen(argv[i], "r");
  if (!f) {
    fprintf(stderr, "cat: %s: No such file or directory\n", argv[i]);
  } else {
    int c;
    int count = 0, counter_check = 0, n_flag = 0;
    while ((c = getc(f)) != EOF) {
      if ((n_flag == 2) && (opt.s) && (c == '\n')) {
        continue;
      }
      if (opt.b) {
        counter_check = number_nonblank(counter_check, c, &count);
      } else if (opt.n) {
        counter_check = number(counter_check, c, &count);
      }
      if ((opt.T || opt.t) && c == '	') {
        printf("^");
        c = 'I';
      }
      if (c == '\n') {
        if (opt.E || opt.e) {
          printf("$");
        }
        n_flag++;
      } else {
        if (n_flag) {
          n_flag = 0;
        }
      }
      if ((opt.v) && (c >= 0)) {
        vvvv(&c);
      }
      printf("%c", c);
    }
    fclose(f);
    f = NULL;
  }
}
