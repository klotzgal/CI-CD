#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "grep_lib.h"

int parser();
void grep();

int main(int argc, char* argv[]) {
  opt opt = {0};
  ind ind = {0};  // Инициализация входных опций
  int file_c = 0;
  int temp_c = 0;
  temp_c = parser(argc, argv, &opt, &ind);
  if (!opt.e && !opt.f) {
    ind.templates[temp_c] = ind.files[file_c];
    del_first_sell(ind.files);
  }
  while (ind.files[file_c] != 0) {
    grep(ind.files[file_c++], argv, opt, ind);
  }
  return 0;
}

int parser(int argc, char* argv[], opt* opt, ind* ind) {
  int temp_c = 0, file_c = 0, f_arg_c = 0;
  const char* short_options = "eivclnhsfo";  // Короткие опции
  while (optind < argc) {
    int res = getopt(argc, argv, short_options);
    switch (res) {
      case 'e':
        opt->e = 1;
        ind->templates[temp_c++] = optind++;
        break;
      case 'i':
        opt->i = 1;
        break;
      case 'v':
        opt->v = 1;
        break;
      case 'c':
        opt->c = 1;
        break;
      case 'l':
        opt->l = 1;
        break;
      case 'n':
        opt->n = 1;
        break;
      case 'h':
        opt->h = 1;
        break;
      case 's':
        opt->s = 1;
        break;
      case 'f':
        opt->f = 1;
        ind->f_arg[f_arg_c++] = optind++;
        break;
      case 'o':
        opt->o = 1;
        break;
      case -1:
        ind->files[file_c++] = optind++;
        break;
      default:
        if (!opt->s) {
          fprintf(stderr, "grep: invalid option -- %c", res);
        }
        exit(1);
        break;
    }
  }
  if (opt->v) opt->o = 0;
  return temp_c;
}

void grep(int file_c, char* argv[], opt opt, ind ind) {
  regmatch_t pmatch[1] = {0};  // массив структур для сохранения начального
                               // и конечного индекса совпадения
  FILE* f = fopen(argv[file_c], "r");
  if (f == NULL) {
    if (!opt.s)
      fprintf(stderr, "grep: %s: No such file or directory\n", argv[file_c]);
  } else {
    char str[length];
    int str_count = 0;
    int n_trig = 0;
    int match_count = 0;
    while (fgets(str, length, f)) {  // проход по файлам
      str_count++;
      if (find_templ(str, argv, opt, ind, pmatch)) {
        match_count++;
        n_trig = 1;
        if (!(opt.c || opt.l)) {
          if ((ind.files[1] != 0) && (!opt.h)) {
            printf("%s:", argv[file_c]);  // название файла и -h
          }
          if (opt.n) {
            printf("%d:", str_count);  // номер строки  (-n)
          }
          if (!opt.o) {
            printf("%s", str);
          }  // -о или строка
        }
      } else {
        n_trig = 0;
      }
      if (opt.o && !opt.c && !opt.l) {
        o_flag(str, argv, opt, ind, pmatch);
      }
    }
    if (opt.l) {
      if (match_count) {
        match_count = 1;
      }
    }
    if (opt.c) {
      if ((ind.files[1] != 0) && (!opt.h)) {
        printf("%s:", argv[file_c]);
      }
      printf("%d\n", match_count);
    }
    if (opt.l && match_count) {
      printf("%s\n", argv[file_c]);
    }
    if (((strchr(str, '\n') == NULL) && !opt.o && !opt.l && !opt.c && n_trig)) {
      printf("\n");
    }
    fclose(f);
    f = NULL;
  }
}
