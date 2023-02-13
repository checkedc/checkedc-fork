// Feature tests of parsing new Checked C bounds declarations.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -Wno-check-bounds-decls -verify %s

#include <stdchecked.h>

// Top int* _Arraylevel declarations with different storage classes and
// storage classes omitted.
extern int* _Array a _Count(5);
extern int* _Array b  _Count(3 + 2);
extern int cLen;
extern int* _Array c  _Count(cLen);
extern int* _Array d  _Byte_count(20);
extern int* _Array e  _Byte_count(5 * sizeof(int));
extern int* _Array f  _Byte_count(cLen * sizeof(int));
extern int* _Array g  _Bounds(unknown);
extern int* _Array h  _Bounds(f - 2, f + 3);
extern int* _Array i  _Count(5),* _Array j  _Bounds(i - 2, i + 3),
    * _Array k  _Bounds(j + 2, j + 5);

static float vals[5] = { 0.0, 1.0, 2.0, 3.0, 4.0 };
static float* _Array l  _Count(5) = vals;
static _Thread_local int* _Array m  _Count(5);

int* _Array b  _Count(3 + 2) = 0;
int* _Array d  _Byte_count(20) = 0;
int* _Array g  _Bounds(unknown) = 0;

// Declare a bounds for a checked array.  This is legal, but
// unnecessary.
char buf checked[128]  _Count(128);
char* _Array cursor  _Bounds(buf, buf + 128) = buf + 64;

// Local variable declarations.
extern void f1(int* _Array arr  _Count(5)) {
  int* _Array p  _Count(5) = arr;
  int* _Array q  _Count(3 + 2) = arr;
  int len = 5;
  int* _Array r  _Count(len) = arr;
  int* _Array s  _Byte_count(20) = arr;
  int* _Array t  _Byte_count(5 * sizeof(int)) = arr;
  int* _Array u  _Byte_count(len * sizeof(int)) = arr;
  int* _Array v  _Bounds(v, v + 5) = arr;
  int* _Array w  _Bounds(unknown) = arr;
  int* _Array x  _Bounds(unknown);
  int* _Array midarr  _Bounds(midarr - 1, midarr - 1 + 2) = arr + 2;

  static int* _Array cache1  _Count(5);
  static int* _Array cache1_ptr  _Bounds(cache1 - 2, cache1 + 3);
}

// Multiple declarators in one declaration
extern void f2(int* _Array arr  _Count(5)) {
  int* _Array p  _Count(5) = 0, * _Array q  _Count(3 + 2) = arr;
  int len = 5;
  int* _Array r  _Count(len) = arr, * _Array s  _Byte_count(20) = arr;
  int* _Array t  _Byte_count(5 * sizeof(int)) = arr,
                                * _Array u  _Bounds(u, u + 5) = arr,
                                * _Array v  _Bounds(unknown) = arr;
}

// Parenthesized subexpressions
extern void f3(int* _Array arr  _Count(5)) {
  int* _Array p _Count((5)) = arr;
  int* _Array q  _Count(3 + (2)) = arr;
  int len = 5;
  int* _Array r _Count((len)) = arr;
  int* _Array s  _Byte_count((20)) = arr;
  int* _Array t  _Byte_count(5 * (sizeof(int))) = arr;
  int* _Array u  _Byte_count((len) * sizeof(int)) = arr;
  int* _Array v  _Bounds(v, (v + 5) + len - len) = arr;
  int* _Array w _Bounds((w + len - (len)), (w + len)) = arr;
  int* _Array midarr  _Bounds(midarr - 1, (midarr - 1) + 2) = arr + 2;
}


// Contextual keywords
extern void f4(int* _Array arr  _Count(len), int len) {
  int count = len;
  int* _Array p  _Count(count) = arr;
  int unknown = 0;
  // unknown is a contextual a keyword if it follows 'bounds' '('
  int* _Array q  _Bounds(unknown) = arr;
  int* _Array r  _Bounds(unknown + arr, arr + len) = arr; // expected-error {{expected ')'}} expected-note {{to match this '('}}
  int* _Array s  _Bounds(arr + unknown, arr + len) = arr;
  int* _Array t  _Bounds(t, t + count) = arr;
  int bounds = len;
  int* _Array u  _Bounds(u, u + bounds) = arr;
}

// Checked array declarations
extern void f5(void) {
  int arr1 checked[5]  _Count(5);
  int arr2 checked[5] _Count(2 + 3);
  int arr3 checked[6]  _Count(5);
  int len = 5;
  int arr4 checked[5]  _Count(len);
  int arr7 checked[5]  _Byte_count(5 * sizeof(int));
  int arr8 checked[5]  _Byte_count(len * sizeof(int));
  auto int arr5 checked[5]  _Bounds(arr5, arr5 + 5);
  auto int arr6 checked[5]  _Bounds(arr5, arr5 + len);

  static int cache checked[5]  _Count(5);
  static int* _Array cache_ptr  _Bounds(cache - 2, cache + 3);
}

// Parsing of more complicated declarations with bounds declarations
// 
extern void f6(int *arr checked[]  _Count(5)) {
  array_ptr<int *> p  _Count(5) = arr;
  array_ptr<int *> q  _Count(3 + 2) = arr;
  int len = 5;
  array_ptr<int *> r  _Count(len) = arr;
  array_ptr<int *> s  _Byte_count(5 * sizeof(int)) = arr;
  array_ptr<int *> t  _Byte_count(len * sizeof(int)) = arr;
  // redundant but legal bounds expressions on arrays.
  int *arr1 checked[5]  _Count(5);
  int *arr2 checked[5]  _Count(3 + 2);
  int *arr3 checked[5]  _Count(len);
  // Checked array of unchecked pointers to functions.  We have to parenthesize
  // the inner declarator to avoid  _Count(len) being parsed as part of the 
  // type of arr, not a bounds expression.
  int ((*arr4 checked[5])(int, int))  _Count(5);
  int ((*arr5 checked[5])(int, int))  _Count(len);
  int ((*arr6 checked[5])(int, int))  _Bounds(arr5, arr5 + len);
  // Checked array of checked pointers to functions
  typedef int (*_Single func_arr7)(int, int);
  func_arr7 arr7  checked[5]  _Count(5)  = {0};
  typedef int (*_Single func_arr8)(int, int);
  func_arr8 arr8 checked[5] _Bounds(arr8, arr8 + 5) = {0};
  // Array_ptrs to checked pointers to functions.
 typedef int (*_Single func_u)(int, int);
 typedef int (*_Single func_v)(int, int);
 typedef int (*_Single func_w)(int, int);
 typedef int (*_Single func_x)(int len, int* _Array _Count(len));

  func_u* _Array u _Count(5) = 0;
  func_v* _Array v _Bounds(v, v + 5) = 0;
  func_w* _Array w _Bounds(v, v + 5) = 0;
  func_x* _Array x _Bounds(x, x + 5) = 0;
}

// Bounds expressions with incorrect syntax or semantics
extern void f7(int* _Array arr  _Count(5)) {
  int* _Array p  _Bounds(start, end + 5) = 0; // expected-error {{undeclared identifier 'start'}} expected-error {{undeclared identifier 'end'}}
  int* _Array q  _Count(len) = 0;       // expected-error {{undeclared identifier 'len'}}
  int* _Array r  _Byte_count(len) = 0;  // expected-error {{undeclared identifier 'len'}}
  int* _Array s : 6 + 6 = 0;            // expected-error {{expected bounds expression}}
  int* _Array t : 6 + 6, * _Array u  _Count(5) = arr; // expected-error {{expected bounds expression}}
  int* _Array v  _Boounds(a, b + 5) = 0;  // expected-error {{expected ';' at end of declaration}}
  int* _Array w  _Coount(5) = 0;         // expected-error {{expected ';' at end of declaration}}
  typedef int (*_Single func_xx)(int len, int* _Array _Count(badvar)); // expected-error {{undeclared identifier 'badvar'}}
  func_xx *_Array x
     _Bounds(x, x + 5) = 0;
}

