#include <stdio.h>
#include <stdlib.h>

enum {all_nums = 9000};

static double sum_nums(double a, double b) { return a + b; }
static double dif_nums(double a, double b) { return a - b; }
static double mul_nums(double a, double b) { return a * b; }
static double div_nums(double a, double b) { return a / b; }

static int find_mult_div(const char *opers)
{
  const char *t;
  for(t = opers; *t; t++) {
    if(*t == '*' || *t == '/')
      return t - opers;
  }
  return -1;
}


static void opers_arr_error(int lc, int c, int a_num, int n)
{
  if(lc == '+' || lc == '-' || lc == '*' || lc == '/') {
    fprintf(stderr, "Duplicate operation, "
             "error in pos %d\n", n);
    exit(1); 
  }
  if(!a_num) {
    fprintf(stderr, "Operation before num!\n");
    exit(1);        
  }
}

static void operation_to_arr(int lc, int c, int a_num, int n, char *opers)
{
  opers_arr_error(lc, c, a_num, n); 
  *(opers) = c;
}

static void mul_and_div(double *arr, char *opers)
{
  int mult_d = 0;
  double r = 0;
  int i;
  while((mult_d = find_mult_div(opers)) > -1) {
    if(opers[mult_d] == '*') {
      r = mul_nums(arr[mult_d], arr[mult_d + 1]);
    } else 
    if(opers[mult_d] == '/') {
      r = div_nums(arr[mult_d], arr[mult_d + 1]);
    }
    arr[mult_d] = r;
    for(i = mult_d + 1; arr[i]; i++) {
      arr[i] = arr[i + 1];
    }
    if(opers[mult_d + 1]) { 
      opers[mult_d] = opers[mult_d + 1];
    } else
      opers[mult_d] = 0;
    for(i = mult_d + 1; opers[i]; i++) {
      opers[i] = opers[i + 1];
    }  
  }
}

static void sum_and_dif(double *arr, char *opers)
{
  double r = 0;
  double *t;
  char *p;
  while(*opers == '+' || *opers == '-') {
    if(*opers == '+') 
      r = sum_nums(*arr, *(arr + 1));
    else 
    if(*opers == '-')
      r = dif_nums(*arr, *(arr + 1));
    *arr = r;
    for(t = (arr + 1); *t; t++)
      *t = *(t+1);
    if(*(opers+1))
      *opers = *(opers + 1);
    else
      *opers = 0;
    for(p = (opers + 1); *p; p++)
      *p = *(p + 1);
  }
}

static void input_data(double *arr, char *opers, int *pos, 
		        int *ob, int *cb);

static void calc_res(double *n, int *pos, int *ob, int *cb)
{
  double arr[all_nums] = {0};
  char opers[all_nums];
  double *ptrarr = arr;
  char *ptropers = opers;
  input_data(ptrarr, ptropers, pos, ob, cb);
  mul_and_div(ptrarr, ptropers);
  sum_and_dif(ptrarr, ptropers);
  *n = *arr;
}
static void close_bracket(int *cb, int ob, int pos, int c)
{
  *cb = *cb + 1;
  if(*cb > ob) {
    fprintf(stderr, ") > (, error in pos %d:[%c]\n", pos, c);
    exit(1);
  }	
}

static void open_bracket(int *ob, int *pos, int *cb, 
	int *arr_i, double *n, int *next_n, int *af_n)
{
  *ob = *ob + 1;
  calc_res(n, pos, ob, cb);
  if(*arr_i < 0)
    *arr_i = 0;
  else 
    *arr_i = *arr_i + 1;
  *next_n = 0;
  *af_n = 1;
}

static void operation(char **t, int *next_n, int *af_n)
{
  *t = *t + 1;
  *next_n = 1;
  *af_n = 0;
}

static void space(char lc, int *next_n)
{
  if((lc == '+' || lc == '-' || lc == '*' || lc == '/') || 
    (lc >= '0' && lc <= '9')) {
        *next_n = 1;
      }
}

static void next_index(int *arr_i, int *next_n)
{
  if(*arr_i < 0)
    *arr_i = 0;
  else 
    *arr_i = *arr_i + 1;
  *next_n = 0;
}

static void check_num(char c, int pos)
{
  if(c < '0' || c > '9') {
    fprintf(stderr, "Please, enter only nums, "
             "error in pos %d:[%c]\n", pos, c);
    exit(1);
  }
}

static void check_args(int opers_count, int arr_i)
{
  if(opers_count > arr_i) {
    fprintf(stderr, "Too many operators\n");
    exit(1);
  }
  if(arr_i > all_nums) {
    fprintf(stderr, "NUMS > %d\n", all_nums);
    exit(1); 
  }
}

static void input_data(double *arr, char *opers, int *pos, 
         int *ob, int *cb)
{
  char *t;
  int c, arr_i = 0;
  int next_num, lc, after_num;  
  next_num = lc = after_num = 0;
  t = opers;
  arr_i = -1;
  while((c = getchar()) != EOF && c != '\n') {
    *pos = *pos + 1;
    if(c == ')') {
      close_bracket(cb, *ob, *pos, c); 
      break;
    }
    if(c == '(') {
      double n;
      open_bracket(ob, pos, cb, &arr_i, &n, &next_num, &after_num);
      arr[arr_i] = n;
      goto next_iter; 
    }
    if(c == '+' || c == '-' || c == '*' || c == '/') {
      operation_to_arr(lc, c, after_num, *pos, t);
      operation(&t, &next_num, &after_num);
      goto next_iter;
    }
    if(c == ' ') {
      space(lc, &next_num);
      goto next_iter;
    }
    if(next_num) 
      next_index(&arr_i, &next_num);
    check_num(c, *pos);
    if(arr_i < 0)
      arr_i = 0;
    arr[arr_i] = arr[arr_i] * 10 + c - '0';
    after_num = 1;
    next_iter:;
    lc = c;
  }
  check_args(t - opers, arr_i);
}

int main()
{
  double n;
  int pos, open_bracket, close_bracket;
  pos = open_bracket = close_bracket = 0;
  calc_res(&n, &pos, &open_bracket, &close_bracket);
  double res = 0;
  int flag = 1;
  if(n < 0 ) {flag = -1;}
  res = (flag * n) - ((int)n * flag);
  if(res > 0)  
    printf("%f\n", n);
  else 
    printf("%ld\n", (long int)n);
  return 0;
}
