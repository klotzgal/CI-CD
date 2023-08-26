#ifndef SRC_GREP_GREP_LIB_H_
#define SRC_GREP_GREP_LIB_H_

#define length 1000

typedef struct opt {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
} opt;  // Структура входных опций

typedef struct index {
  int files[60];
  int templates[60];
  int f_arg[60];
} ind;

int templ_check();
int find_templ();
int f_flag();
void o_flag();
void del_first_sell();

#endif  // SRC_GREP_GREP_LIB_H_