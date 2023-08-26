#include "grep_lib.h"

#include <regex.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int templ_check(char pattern[length], char str[length], opt opt,
                regmatch_t pmatch[1]) {
  int res;
  int cflags = REG_NEWLINE;
  regex_t preg;  // скомпилированый шаблон из регулярного выражения
  const size_t nmatch = 1;
  if (regcomp(&preg, pattern, cflags)) {
  } else {
    res = regexec(&preg, str, nmatch, pmatch, 0);
    regfree(&preg);  // очистка рег выражения (шаблона)
  }
  if (opt.i) {
    cflags = REG_ICASE;
    regcomp(&preg, pattern, cflags);
    res *= regexec(&preg, str, nmatch, pmatch, 0);
    regfree(&preg);
  }
  return !res;
}

int find_templ(char str[length], char* argv[], opt opt, ind ind,
               regmatch_t pmatch[1]) {
  int temp_c = 0, find = 0;
  while (ind.templates[temp_c] != '\0') {
    char pattern[length];
    strcpy(pattern, argv[ind.templates[temp_c]]);
    if ((find = templ_check(pattern, str, opt, pmatch))) {
      break;
    }  // если найден шаблон, то прекращает поиск
    temp_c++;
  }  // очередь шаблонов
  if ((find == 0) && (opt.f)) {
    find = f_flag(argv, str, opt, ind, pmatch);
  }  // шаблоны из файлов флага f
  if (opt.v) {
    find = !find;
  }  // инверсия (-v)
  return find;
}

int f_flag(char* argv[], char str[length], opt opt, ind ind,
           regmatch_t pmatch[1]) {
  int find = 0;
  FILE* f = fopen(argv[ind.f_arg[0]], "r");
  if (f == NULL) {
    if (!opt.s)
      fprintf(stderr, "grep: %s: No such file or directory\n",
              argv[ind.f_arg[0]]);
  } else {
    char f_str[length];
    while (fgets(f_str, length, f)) {
      if (f_str[strlen(f_str) - 1] == '\n') f_str[strlen(f_str) - 1] = '\0';
      char pattern[length] = {0};
      strcpy(pattern, f_str);
      if ((find = templ_check(pattern, str, opt, pmatch))) {
        break;
      }  // если найден шаблон, то прекращает поиск
    }
    fclose(f);
    f = NULL;
  }
  return find;
}

void o_flag(char str[length], char* argv[], opt opt, ind ind,
            regmatch_t pmatch[1]) {
  while (find_templ(str, argv, opt, ind, pmatch)) {
    for (int j = 0; j < pmatch[0].rm_eo; j++) {
      if ((j >= pmatch[0].rm_so)) {
        printf("%c", str[j]);
      }
      str[j] = 128;
    }
    printf("\n");
  }
}

void del_first_sell(int* arr) {
  int i = 0;
  while (arr[i] != 0) {
    arr[i] = arr[i + 1];
    i++;
  }
}
