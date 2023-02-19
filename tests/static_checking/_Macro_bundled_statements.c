// Feature tests for bundled statements
//
// The following line is for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -verify-ignore-unexpected=note  %s


void f1()
_Checked{
  int* _Array p  _Count(2) = 0;
  int val = 5;
  int val1 _Checked[3];
  int* _Array q  _Count(1) = &val;
  _Bundled {
    p = q;
    *(p+1) = 4; // expected-error {{out-of-bounds memory access}} 
    p = val1;
  }
  p = q,
  *(p+1) = 4,   // expected-error {{out-of-bounds memory access}} 
  p = val1;
}

void f2()
_Checked{
  int* _Array p  _Count(2) = 0;
  int val = 5;
  int val1 _Checked[3];
  int* _Array q  _Count(1) = &val;
  _Bundled {
    p = val1;      // expected-warning {{cannot prove declared bounds for 'p' are valid after assignment}}
    p++;
    p++;
    *(p+4) = 4; 
  }
  p = val1,        // expected-warning {{cannot prove declared bounds for 'p' are valid after assignment}}
  p++,
  p++,
  *(p+4) = 4; 
}

void f3()
_Checked{
  int* _Array p  _Count(2) = 0;
  int val = 5;
  int val1 _Checked[3];
  int* _Array q  _Count(1) = &val;
  _Bundled {
    p = q,
    *(p+1) = 4, // expected-error {{out-of-bounds memory access}}
    p = val1;
  }
  _Bundled {
    p = q,
    *(p+1) = 4, // expected-error {{out-of-bounds memory access}}
    p = val1;
  }
  _Bundled {}
}

void f4()
_Checked{
  int* _Array p  _Count(2) = 0;
  int val = 5;
  int val1 _Checked[8];
  int* _Array q  _Count(1) = &val;
  _Bundled {
    p = val1;       // expected-warning {{cannot prove declared bounds for 'p' are valid after assignment}}
    p++;
    p++;
    *(p+4) = 4; 
  }
  p = val1,         // expected-warning {{cannot prove declared bounds for 'p' are valid after assignment}}
  p++,
  p++,
  *(p+4) = 4; 
}

void f5(int flag)
_Checked{
  int* _Array p  _Count(2) = 0;
  int val = 5;
  int val1 _Checked[3];
  int* _Array q  _Count(1) = &val;
  _Bundled {
    p = flag ? q : val1;
    *(p+1) = 4;   // expected-error {{expression has unknown bounds}}
    p = val1;
  }
  p = flag ? q : val1,
  *(p+1) = 4,     // expected-error {{expression has unknown bounds}}
  p = val1;
}

void f6(int flag)
_Checked{
  int* _Array p  _Count(2) = 0;
  int val = 5;
  int val1 _Checked[3];
  int* _Array q  _Count(1) = &val;
  _Bundled {
    p = val1;
    p++;
    p++;
    *(p+1) = 4; 
    p = flag ? q : val1;  // expected-error {{inferred bounds for 'p' are unknown after assignment}}
  }
  p = val1,
  p++,
  p++,
  *(p+1) = 4, 
  p = flag ? q : val1;    // expected-error {{inferred bounds for 'p' are unknown after assignment}}
}

void f7(int flag)
_Checked{
  int* _Array p  _Count(2) = 0;
  int val = 5;
  int val1 _Checked[3];
  int* _Array q  _Count(1) = &val;
  _Bundled {
    p = val1;
    p++;
    p++;
    p = flag ? q : val1; // expected-error {{inferred bounds for 'p' are unknown after assignment}}
    *(p+1) = 4;          // expected-error {{expression has unknown bounds}}
  }
  p = val1,
  p++,
  p++,
  p = flag ? q : val1,   // expected-error {{inferred bounds for 'p' are unknown after assignment}}
  *(p+1) = 4;            // expected-error {{expression has unknown bounds}}
}

void f8(int flag)
_Checked{
  _Bundled {
    int* _Array p  _Count(2) = 0;
    int val = 5;
    int val1 _Checked[3];
    int* _Array q  _Count(1) = &val;
    p = val1;
    p++;
    p++;
    *(p+1) = 4;
    p = flag ? q : val1; // expected-error {{inferred bounds for 'p' are unknown after assignment}}
  }
}


void f9(int flag)
_Checked{
  int* _Array p  _Count(2) = 0;
  int val = 5;
  int val1 _Checked[3];
  int* _Array q  _Count(1) = &val;
  p = val1,
  p++,
  p++,
  *(p+1) = 4,
  p = flag ? q : val1;   // expected-error {{inferred bounds for 'p' are unknown after assignment}}
}

void f10()
_Checked{
  int val = 5;
  int* _Array q  _Count(2) = &val;   // expected-error {{declared bounds for 'q' are invalid after initialization}}
  _Bundled {
    int* _Array r  _Count(2) = &val; // expected-error {{declared bounds for 'r' are invalid after initialization}}
  }
}

void f11()
_Checked{
  int val = 5;
  int valarr _Checked[4];
  int* _Array q  _Count(2) = &val;   // expected-error {{declared bounds for 'q' are invalid after initialization}}
  _Bundled {
    int* _Array r  _Count(2) = &val;
    r = valarr;
  }
}

void f12(int flag1, int flag2)
_Checked{
  int* _Array p  _Count(2) = 0;
  int val = 5;
  int val1 _Checked[3];
  int* _Array q  _Count(1) = &val;
  L1:
  _Bundled {
    p = val1;
    p++;
    p = flag1 ? q : flag2 ? q : val1; // expected-error {{inferred bounds for 'p' are unknown after assignment}}
    *(p+1) = 4;                       // expected-error {{expression has unknown bounds}}
  }
  p = val1,
  p++,
  p = flag1 ? q : flag2 ? q : val1,   // expected-error {{inferred bounds for 'p' are unknown after assignment}}
  *(p+1) = 4;                         // expected-error {{expression has unknown bounds}}
}

extern int* _Array my_malloc(int len)  _Count(len);
extern void copy1(int* _Array to, int* _Array from  _Count(n), int n);
extern int* _Array copy2(int* _Array to, int* _Array from  _Count(n), int n, int new_n)  _Count(new_n);


int* _Array resize1(int* _Array buf  _Count(buflen), int buflen, int len)  _Count(len) {
  int* _Array tmp  _Count(len) = my_malloc(len);
  copy1(tmp, buf, buflen);
  _Bundled {
    buflen = len;
    buf = tmp;
  }
  return buf;
}

int buflen = 0;
int* _Array buf  _Count(buflen) = 0;

void resize2(int newlen) {
  int* _Array tmp  _Count(newlen) = my_malloc(newlen);
  _Bundled {
    buf = copy2(tmp, buf, buflen, newlen);
    buflen = newlen;
  }
}

int gtmp;
int *g1 = &gtmp;
int* _Single g2 = &gtmp;
int* _Array g3 = &gtmp;
int* _Array g4  _Count(1) = &gtmp;

int *h1 = &gtmp;
int* _Single h2 = &gtmp;
int* _Array h3 = &gtmp;
int* _Array h4  _Count(1) = &gtmp;


void gf0(void) {
  _Bundled {
    g1 = g3;  // expected-error {{assigning to 'int *' from incompatible type '_Array_ptr<int>'}}
    g2 = g3;  // expected-error {{expression has unknown bounds, cast to ptr<T> expects source to have bounds}}
    g4 = g3;  // expected-error {{inferred bounds for 'g4' are unknown after assignment}}
  }
  // Semantic error present in the expression statement.
  g1 = g3,    // expected-error {{assigning to 'int *' from incompatible type '_Array_ptr<int>'}}
  g2 = g3,
  g4 = g3;
}

void gf1(void) {
  _Bundled {
    g2 = g3;  // expected-error {{expression has unknown bounds, cast to ptr<T> expects source to have bounds}}
    g4 = g3;  // expected-error {{inferred bounds for 'g4' are unknown after assignment}}
  }

  g2 = g3,    // expected-error {{expression has unknown bounds, cast to ptr<T> expects source to have bounds}}
  g4 = g3;    // expected-error {{inferred bounds for 'g4' are unknown after assignment}}
}

void gf2(void) {
  _Bundled {
    g4 = g1;
    g4 = g2;
    g4 = g3;  // expected-error {{inferred bounds for 'g4' are unknown after assignment}}
  }

  g4 = g1,
  g4 = g2,
  g4 = g3;    // expected-error {{inferred bounds for 'g4' are unknown after assignment}}
}

void gf3(void) {
  _Bundled {
    g1 = g3;  // expected-error {{assigning to 'int *' from incompatible type '_Array_ptr<int>'}}
    g2 = g3;  // expected-error {{expression has unknown bounds, cast to ptr<T> expects source to have bounds}}
    g4 = g3;
    g1 = h1;
    g2 = h2;
    g4 = h4;
  }
  // Semantic error present in the expression statement.
  g1 = g3,    // expected-error {{assigning to 'int *' from incompatible type '_Array_ptr<int>'}}
  g2 = g3,
  g4 = g3,
  g1 = h1,
  g2 = h2,
  g4 = h4;
}

void gf4(void) {
  _Bundled {
    g4 = g1;
    g4 = g2;
    g4 = g3;
    g4 = h4;
  }

  g4 = g1,
  g4 = g2,
  g4 = g3,
  g4 = h4;
}

extern void test_bsi_f1(int *p);
extern void test_bsi_f2(int *p  _Itype(int* _Single));
extern void test_bsi_f3(int *p  _Itype(int* _Array));
extern void test_bsi_f4(int *p  _Count(1));
extern void test_bsi_f5(int *p  _Count(len), int len);
extern void test_bsi_f6(int((*compar)(const int *, const int *))
                            _Itype(int (*_Single)(const int* _Single, const int* _Single)));
extern int test_cmp(const int* _Single a, const int* _Single b);

extern void check_call_bsi(int *arg1, int* _Single arg2, int* _Array arg3,
                           int* _Array arg4  _Count(1),
                           int* _Array arg5  _Count(arglen), int arglen) {

  _Bundled {
    test_bsi_f1(arg1);      // no checking expected when passing unchecked pointers.
    test_bsi_f2(arg1);
    test_bsi_f3(arg1);
    test_bsi_f4(arg1);
    test_bsi_f5(arg1, 1);
  }

  _Bundled {
    test_bsi_f1(arg2);     // expected-error {{incompatible type}}
    test_bsi_f5(arg2, 1);
  }
  test_bsi_f1(arg2),       // expected-error {{incompatible type}}
  test_bsi_f5(arg2, 1);

  _Bundled {
    test_bsi_f2(arg3);     // expected-error {{argument has unknown bounds}}
    test_bsi_f3(arg3);
    test_bsi_f4(arg3);     // expected-error {{argument has unknown bounds}}
    test_bsi_f5(arg3, 1);  // expected-error {{argument has unknown bounds}}
  }
  test_bsi_f2(arg3),       // expected-error {{argument has unknown bounds}}
  test_bsi_f3(arg3),
  test_bsi_f4(arg3),       // expected-error {{argument has unknown bounds}}
  test_bsi_f5(arg3, 1);    // expected-error {{argument has unknown bounds}}

  _Bundled {
    test_bsi_f1(arg4);     // expected-error {{incompatible type}}
    test_bsi_f2(arg4);
  }
  test_bsi_f1(arg4),       // expected-error {{incompatible type}}
  test_bsi_f2(arg4);

  _Bundled {
    test_bsi_f1(arg5);     // expected-error {{incompatible type}}
    test_bsi_f2(arg5);     // expected-error {{it is not possible to prove argument meets declared bounds for 1st parameter}}
    test_bsi_f3(arg5);
    test_bsi_f4(arg5);     // expected-error {{it is not possible to prove argument meets declared bounds for 1st parameter}}
    test_bsi_f5(arg5, 1);  // expected-error {{it is not possible to prove argument meets declared bounds for 1st parameter}}
  }
  // Semantic error present in the expression statement.
  test_bsi_f1(arg5),       // expected-error {{incompatible type}}
  test_bsi_f2(arg5),
  test_bsi_f3(arg5),
  test_bsi_f4(arg5),
  test_bsi_f5(arg5, 1);

   _Checked {
    _Bundled {
      test_bsi_f3(arg5);
      test_bsi_f6(test_cmp);
    }
  }

  _Bundled {
    int count = arglen - 1;
    test_bsi_f5(arg5, ++count);  // expected-error {{increment expression not allowed}}
    test_bsi_f5(arg5, count++);  // expected-error {{increment expression not allowed}}
  }
  int count = arglen - 1;
  test_bsi_f5(arg5, ++count),    // expected-error {{increment expression not allowed}}
  test_bsi_f5(arg5, count++);    // expected-error {{increment expression not allowed}}
}


struct Node {
  char* _Nt_array name;
  unsigned int age;
};

struct Group {
  struct Node* _Array list _Count(n);
  unsigned int n;
};

extern unsigned int my_strlen(char* _Nt_array);

// get the first name that starts with the letters 'Ar'
char* _Nt_array get_name(struct Group* _Array groups  _Count(gcnt), unsigned int gcnt)
_Checked {
  unsigned int n = 0;
  struct Node* _Array group  _Count(n) = 0;

  for (int i = 0; i < gcnt; i++) {
    _Bundled {
      group = groups[i].list;
      n = groups[i].n;
    }
    for (int j = 0; j < n; j++) {
      char* _Nt_array name = group[j].name;
      unsigned int m = my_strlen(name) _Where_M (name : count(m));
      if (m >= 2 && name[0] == 'A' && name[1] == 'r')
        return name;
    }
  }
  return "";
}
