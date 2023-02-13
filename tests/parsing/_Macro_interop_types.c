// Feature tests of parsing Checked C interop annotations that declare
// an alternate checked pointer type for a variable or member with
// unchecked pointer type.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify %s

#include <stdchecked.h>

//
// parameters with interop type annotations
//

// first parameter has interop type annotation
extern void f1(int *p _Itype(int* _Single), int y) {
   *p = y;
}

extern void f2(int *p _Itype(int* _Array), int y) {
}

extern void f3(int *p _Itype(int checked[]), int y) {
}

extern void f4(int *p _Itype(int checked[10]), int y) {
}

extern void f5(int **p _Itype(int* _Single *_Single), int y) {
  **p = y;
}

extern void f6(int **p _Itype(int *_Single  *_Array), int y) {
}

extern void f7(int **p _Itype(int* _Single checked[]), int y) {
}

extern void f8(int **p _Itype(int * checked[10]), int y) {
}

// Second parameter has interop type annotation
extern void g1(int y, int *p _Itype(int* _Single)) {
   *p = y;
}

extern void g2(int y, int *p _Itype(int* _Array)) {
}

extern void g3(int y, int *p _Itype(int checked[])) {
}

extern void g4(int y, int *p _Itype(int checked[10])) {
}

extern void g5(int y, int **p _Itype(int* _Single *_Single)) {
   y = **p;
}

extern void g6(int y, int **p _Itype(int *_Array *_Single)) {
}

extern void g7(int y, int **p _Itype(int *_Single  *_Array)) {
}

extern void g8(int y, int **p _Itype(int* _Single checked[])) {
}

extern void g9(int y, int **p _Itype(int * checked[10])) {
}
//
// returns an unchecked pointer type with an
// interop type annotation.
//

extern int *h1(int y, int* _Single p) _Itype(int* _Single) {
   *p = y;
   return 0;
}

extern int *h2 (int y, const int* _Single p) _Itype(int* _Array) {
   return 0;
}

extern int **h3(void) _Itype(int* _Single *_Single) {
   return 0;
}

extern int **h4(void) _Itype(int *_Single  *_Array) {
   return 0;
}

//
// Global variables with interop type annotations
//

int *a1 _Itype(int* _Single) = 0;
int *a2 _Itype(int* _Array) = 0;
int **a3 _Itype(int* _Single *_Single) = 0;
int **a4 _Itype(int *_Array *_Single) = 0;
int **a5 _Itype(int *_Single  *_Array) = 0;
int **a6 _Itype(array_ptr<array_ptr<int>>) = 0;
int ***a7 _Itype(ptr<ptr<ptr<int>>>) = 0;
int a8[10] _Itype(int checked[10]);
extern int a9[] _Itype(int checked[]);
//
// Structure members with interop pointer type annotations
//

struct S1 {
  float *data1 _Itype(float* _Single);
  float *data2 _Itype(float* _Array);
  float **data3 _Itype(float *_Single *_Single);
  float **data4 _Itype(float *_Array *_Single);
  float **data5 _Itype(float *_Single *_Array);
  float ***data6 _Itype(float *_Single *_Single *_Single);
  float data7[4] _Itype(float checked[4]);
  float data8[] _Itype(float checked[]);
};

///
/// The interop type can have modifiers
///
extern void f30(const int * const x _Itype(const int * const _Single )) {
}

extern void f31(const int a[] _Itype(const int checked[])) {
}

extern void f32(const int a[10] _Itype(const int checked[10])) {
}

extern void f33(const int *x _Itype(const int *  _Single )) {
}

extern const int *f34(void) _Itype(const int *  _Single ) {
  return 0;
}

const int *a10 _Itype(const int *  _Single ) = 0;
int *const a11 _Itype(int* const _Single) = 0;

// First dimension of an array interop type for a parameter can
// have modifiers or the static keyword

extern void f35(int a[const 10] _Itype(int checked[const 10])) {
}

extern void f36(int a[static const 10] _Itype(int checked[static const 10])) {
}

extern void f37(int a[volatile 10] _Itype(int checked[volatile 10])) {
}

extern void f38(const int *const x _Itype(const int checked[const])) {
}

///
/// Typedef'ed names can be used as interop types
///

typedef int* _Single pint;
typedef const int *  _Single  pcint;

extern void f40(int *x _Itype(pint)) {
}

extern void f41(const int *x _Itype(pcint)) {
}

// Identifier not allowed in a type name

void f50(int *p _Itype(int* _Single a)) {  // expected-error {{type name cannot have identifier in it}}
}
