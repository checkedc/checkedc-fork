// Feature tests of typechecking new Checked C bounds declarations.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -Wno-check-bounds-decls -verify -verify-ignore-unexpected=note %s

#include <stdchecked.h>

static int A = 8;
static long long int B = 8;

struct S1 {
  int f;
};

struct S2 {
  float f;
};

union U1 {
  int f;
};

enum E1 {
  EnumVal1,
  EnumVal2
};

// Test expressions with standard signed and unsigned integers types as
// arguments to count and byte_count.

extern void count_exprs(void) {
  char c1 = 8;
  short c2 = 8;
  int c3 = 8;
  long int c4 = 8;
  long long int c5 = 8;

  _Bool c6 = 1;
  unsigned char c7 = 8;
  unsigned short c8 = 8;
  unsigned int c9 = 8;
  unsigned long int c10 = 8;
  unsigned long long int c11 = 8;

  int* _Array t1 : count(c1) = 0;
  int* _Array t2 : count(c2) = 0;
  int* _Array t3 : count(c3) = 0;
  int* _Array t4 : count(c4) = 0;
  int* _Array t5 : count(c5) = 0;
  int* _Array t6 : count(c6) = 0;
  int* _Array t7 : count(c7) = 0;
  int* _Array t8 : count(c8) = 0;
  int* _Array t9 : count(c9) = 0;
  int* _Array t10 : count(c10) = 0;
  int* _Array t11 : count(c11) = 0;
  // Spot-check type checking of count expressions involving file-scoped
  // variables.
  int* _Array t12 : count(A) = 0;
  int* _Array t13 : count(B) = 0;
  int* _Array t14 : count(12) = 0;
  // This will eventually fail static checking of bounds declarations, at which
  // point we'll need to add an expected error message.
  int* _Array t15 : count(-5) =  0;

  int* _Array t16 : byte_count(c1) = 0;
  int* _Array t17 : byte_count(c2) = 0;
  int* _Array t18 : byte_count(c3) = 0;
  int* _Array t19 : byte_count(c4) = 0;
  int* _Array t20 : byte_count(c5) = 0;
  int* _Array t21 : byte_count(c6) = 0;
  int* _Array t22 : byte_count(c7) = 0;
  int* _Array t23 : byte_count(c8) = 0;
  int* _Array t24 : byte_count(c9) = 0;
  int* _Array t25 : byte_count(c10) = 0;
  int* _Array t26 : byte_count(c11) = 0;
  // Spot-check type checking of byte_count expressions involving file-scoped
  // variables.
  int* _Array t27 : byte_count(A) = 0;
  int* _Array t28 : byte_count(B) = 0;
  int* _Array t29 : byte_count(12) = 0;
  // This will eventually fail static checking of bounds declarations, at which
  // point we'll need to add an expected error message.
  int* _Array t30 : byte_count(-8) = 0;

  // Spot check nt_array_ptr.  This should be treated the same as
  // array_ptr.

  nt_array_ptr<char> t50 : count(c1) = 0;
  nt_array_ptr<char> t51 : count(c2) = 0;
  nt_array_ptr<char> t52 : count(c3) = 0;
  nt_array_ptr<char> t53 : count(10) = 0;

  nt_array_ptr<char> t54 : byte_count(c7) = 0;
  nt_array_ptr<char> t55 : byte_count(c8) = 0;
  nt_array_ptr<char> t56 : byte_count(c9) = 0;
  nt_array_ptr<char> t57 : byte_count(A) = 0;

}

// Test expressions involving enum names and member bit fields that can be
// converted to integer types as arguments to count and byte_count.
extern void count_exprs_with_integral_operands(void) {
  enum E2 {
    c1 = 8,
    c2
  };

  struct S3 {
    int f : 5;
  } s;
  s.f = 8;

  int* _Array t1 : count(c1) = 0;
  int* _Array t2 : count(c2) = 0;
  int* _Array t3 : count(s.f) = 0;
  int* _Nt_array t4 : count(c2) = 0;
}

float globalFloat = 8;

extern void test_func(void) {}

// Test types that should not work as arguments for count and byte_count.
extern void invalid_count_exprs(void) {

  float c1 = 8.0;
  double c2 = 8.0;
  char *c3 = 0;
  double *c4 = 0;

  struct S1 c5 = {0};
  union U1 c6 = {0};

  double c7[10];

  struct S2 s = { 8.0 };

  void (*func_ptr)(void) = test_func;

#ifndef __STDC_NO_COMPLEX__
 float _Complex c8 = 8.0;
#endif

  int* _Array t1 : count(c1) = 0;   // expected-error {{invalid argument type 'float' to count expression}}
  int* _Array t2 : count(c2) = 0;   // expected-error {{invalid argument type}}
  int* _Array t3 : count(c3) = 0;   // expected-error {{invalid argument type}}
  int* _Array t4 : count(c4) = 0;   // expected-error {{invalid argument type}}
  int* _Array t5 : count(c5) = 0;   // expected-error {{invalid argument type}}
  int* _Array t6 : count(c6) = 0;   // expected-error {{invalid argument type}}
  int* _Array t7 : count(c7) = 0;   // expected-error {{invalid argument type}}
  int* _Array t8 : count(s.f) = 0;  // expected-error {{invalid argument type}}
  int* _Array t9 : count(test_func) = 0; // expected-error {{invalid argument type}}
  int* _Array t10 : count(func_ptr) = 0; // expected-error {{invalid argument type}}
  int* _Array t11 : count("test") = 0; // expected-error {{invalid argument type}}
  int* _Array t12 : count(5.0f) = 0; // expected-error {{invalid argument type}}
  int* _Array t13 : count(globalFloat) = 0; // expected-error {{invalid argument type}}

#ifndef __STDC_NO_COMPLEX__
  int* _Array t14 : count(c8) = 0;   // expected-error {{invalid argument type}}
#endif

  int* _Array t15 : byte_count(c1) = 0;  // expected-error {{invalid argument type 'float' to byte_count expression}}
  int* _Array t16 : byte_count(c2) = 0;  // expected-error {{invalid argument type}}
  int* _Array t17 : byte_count(c3) = 0;  // expected-error {{invalid argument type}}
  int* _Array t18 : byte_count(c4) = 0;  // expected-error {{invalid argument type}}
  int* _Array t19 : byte_count(c5) = 0;  // expected-error {{invalid argument type}}
  int* _Array t20 : byte_count(c6) = 0;  // expected-error {{invalid argument type}}
  int* _Array t21 : byte_count(c7) = 0;  // expected-error {{invalid argument type}}
  int* _Array t22 : byte_count(s.f) = 0; // expected-error {{invalid argument type}}
  int* _Array t23 : byte_count(func_ptr) = 0;  // expected-error {{invalid argument type}}
  int* _Array t24 : byte_count("test") = 0; // expected-error {{invalid argument type}}
  int* _Array t25 : byte_count(5.0f) = 0; // expected-error {{invalid argument type}}
  int* _Array t26 : byte_count(globalFloat) = 0; // expected-error {{invalid argument type}}

#ifndef __STDC_NO_COMPLEX__
  int* _Array t27 : byte_count(c8) = 0;   // expected-error {{invalid argument type}}
#endif

  // spot check nt_array_ptr
  int* _Nt_array t40 : count(c7) = 0;   // expected-error {{invalid argument type}}
}

extern void bounds_exprs(void) {
  int i[3] = { 0, 1, 0 };
   // check combinations of different kinds of pointers to the same
   // object type.

   int* _Array array_ptr_lb = i, *_Array array_ptr_ub = i + 1;
   int* _Single ptr_lb = i, *_Single ptr_ub = i + 1;
   int *unchecked_ptr_lb = i, *unchecked_ptr_ub = i + 1;
   // unsafe cast
   int* _Nt_array nt_array_ptr_lb = (int* _Nt_array) i,
                     *_Nt_array nt_array_ptr_ub = (int* _Nt_array) i + 1;

   int* _Array t1 : bounds(array_ptr_lb, array_ptr_ub) = i;
   int* _Array t2 : bounds(ptr_lb, array_ptr_ub) = i;
   int* _Array t3 : bounds(array_ptr_lb, ptr_ub) = i;
   int* _Array t4 : bounds(unchecked_ptr_lb, array_ptr_ub) = i;
   int* _Array t5 : bounds(array_ptr_lb, unchecked_ptr_ub) = i;
   int* _Array t6 : bounds(ptr_lb, ptr_ub) = i;
   int* _Array t7 : bounds(unchecked_ptr_lb, ptr_ub) = i;
   int* _Array t8 : bounds(ptr_lb, unchecked_ptr_ub) = i;
   int* _Array t9 : bounds(unchecked_ptr_lb, unchecked_ptr_ub) = i;

   // spot check mixed uses of nt_array_ptr and other pointer types.
   int* _Array t9a : bounds(nt_array_ptr_lb, ptr_ub) = i;
   int* _Array t9b : bounds(array_ptr_lb, nt_array_ptr_ub) = i;
   int* _Array t9c : bounds(ptr_lb, array_ptr_ub) = i;

   // use an array-typed value as the lower bound.  This value
   // should be converted implicitly to be a pointer type.

   int* _Array t10 : bounds(i, i + 1) = i;
   int* _Array t11 : bounds(i, array_ptr_ub) = i;
   int* _Array t11a : bounds(i, nt_array_ptr_ub) = i;
   int* _Array t13 : bounds(i, ptr_ub) = i;

   void* _Array void_array_ptr_lb = i, *_Array  void_array_ptr_ub = i + 1;
   void* _Array void_ptr_lb = i, *_Array void_ptr_ub = i + 1;
   void *void_unchecked_ptr_lb = i, *void_unchecked_ptr_ub = i + 1;

   // check combinations of differents kinds of pointers to void
   int* _Array t21 : bounds(void_array_ptr_lb, void_array_ptr_ub) = i;
   int* _Array t22 : bounds(void_ptr_lb, void_array_ptr_ub) = i;
   int* _Array t23 : bounds(void_array_ptr_lb, void_ptr_ub) = i;
   int* _Array t24 : bounds(void_unchecked_ptr_lb, void_array_ptr_ub) = i;
   int* _Array t25 : bounds(void_array_ptr_lb, void_unchecked_ptr_ub) = i;
   int* _Array t26 : bounds(void_ptr_lb, void_ptr_ub) = i;
   int* _Array t27 : bounds(void_unchecked_ptr_lb, void_ptr_ub) = i;
   int* _Array t28 : bounds(void_ptr_lb, void_unchecked_ptr_ub) = i;
   int* _Array t29 : bounds(void_unchecked_ptr_lb, void_unchecked_ptr_ub) = i;

   // check combinations of pointers to void and pointers to non-void types

   int* _Array t42 : bounds(array_ptr_lb, void_array_ptr_ub) = i;
   int* _Array t43 : bounds(ptr_lb, void_array_ptr_ub) = i;
   int* _Array t44 : bounds(void_ptr_lb, array_ptr_ub) = i;
   int* _Array t45 : bounds(void_array_ptr_lb, ptr_ub) = i;
   int* _Array t46 : bounds(array_ptr_lb, void_ptr_ub) = i;
   int* _Array t47 : bounds(unchecked_ptr_lb, void_array_ptr_ub) = i;
   int* _Array t48 : bounds(void_unchecked_ptr_lb, array_ptr_ub) = i;
   int* _Array t49 : bounds(void_array_ptr_lb, unchecked_ptr_ub) = i;
   int* _Array t50 : bounds(array_ptr_lb, void_unchecked_ptr_ub) = i;

   int* _Array t51 : bounds(void_ptr_lb, ptr_ub) = i;
   int* _Array t52 : bounds(ptr_lb, void_ptr_ub) = i;
   int* _Array t53 : bounds(unchecked_ptr_lb, void_ptr_ub) = i;
   int* _Array t54 : bounds(void_unchecked_ptr_lb, ptr_ub) = i;
   int* _Array t55 : bounds(void_ptr_lb, unchecked_ptr_ub) = i;
   int* _Array t56 : bounds(ptr_lb, void_unchecked_ptr_ub) = i;

   // spot check mixed uses of pointers to void and null-terminated pointers
   int* _Array t60 : bounds(void_array_ptr_lb, nt_array_ptr_ub) = i;
   int* _Array t61 : bounds(nt_array_ptr_lb, void_ptr_ub) = i;

   // spot check cases where the value being declared has a different pointer type
   // than the bounds.
   char* _Array t71 : bounds(array_ptr_lb, array_ptr_ub) = (char* _Array) i;
   char* _Array t72 : bounds(ptr_lb, array_ptr_ub) = (char* _Array) i;
   char* _Array t73 : bounds(unchecked_ptr_lb, ptr_ub) = (char* _Array) i;
   char* _Array t75 : bounds(void_array_ptr_lb, array_ptr_ub) = (char* _Array) i;
   char* _Array t76 : bounds(void_unchecked_ptr_lb, array_ptr_ub) = (char* _Array) i;
   char* _Array t77 : bounds(array_ptr_lb, void_unchecked_ptr_ub) = (char* _Array) i;
   char* _Array t77a : bounds(nt_array_ptr_lb, void_unchecked_ptr_ub) = (char* _Array) i;

   // use an array-typed value as the lower bound.  This should be converted
   // implicitly to be a pointer type.
   char* _Array t78 : bounds(i, array_ptr_ub) = (char* _Array) i;
   char* _Array t79 : bounds(i, ptr_ub) = (char* _Array) i;
   char* _Array t80 : bounds(i, nt_array_ptr_ub) = (char* _Array) i;

   // spot check that typechecking looks through typedefs
   typedef int* _Array int_array_ptr;
   typedef int* _Single int_ptr;
   typedef int* int_unchecked_ptr;
   typedef int* _Nt_array int_nt_array_ptr;

   int_array_ptr typedef_array_ptr_lb = i, typedef_array_ptr_ub = i + 1;
   int_ptr typedef_ptr_lb = i, typedef_ptr_ub = i + 1;
   int* typedef_unchecked_ptr_lb = i;
   int* typedef_unchecked_ptr_ub = i + 1;
   int_nt_array_ptr typedef_nt_array_ptr_lb = (int_nt_array_ptr)i,
                    typedef_nt_array_ptr_ub = (int_nt_array_ptr)(i + 1);

   int* _Array t91 : bounds(typedef_array_ptr_lb, array_ptr_ub) = i;
   int* _Array t92 : bounds(ptr_lb, typedef_array_ptr_ub) = i;
   int* _Array t93 : bounds(array_ptr_lb, typedef_ptr_ub) = i;
   int* _Array t94 : bounds(typedef_unchecked_ptr_lb, array_ptr_ub) = i;
   int* _Array t95 : bounds(typedef_array_ptr_lb, unchecked_ptr_ub) = i;
   int* _Array t96 : bounds(typedef_ptr_lb, ptr_ub) = i;
   int* _Array t97 : bounds(unchecked_ptr_lb, typedef_ptr_ub) = i;
   int* _Array t98 : bounds(ptr_lb, typedef_unchecked_ptr_ub) = i;
   int* _Array t99 : bounds(typedef_unchecked_ptr_lb, unchecked_ptr_ub) = i;
   int* _Array t100 : bounds(typedef_nt_array_ptr_lb, unchecked_ptr_ub) = i;
   int* _Array t101 : bounds(typedef_array_ptr_lb, typedef_nt_array_ptr_ub) = i;

   // check that type qualifiers are discarded when comparing pointer types
   // in bounds expressions

   // permutations of array_ptr and const
   array_ptr<const int> array_ptr_const_lb = i;
   const int* _Array const_array_ptr_lb = i;
   const array_ptr<const int> const_array_ptr_const_lb = i;
   array_ptr<const int> array_ptr_const_ub = i + 1;
   const int* _Array const_array_ptr_ub = i + 1;
   const array_ptr<const int> const_array_ptr_const_ub = i + 1;
   const int* _Nt_array const_nt_array_ptr_lb = (const int* _Nt_array) i;
   const int* _Nt_array const_nt_array_ptr_ub = (const int* _Nt_array) i + 1;

   // permutations of ptr and const
   ptr<int const> ptr_const_lb = i;
   const int* _Single const_ptr_lb = i;
   const ptr<const int> const_ptr_const_lb = i;
   ptr<int const> ptr_const_ub = i + 1;
   const int* _Single const_ptr_ub = i + 1;
   const ptr<const int> const_ptr_const_ub = i + 1;

   // permutations of unchecked pointers and const
   int *const const_unchecked_ptr_lb = i;
   const int *unchecked_ptr_const_lb = i;
   const int *const const_unchecked_ptr_const_lb = i;
   int *const const_unchecked_ptr_ub = i + 1;
   const int *unchecked_ptr_const_ub = i + 1;
   const int *const const_unchecked_ptr_const_ub = i + 1;

   int* _Array t121 : bounds(array_ptr_const_lb, array_ptr_ub) = i;
   int* _Array t122 : bounds(const_array_ptr_lb, array_ptr_ub) = i;
   int* _Array t123 : bounds(const_array_ptr_const_lb, array_ptr_ub) = i;

   int* _Array t124 : bounds(array_ptr_lb, array_ptr_const_ub) = i;
   int* _Array t125 : bounds(array_ptr_lb, const_array_ptr_ub) = i;
   int* _Array t126 : bounds(array_ptr_lb, const_array_ptr_const_ub) = i;

   int* _Array t127 : bounds(const_array_ptr_lb, array_ptr_const_ub) = i;
   int* _Array t128 : bounds(array_ptr_const_lb, const_array_ptr_ub) = i;
   int* _Array t129 : bounds(const_array_ptr_const_lb, const_array_ptr_const_ub) = i;

   int* _Array t130 : bounds(ptr_lb, array_ptr_ub) = i;
   int* _Array t131 : bounds(array_ptr_lb, ptr_ub) = i;
   int* _Array t132 : bounds(unchecked_ptr_lb, array_ptr_ub) = i;
   int* _Array t133 : bounds(array_ptr_lb, unchecked_ptr_ub) = i;
   int* _Array t134 : bounds(ptr_lb, ptr_ub) = i;
   int* _Array t135 : bounds(unchecked_ptr_lb, ptr_ub) = i;
   int* _Array t136 : bounds(ptr_lb, unchecked_ptr_ub) = i;
   int* _Array t137 : bounds(unchecked_ptr_lb, unchecked_ptr_ub) = i;

   // spot check const nt_array_ptr
   int* _Array t140 : bounds(nt_array_ptr_lb, const_nt_array_ptr_ub) = i;
   int* _Array t141 : bounds(const_nt_array_ptr_lb, nt_array_ptr_ub) = i;
 }

 extern void invalid_bounds_exprs(void) {
   // test types that should not work as arguments to bounds expressions
   char c1 = 8;
   short c2 = 8;
   int c3 = 8;
   long int c4 = 8;
   long long int c5 = 8;

   _Bool c6 = 1;
   unsigned char c7 = 8;
   unsigned short c8 = 8;
   unsigned int c9 = 8;
   unsigned long int c10 = 8;
   unsigned long long int c11 = 8;

   float c12 = 8.0;
   double c13 = 8.0;
   struct S1 c14 = { 0 };
   union U1 c15 = { 0 };

   void(*func_ptr)(void) = test_func;
   int *single_indir = 0;
   int **double_indir = 0;

#ifndef __STDC_NO_COMPLEX__
   float _Complex c17 = 8.0;
#endif

   int* _Array t1 : bounds(c1, c1) = 0; // expected-error 2 {{expected expression with pointer type}}
   int* _Array t2 : bounds(c2, c2) = 0; // expected-error 2 {{expected expression with pointer type}}
   int* _Array t3 : bounds(c3, c3) = 0; // expected-error 2 {{expected expression with pointer type}}
   int* _Array t4 : bounds(c4, c4) = 0; // expected-error 2 {{expected expression with pointer type}}
   int* _Array t5 : bounds(c5, c5) = 0; // expected-error 2 {{expected expression with pointer type}}
   int* _Array t6 : bounds(c6, c6) = 0; // expected-error 2 {{expected expression with pointer type}}
   int* _Array t7 : bounds(c7, c7) = 0; // expected-error 2 {{expected expression with pointer type}}
   int* _Array t8 : bounds(c8, c8) = 0; // expected-error 2 {{expected expression with pointer type}}
   int* _Array t9 : bounds(c9, c9) = 0; // expected-error 2 {{expected expression with pointer type}}
   int* _Array t10 : bounds(c10, c10) = 0; // expected-error 2 {{expected expression with pointer type}}
   int* _Array t11 : bounds(c11, c11) = 0; // expected-error 2 {{expected expression with pointer type}}
   int* _Array t12 : bounds(c12, c12) = 0; // expected-error 2 {{expected expression with pointer type}}
   int* _Array t13 : bounds(c13, c13) = 0; // expected-error 2 {{expected expression with pointer type}}
   int* _Array t14 : bounds(c14, c14) = 0; // expected-error 2 {{expected expression with pointer type}}
   int* _Array t15 : bounds(c15, c15) = 0; // expected-error 2 {{expected expression with pointer type}}
   int* _Array t16 : bounds(test_func, test_func) = 0; // expected-error 2 {{invalid argument type 'void (*)(void)' to bounds expression}}

   // have values with different levels of indirection
   int* _Array t17 : bounds(double_indir, c3) = 0; // expected-error {{expected expression with pointer type}}
   int* _Array t18 : bounds(double_indir, single_indir) = 0; // expected-error {{pointer type mismatch}}

   // test mismatched bounds expression types
   int i[2];
   int* _Array int_array_ptr_lb = i;
   int* _Array int_array_ptr_ub = i + 1;
   int* _Single int_ptr_lb = i, *_Single int_ptr_ub = i + 1;
   int *int_unchecked_ptr_lb = i, *int_unchecked_ptr_ub = i + 1;

   char* _Array char_array_ptr_lb = (char* _Array) i, *_Array char_array_ptr_ub = (char* _Array) i + 1;
   char* _Single char_ptr_lb = (char* _Single) i, * _Single char_ptr_ub = (char* _Single)  (i + 1);
   char *char_unchecked_ptr_lb = (char *) i, *char_unchecked_ptr_ub = (char *) i + 1;

   int* _Array t20 : bounds(int_array_ptr_lb, char_array_ptr_ub) = i;     // expected-error {{pointer type mismatch}}
   int* _Array t21 : bounds(int_ptr_lb, char_array_ptr_ub) = i;           // expected-error {{pointer type mismatch}}
   int* _Array t22 : bounds(int_unchecked_ptr_lb, char_array_ptr_ub) = i; // expected-error {{pointer type mismatch}}
   char* _Array t23 : bounds(char_array_ptr_lb, int_array_ptr_ub) = (char* _Array) i;     // expected-error {{pointer type mismatch}}
   char* _Array t24 : bounds(char_ptr_lb, int_array_ptr_ub) = (char* _Array) i;           // expected-error {{pointer type mismatch}}
   char* _Array t25 : bounds(char_unchecked_ptr_lb, int_array_ptr_ub) = (char* _Array) i; // expected-error {{pointer type mismatch}}

   int* _Array t30 : bounds(int_array_ptr_lb, char_ptr_ub) = i;     // expected-error {{pointer type mismatch}}
   int* _Array t31 : bounds(int_ptr_lb, char_ptr_ub) = i;           // expected-error {{pointer type mismatch}}
   int* _Array t32 : bounds(int_unchecked_ptr_lb, char_ptr_ub) = i; // expected-error {{pointer type mismatch}}
   char* _Array t33 : bounds(char_array_ptr_lb, int_ptr_ub) = (char* _Array) i;     // expected-error {{pointer type mismatch}}
   char* _Array t34 : bounds(char_ptr_lb, int_ptr_ub) = (char* _Array) i;           // expected-error {{pointer type mismatch}}
   char* _Array t35 : bounds(char_unchecked_ptr_lb, int_ptr_ub) = (char* _Array) i; // expected-error {{pointer type mismatch}}

   // spot check nt_array_ptr with invalid bounds expression
   nt_array_ptr<char> t40 : bounds(int_array_ptr_lb, char_array_ptr_ub) = 0;     // expected-error {{pointer type mismatch}}
}

//
// Test type requirements for bounds declarations.   There are various
// requirements for the types of variables with bounds declaration.
//
// We need to test the cross-product of (variable or member
//  declaration, scope for variables (global or local), the kind of the bounds
// expression, and the type for the variable).
//
// The tests follow a specific pattern: there is a set of tests for global
// variables that covers most of the different requirements.   The tests
// for the other cases (local variables, parameter variables, member
// declarations, and return bounds declarations) are specialized clones
//  of the cases for global variables.  The syntax is altered where necessary,
// prefixes for variables and members are renamed to avoid overlapping names
// and a small number of tests are added/removed.  The numbering for variables
//  and members should be kept the same across the different cases.
//
// The tests themselves are ordered by the kind of bounds expression and then
// grouped by kind of type within that.

//
// Test global variable declarations
//

//
// Valid bounds declarations for global variables
//

// count
int* _Array g1 : count(5) = 0;
int *g2 : count(5) = 0;
int g3 checked[5] : count(5);
int g3a[5] : count(5);
int g3b nt_checked[5]: count(4);

// byte_count
int* _Array g4 : byte_count(5 * sizeof(int)) = 0;
void* _Array g5 : byte_count(5 * sizeof(int)) = 0;
int *g6 : byte_count(5 * sizeof(int)) = 0;
int g7 checked[5] : byte_count(5 * sizeof(int));
unsigned int g8 checked[5] : byte_count(5 * sizeof(int));
unsigned int g8a nt_checked[5] : byte_count(4 * sizeof(int));
unsigned int g9[5] : byte_count(5 * sizeof(int));

// bounds
int* _Array g10 : bounds(g10, g10 + 5) = 0;
// The use of 'g3' is intentional - pointer arithmetic is not
// allowed on pointers to void types
void* _Array g11 : bounds(g3, g3 + 5);
int *g12 : bounds(g12, g12 + 5) = 0;
int g13 checked[5] : bounds(g13, g13 + 5);
unsigned int g14 checked[5] : bounds(g14, g14 + 5);
unsigned int g14a nt_checked[5] : bounds(g14a, g14a + 5);
int g15[5] : bounds(g15, g15 + 5);
unsigned int g16[5] : bounds(g16, g16 + 5);

//
// Valid bounds declarations for integer-typed global variables.
//

int s1 checked[16];



//
// Test invalid bounds declarations for global variables
//

// count
char g41 : count(5) = 0;           // expected-error {{expected 'g41' to have a pointer or array type}}
_Bool g42 : count(5) = 0;          // expected-error {{expected 'g42' to have a pointer or array type}}
short int g43 : count(5) = 0;      // expected-error {{expected 'g43' to have a pointer or array type}}
int g44 : count(5) = 0;            // expected-error {{expected 'g44' to have a pointer or array type}}
long int g45 : count(5) = 0;       // expected-error {{expected 'g45' to have a pointer or array type}}
unsigned short int g46 : count(5) = 0; // expected-error {{expected 'g46' to have a pointer or array type}}
unsigned int g47 : count(5) = 0;       // expected-error {{expected 'g47' to have a pointer or array type}}
unsigned long int g48 : count(5) = 0;  // expected-error {{expected 'g48' to have a pointer or array type}}

float g49 : count(5) = 0;           // expected-error {{expected 'g49' to have a pointer or array type}}
double g50 : count(5) = 0;          // expected-error {{expected 'g50' to have a pointer or array type}}
struct S1 g51 : count(5) = { 0 };   // expected-error {{expected 'g51' to have a pointer or array type}}
union U1 g52 : count(5) = { 0 };    // expected-error {{expected 'g52' to have a pointer or array type}}
enum E1 g53 : count(5) = EnumVal1;  // expected-error {{expected 'g53' to have a pointer or array type}}
int* _Single g54: count(1) = 0;         // expected-error {{bounds declaration not allowed because 'g54' has a _Ptr type}}
void* _Array g55 : count(1) = 0; // expected-error {{expected 'g55' to have a non-void pointer type}}
void((*g56)(void)) : count(1);      // expected-error {{bounds declaration not allowed because 'g56' has a function pointer type}}

// byte_count
float g60 : byte_count(8);             // expected-error {{expected 'g60' to have a pointer, array, or integer type}}
double g61 : byte_count(8);            // expected-error {{expected 'g61' to have a pointer, array, or integer type}}
struct S1 g62 : byte_count(8) = { 0 }; // expected-error {{expected 'g62' to have a pointer, array, or integer type}}
union U1 g63 : byte_count(8) = { 0 };  // expected-error {{expected 'g63' to have a pointer, array, or integer type}}
int* _Single g64 : byte_count(8) = 0;      // expected-error {{bounds declaration not allowed because 'g64' has a _Ptr type}}
void((*g65)(void)) : byte_count(1);    // expected-error {{bounds declaration not allowed because 'g65' has a function pointer type}}

// bounds
float g70 : bounds(s1, s1 + 1);             // expected-error {{expected 'g70' to have a pointer, array, or integer type}}
double g71 : bounds(s1, s1 + 1);            // expected-error {{expected 'g71' to have a pointer, array, or integer type}}
struct S1 g72 : bounds(s1, s1 + 1) = { 0 }; // expected-error {{expected 'g72' to have a pointer, array, or integer type}}
union U1 g73 : bounds(s1, s1 + 1) = { 0 };  // expected-error {{expected 'g73' to have a pointer, array, or integer type}}
int* _Single g74 : bounds(s1, s1 + 1) = 0;      // expected-error {{bounds declaration not allowed because 'g74' has a _Ptr type}}
void((*g75)(void)) : bounds(s1, s1 + 1);    // expected-error {{bounds declaration not allowed because 'g75' has a function pointer type}}

//
// Test bounds declarations for local variables.
//

void local_var_bounds_decl(void)
{
  // count
  int* _Array t1 : count(5) = 0;
  int t3 checked[5] : count(5);
  int t3a nt_checked[5] : count(4);

  // byte_count
  int* _Array t4 : byte_count(5 * sizeof(int)) = 0;
  void* _Array t5 : byte_count(5 * sizeof(int)) = 0;
  int t7 checked[5] : byte_count(5 * sizeof(int));
  unsigned int t8 checked[5] : byte_count(5 * sizeof(int));
  unsigned int t8a nt_checked[5] : byte_count(4 * sizeof(int));

  // bounds
  int* _Array t10 : bounds(t10, t10 + 5) = 0;
  // The use of 't3' is intentional - pointer arithmetic is not
  // allowed on pointers to void types.
  void* _Array t11 : bounds(t3, t3 + 5) = t3;
  int t13 checked[5] : bounds(t13, t13 + 5);
  unsigned int t14 checked[5] : bounds(t14, t14 + 5);
  unsigned int t14a nt_checked[5] : bounds(t14, t14 + 4);
}

void int_local_var_bounds_decl(void) {
  // bounds declarations are allowed for integer variables to support
  // casting of pointers to integers and back.  We usually expect this
  // to happen within expressions, but to allow uniform use of language
  // features, we allow bounds on integer-typed variables.
  int a1 checked[5];

  // byte_count
  short int t20 : byte_count(5 * sizeof(int)) = (short int)a1; // expected-warning {{cast to smaller integer type 'short' from '_Array_ptr<int>'}}
  enum E1 t24 : byte_count(8) = EnumVal1;
}

void invalid_local_var_bounds_decl(void)
{
  int arr[16];
  char t41 : count(5) = 0;           // expected-error {{expected 't41' to have a pointer or array type}}
  _Bool t42 : count(5) = 0;          // expected-error {{expected 't42' to have a pointer or array type}}
  short int t43 : count(5) = 0;      // expected-error {{expected 't43' to have a pointer or array type}}
  int t44 : count(5) = 0;            // expected-error {{expected 't44' to have a pointer or array type}}
  long int t45 : count(5) = 0;       // expected-error {{expected 't45' to have a pointer or array type}}
  unsigned short int t46 : count(5) = 0; // expected-error {{expected 't46' to have a pointer or array type}}
  unsigned int t47 : count(5) = 0;       // expected-error {{expected 't47' to have a pointer or array type}}
  unsigned long int t48 : count(5) = 0;  // expected-error {{expected 't48' to have a pointer or array type}}

  float t49 : count(5) = 0;          // expected-error {{expected 't49' to have a pointer or array type}}
  double t50 : count(5) = 0;         // expected-error {{expected 't50' to have a pointer or array type}}
  struct S1 t51 : count(5) = { 0 };  // expected-error {{expected 't51' to have a pointer or array type}}
  union U1 t52 : count(5) = { 0 };   // expected-error {{expected 't52' to have a pointer or array type}}
  enum E1 t53 : count(5) = EnumVal1; // expected-error {{expected 't53' to have a pointer or array type}}
  int* _Single t54 : count(1) = 0;       // expected-error {{bounds declaration not allowed because 't54' has a _Ptr type}}
  void* _Array t55 : count(1) = 0; // expected-error {{expected 't55' to have a non-void pointer type}}
  array_ptr<void (void)> t56 : count(1);  // expected-error {{declared as _Array_ptr to function of type 'void (void)'; use _Ptr to function instead}}

  int *t57 : count(1) = 0;          // expected-error {{bounds declaration not allowed for local variable with unchecked pointer type}}
  int t58[5] : count(5);            // expected-error {{bounds declaration not allowed for local variable with unchecked array type}}

  // byte_count
  float t60 : byte_count(8);                  // expected-error {{expected 't60' to have a pointer, array, or integer type}}
  double t61 : byte_count(8);                 // expected-error {{expected 't61' to have a pointer, array, or integer type}}
  struct S1 t62 : byte_count(8) = { 0 };      // expected-error {{expected 't62' to have a pointer, array, or integer type}}
  union U1 t63 : byte_count(8) = { 0 };       // expected-error {{expected 't63' to have a pointer, array, or integer type}}
  int* _Single t64 : byte_count(sizeof(int)) = 0; // expected-error {{bounds declaration not allowed because 't64' has a _Ptr type}}
  array_ptr<void (void)> t65 : byte_count(1); // expected-error {{declared as _Array_ptr to function of type 'void (void)'; use _Ptr to function instead}}

  int *t67 : byte_count(sizeof(int)) = 0;     // expected-error {{bounds declaration not allowed for local variable with unchecked pointer type}}
  int t68[5] : byte_count(5 * sizeof(int));   // expected-error {{bounds declaration not allowed for local variable with unchecked array type}}

  // bounds
  float t70 : bounds(arr, arr + 1);             // expected-error {{expected 't70' to have a pointer, array, or integer type}}
  double t71 : bounds(arr, arr + 1);            // expected-error {{expected 't71' to have a pointer, array, or integer type}}
  struct S1 t72 : bounds(arr, arr + 1) = { 0 }; // expected-error {{expected 't72' to have a pointer, array, or integer type}}
  union U1 t73 : bounds(arr, arr + 1) = { 0 };  // expected-error {{expected 't73' to have a pointer, array, or integer type}}
  int* _Single t74 : bounds(arr, arr + 1) = 0;      // expected-error {{bounds declaration not allowed because 't74' has a _Ptr type}}
  array_ptr<void (void)> t75 : bounds(arr, arr + 1);  // expected-error {{declared as _Array_ptr to function of type 'void (void)'; use _Ptr to function instead}}

  int *t78 : bounds(arr, arr + 1) = 0;          // expected-error {{bounds declaration not allowed for local variable with unchecked pointer type}}
  int t79[5] : bounds(arr, arr + 1);            // expected-error {{bounds declaration not allowed for local variable with unchecked array type}}
}

//
// Test parameter variable bounds declarations.
//

void param_var_bounds_decl(
  // count
  int* _Array t1 : count(5),
  int* _Nt_array t1a : count(5),
  int *t2 : count(5),
  int t3 checked[5] : count(5),
  int t3a[5] : count(5),
  int t3b nt_checked[5] : count(4),
  int t3c nt_checked[] : count(0),

  // byte_count
  int* _Array t4 : byte_count(5 * sizeof(int)),
  void* _Array t5 : byte_count(5 * sizeof(int)),
  int *t6 : byte_count(5 * sizeof(int)),
  int t7 checked[5] : byte_count(5 * sizeof(int)),
  unsigned int t8 checked[5] : byte_count(5 * sizeof(int)),
  unsigned int t9[5] : byte_count(5 * sizeof(int)),

  // bounds
  int* _Array t10 : bounds(t10, t10 + 5),
  // The use of 't3' is intentional - pointer arithmetic is not
  // allowed on pointers to void types.
  void* _Array t11 : bounds(t3, t3 + 5),
  int *t12 : bounds(t12, t12 + 5),
  int t13 checked[5] : bounds(t13, t13 + 5),
  unsigned int t14 checked[5] : bounds(t14, t14 + 5),
  int t15[5] : bounds(t15, t15 + 5),
  unsigned int t16[5] : bounds(t16, t16 + 5))
{
}

extern int garr[10];

extern void anonymous_param_var_bounds_decl(
  // count
  int* _Array : count(5),
  int* _Nt_array : count(5),
  int * : count(5),
  int checked[5] : count(5),
  int[5] : count(5),
  int nt_checked[6] : count(5),
  int nt_checked[] : count(0),

  // byte_count
  int* _Array : byte_count(5 * sizeof(int)),
  void* _Array : byte_count(5 * sizeof(int)),
  int * : byte_count(5 * sizeof(int)),
  int checked[5] : byte_count(5 * sizeof(int)),
  unsigned int checked[5] : byte_count(5 * sizeof(int)),
  unsigned int [5] : byte_count(5 * sizeof(int)),

  // bounds
  int* _Array : bounds(garr, garr + 5),
  void* _Array : bounds(garr, garr + 5),
  int * : bounds(garr, garr + 5),
  int t13 checked[5] : bounds(garr, garr + 5),
  int t15[5] : bounds(garr, garr + 5));

void int_param_var_bounds_decl(
  int s1 checked[16],
  int t21 : byte_count(5 * sizeof(int)),
  long int t22 : byte_count(5 * sizeof(int)),
  unsigned long int t23 : byte_count(5 * sizeof(int)),
  enum E1 t24 : byte_count(8),

  // bounds
  int t25 : bounds(s1, s1 + 5),
  long int t26 : bounds(s1, s1 + 5),
  unsigned long int t27 : bounds(s1, s1 + 5),
  enum E1 t28 : bounds(s1, s1 + 5))
{
}

void invalid_param_var_bounds_decl(
  // count
  int s1 checked[16],
  char t41 : count(5),           // expected-error {{expected 't41' to have a pointer or array type}}
  _Bool t42 : count(5),          // expected-error {{expected 't42' to have a pointer or array type}}
  short int t43 : count(5),      // expected-error {{expected 't43' to have a pointer or array type}}
  int t44 : count(5),            // expected-error {{expected 't44' to have a pointer or array type}}
  long int t45 : count(5),       // expected-error {{expected 't45' to have a pointer or array type}}
  unsigned short int t46 : count(5), // expected-error {{expected 't46' to have a pointer or array type}}
  unsigned int t47 : count(5),       // expected-error {{expected 't47' to have a pointer or array type}}
  unsigned long int t48 : count(5),  // expected-error {{expected 't48' to have a pointer or array type}}

  float t49 : count(5),          // expected-error {{expected 't49' to have a pointer or array type}}
  double t50 : count(5),         // expected-error {{expected 't50' to have a pointer or array type}}
  struct S1 t51 : count(5),      // expected-error {{expected 't51' to have a pointer or array type}}
  union U1 t52 : count(5),       // expected-error {{expected 't52' to have a pointer or array type}}
  enum E1 t53 : count(5),        // expected-error {{expected 't53' to have a pointer or array type}}
  int* _Single t54: count(1),        // expected-error {{bounds declaration not allowed because 't54' has a _Ptr type}}
  void* _Array t55 : count(1), // expected-error {{expected 't55' to have a non-void pointer type}}
  void((*t56)(void)) : count(1),  // expected-error {{bounds declaration not allowed because 't56' has a function pointer type}}

  // byte_count
  float t60 : byte_count(8),         // expected-error {{expected 't60' to have a pointer, array, or integer type}}
  double t61 : byte_count(8),        // expected-error {{expected 't61' to have a pointer, array, or integer type}}
  struct S1 t62 : byte_count(8),     // expected-error {{expected 't62' to have a pointer, array, or integer type}}
  union U1 t63 : byte_count(8),      // expected-error {{expected 't63' to have a pointer, array, or integer type}}
  int* _Single t64 : byte_count(8),      // expected-error {{bounds declaration not allowed because 't64' has a _Ptr type}}
  void((*t65)(void)) : byte_count(1),// expected-error {{bounds declaration not allowed because 't65' has a function pointer type}}

  // bounds
  float t70 : bounds(s1, s1 + 1),          // expected-error {{expected 't70' to have a pointer, array, or integer type}}
  double t71 : bounds(s1, s1 + 1),         // expected-error {{expected 't71' to have a pointer, array, or integer type}}
  struct S1 t72 : bounds(s1, s1 + 1),      // expected-error {{expected 't72' to have a pointer, array, or integer type}}
  union U1 t73 : bounds(s1, s1 + 1),       // expected-error {{expected 't73' to have a pointer, array, or integer type}}
  int* _Single t74 : bounds(s1, s1 + 1),       // expected-error {{bounds declaration not allowed because 't74' has a _Ptr type}}
  void((*t75)(void)) : bounds(s1, s1 + 1) // expected-error {{bounds declaration not allowed because 't75' has a function pointer type}}
  )
{
}

extern void anonymous_invalid_param_var_bounds_decl(
  // count
  int s1 checked[16],
  char : count(5),           // expected-error {{expected '' to have a pointer or array type}}
  _Bool : count(5),          // expected-error {{expected '' to have a pointer or array type}}
  short int  : count(5),     // expected-error {{expected '' to have a pointer or array type}}
  int : count(5),            // expected-error {{expected '' to have a pointer or array type}}
  long int : count(5),       // expected-error {{expected '' to have a pointer or array type}}
  unsigned short int : count(5), // expected-error {{expected '' to have a pointer or array type}}
  unsigned int : count(5),       // expected-error {{expected '' to have a pointer or array type}}
  unsigned long : count(5),  // expected-error {{expected '' to have a pointer or array type}}

  float : count(5),          // expected-error {{expected '' to have a pointer or array type}}
  double : count(5),         // expected-error {{expected '' to have a pointer or array type}}
  struct S1 : count(5),      // expected-error {{expected '' to have a pointer or array type}}
  union U1 : count(5),       // expected-error {{expected '' to have a pointer or array type}}
  enum E1 : count(5),        // expected-error {{expected '' to have a pointer or array type}}
  int* _Single : count(1),        // expected-error {{bounds declaration not allowed because '' has a _Ptr type}}
  void* _Array : count(1), // expected-error {{expected '' to have a non-void pointer type}}
  void((*)(void)) : count(1),  // expected-error {{bounds declaration not allowed because '' has a function pointer type}}

  // byte_count
  float : byte_count(8),         // expected-error {{expected '' to have a pointer, array, or integer type}}
  double : byte_count(8),        // expected-error {{expected '' to have a pointer, array, or integer type}}
  struct S1 : byte_count(8),     // expected-error {{expected '' to have a pointer, array, or integer type}}
  union U1 : byte_count(8),      // expected-error {{expected '' to have a pointer, array, or integer type}}
  int* _Single : byte_count(8),      // expected-error {{bounds declaration not allowed because '' has a _Ptr type}}
  void((*)(void)) : byte_count(1),// expected-error {{bounds declaration not allowed because '' has a function pointer type}}

  // bounds
  float : bounds(s1, s1 + 1),          // expected-error {{expected '' to have a pointer, array, or integer type}}
  double : bounds(s1, s1 + 1),         // expected-error {{expected '' to have a pointer, array, or integer type}}
  struct S1 : bounds(s1, s1 + 1),      // expected-error {{expected '' to have a pointer, array, or integer type}}
  union U1 : bounds(s1, s1 + 1),       // expected-error {{expected '' to have a pointer, array, or integer type}}
  int* _Single : bounds(s1, s1 + 1),       // expected-error {{bounds declaration not allowed because '' has a _Ptr type}}
  void((*)(void)) : bounds(s1, s1 + 1) // expected-error {{bounds declaration not allowed because '' has a function pointer type}}
  );


//
// Test member bounds declarations
//

//
// Test valid member bounds declarations
//

// count
struct S3 {
  int* _Array f1 : count(5);
  int* _Nt_array f1a : count(5);
  int *f2 : count(5);
  int f3 checked[5] : count(5);
  int f3a[5] : count(5);
  int f3b nt_checked[5] : count(4);
};

// byte_count
struct S4 {
  int* _Array f4 : byte_count(5 * sizeof(int));
  void* _Array f5 : byte_count(5 * sizeof(int));
  int *f6 : byte_count(5 * sizeof(int));
  int f7 checked[5] : byte_count(5 * sizeof(int));
  int f7a nt_checked[5] : byte_count(4 * sizeof(int));
  unsigned int f8 checked[5] : byte_count(5 * sizeof(int));
  unsigned int f9[5] : byte_count(5 * sizeof(int));
};

// bounds
struct S6 {
  int* _Array f10 : bounds(f10, f10 + 5);
  // The use of 'f10' is intentional - pointer arithmetic is not
  // allowed on pointers to void types
  void* _Array f11 : bounds(f10, f10 + 5);
  int *f12 : bounds(f12, f12 + 5);
  int f13 checked[5] : bounds(f13, f13 + 5);
  unsigned int f14 checked[5] : bounds(f14, f14 + 5);
  unsigned int f14a nt_checked[5] : bounds(f14a, f14a + 5);
  int f15[5] : bounds(f15, f15 + 5);
  unsigned int f16[5] : bounds(f16, f16 + 5);
};

//
// Test valid member bounds declarations on integer-typed members
//

struct S7 {
  int f1 checked[16];
  short int f20 : byte_count(5 * sizeof(int));
  int f21 : byte_count(5 * sizeof(int));
  long int f22 : byte_count(5 * sizeof(int));
  unsigned long int f23 : byte_count(5 * sizeof(int));
  enum E1 f24 : byte_count(8);

  // bounds
  int f25 : bounds(f1, f1 + 5);
  long int f26 : bounds(f1, f1 + 5);
  unsigned long int f27 : bounds(f1, f1 + 5);
  enum E1 f28 : bounds(f1, f1 + 5);

};

//
// Test invalid member bounds declarations.
//

struct s8 {
  // count
  int s1 checked[16];
  char g41 : count(5);           // expected-error {{expected 'g41' to have a pointer or array type}}
  _Bool g42 : count(5);          // expected-error {{expected 'g42' to have a pointer or array type}}
  short int g43 : count(5);      // expected-error {{expected 'g43' to have a pointer or array type}}
  int g44 : count(5);            // expected-error {{expected 'g44' to have a pointer or array type}}
  long int g45 : count(5);       // expected-error {{expected 'g45' to have a pointer or array type}}
  unsigned short int g46 : count(5); // expected-error {{expected 'g46' to have a pointer or array type}}
  unsigned int g47 : count(5);       // expected-error {{expected 'g47' to have a pointer or array type}}
  unsigned long int g48 : count(5);  // expected-error {{expected 'g48' to have a pointer or array type}}

  float g49 : count(5);           // expected-error {{expected 'g49' to have a pointer or array type}}
  double g50 : count(5);          // expected-error {{expected 'g50' to have a pointer or array type}}
  struct S1 g51 : count(5);       // expected-error {{expected 'g51' to have a pointer or array type}}
  union U1 g52 : count(5);        // expected-error {{expected 'g52' to have a pointer or array type}}
  enum E1 g53 : count(5);         // expected-error {{expected 'g53' to have a pointer or array type}}
  int* _Single g54: count(1);         // expected-error {{bounds declaration not allowed because 'g54' has a _Ptr type}}
  void* _Array g55 : count(1); // expected-error {{expected 'g55' to have a non-void pointer type}}
  void((*g56)(void)) : count(1);  // expected-error {{bounds declaration not allowed because 'g56' has a function pointer type}}

  // byte_count
  float g60 : byte_count(8);      // expected-error {{expected 'g60' to have a pointer, array, or integer type}}
  double g61 : byte_count(8);     // expected-error {{expected 'g61' to have a pointer, array, or integer type}}
  struct S1 g62 : byte_count(8);  // expected-error {{expected 'g62' to have a pointer, array, or integer type}}
  union U1 g63 : byte_count(8);   // expected-error {{expected 'g63' to have a pointer, array, or integer type}}
  int* _Single g64 : byte_count(8);   // expected-error {{bounds declaration not allowed because 'g64' has a _Ptr type}}
  void((*g65)(void)) : byte_count(1);    // expected-error {{bounds declaration not allowed because 'g65' has a function pointer type}}

  // bounds
  float g70 : bounds(s1, s1 + 1);          // expected-error {{expected 'g70' to have a pointer, array, or integer type}}
  double g71 : bounds(s1, s1 + 1);         // expected-error {{expected 'g71' to have a pointer, array, or integer type}}
  struct S1 g72 : bounds(s1, s1 + 1);      // expected-error {{expected 'g72' to have a pointer, array, or integer type}}
  union U1 g73 : bounds(s1, s1 + 1);       // expected-error {{expected 'g73' to have a pointer, array, or integer type}}
  int* _Single g74 : bounds(s1, s1 + 1);       // expected-error {{bounds declaration not allowed because 'g74' has a _Ptr type}}
  void((*g75)(void)) : bounds(s1, s1 + 1); // expected-error {{bounds declaration not allowed because 'g75' has a function pointer type}}
};

//
// Test function return bounds declarations.
//

//
// Test valid function return bounds declarations
//

// count
int* _Array fn1(void) : count(5) { return 0; }
int* _Nt_array fn1a(void) : count(0) { return 0; }
int* fn2(void) : count(5) { return 0; }
int *fn3(int len) : count(len) { return 0; }

// byte_count
extern int* _Array fn4(void) : byte_count(5 * sizeof(int));
extern int* _Nt_array fn4a(void) : byte_count(5 * sizeof(int));
extern void* _Array fn5(void) : byte_count(5 * sizeof(int));
extern int *fn6(void) : byte_count(5 * sizeof(int));

// bounds
int* _Array fn10(void) : bounds(s1, s1 + 5) { return 0; }
int* _Nt_array fn10a(void) : bounds(s1, s1 + 5) { return 0; }
void* _Array fn11(void) : bounds(s1, s1 + 5) { return 0; }
int *fn12(void) : bounds(s1, s1 + 5) { return 0; }

// Test valid return bounds declarations for integer-typed values
short int fn20(void) : byte_count(5 * sizeof(int)) { return (short int) s1; } // expected-warning {{cast to smaller integer type 'short' from '_Array_ptr<int>'}} \
                                                                              // expected-error {{it is not possible to prove that return value bounds imply declared return bounds for 'fn20'}}
int fn21(void) : byte_count(5 * sizeof(int)) { return (short int)s1; } // expected-warning {{cast to smaller integer type 'short' from '_Array_ptr<int>'}} \
                                                                       // expected-error {{it is not possible to prove that return value bounds imply declared return bounds for 'fn21'}}
long int fn22(void) : byte_count(5 * sizeof(int)) { return (short int)s1; } // expected-warning {{cast to smaller integer type 'short' from '_Array_ptr<int>'}} \
                                                                            // expected-error {{it is not possible to prove that return value bounds imply declared return bounds for 'fn22'}}
unsigned long int fn23(void) : byte_count(5 * sizeof(int)) { return (short int)s1; } // expected-warning {{cast to smaller integer type 'short' from '_Array_ptr<int>'}} \
                                                                                     // expected-error {{it is not possible to prove that return value bounds imply declared return bounds for 'fn23'}}
enum E1 fn24(void) : byte_count(8) { return (short int)s1; } // expected-warning {{cast to smaller integer type 'short' from '_Array_ptr<int>'}} \
                                                             // expected-error {{it is not possible to prove that return value bounds imply declared return bounds for 'fn24'}}

// bounds
extern int fn25(void) : bounds(s1, s1 + 5);
extern long int fn26(void) : bounds(s1, s1 + 5);
extern unsigned long int fn27(void) : bounds(s1, s1 + 5);
extern enum E1 fn28(void) : bounds(s1, s1 + 5);

//
// Test invalid return bounds declarations
//

// count
char fn41(void) : count(5);         // expected-error {{count bounds expression only allowed for pointer return type}}
_Bool fn42(void) : count(5);        // expected-error {{count bounds expression only allowed for pointer return type}}
short int fn43(void) : count(5);    // expected-error {{count bounds expression only allowed for pointer return type}}
int fn44(void) : count(5);          // expected-error {{count bounds expression only allowed for pointer return type}}
long int fn45(void) : count(5);     // expected-error {{count bounds expression only allowed for pointer return type}}
unsigned short int fn46(void) : count(5); // expected-error {{count bounds expression only allowed for pointer return type}}
unsigned int fn47(void) : count(5);       // expected-error {{count bounds expression only allowed for pointer return type}}
unsigned long int fn48(void) : count(5);  // expected-error {{count bounds expression only allowed for pointer return type}}

float fn49(void) : count(5);        // expected-error {{count bounds expression only allowed for pointer return type}}
double fn50(void) : count(5);       // expected-error {{count bounds expression only allowed for pointer return type}}
struct S1 fn51(void) : count(5);    // expected-error {{count bounds expression only allowed for pointer return type}}
union U1 fn52(void) : count(5);     // expected-error {{count bounds expression only allowed for pointer return type}}
enum E1 fn53(void) : count(5);      // expected-error {{count bounds expression only allowed for pointer return type}}
int* _Single fn54(void) : count(1);     // expected-error {{bounds declaration not allowed for a _Ptr return type}}
void* _Array fn55(void) : count(1); // expected-error {{count bounds expression not allowed for a void pointer return type}}
void (*fn56(void) : count(1))(int);    // expected-error {{bounds declaration not allowed for a function pointer return type}}
ptr<void(int)> fn57(void) : count(1); // expected-error {{bounds declaration not allowed for a _Ptr return type}}

// byte_count
float fn60(void) : byte_count(8);     // expected-error {{bounds declaration only allowed for a pointer or integer return type}}
double fn61(void) : byte_count(8);    // expected-error {{bounds declaration only allowed for a pointer or integer return type}}
struct S1 fn62(void) : byte_count(8); // expected-error {{bounds declaration only allowed for a pointer or integer return type}}
union U1 fn63(void) : byte_count(8);  // expected-error {{bounds declaration only allowed for a pointer or integer return type}}
int* _Single fn64(void) : byte_count(sizeof(int)); // expected-error {{bounds declaration not allowed for a _Ptr return type}}
void (*fn65(void) : byte_count(1))(int);   // expected-error {{bounds declaration not allowed for a function pointer return type}}
ptr<void(int)> fn66(void) : byte_count(1); // expected-error {{bounds declaration not allowed for a _Ptr return type}}

// bounds
float fn70(void) : bounds(s1, s1 + 1);      // expected-error {{bounds declaration only allowed for a pointer or integer return type}}
double fn71(void) : bounds(s1, s1 + 1);     // expected-error {{bounds declaration only allowed for a pointer or integer return type}}
struct S1 fn72(void) : bounds(s1, s1 + 1);  // expected-error {{bounds declaration only allowed for a pointer or integer return type}}
union U1 fn73(void) : bounds(s1, s1 + 1);   // expected-error {{bounds declaration only allowed for a pointer or integer return type}}
int* _Single fn74(void) : bounds(s1, s1 + 1);   // expected-error {{bounds declaration not allowed for a _Ptr return type}}
void (*fn75(void) : bounds(s1, s1 + 1))(int);  // expected-error {{bounds declaration not allowed for a function pointer return type}}
ptr<void(int)> fn76(void) : bounds(s1, s1 + 1);  // expected-error {{bounds declaration not allowed for a _Ptr return type}}

//
// Test bounds declarations on function parameters
//

// These are numbered so that they correspond to the numbering of
// functions with function pointer parameters in the next section of
// tests.
void fn100(int* _Array p1 : count(5));
void fn100a(int* _Array p1 : count(6));
void fn101(int p1 : count(5));             // expected-error {{expected 'p1' to have a pointer or array type}}
void fn102(void* _Array p1 : count(5)); // expected-error {{expected 'p1' to have a non-void pointer type}}

int fn103(int* _Array p1 : byte_count(5 * sizeof(int)));
int fn103a(int* _Array p1 : byte_count(7 * sizeof(int)));
int fn104(int *p1 : byte_count(5 * sizeof(int)));
int fn104a(int *p1 : byte_count(6 * sizeof(int)));
int fn104b(float *p1 : byte_count(6 * sizeof(float)));
int fn105(int p1 : byte_count(5 * sizeof(int)));
int fn106(float p1 : byte_count(5 * sizeof(int))); // expected-error {{expected 'p1' to have a pointer, array, or integer type}}
int fn107(void* _Array p1 : byte_count(5 * sizeof(int)));

void fn108(int* _Array p1 : bounds(p1, p1 + 5));
void fn108a(int* _Nt_array p1 : bounds(p1, p1 + 5));
void fn109(int* _Array p1, int p2 : bounds(p1, p1 + 5));
void fn110(int* _Array p1, float p2 : bounds(p1, p1 + 5)); // expected-error {{expected 'p2' to have a pointer, array, or integer type}}
void fn111(void* _Array p1 : bounds((char *)p1, (char *)p1 + (5 * sizeof(int))));

// A few functions with multiple arguments.
void fn120(int* _Array mid : bounds(p1, p1 + 5), int* _Array p1);
void fn122(void* _Array mid : bounds((char *)p1, (char *)p1 + (5 * sizeof(int))),
           int* _Array p1);

//
// Test bounds declaration on parameters of function pointer types
//

// These are numbered so that they correspond to the numbering of
// functions in the prior section of tests: fn200 - fn211 take fn100 - fn111 as
//  arguments.  fn220-231 are versions of fn200 - fn211 that take checked
// function pointers.

// Unchecked function pointers
void fn200(void (*fnptr)(int* _Array p1 : count(5)));
void fn201(void (*fnptr)(int p1 : count(5)));             // expected-error {{expected 'p1' to have a pointer or array type}}
void fn202(void (*fnptr)(void* _Array p1 : count(5))); // expected-error {{expected 'p1' to have a non-void pointer type}}

void fn203(int (*fnptr)(int* _Array p1 : byte_count(5 * sizeof(int))));
void fn204(int (*fnptr)(int * : byte_count(5 * sizeof(int))));
void fn205(int (*fnptr)(int p1 : byte_count(5 * sizeof(int))));
void fn206(int (*fnptr)(float p1 : byte_count(5 * sizeof(int)))); // expected-error {{expected 'p1' to have a pointer, array, or integer type}}
void fn207(int (*fnptr)(void* _Array p1 : byte_count(5 * sizeof(int))));

void fn208(void (*fnptr)(int* _Array p1 : bounds(p1, p1 + 5)));
void fn208a(void(*fnptr)(int* _Nt_array p1 : bounds(p1, p1 + 5)));
void fn209(void (*fnptr)(int* _Array p1, int p2 : bounds(p1, p1 + 5)));
void fn210(void (*fnptr)(int* _Array p1, float p2 : bounds(p1, p1 + 5))); // expected-error {{expected 'p2' to have a pointer, array, or integer type}}
void fn211(void (*fnptr)(void* _Array p1 : bounds((char *) p1, (char *) p1 + (5 * sizeof(int)))));

// Checked function pointers
void fn220(ptr<void (int* _Array p1 : count(5))> fnptr);
void fn221(ptr<void (int p1 : count(5))> fnptr);             // expected-error {{expected 'p1' to have a pointer or array type}}
void fn222(ptr<void (void* _Array p1 : count(5))> fnptr); // expected-error {{expected 'p1' to have a non-void pointer type}}

void fn223(ptr<int (int* _Array p1 : byte_count(5 * sizeof(int)))> fnptr);
void fn224(ptr<int (int * : byte_count(5 * sizeof(int)))> fnptr);
void fn225(ptr<int (int p1 : byte_count(5 * sizeof(int)))> fnptr);
void fn226(ptr<int (float p1 : byte_count(5 * sizeof(int)))> fnptr); // expected-error {{expected 'p1' to have a pointer, array, or integer type}}
void fn227(ptr<int (void* _Array p1 : byte_count(5 * sizeof(int)))> fnptr);

void fn228(ptr<void (int* _Array p1 : bounds(p1, p1 + 5))> fnptr);
void fn229(ptr<void (int* _Array p1, int p2 : bounds(p1, p1 + 5))> fnptr);
void fn230(ptr<void (int* _Array p1, float p2 : bounds(p1, p1 + 5))> fnptr); // expected-error {{expected 'p2' to have a pointer, array, or integer type}}
void fn231(ptr<void (void* _Array p1 : bounds((char *)p1, (char *)p1 + (5 * sizeof(int))))> fnptr);

// Function pointers with multiple arguments.
void fn240(ptr<int (int* _Array mid : bounds(p1, p1 + 5), int* _Array p1)> fnptr);
void fn241(ptr<int (void* _Array mid : bounds((char *)p1, (char *)p1 + (5 * sizeof(int))),
                    int* _Array p1)> fnptr);

//
// Spot check bounds-safe interfaces on parameters of function pointer types
//

void fn250(int(*fnptr)(int *p1 : count(5)));
void fn251(int(*fnptr)(int *p1 : byte_count(5 * sizeof(int))));
void fn252(int(*fnptr)(int *p1 : bounds(p1, p1 + 5)));
//
// Spot check bounds declaration for return values of function pointer types
//

void fn260(int* _Array (*fnptr)(void) : count(5));
void fn261(int* _Array(*fnptr)(int i) : count(i));
void fn262(int (*fnptr)(void) : count(5)); // expected-error {{count bounds expression only allowed for pointer return type}}
void fn263(void* _Array (*fnptr)(void) : count(5)); // expected-error {{count bounds expression not allowed for a void pointer return type}}

//
// Test bounds declarations for function pointers
//

void function_pointers(void) {
  // Assignments to function pointers with return bounds on array_ptr types
  int* _Array(*t1)(void) : count(5) = fn1;
  int* _Nt_array(*t1a)(void) : count(0) = fn1a;
  // Assignment to function pointers with bounds-safe interfaces on
  // unchecked pointer return types.  Unchecked pointers are compatible with
  // unchecked pointers with bounds-safe interfaces.  This extends recursively
  // to parameters and returns of function types.
  int *(*t2)(void) = fn2;
  int *(*t3)(void) : count(5) = fn2;
  ptr<int *(void) : count(5)> t4 = fn2;

  int *(*t5)(int i) = fn3;
  int *(*t6)(int i) : count(i) = fn3;
  ptr<int *(int j) : count(j)> t7 = fn3;

  int* _Array(*t8)(void) : byte_count(5 * sizeof(int)) = fn4;
  void* _Array(*t9)(void) : byte_count(5 * sizeof(int)) = fn5;
  int *(*t10)(void) = fn6;
  int *(*t11)(void) : byte_count(5*sizeof(int)) = fn6;
  ptr<int *(void) : byte_count(5*sizeof(int))> t12 = fn6;

  int* _Array(*t13)(void) : bounds(s1, s1 + 5) = fn10;
  int* _Nt_array(*t13a)(void) : bounds(s1, s1 + 5) = fn10a;
  int *(*t14)(void) = fn12;
  int *(*t15)(void) : bounds(s1, s1 + 5) = fn12;
  int *(*t16)(void) : bounds(s1, s1 + 6) = fn12;    // expected-warning {{incompatible function pointer types}}
  ptr<int *(void) : bounds(s1, s1 + 6)> t17 = fn12; // expected-error {{incompatible type}}

  // Unchecked pointer to function assigned to checked pointer to
  // function.
  ptr<int* _Array(void) : count(5)> t100 = fn1;
  // The reverse is not allowed
  int* _Array(*t101)(void) : count(5) = t100; // expected-error {{incompatible type}}

  // Calls that pass function pointers with bounds
  fn200(fn100);
  fn200(fn100a); // expected-error {{parameter of incompatible type}}
  fn201(fn101);
  fn202(fn102);
  fn203(fn103);
  fn203(fn103a); // expected-error {{parameter of incompatible type}}
  fn204(fn104);
  // These are mismatched unchecked function pointers with bounds-safe interfaces
  // on parameters.
  fn204(fn104a); // expected-warning {{incompatible function pointer types}}
  fn204(fn104b); // expected-warning {{incompatible function pointer types}}
  fn205(fn105);
  fn206(fn106);
  fn207(fn107);
  fn208(fn108);
  fn208(fn108a); // expected-error {{parameter of incompatible type}}
  fn208a(fn108a);
  fn209(fn109);
  fn210(fn110);
  fn211(fn111);

  fn220(fn100);
  fn220(fn100a); // expected-error {{parameter of incompatible type}}
  fn221(fn101);
  fn222(fn102);
  fn223(fn103);
  fn223(fn103a); // expected-error {{parameter of incompatible type}}
  fn224(fn104);
  // These are mismatched checked function pointers with bounds-safe interfaces
  // on parameters.
  fn224(fn104a); // expected-error {{parameter of incompatible type}}
  fn224(fn104b); // expected-error {{parameter of incompatible type}}
  fn225(fn105);
  fn226(fn106);
  fn227(fn107);
  fn228(fn108);
  fn229(fn109);
  fn230(fn110);
  fn231(fn111);
}

void invalid_function_pointers(void) {
  int* _Array(*t1)(void) : count(4) = fn1;  // expected-error {{incompatible type}}
  ptr<int* _Array(void) : count(4)> t1a = fn1;  // expected-error {{incompatible type}}
  int* _Array(*t4)(void) : byte_count(6 * sizeof(int)) = fn4; // expected-error {{incompatible type}}
  int* _Array(*t10)(void) : bounds(s1, s1 + 4) = fn10; // expected-error {{incompatible type}}
}

// Test type checking of return_value

extern int arr[10];
int f300(void) : bounds(arr, arr + return_value);
int* _Array f301(void) : bounds(return_value, return_value + 10);
int* _Array f302(void) : bounds(return_value - 5, return_value + 5);
int* _Array f303(void) : count(return_value); // expected-error {{invalid argument type}}
int* _Array f304(void) : bounds(arr, arr + (return_value << 5)); // expected-error {{invalid operands to binary expression}}
void* _Array f305(void) : bounds(return_value, return_value + 5); // expected-error {{arithmetic on a pointer to void}}
