// Feature tests of typechecking of uses of Checked C checked scopes,
// introduced by using #program checked_scope.
//
// The file is parameterized by the BOUNDS_ONLY #define to allow it to test
// regular checked scopes and checked scopes with modifiers that change
// the checking done within the scope.
//
// #pragmas may not be subjec to macro-replacement, so we conditional compilation
// to test the different arguments to #pragma.
//
// RUN: %clang_cc1 -Wno-unused-value -Wno-pointer-bool-conversion -verify -verify-ignore-unexpected=note -DBOUNDS_ONLY=0 %s
//

// Top level scope is a checked scope.
// - By default, global variables are checked type within a checked scope.
// - By default, all functions are checked type within a checked scope.
//   Function parameters & return SHOULD have checked pointer type
//   Function body is also checked scope within checked function
// - By default, all structure/union declarations is checked type
// - To declare function as undeclared, use unchecked function specifier

#include <stdchecked.h>

int printf(const char * restrict format  _Itype( const char* _Nt_array restrict ), ...);

// Test for pragma set/clear/set.
#if BOUNDS_ONLY
#pragma CHECKED_SCOPE _Bounds_only
#else
#pragma CHECKED_SCOPE ON
#endif

#pragma CHECKED_SCOPE OFF

#if BOUNDS_ONLY
#pragma CHECKED_SCOPE _Bounds_only
#else
#pragma CHECKED_SCOPE on
#endif

// Test for checked function in top-level checked scope.
// Check if paremeter, return, local variable is checked type
int* checked_func0(int* _Single p, int *q  _Itype(int* _Single), int r[], int* _Array s _Count(len), int len) { // expected-error {{return in a checked scope must have a checked type or a bounds-safe interface}} \
                                                                                                            // expected-error {{parameter in a checked scope must have a checked type or a bounds-safe interface}}
  int a = 5;
  int *upa = &a;  // expected-error {{local variable in a checked scope must have a checked type}}
  int *upb = q;   // expected-error {{local variable in a checked scope must have a checked type}}
  int* _Single pc = p;
  int* _Array pd = q;
  int e[5][5];    // expected-error {{local variable in a checked scope must have a checked type}}
  return upa;
}

unchecked int* unchecked_func0(int* _Single p, int *q  _Itype(int* _Single), int r[], int* _Array s _Count(len), int len) {
  int a = 5;
  int *upa = &a;
  int *upb = q;
  int* _Single pc = p;
  int* _Array pd = q;
  int e[5][5];
  return upa;
}


int* _Array checked_func1_checked_body(int *x, int *y) {   // expected-error 2 {{parameter in a checked scope must have a checked type or a bounds-safe interface}}
  int *upa = x; // expected-error {{local variable in a checked scope must have a checked type}}
  int *upb = y; // expected-error {{local variable in a checked scope must have a checked type}}
  return upb;
}

unchecked int* _Array unchecked_func1_checked_body(int *x, int *y) {
#if BOUNDS_ONLY
#pragma CHECKED_SCOPE _Bounds_only
#else
#pragma CHECKED_SCOPE ON
#endif

  int *upa = x; // expected-error {{local variable in a checked scope must have a checked type}} \
                // expected-error {{parameter used in a checked scope must have a checked type or a bounds-safe interface}}
  int *upb = y; // expected-error {{local variable in a checked scope must have a checked type}} \
                // expected-error {{parameter used in a checked scope must have a checked type or a bounds-safe interface}}
  return upb;
}

unchecked int* _Array unchecked_func1_unchecked_body(int *x, int *y) {
  int *upa = x;
  int *upb = y;
  return upb;
}

int* _Array checked_func1_unchecked_body(int *x, int *y) {  // expected-error 2 {{parameter in a checked scope must have a checked type or a bounds-safe interface}}
#pragma CHECKED_SCOPE OFF
  int *upa = x;
  int *upb = y;
  return upb;
}


int* checked_func2_checked_parm_checked_ret(int a checked[][5], int b checked[][5])  _Itype(int* _Single) {
}

int* checked_func2_unchecked_parm_checked_ret(int a [][5], int b [][5])  _Itype(int* _Single) { // expected-error 2 {{parameter in a checked scope must have a checked type or a bounds-safe interface}}
}

int* checked_func2_checked_parm_unchecked_ret(int a checked[][5], int b checked[][5]) {  // expected-error {{return in a checked scope must have a checked type or a bounds-safe interface}}
}

int* checked_func2_unchecked_parm_unchecked_ret(int a [][5], int b [][5]) { // expected-error 2 {{parameter in a checked scope must have a checked type or a bounds-safe interface}} \
                                                                            // expected-error {{return in a checked scope must have a checked type or a bounds-safe interface}}
}

int* checked_func_check_call(int *a  _Itype(int* _Single), int *b  _Itype(int* _Array))  _Itype(int* _Array) {
  int e checked[5][5];
  int f[5][5];                // expected-error {{local variable in a checked scope must have a checked type}}
  int *upa = checked_func2_checked_parm_checked_ret(e, e);    // expected-error {{local variable in a checked scope must have a checked type}}
  int* _Single pb = checked_func2_checked_parm_checked_ret(e, e);
  int* _Single pc = 0;
  pc = checked_func2_unchecked_parm_checked_ret(f, f);
  return upa;
}

int* checked_func_itype(int a[]  _Itype(int *), char b[]  _Itype(char *))  _Itype(int *) {// expected-error 3 {{type must be a checked type}}
}

// Test for checked function with mixed scope in top-level checked scope.
int * checked_func_u(int *p, int *q) unchecked {// expected-error {{return in a checked scope must have a checked type or a bounds-safe interface}} \
                                                // expected-error 2 {{parameter in a checked scope must have a checked type or a bounds-safe interface}}
  int a = 5;
  *p = *q = 0;
  int* _Single pa = &a;
  int b[5][5];
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      b[i][j] = -1;
    }
  }
  return 0;
}

int * checked_func_u_pragma(int *p, int *q) { // expected-error {{return in a checked scope must have a checked type or a bounds-safe interface}} \
                                              // expected-error 2 {{parameter in a checked scope must have a checked type or a bounds-safe interface}}
#pragma CHECKED_SCOPE OFF
  int a = 5;
  *p = *q = 0;
  int* _Single pa = &a;
  int b[5][5];
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      b[i][j] = -1;
    }
  }
  return 0;
}

int * checked_func_uc(int *p, int *q) unchecked { // expected-error {{return in a checked scope must have a checked type or a bounds-safe interface}} \
                                                  // expected-error 2 {{parameter in a checked scope must have a checked type or a bounds-safe interface}}
  int *upa;
  checked {
    int a = 5;
    *p = *q = 0;
    int* _Single pa = &a;
    int b[5][5];  // expected-error {{local variable in a checked scope must have a checked type}}
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        b[i][j] = -1;
      }
    }
  }
  return upa;
}

int * checked_func_uc_pragma(int *p, int *q) {// expected-error {{return in a checked scope must have a checked type or a bounds-safe interface}} \
                                              // expected-error 2 {{parameter in a checked scope must have a checked type or a bounds-safe interface}}
#pragma CHECKED_SCOPE OFF
  int *upa;
#if BOUNDS_ONLY
#pragma CHECKED_SCOPE _Bounds_only
#else
#pragma CHECKED_SCOPE ON
#endif
  int a = 5;
  *p = *q = 0;
  int* _Single pa = &a;
  int b[5][5];  // expected-error {{local variable in a checked scope must have a checked type}}
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      b[i][j] = -1;
    }
  }
#pragma CHECKED_SCOPE OFF
  return upa;
}

int * checked_func_uc1(int *p, int *q) unchecked {// expected-error {{return in a checked scope must have a checked type or a bounds-safe interface}} \
                                                  // expected-error 2 {{parameter in a checked scope must have a checked type or a bounds-safe interface}}
  int *upa;
  int a = 5;
  *p = *q = 0;
  int* _Single pa = &a;
  int b[5][5];
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) checked {
      b[i][j] = -1; // expected-error {{local variable used in a checked scope must have a checked type}}
    }
  }
  checked {
  return upa; // expected-error {{local variable used in a checked scope must have a checked type}}
  }
}

int * checked_func_uc1_pragma(int *p, int *q) { // expected-error {{return in a checked scope must have a checked type or a bounds-safe interface}} \
                                                // expected-error 2 {{parameter in a checked scope must have a checked type or a bounds-safe interface}}
#pragma CHECKED_SCOPE OFF
  int *upa;
  int a = 5;
  *p = *q = 0;
  int* _Single pa = &a;
  int b[5][5];
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
#if BOUNDS_ONLY
#pragma CHECKED_SCOPE _Bounds_only
#else
#pragma CHECKED_SCOPE ON
#endif

      b[i][j] = -1; // expected-error {{local variable used in a checked scope must have a checked type}}
    }
  }
#if BOUNDS_ONLY
#pragma CHECKED_SCOPE _Bounds_only
#else
#pragma CHECKED_SCOPE ON
#endif
  return upa; // expected-error {{local variable used in a checked scope must have a checked type}}
}

int* _Single checked_func_u1(int *p, int* _Single q, int* _Array r, int* _Array s _Count(2)) { // expected-error {{parameter in a checked scope must have a checked type or a bounds-safe interface}}
  int a = 5;
  *p = 1;
  *q = 2;
  *r = 3; // expected-error {{expression has unknown bounds}}
  *s = 4;
  unchecked {
    int* _Single pa = &a;
    int b checked[5][5];
    for (int i = 0; i < 5; i++) checked {
      for (int j = 0; j < 5; j++) unchecked {
        b[i][j] += *q + *r; // expected-error {{expression has unknown bounds}}
      }
      b[i][4] += *p + *q + *r + *s;
    }
  }
  return q;
}

int* _Single checked_func_u1_pragma(int *p, int* _Single q, int* _Array r, int* _Array s _Count(2)) {// expected-error {{parameter in a checked scope must have a checked type or a bounds-safe interface}}
  int a = 5;
  *p = 1;
  *q = 2;
  *r = 3; // expected-error {{expression has unknown bounds}}
  *s = 4;
#pragma CHECKED_SCOPE OFF
  int* _Single pa = &a;
  int b checked[5][5];
  for (int i = 0; i < 5; i++) {
#if BOUNDS_ONLY
#pragma CHECKED_SCOPE _Bounds_only
#else
#pragma CHECKED_SCOPE ON
#endif
    for (int j = 0; j < 5; j++) {
#pragma CHECKED_SCOPE OFF
      b[i][j] += *q + *r; // expected-error {{expression has unknown bounds}}
    }
    b[i][4] += *p + *q + *r + *s;
  }
#if BOUNDS_ONLY
#pragma CHECKED_SCOPE _Bounds_only
#else
#pragma CHECKED_SCOPE ON
#endif
  return q;
}

int * checked_func_uc(void)  _Itype(int* _Array) unchecked {
  int a = 5;
  int *upa = &a;
  int b[5][5];
  checked {
    int c checked[5][5];
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        b[i][j] += c[i][j];   // expected-error {{local variable used in a checked scope must have a checked type}}
      }
    }
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) unchecked {
        c[i][j] += b[i][j];
      }
    }
  }
  return upa;
}

int * checked_func_uc_pragma(void)  _Itype(int* _Array) {
#pragma CHECKED_SCOPE OFF
  int a = 5;
  int *upa = &a;
  int b[5][5];
#if BOUNDS_ONLY
#pragma CHECKED_SCOPE _Bounds_only
#else
#pragma CHECKED_SCOPE ON
#endif
  int c checked[5][5];
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      b[i][j] += c[i][j];   // expected-error {{local variable used in a checked scope must have a checked type}}
    }
  }
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
#pragma CHECKED_SCOPE OFF
      c[i][j] += b[i][j];
    }
  }
#pragma CHECKED_SCOPE OFF
  return upa;
}

int checked_func_ucu1(int *p, int *q, int *r  _Itype(int* _Single), int *s  _Itype(int* _Array)) unchecked { // expected-error 2 {{parameter in a checked scope must have a checked type or a bounds-safe interface}}
  int sum = 0;
  int a = 5;
  int *upa = &a;
  int b[5][5];
  for (int i = 0; i < 5; i++) checked {
    sum += b[i][0] + *upa; // expected-error 2 {{local variable used in a checked scope must have a checked type}}
    for (int j = 1; j < 5; j++) unchecked {
      sum += b[i][j];
    }
  }
  return sum;
}

int checked_func_ucu1_pragma(int *p, int *q, int *r  _Itype(int* _Single), int *s  _Itype(int* _Array)) { // expected-error 2 {{parameter in a checked scope must have a checked type or a bounds-safe interface}}
#pragma CHECKED_SCOPE OFF
  int sum = 0;
  int a = 5;
  int *upa = &a;
  int b[5][5];
  for (int i = 0; i < 5; i++) {
#if BOUNDS_ONLY
#pragma CHECKED_SCOPE _Bounds_only
#else
#pragma CHECKED_SCOPE ON
#endif
    sum += b[i][0] + *upa; // expected-error 2 {{local variable used in a checked scope must have a checked type}}
    for (int j = 1; j < 5; j++) {
#pragma CHECKED_SCOPE OFF
      sum += b[i][j];
    }
  }
  return sum;
}

int checked_func_ucu2(int* _Single p, int *q) unchecked {  // expected-error {{parameter in a checked scope must have a checked type or a bounds-safe interface}}
  int a = 5;
  int *upa;
  int* _Array pc;
  upa = pc;   // expected-error {{assigning to 'int *' from incompatible type '_Array_ptr<int>'}}
  checked {
    upa = &a; // expected-error {{local variable used in a checked scope must have a checked type}}
    pc = &a;
    unchecked {
      upa = &a;
      pc = &a;
    }
    upa = &a; // expected-error {{local variable used in a checked scope must have a checked type}}
    pc = &a;
  }
  int* _Single pb = p;
  int *upc;
  int *upd;
  return *pb;
}

int checked_func_ucu2_pragma(int* _Single p, int *q) {// expected-error {{parameter in a checked scope must have a checked type or a bounds-safe interface}}
#pragma CHECKED_SCOPE OFF
  int a = 5;
  int *upa;
  int* _Array pc;
  upa = pc; // expected-error {{assigning to 'int *' from incompatible type '_Array_ptr<int>'}}
#if BOUNDS_ONLY
#pragma CHECKED_SCOPE _Bounds_only
#else
#pragma CHECKED_SCOPE ON
#endif
  upa = &a; // expected-error {{local variable used in a checked scope must have a checked type}}
  pc = &a;
#pragma CHECKED_SCOPE OFF
  upa = &a;
  pc = &a;
#if BOUNDS_ONLY
#pragma CHECKED_SCOPE _Bounds_only
#else
#pragma CHECKED_SCOPE ON
#endif
  upa = &a; // expected-error {{local variable used in a checked scope must have a checked type}}
  pc = &a;
#pragma CHECKED_SCOPE OFF
  int* _Single pb = p;
  int *upc;
  int *upd;
  return *pb;
}

// Test for unchecked function specifier with top-level scope pragma.
unchecked int * unchecked_func3_unchecked_body(int *p, int q[]) {
  int a = 5;
  *p = *q = 0;
  int* _Single pa = &a;
  int b[5][5];
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      b[i][j] = -1;
    }
  }
  return 0;
}

unchecked int * unchecked_func3_checked_body(int *p, int q[]) {
#if BOUNDS_ONLY
#pragma CHECKED_SCOPE _Bounds_only
#else
#pragma CHECKED_SCOPE ON
#endif
  int a = 5;
  *p = *q = 0;  // expected-error 2 {{parameter used in a checked scope must have a checked type or a bounds-safe interface}}
  int* _Single pa = &a;
  int b[5][5];  // expected-error {{local variable in a checked scope must have a checked type}}
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      b[i][j] = -1;
    }
  }
  return 0;
}

unchecked int* _Array unchecked_func4_unchecked_body(int p[], int q[10], int r[][10]) {
  int *upa;
  int *upb;
  int* _Array pb;
  int* _Single pc = 0;
  upa = pb;   // expected-error {{assigning to 'int *' from incompatible type '_Array_ptr<int>'}}
  upa = pc;   // expected-error {{assigning to 'int *' from incompatible type '_Ptr<int>'}}
  upb = pb;   // expected-error {{assigning to 'int *' from incompatible type '_Array_ptr<int>'}}
  upb = pc;   // expected-error {{assigning to 'int *' from incompatible type '_Ptr<int>'}}
  return upb; // int* _Array = int *, OK
}

unchecked int* _Array unchecked_func4_checked_body(int p[], int q[10], int r[][10]) {
#if BOUNDS_ONLY
#pragma CHECKED_SCOPE _Bounds_only
#else
#pragma CHECKED_SCOPE ON
#endif
  int *upa;   // expected-error {{local variable in a checked scope must have a checked type}}
  int *upb;   // expected-error {{local variable in a checked scope must have a checked type}}
  int* _Array pb;
  int* _Single pc = 0;
  upa = pb;
  upa = pc;
  upb = pb;
  upb = pc;
  return upb;
}

unchecked int * unchecked_func_cu(int *p, int* _Single q, int* _Array r, int* _Array s _Count(2)) {
#if BOUNDS_ONLY
#pragma CHECKED_SCOPE _Bounds_only
#else
#pragma CHECKED_SCOPE ON
#endif
  int a = 5;
  *p = 1; // expected-error {{parameter used in a checked scope must have a checked type or a bounds-safe interface}}
  *q = 2;
  *r = 3; // expected-error {{expression has unknown bounds}}
  *s = 4;
#pragma CHECKED_SCOPE OFF
  int* _Single pa = &a;
  int b checked[5][5];
  int c[5][5];
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      b[i][j] = *p + *q + *r + *s;  // expected-error {{expression has unknown bounds}}
    }
  }
  return 0;
}

unchecked int unchecked_func_cu2(int *p, int *q, int *r  _Itype(int* _Single), int *s  _Itype(int* _Array)) {
  int sum = 0;
  int a = 5;
  int *upa = &a;
  int b[5][5];
  for (int i = 0; i < 5; i++) {
#if BOUNDS_ONLY
#pragma CHECKED_SCOPE _Bounds_only
#else
#pragma CHECKED_SCOPE ON
#endif
    sum += b[i][0] + *upa; // expected-error 2 {{local variable used in a checked scope must have a checked type}}
    for (int j = 1; j < 5; j++) {
#pragma CHECKED_SCOPE OFF
      sum += b[i][j];
    }
  }
  return sum;
}

typedef int unchecked_arr_type[10];
typedef int checked_arr_type[10];

// Test for global variables in top-level checked scope
int *a1  _Itype(int* _Single) = 0;
int *a2  _Itype(int* _Array) = 0;
int **a3  _Itype(int*_Single*_Single) = 0;
int **a4  _Itype(int*_Array*_Single) = 0;
int **a5  _Itype(int*_Single*_Array) = 0;
int **a6  _Itype(int*_Array*_Array) = 0;
int ***a7  _Itype(int*_Single*_Single*_Single) = 0;
int a8[10]  _Itype(int checked[10]);
extern int a9[]  _Itype(int checked[]);
#pragma CHECKED_SCOPE OFF
int *a10;
#if BOUNDS_ONLY
#pragma CHECKED_SCOPE _Bounds_only
#else
#pragma CHECKED_SCOPE ON
#endif
int **a11;        // expected-error {{global variable in a checked scope must have a checked type or a bounds-safe interface}}
#pragma CHECKED_SCOPE OFF
int ***a12;
#if BOUNDS_ONLY
#pragma CHECKED_SCOPE _Bounds_only
#else
#pragma CHECKED_SCOPE ON
#endif
int a13[10];      // expected-error {{global variable in a checked scope must have a checked type or a bounds-safe interface}}
#pragma CHECKED_SCOPE OFF
extern int a14[];

#if BOUNDS_ONLY
#pragma CHECKED_SCOPE _Bounds_only
#else
#pragma CHECKED_SCOPE ON
#endif
int global[10]; // expected-error {{global variable in a checked scope must have a checked type or a bounds-safe interface}}
int checked_global checked[10];
int global_arr1[10];  // expected-error {{global variable in a checked scope must have a checked type or a bounds-safe interface}}

// Test for structure members in top-level checked scope
struct S0 {
  float *data1  _Itype(ptr<float>);
  float *data2  _Itype(array_ptr<float>);
  float **data3  _Itype(ptr<ptr<float>>);
  float **data4  _Itype(ptr<array_ptr<float>>);
  float **data5  _Itype(array_ptr<ptr<float>>);
  float ***data6  _Itype(ptr<ptr<ptr<float>>>);
  float data7[4]  _Itype(float checked[4]);
  float data8[]  _Itype(float checked[]);   // expected-error {{flexible array member 'data8' with type 'float []' is not at the end of struct}}
  float data9[];  // expected-error {{flexible array member 'data9' with type 'float []' is not at the end of struct}}
  float data10[4];// expected-error {{member in a checked scope must have a checked type or a bounds-safe interface}}
  float *data11;  // expected-error {{member in a checked scope must have a checked type or a bounds-safe interface}}
  float **data12; // expected-error {{member in a checked scope must have a checked type or a bounds-safe interface}}
  float ***data13;// expected-error {{member in a checked scope must have a checked type or a bounds-safe interface}}
  float *data14  _Itype(float *); // expected-error {{type must be a checked type}}
};

typedef struct _S1 {
#if BOUNDS_ONLY
#pragma CHECKED_SCOPE _Bounds_only
#else
#pragma CHECKED_SCOPE ON
#endif
  int *a;       // expected-error {{member in a checked scope must have a checked type or a bounds-safe interface}}
#pragma CHECKED_SCOPE OFF
#if BOUNDS_ONLY
#pragma CHECKED_SCOPE _Bounds_only
#else
#pragma CHECKED_SCOPE ON
#endif
  char *b;      // expected-error {{member in a checked scope must have a checked type or a bounds-safe interface}}
  int* _Single pc;
  int* _Array pd _Count(len);
#pragma CHECKED_SCOPE OFF
#if BOUNDS_ONLY
#pragma CHECKED_SCOPE _Bounds_only
#else
#pragma CHECKED_SCOPE ON
#endif
  short e[10];  // expected-error {{member in a checked scope must have a checked type or a bounds-safe interface}}
  char f[10];   // expected-error {{member in a checked scope must have a checked type or a bounds-safe interface}}
  int len;
} S1;

typedef struct _S2 {
#pragma CHECKED_SCOPE OFF
#if BOUNDS_ONLY
#pragma CHECKED_SCOPE _Bounds_only
#else
#pragma CHECKED_SCOPE ON
#endif
#pragma CHECKED_SCOPE OFF
  int *a;
  char *b;
  int* _Single pc;
  int* _Array pd _Count(len);
  short e[10];
  char f[10];
  int len;
} S2;

int checked_func_with_checked_struct(void) {
  struct local_checked_s {
#pragma CHECKED_SCOPE OFF
#if BOUNDS_ONLY
#pragma CHECKED_SCOPE _Bounds_only
#else
#pragma CHECKED_SCOPE ON
#endif
    int *a;       // expected-error {{member in a checked scope must have a checked type or a bounds-safe interface}}
    char *b;      // expected-error {{member in a checked scope must have a checked type or a bounds-safe interface}}
    int* _Single pc;
    int* _Array pd _Count(len);
    short e[10];  // expected-error {{member in a checked scope must have a checked type or a bounds-safe interface}}
    char f[10];   // expected-error {{member in a checked scope must have a checked type or a bounds-safe interface}}
    int len;
  } a; // expected-error {{containing a checked pointer must have an initializer}}
  return 0;
}

int checked_func_with_unchecked_struct(void) {
  struct local_unchecked_s {
#pragma CHECKED_SCOPE OFF
    int *a;
    char *b;
    int* _Single pc;
    int* _Array pd _Count(len);
    short e[10];
    char f[10];
    int len;
  } a; // expected-error {{containing a checked pointer must have an initializer}}
  typedef struct _S {
#if BOUNDS_ONLY
#pragma CHECKED_SCOPE _Bounds_only
#else
#pragma CHECKED_SCOPE ON
#endif
    int *a; // expected-error {{member in a checked scope must have a checked type or a bounds-safe interface}}
  } S;
  typedef struct _SS {
#pragma CHECKED_SCOPE OFF
    int *a;
  } *SS;
  S b;
  SS c; // expected-error {{local variable in a checked scope must have a checked type}}
  return 0;
}

#pragma CHECKED_SCOPE OFF
extern void variadic_func0(int cnt, ...) {
  for (int i = 0; i < cnt; i++) {}
}

extern void variadic_func1(int cnt, int* _Single p, int* _Array q, ...) {
}

extern void variadic_func2(int cnt, int* _Single p, ...) {
}

#if BOUNDS_ONLY
#pragma CHECKED_SCOPE _Bounds_only
#else
#pragma CHECKED_SCOPE ON
#endif
extern void variadic_func3(int cnt, ...) {  // expected-error {{variable arguments function cannot be made}}
}

extern void variadic_func4(int cnt, int* _Single p, int* _Array q, ...) {  // expected-error {{variable arguments function cannot be made}}
}

extern void variadic_func5(int cnt, int* _Single p, ...) {  // expected-error {{variable arguments function cannot be made}}
}


#if BOUNDS_ONLY
#pragma CHECKED_SCOPE _Bounds_only
#else
#pragma CHECKED_SCOPE ON
#endif
void checked_func_call_variadic(void) {
  int* _Single a = 0;
  int* _Array b;
  printf("check function call variadic %d\n", *a);
#pragma CHECKED_SCOPE OFF
  printf("check function call variadic %d\n", *a);
#if BOUNDS_ONLY
#pragma CHECKED_SCOPE _Bounds_only
#else
#pragma CHECKED_SCOPE ON
#endif
  printf("check function call variadic %d\n", *a);
#if BOUNDS_ONLY
#pragma CHECKED_SCOPE _Bounds_only
#else
#pragma CHECKED_SCOPE ON
#endif
  variadic_func0(10); // expected-error {{cannot use this variable arguments function in a checked scope or function}}
  variadic_func1(10, a, b); // expected-error {{cannot use this variable arguments function in a checked scope or function}}
  variadic_func2(10, a, a, a); // expected-error {{cannot use this variable arguments function in a checked scope or function}}
}

#if BOUNDS_ONLY
#pragma CHECKED_SCOPE _Bounds_only
#else
#pragma CHECKED_SCOPE ON
#endif
void (*_Single fptr0)(int , ...);                             // expected-error {{variable in a checked scope cannot have variable arguments}}
void (*_Single fptr1)(int , int* _Single, int* _Array , ...);  // expected-error {{variable in a checked scope cannot have variable arguments}}
void (*fptr2)(int , ...); // expected-error {{global variable in a checked scope must have a checked type or a bounds-safe interface}} \
                          // expected-error {{global variable in a checked scope cannot have variable arguments}}

#pragma CHECKED_SCOPE OFF
typedef struct _S3 {
#if BOUNDS_ONLY
#pragma CHECKED_SCOPE _Bounds_only
#else
#pragma CHECKED_SCOPE ON
#endif
#pragma CHECKED_SCOPE OFF
#if BOUNDS_ONLY
#pragma CHECKED_SCOPE _Bounds_only
#else
#pragma CHECKED_SCOPE ON
#endif
  int ((*gptr0)(int, ...));                               // expected-error {{member in a checked scope must have a checked type or a bounds-safe interface}} \
                                                          // expected-error {{member in a checked scope cannot have variable arguments}}
  int* (*_Single gptr1)(int, int* _Single, int* _Array, ...);    // expected-error {{member in a checked scope must have a pointer, array or function type that uses only checked types or parameter/return types with bounds-safe interfaces}} \
                                                          // expected-error {{member in a checked scope cannot have variable arguments}}
  int (*_Single gptr2)(int, int* _Single, ...);                     // expected-error {{member in a checked scope cannot have variable arguments}}
  int (*_Single gptr3)(int, ...);                               // expected-error {{member in a checked scope cannot have variable arguments}}
  int (*_Single gptr4)(int, int (*_Single)(int, int (*_Single)(int, ...))); // expected-error {{member in a checked scope cannot have variable arguments}}
} S3;

#if BOUNDS_ONLY
#pragma CHECKED_SCOPE _Bounds_only
#else
#pragma CHECKED_SCOPE ON
#endif
void checked_check_variadic(void) {
  int* _Single a = 0;
  int* _Array b;
  fptr0 = &variadic_func0; // expected-error {{cannot use this variable arguments function in a checked scope or function}}
  fptr1 = &variadic_func1; // expected-error {{cannot use this variable arguments function in a checked scope or function}}
  fptr2 = &variadic_func2; // expected-error {{cannot use this variable arguments function in a checked scope or function}}

  (*fptr0)(5, a, a, b, b, b);
  (*fptr1)(5, a, b, a, a, b);
}

void check_checked_constructed_type_variadic(void) {
#if BOUNDS_ONLY
#pragma CHECKED_SCOPE _Bounds_only
#else
#pragma CHECKED_SCOPE ON
#endif
  int* _Single pa = 0;
  int* _Array apa;

  void (*_Single a)(int,int) = 0;
  int (*_Single b)(int,int) = 0;
  int (*_Single c)(int, int* _Single, ...) = 0;           // expected-error {{variable in a checked scope cannot have variable arguments}}
  int (*_Single d)(int, int(*_Single)(int, ...)) = 0;      // expected-error {{variable in a checked scope cannot have variable arguments}}
  int (*_Single e)(int, int(*_Single)(int,int), ...) = 0;  // expected-error {{variable in a checked scope cannot have variable arguments}}
  int (*_Single f)(int, int(*_Single)(int,...), ...) = 0;  // expected-error {{variable in a checked scope cannot have variable arguments}}
  int (*g)(int, ...);                           // expected-error {{local variable in a checked scope must have a checked type}} \\
                                                // expected-error {{local variable in a checked scope cannot have variable arguments}}
  int* (*_Single h)(int, int) = 0;                    // expected-error {{local variable in a checked scope must have a pointer, array or function type that uses only checked types or parameter/return types with bounds-safe interface}}

#pragma CHECKED_SCOPE OFF
  {
  int (*_Single var_b)(int,...)= 0;
  int (*_Single var_c)(int,int* _Single,...) = 0;
  int (*_Single var_d)(int,int(*_Single)(int,...)) = 0;
  int (*_Single var_e)(int,int (*_Single)(int,int* _Single,...), ...)= 0;
  int (*_Single var_f)(int, int (*_Single)(int,int (*_Single)(int,...)),int (*_Single)(int,int (*_Single)(int,int* _Single,...), ...),...) = 0;
  int (*g)(int, ...);
  int* (*_Single h)(int, int) = 0;
  (*var_c)(2, pa, pa);
  (*var_d)(3, var_b);
  (*var_e)(4, var_c, var_d);
  (*var_f)(6, var_d, var_e, a, b, g, h);
  (*h)(10,10);
#if BOUNDS_ONLY
#pragma CHECKED_SCOPE _Bounds_only
#else
#pragma CHECKED_SCOPE ON
#endif
  (*var_c)(2, pa, pa);  // expected-error {{variable used in a checked scope cannot have variable arguments}}
  (*var_d)(3, var_b);   // expected-error 2 {{variable used in a checked scope cannot have variable arguments}}
  (*var_e)(4, var_c, var_d);  // expected-error 3 {{variable used in a checked scope cannot have variable arguments}}
  (*var_f)(6, var_d, var_e, a, b, g, h);  // expected-error 4 {{variable used in a checked scope cannot have variable arguments}} \
                                          // expected-error {{local variable used in a checked scope must have a checked type}} \
                                          // expected-error {{must have a pointer, array or function type that uses only checked types or parameter/return types with bounds-safe interfaces}}

  }
}

// Test push/pop functionality.   We should still be within a checked scope
// after the push/pop

#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE off
int *g1 ;
#pragma CHECKED_SCOPE pop
int *g2; // expected-error {{global variable in a checked scope must have a checked type}}

// Check for an unmatched pop.
#pragma CHECKED_SCOPE pop // expected-error {{no matching #pragma CHECKED_SCOPE push}}

// Check that state is preserved with a sequence of pushes followed by pops.
#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE off
int *g3;
#pragma CHECKED_SCOPE push
int *g4;
#pragma CHECKED_SCOPE on
int *g5;   // expected-error {{global variable in a checked scope must have a checked type}}
#pragma CHECKED_SCOPE push
int *g6;   // expected-error {{global variable in a checked scope must have a checked type}}
#pragma CHECKED_SCOPE off
#pragma CHECKED_SCOPE pop
int *g7;    // expected-error {{global variable in a checked scope must have a checked type}}
#pragma CHECKED_SCOPE pop
int *g8;
#pragma CHECKED_SCOPE pop
int *g9;    // expected-error {{global variable in a checked scope must have a checked type}}

#pragma CHECKED_SCOPE push
#pragma CHECKED_SCOPE off
void pop_in_f1(void) {
  int *a;
#pragma CHECKED_SCOPE pop
  int *b; // expected-error {{local variable in a checked scope must have a checked type}}
}

// Pushing within a scope pushes the checked state of the scope.
void push_in_f1(void) _Checked {
  _Unchecked {
#pragma CHECKED_SCOPE push
  }
#pragma CHECKED_SCOPE pop
  int *a;   // This would be in a checked scope if it weren't for the pop.
}
#pragma CHECKED_SCOPE on

void no_push_in_f1(void) {
  _Unchecked {
  }
  int *a;  // expected-error {{local variable in a checked scope must have a checked type}}
}

#pragma CHECKED_SCOPE push   // expected-error {{unterminated '#pragma CHECKED_SCOPE push' at end of f}}
