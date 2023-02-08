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

  int* _Array t1  _Count(c1) = 0;
  int* _Array t2  _Count(c2) = 0;
  int* _Array t3  _Count(c3) = 0;
  int* _Array t4  _Count(c4) = 0;
  int* _Array t5  _Count(c5) = 0;
  int* _Array t6  _Count(c6) = 0;
  int* _Array t7  _Count(c7) = 0;
  int* _Array t8  _Count(c8) = 0;
  int* _Array t9  _Count(c9) = 0;
  int* _Array t10  _Count(c10) = 0;
  int* _Array t11  _Count(c11) = 0;
  // Spot-check type checking of count expressions involving file-scoped
  // variables.
  int* _Array t12  _Count(A) = 0;
  int* _Array t13  _Count(B) = 0;
  int* _Array t14  _Count(12) = 0;
  // This will eventually fail static checking of bounds declarations, at which
  // point we'll need to add an expected error message.
  int* _Array t15  _Count(-5) =  0;

  int* _Array t16  _Byte_count(c1) = 0;
  int* _Array t17  _Byte_count(c2) = 0;
  int* _Array t18  _Byte_count(c3) = 0;
  int* _Array t19  _Byte_count(c4) = 0;
  int* _Array t20  _Byte_count(c5) = 0;
  int* _Array t21  _Byte_count(c6) = 0;
  int* _Array t22  _Byte_count(c7) = 0;
  int* _Array t23  _Byte_count(c8) = 0;
  int* _Array t24  _Byte_count(c9) = 0;
  int* _Array t25  _Byte_count(c10) = 0;
  int* _Array t26  _Byte_count(c11) = 0;
  // Spot-check type checking of byte_count expressions involving file-scoped
  // variables.
  int* _Array t27  _Byte_count(A) = 0;
  int* _Array t28  _Byte_count(B) = 0;
  int* _Array t29  _Byte_count(12) = 0;
  // This will eventually fail static checking of bounds declarations, at which
  // point we'll need to add an expected error message.
  int* _Array t30  _Byte_count(-8) = 0;

  // Spot check nt_array_ptr.  This should be treated the same as
  // array_ptr.

  char* _Nt_array t50  _Count(c1) = 0;
  char* _Nt_array t51  _Count(c2) = 0;
  char* _Nt_array t52  _Count(c3) = 0;
  char* _Nt_array t53  _Count(10) = 0;

  char* _Nt_array t54  _Byte_count(c7) = 0;
  char* _Nt_array t55  _Byte_count(c8) = 0;
  char* _Nt_array t56  _Byte_count(c9) = 0;
  char* _Nt_array t57  _Byte_count(A) = 0;

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

  int* _Array t1  _Count(c1) = 0;
  int* _Array t2  _Count(c2) = 0;
  int* _Array t3  _Count(s.f) = 0;
  int* _Nt_array t4  _Count(c2) = 0;
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

  int* _Array t1  _Count(c1) = 0;   // expected-error {{invalid argument type 'float' to count expression}}
  int* _Array t2  _Count(c2) = 0;   // expected-error {{invalid argument type}}
  int* _Array t3  _Count(c3) = 0;   // expected-error {{invalid argument type}}
  int* _Array t4  _Count(c4) = 0;   // expected-error {{invalid argument type}}
  int* _Array t5  _Count(c5) = 0;   // expected-error {{invalid argument type}}
  int* _Array t6  _Count(c6) = 0;   // expected-error {{invalid argument type}}
  int* _Array t7  _Count(c7) = 0;   // expected-error {{invalid argument type}}
  int* _Array t8  _Count(s.f) = 0;  // expected-error {{invalid argument type}}
  int* _Array t9  _Count(test_func) = 0; // expected-error {{invalid argument type}}
  int* _Array t10  _Count(func_ptr) = 0; // expected-error {{invalid argument type}}
  int* _Array t11  _Count("test") = 0; // expected-error {{invalid argument type}}
  int* _Array t12  _Count(5.0f) = 0; // expected-error {{invalid argument type}}
  int* _Array t13  _Count(globalFloat) = 0; // expected-error {{invalid argument type}}

#ifndef __STDC_NO_COMPLEX__
  int* _Array t14  _Count(c8) = 0;   // expected-error {{invalid argument type}}
#endif

  int* _Array t15  _Byte_count(c1) = 0;  // expected-error {{invalid argument type 'float' to byte_count expression}}
  int* _Array t16  _Byte_count(c2) = 0;  // expected-error {{invalid argument type}}
  int* _Array t17  _Byte_count(c3) = 0;  // expected-error {{invalid argument type}}
  int* _Array t18  _Byte_count(c4) = 0;  // expected-error {{invalid argument type}}
  int* _Array t19  _Byte_count(c5) = 0;  // expected-error {{invalid argument type}}
  int* _Array t20  _Byte_count(c6) = 0;  // expected-error {{invalid argument type}}
  int* _Array t21  _Byte_count(c7) = 0;  // expected-error {{invalid argument type}}
  int* _Array t22  _Byte_count(s.f) = 0; // expected-error {{invalid argument type}}
  int* _Array t23  _Byte_count(func_ptr) = 0;  // expected-error {{invalid argument type}}
  int* _Array t24  _Byte_count("test") = 0; // expected-error {{invalid argument type}}
  int* _Array t25  _Byte_count(5.0f) = 0; // expected-error {{invalid argument type}}
  int* _Array t26  _Byte_count(globalFloat) = 0; // expected-error {{invalid argument type}}

#ifndef __STDC_NO_COMPLEX__
  int* _Array t27  _Byte_count(c8) = 0;   // expected-error {{invalid argument type}}
#endif

  // spot check nt_array_ptr
  int* _Nt_array t40  _Count(c7) = 0;   // expected-error {{invalid argument type}}
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

   int* _Array t1  _Bounds(array_ptr_lb, array_ptr_ub) = i;
   int* _Array t2  _Bounds(ptr_lb, array_ptr_ub) = i;
   int* _Array t3  _Bounds(array_ptr_lb, ptr_ub) = i;
   int* _Array t4  _Bounds(unchecked_ptr_lb, array_ptr_ub) = i;
   int* _Array t5  _Bounds(array_ptr_lb, unchecked_ptr_ub) = i;
   int* _Array t6  _Bounds(ptr_lb, ptr_ub) = i;
   int* _Array t7  _Bounds(unchecked_ptr_lb, ptr_ub) = i;
   int* _Array t8  _Bounds(ptr_lb, unchecked_ptr_ub) = i;
   int* _Array t9  _Bounds(unchecked_ptr_lb, unchecked_ptr_ub) = i;

   // spot check mixed uses of nt_array_ptr and other pointer types.
   int* _Array t9a  _Bounds(nt_array_ptr_lb, ptr_ub) = i;
   int* _Array t9b  _Bounds(array_ptr_lb, nt_array_ptr_ub) = i;
   int* _Array t9c  _Bounds(ptr_lb, array_ptr_ub) = i;

   // use an array-typed value as the lower bound.  This value
   // should be converted implicitly to be a pointer type.

   int* _Array t10  _Bounds(i, i + 1) = i;
   int* _Array t11  _Bounds(i, array_ptr_ub) = i;
   int* _Array t11a  _Bounds(i, nt_array_ptr_ub) = i;
   int* _Array t13  _Bounds(i, ptr_ub) = i;

   void* _Array void_array_ptr_lb = i, *_Array  void_array_ptr_ub = i + 1;
   void* _Array void_ptr_lb = i, *_Array void_ptr_ub = i + 1;
   void *void_unchecked_ptr_lb = i, *void_unchecked_ptr_ub = i + 1;

   // check combinations of differents kinds of pointers to void
   int* _Array t21  _Bounds(void_array_ptr_lb, void_array_ptr_ub) = i;
   int* _Array t22  _Bounds(void_ptr_lb, void_array_ptr_ub) = i;
   int* _Array t23  _Bounds(void_array_ptr_lb, void_ptr_ub) = i;
   int* _Array t24  _Bounds(void_unchecked_ptr_lb, void_array_ptr_ub) = i;
   int* _Array t25  _Bounds(void_array_ptr_lb, void_unchecked_ptr_ub) = i;
   int* _Array t26  _Bounds(void_ptr_lb, void_ptr_ub) = i;
   int* _Array t27  _Bounds(void_unchecked_ptr_lb, void_ptr_ub) = i;
   int* _Array t28  _Bounds(void_ptr_lb, void_unchecked_ptr_ub) = i;
   int* _Array t29  _Bounds(void_unchecked_ptr_lb, void_unchecked_ptr_ub) = i;

   // check combinations of pointers to void and pointers to non-void types

   int* _Array t42  _Bounds(array_ptr_lb, void_array_ptr_ub) = i;
   int* _Array t43  _Bounds(ptr_lb, void_array_ptr_ub) = i;
   int* _Array t44  _Bounds(void_ptr_lb, array_ptr_ub) = i;
   int* _Array t45  _Bounds(void_array_ptr_lb, ptr_ub) = i;
   int* _Array t46  _Bounds(array_ptr_lb, void_ptr_ub) = i;
   int* _Array t47  _Bounds(unchecked_ptr_lb, void_array_ptr_ub) = i;
   int* _Array t48  _Bounds(void_unchecked_ptr_lb, array_ptr_ub) = i;
   int* _Array t49  _Bounds(void_array_ptr_lb, unchecked_ptr_ub) = i;
   int* _Array t50  _Bounds(array_ptr_lb, void_unchecked_ptr_ub) = i;

   int* _Array t51  _Bounds(void_ptr_lb, ptr_ub) = i;
   int* _Array t52  _Bounds(ptr_lb, void_ptr_ub) = i;
   int* _Array t53  _Bounds(unchecked_ptr_lb, void_ptr_ub) = i;
   int* _Array t54  _Bounds(void_unchecked_ptr_lb, ptr_ub) = i;
   int* _Array t55  _Bounds(void_ptr_lb, unchecked_ptr_ub) = i;
   int* _Array t56  _Bounds(ptr_lb, void_unchecked_ptr_ub) = i;

   // spot check mixed uses of pointers to void and null-terminated pointers
   int* _Array t60  _Bounds(void_array_ptr_lb, nt_array_ptr_ub) = i;
   int* _Array t61  _Bounds(nt_array_ptr_lb, void_ptr_ub) = i;

   // spot check cases where the value being declared has a different pointer type
   // than the bounds.
   char* _Array t71  _Bounds(array_ptr_lb, array_ptr_ub) = (char* _Array) i;
   char* _Array t72  _Bounds(ptr_lb, array_ptr_ub) = (char* _Array) i;
   char* _Array t73  _Bounds(unchecked_ptr_lb, ptr_ub) = (char* _Array) i;
   char* _Array t75  _Bounds(void_array_ptr_lb, array_ptr_ub) = (char* _Array) i;
   char* _Array t76  _Bounds(void_unchecked_ptr_lb, array_ptr_ub) = (char* _Array) i;
   char* _Array t77  _Bounds(array_ptr_lb, void_unchecked_ptr_ub) = (char* _Array) i;
   char* _Array t77a  _Bounds(nt_array_ptr_lb, void_unchecked_ptr_ub) = (char* _Array) i;

   // use an array-typed value as the lower bound.  This should be converted
   // implicitly to be a pointer type.
   char* _Array t78  _Bounds(i, array_ptr_ub) = (char* _Array) i;
   char* _Array t79  _Bounds(i, ptr_ub) = (char* _Array) i;
   char* _Array t80  _Bounds(i, nt_array_ptr_ub) = (char* _Array) i;

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

   int* _Array t91  _Bounds(typedef_array_ptr_lb, array_ptr_ub) = i;
   int* _Array t92  _Bounds(ptr_lb, typedef_array_ptr_ub) = i;
   int* _Array t93  _Bounds(array_ptr_lb, typedef_ptr_ub) = i;
   int* _Array t94  _Bounds(typedef_unchecked_ptr_lb, array_ptr_ub) = i;
   int* _Array t95  _Bounds(typedef_array_ptr_lb, unchecked_ptr_ub) = i;
   int* _Array t96  _Bounds(typedef_ptr_lb, ptr_ub) = i;
   int* _Array t97  _Bounds(unchecked_ptr_lb, typedef_ptr_ub) = i;
   int* _Array t98  _Bounds(ptr_lb, typedef_unchecked_ptr_ub) = i;
   int* _Array t99  _Bounds(typedef_unchecked_ptr_lb, unchecked_ptr_ub) = i;
   int* _Array t100  _Bounds(typedef_nt_array_ptr_lb, unchecked_ptr_ub) = i;
   int* _Array t101  _Bounds(typedef_array_ptr_lb, typedef_nt_array_ptr_ub) = i;

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

   int* _Array t121  _Bounds(array_ptr_const_lb, array_ptr_ub) = i;
   int* _Array t122  _Bounds(const_array_ptr_lb, array_ptr_ub) = i;
   int* _Array t123  _Bounds(const_array_ptr_const_lb, array_ptr_ub) = i;

   int* _Array t124  _Bounds(array_ptr_lb, array_ptr_const_ub) = i;
   int* _Array t125  _Bounds(array_ptr_lb, const_array_ptr_ub) = i;
   int* _Array t126  _Bounds(array_ptr_lb, const_array_ptr_const_ub) = i;

   int* _Array t127  _Bounds(const_array_ptr_lb, array_ptr_const_ub) = i;
   int* _Array t128  _Bounds(array_ptr_const_lb, const_array_ptr_ub) = i;
   int* _Array t129  _Bounds(const_array_ptr_const_lb, const_array_ptr_const_ub) = i;

   int* _Array t130  _Bounds(ptr_lb, array_ptr_ub) = i;
   int* _Array t131  _Bounds(array_ptr_lb, ptr_ub) = i;
   int* _Array t132  _Bounds(unchecked_ptr_lb, array_ptr_ub) = i;
   int* _Array t133  _Bounds(array_ptr_lb, unchecked_ptr_ub) = i;
   int* _Array t134  _Bounds(ptr_lb, ptr_ub) = i;
   int* _Array t135  _Bounds(unchecked_ptr_lb, ptr_ub) = i;
   int* _Array t136  _Bounds(ptr_lb, unchecked_ptr_ub) = i;
   int* _Array t137  _Bounds(unchecked_ptr_lb, unchecked_ptr_ub) = i;

   // spot check const nt_array_ptr
   int* _Array t140  _Bounds(nt_array_ptr_lb, const_nt_array_ptr_ub) = i;
   int* _Array t141  _Bounds(const_nt_array_ptr_lb, nt_array_ptr_ub) = i;
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

   int* _Array t1  _Bounds(c1, c1) = 0; // expected-error 2 {{expected expression with pointer type}}
   int* _Array t2  _Bounds(c2, c2) = 0; // expected-error 2 {{expected expression with pointer type}}
   int* _Array t3  _Bounds(c3, c3) = 0; // expected-error 2 {{expected expression with pointer type}}
   int* _Array t4  _Bounds(c4, c4) = 0; // expected-error 2 {{expected expression with pointer type}}
   int* _Array t5  _Bounds(c5, c5) = 0; // expected-error 2 {{expected expression with pointer type}}
   int* _Array t6  _Bounds(c6, c6) = 0; // expected-error 2 {{expected expression with pointer type}}
   int* _Array t7  _Bounds(c7, c7) = 0; // expected-error 2 {{expected expression with pointer type}}
   int* _Array t8  _Bounds(c8, c8) = 0; // expected-error 2 {{expected expression with pointer type}}
   int* _Array t9  _Bounds(c9, c9) = 0; // expected-error 2 {{expected expression with pointer type}}
   int* _Array t10  _Bounds(c10, c10) = 0; // expected-error 2 {{expected expression with pointer type}}
   int* _Array t11  _Bounds(c11, c11) = 0; // expected-error 2 {{expected expression with pointer type}}
   int* _Array t12  _Bounds(c12, c12) = 0; // expected-error 2 {{expected expression with pointer type}}
   int* _Array t13  _Bounds(c13, c13) = 0; // expected-error 2 {{expected expression with pointer type}}
   int* _Array t14  _Bounds(c14, c14) = 0; // expected-error 2 {{expected expression with pointer type}}
   int* _Array t15  _Bounds(c15, c15) = 0; // expected-error 2 {{expected expression with pointer type}}
   int* _Array t16  _Bounds(test_func, test_func) = 0; // expected-error 2 {{invalid argument type 'void (*)(void)' to bounds expression}}

   // have values with different levels of indirection
   int* _Array t17  _Bounds(double_indir, c3) = 0; // expected-error {{expected expression with pointer type}}
   int* _Array t18  _Bounds(double_indir, single_indir) = 0; // expected-error {{pointer type mismatch}}

   // test mismatched bounds expression types
   int i[2];
   int* _Array int_array_ptr_lb = i;
   int* _Array int_array_ptr_ub = i + 1;
   int* _Single int_ptr_lb = i, *_Single int_ptr_ub = i + 1;
   int *int_unchecked_ptr_lb = i, *int_unchecked_ptr_ub = i + 1;

   char* _Array char_array_ptr_lb = (char* _Array) i, *_Array char_array_ptr_ub = (char* _Array) i + 1;
   char* _Single char_ptr_lb = (char* _Single) i, * _Single char_ptr_ub = (char* _Single)  (i + 1);
   char *char_unchecked_ptr_lb = (char *) i, *char_unchecked_ptr_ub = (char *) i + 1;

   int* _Array t20  _Bounds(int_array_ptr_lb, char_array_ptr_ub) = i;     // expected-error {{pointer type mismatch}}
   int* _Array t21  _Bounds(int_ptr_lb, char_array_ptr_ub) = i;           // expected-error {{pointer type mismatch}}
   int* _Array t22  _Bounds(int_unchecked_ptr_lb, char_array_ptr_ub) = i; // expected-error {{pointer type mismatch}}
   char* _Array t23  _Bounds(char_array_ptr_lb, int_array_ptr_ub) = (char* _Array) i;     // expected-error {{pointer type mismatch}}
   char* _Array t24  _Bounds(char_ptr_lb, int_array_ptr_ub) = (char* _Array) i;           // expected-error {{pointer type mismatch}}
   char* _Array t25  _Bounds(char_unchecked_ptr_lb, int_array_ptr_ub) = (char* _Array) i; // expected-error {{pointer type mismatch}}

   int* _Array t30  _Bounds(int_array_ptr_lb, char_ptr_ub) = i;     // expected-error {{pointer type mismatch}}
   int* _Array t31  _Bounds(int_ptr_lb, char_ptr_ub) = i;           // expected-error {{pointer type mismatch}}
   int* _Array t32  _Bounds(int_unchecked_ptr_lb, char_ptr_ub) = i; // expected-error {{pointer type mismatch}}
   char* _Array t33  _Bounds(char_array_ptr_lb, int_ptr_ub) = (char* _Array) i;     // expected-error {{pointer type mismatch}}
   char* _Array t34  _Bounds(char_ptr_lb, int_ptr_ub) = (char* _Array) i;           // expected-error {{pointer type mismatch}}
   char* _Array t35  _Bounds(char_unchecked_ptr_lb, int_ptr_ub) = (char* _Array) i; // expected-error {{pointer type mismatch}}

   // spot check nt_array_ptr with invalid bounds expression
   char* _Nt_array t40  _Bounds(int_array_ptr_lb, char_array_ptr_ub) = 0;     // expected-error {{pointer type mismatch}}
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
int* _Array g1  _Count(5) = 0;
int *g2  _Count(5) = 0;
int g3 checked[5]  _Count(5);
int g3a[5]  _Count(5);
int g3b nt_checked[5] _Count(4);

// byte_count
int* _Array g4  _Byte_count(5 * sizeof(int)) = 0;
void* _Array g5  _Byte_count(5 * sizeof(int)) = 0;
int *g6  _Byte_count(5 * sizeof(int)) = 0;
int g7 checked[5]  _Byte_count(5 * sizeof(int));
unsigned int g8 checked[5]  _Byte_count(5 * sizeof(int));
unsigned int g8a nt_checked[5]  _Byte_count(4 * sizeof(int));
unsigned int g9[5]  _Byte_count(5 * sizeof(int));

// bounds
int* _Array g10  _Bounds(g10, g10 + 5) = 0;
// The use of 'g3' is intentional - pointer arithmetic is not
// allowed on pointers to void types
void* _Array g11  _Bounds(g3, g3 + 5);
int *g12  _Bounds(g12, g12 + 5) = 0;
int g13 checked[5]  _Bounds(g13, g13 + 5);
unsigned int g14 checked[5]  _Bounds(g14, g14 + 5);
unsigned int g14a nt_checked[5]  _Bounds(g14a, g14a + 5);
int g15[5]  _Bounds(g15, g15 + 5);
unsigned int g16[5]  _Bounds(g16, g16 + 5);

//
// Valid bounds declarations for integer-typed global variables.
//

int s1 checked[16];



//
// Test invalid bounds declarations for global variables
//

// count
char g41  _Count(5) = 0;           // expected-error {{expected 'g41' to have a pointer or array type}}
_Bool g42  _Count(5) = 0;          // expected-error {{expected 'g42' to have a pointer or array type}}
short int g43  _Count(5) = 0;      // expected-error {{expected 'g43' to have a pointer or array type}}
int g44  _Count(5) = 0;            // expected-error {{expected 'g44' to have a pointer or array type}}
long int g45  _Count(5) = 0;       // expected-error {{expected 'g45' to have a pointer or array type}}
unsigned short int g46  _Count(5) = 0; // expected-error {{expected 'g46' to have a pointer or array type}}
unsigned int g47  _Count(5) = 0;       // expected-error {{expected 'g47' to have a pointer or array type}}
unsigned long int g48  _Count(5) = 0;  // expected-error {{expected 'g48' to have a pointer or array type}}

float g49  _Count(5) = 0;           // expected-error {{expected 'g49' to have a pointer or array type}}
double g50  _Count(5) = 0;          // expected-error {{expected 'g50' to have a pointer or array type}}
struct S1 g51  _Count(5) = { 0 };   // expected-error {{expected 'g51' to have a pointer or array type}}
union U1 g52  _Count(5) = { 0 };    // expected-error {{expected 'g52' to have a pointer or array type}}
enum E1 g53  _Count(5) = EnumVal1;  // expected-error {{expected 'g53' to have a pointer or array type}}
int* _Single g54 _Count(1) = 0;         // expected-error {{bounds declaration not allowed because 'g54' has a _Ptr type}}
void* _Array g55  _Count(1) = 0; // expected-error {{expected 'g55' to have a non-void pointer type}}
void((*g56)(void))  _Count(1);      // expected-error {{bounds declaration not allowed because 'g56' has a function pointer type}}

// byte_count
float g60  _Byte_count(8);             // expected-error {{expected 'g60' to have a pointer, array, or integer type}}
double g61  _Byte_count(8);            // expected-error {{expected 'g61' to have a pointer, array, or integer type}}
struct S1 g62  _Byte_count(8) = { 0 }; // expected-error {{expected 'g62' to have a pointer, array, or integer type}}
union U1 g63  _Byte_count(8) = { 0 };  // expected-error {{expected 'g63' to have a pointer, array, or integer type}}
int* _Single g64  _Byte_count(8) = 0;      // expected-error {{bounds declaration not allowed because 'g64' has a _Ptr type}}
void((*g65)(void))  _Byte_count(1);    // expected-error {{bounds declaration not allowed because 'g65' has a function pointer type}}

// bounds
float g70  _Bounds(s1, s1 + 1);             // expected-error {{expected 'g70' to have a pointer, array, or integer type}}
double g71  _Bounds(s1, s1 + 1);            // expected-error {{expected 'g71' to have a pointer, array, or integer type}}
struct S1 g72  _Bounds(s1, s1 + 1) = { 0 }; // expected-error {{expected 'g72' to have a pointer, array, or integer type}}
union U1 g73  _Bounds(s1, s1 + 1) = { 0 };  // expected-error {{expected 'g73' to have a pointer, array, or integer type}}
int* _Single g74  _Bounds(s1, s1 + 1) = 0;      // expected-error {{bounds declaration not allowed because 'g74' has a _Ptr type}}
void((*g75)(void))  _Bounds(s1, s1 + 1);    // expected-error {{bounds declaration not allowed because 'g75' has a function pointer type}}

//
// Test bounds declarations for local variables.
//

void local_var_bounds_decl(void)
{
  // count
  int* _Array t1  _Count(5) = 0;
  int t3 checked[5]  _Count(5);
  int t3a nt_checked[5]  _Count(4);

  // byte_count
  int* _Array t4  _Byte_count(5 * sizeof(int)) = 0;
  void* _Array t5  _Byte_count(5 * sizeof(int)) = 0;
  int t7 checked[5]  _Byte_count(5 * sizeof(int));
  unsigned int t8 checked[5]  _Byte_count(5 * sizeof(int));
  unsigned int t8a nt_checked[5]  _Byte_count(4 * sizeof(int));

  // bounds
  int* _Array t10  _Bounds(t10, t10 + 5) = 0;
  // The use of 't3' is intentional - pointer arithmetic is not
  // allowed on pointers to void types.
  void* _Array t11  _Bounds(t3, t3 + 5) = t3;
  int t13 checked[5]  _Bounds(t13, t13 + 5);
  unsigned int t14 checked[5]  _Bounds(t14, t14 + 5);
  unsigned int t14a nt_checked[5]  _Bounds(t14, t14 + 4);
}

void int_local_var_bounds_decl(void) {
  // bounds declarations are allowed for integer variables to support
  // casting of pointers to integers and back.  We usually expect this
  // to happen within expressions, but to allow uniform use of language
  // features, we allow bounds on integer-typed variables.
  int a1 checked[5];

  // byte_count
  short int t20  _Byte_count(5 * sizeof(int)) = (short int)a1; // expected-warning {{cast to smaller integer type 'short' from '_Array_ptr<int>'}}
  enum E1 t24  _Byte_count(8) = EnumVal1;
}

void invalid_local_var_bounds_decl(void)
{
  int arr[16];
  char t41  _Count(5) = 0;           // expected-error {{expected 't41' to have a pointer or array type}}
  _Bool t42  _Count(5) = 0;          // expected-error {{expected 't42' to have a pointer or array type}}
  short int t43  _Count(5) = 0;      // expected-error {{expected 't43' to have a pointer or array type}}
  int t44  _Count(5) = 0;            // expected-error {{expected 't44' to have a pointer or array type}}
  long int t45  _Count(5) = 0;       // expected-error {{expected 't45' to have a pointer or array type}}
  unsigned short int t46  _Count(5) = 0; // expected-error {{expected 't46' to have a pointer or array type}}
  unsigned int t47  _Count(5) = 0;       // expected-error {{expected 't47' to have a pointer or array type}}
  unsigned long int t48  _Count(5) = 0;  // expected-error {{expected 't48' to have a pointer or array type}}

  float t49  _Count(5) = 0;          // expected-error {{expected 't49' to have a pointer or array type}}
  double t50  _Count(5) = 0;         // expected-error {{expected 't50' to have a pointer or array type}}
  struct S1 t51  _Count(5) = { 0 };  // expected-error {{expected 't51' to have a pointer or array type}}
  union U1 t52  _Count(5) = { 0 };   // expected-error {{expected 't52' to have a pointer or array type}}
  enum E1 t53  _Count(5) = EnumVal1; // expected-error {{expected 't53' to have a pointer or array type}}
  int* _Single t54  _Count(1) = 0;       // expected-error {{bounds declaration not allowed because 't54' has a _Ptr type}}
  void* _Array t55  _Count(1) = 0; // expected-error {{expected 't55' to have a non-void pointer type}}
  array_ptr<void (void)> t56  _Count(1);  // expected-error {{declared as _Array_ptr to function of type 'void (void)'; use _Ptr to function instead}}

  int *t57  _Count(1) = 0;          // expected-error {{bounds declaration not allowed for local variable with unchecked pointer type}}
  int t58[5]  _Count(5);            // expected-error {{bounds declaration not allowed for local variable with unchecked array type}}

  // byte_count
  float t60  _Byte_count(8);                  // expected-error {{expected 't60' to have a pointer, array, or integer type}}
  double t61  _Byte_count(8);                 // expected-error {{expected 't61' to have a pointer, array, or integer type}}
  struct S1 t62  _Byte_count(8) = { 0 };      // expected-error {{expected 't62' to have a pointer, array, or integer type}}
  union U1 t63  _Byte_count(8) = { 0 };       // expected-error {{expected 't63' to have a pointer, array, or integer type}}
  int* _Single t64  _Byte_count(sizeof(int)) = 0; // expected-error {{bounds declaration not allowed because 't64' has a _Ptr type}}
  array_ptr<void (void)> t65  _Byte_count(1); // expected-error {{declared as _Array_ptr to function of type 'void (void)'; use _Ptr to function instead}}

  int *t67  _Byte_count(sizeof(int)) = 0;     // expected-error {{bounds declaration not allowed for local variable with unchecked pointer type}}
  int t68[5]  _Byte_count(5 * sizeof(int));   // expected-error {{bounds declaration not allowed for local variable with unchecked array type}}

  // bounds
  float t70  _Bounds(arr, arr + 1);             // expected-error {{expected 't70' to have a pointer, array, or integer type}}
  double t71  _Bounds(arr, arr + 1);            // expected-error {{expected 't71' to have a pointer, array, or integer type}}
  struct S1 t72  _Bounds(arr, arr + 1) = { 0 }; // expected-error {{expected 't72' to have a pointer, array, or integer type}}
  union U1 t73  _Bounds(arr, arr + 1) = { 0 };  // expected-error {{expected 't73' to have a pointer, array, or integer type}}
  int* _Single t74  _Bounds(arr, arr + 1) = 0;      // expected-error {{bounds declaration not allowed because 't74' has a _Ptr type}}
  array_ptr<void (void)> t75  _Bounds(arr, arr + 1);  // expected-error {{declared as _Array_ptr to function of type 'void (void)'; use _Ptr to function instead}}

  int *t78  _Bounds(arr, arr + 1) = 0;          // expected-error {{bounds declaration not allowed for local variable with unchecked pointer type}}
  int t79[5]  _Bounds(arr, arr + 1);            // expected-error {{bounds declaration not allowed for local variable with unchecked array type}}
}

//
// Test parameter variable bounds declarations.
//

void param_var_bounds_decl(
  // count
  int* _Array t1  _Count(5),
  int* _Nt_array t1a  _Count(5),
  int *t2  _Count(5),
  int t3 checked[5]  _Count(5),
  int t3a[5]  _Count(5),
  int t3b nt_checked[5]  _Count(4),
  int t3c nt_checked[]  _Count(0),

  // byte_count
  int* _Array t4  _Byte_count(5 * sizeof(int)),
  void* _Array t5  _Byte_count(5 * sizeof(int)),
  int *t6  _Byte_count(5 * sizeof(int)),
  int t7 checked[5]  _Byte_count(5 * sizeof(int)),
  unsigned int t8 checked[5]  _Byte_count(5 * sizeof(int)),
  unsigned int t9[5]  _Byte_count(5 * sizeof(int)),

  // bounds
  int* _Array t10  _Bounds(t10, t10 + 5),
  // The use of 't3' is intentional - pointer arithmetic is not
  // allowed on pointers to void types.
  void* _Array t11  _Bounds(t3, t3 + 5),
  int *t12  _Bounds(t12, t12 + 5),
  int t13 checked[5]  _Bounds(t13, t13 + 5),
  unsigned int t14 checked[5]  _Bounds(t14, t14 + 5),
  int t15[5]  _Bounds(t15, t15 + 5),
  unsigned int t16[5]  _Bounds(t16, t16 + 5))
{
}

extern int garr[10];

extern void anonymous_param_var_bounds_decl(
  // count
  int* _Array _Count(5),
  int* _Nt_array _Count(5),
  int *  _Count(5),
  int checked[5]  _Count(5),
  int[5]  _Count(5),
  int nt_checked[6]  _Count(5),
  int nt_checked[]  _Count(0),

  // byte_count
  int* _Array  _Byte_count(5 * sizeof(int)),
  void* _Array  _Byte_count(5 * sizeof(int)),
  int *  _Byte_count(5 * sizeof(int)),
  int checked[5]  _Byte_count(5 * sizeof(int)),
  unsigned int checked[5]  _Byte_count(5 * sizeof(int)),
  unsigned int [5]  _Byte_count(5 * sizeof(int)),

  // bounds
  int* _Array  _Bounds(garr, garr + 5),
  void* _Array  _Bounds(garr, garr + 5),
  int *  _Bounds(garr, garr + 5),
  int t13 checked[5]  _Bounds(garr, garr + 5),
  int t15[5]  _Bounds(garr, garr + 5));

void int_param_var_bounds_decl(
  int s1 checked[16],
  int t21  _Byte_count(5 * sizeof(int)),
  long int t22  _Byte_count(5 * sizeof(int)),
  unsigned long int t23  _Byte_count(5 * sizeof(int)),
  enum E1 t24  _Byte_count(8),

  // bounds
  int t25  _Bounds(s1, s1 + 5),
  long int t26  _Bounds(s1, s1 + 5),
  unsigned long int t27  _Bounds(s1, s1 + 5),
  enum E1 t28  _Bounds(s1, s1 + 5))
{
}

void invalid_param_var_bounds_decl(
  // count
  int s1 checked[16],
  char t41  _Count(5),           // expected-error {{expected 't41' to have a pointer or array type}}
  _Bool t42  _Count(5),          // expected-error {{expected 't42' to have a pointer or array type}}
  short int t43  _Count(5),      // expected-error {{expected 't43' to have a pointer or array type}}
  int t44  _Count(5),            // expected-error {{expected 't44' to have a pointer or array type}}
  long int t45  _Count(5),       // expected-error {{expected 't45' to have a pointer or array type}}
  unsigned short int t46  _Count(5), // expected-error {{expected 't46' to have a pointer or array type}}
  unsigned int t47  _Count(5),       // expected-error {{expected 't47' to have a pointer or array type}}
  unsigned long int t48  _Count(5),  // expected-error {{expected 't48' to have a pointer or array type}}

  float t49  _Count(5),          // expected-error {{expected 't49' to have a pointer or array type}}
  double t50  _Count(5),         // expected-error {{expected 't50' to have a pointer or array type}}
  struct S1 t51  _Count(5),      // expected-error {{expected 't51' to have a pointer or array type}}
  union U1 t52  _Count(5),       // expected-error {{expected 't52' to have a pointer or array type}}
  enum E1 t53  _Count(5),        // expected-error {{expected 't53' to have a pointer or array type}}
  int* _Single t54 _Count(1),        // expected-error {{bounds declaration not allowed because 't54' has a _Ptr type}}
  void* _Array t55  _Count(1), // expected-error {{expected 't55' to have a non-void pointer type}}
  void((*t56)(void))  _Count(1),  // expected-error {{bounds declaration not allowed because 't56' has a function pointer type}}

  // byte_count
  float t60  _Byte_count(8),         // expected-error {{expected 't60' to have a pointer, array, or integer type}}
  double t61  _Byte_count(8),        // expected-error {{expected 't61' to have a pointer, array, or integer type}}
  struct S1 t62  _Byte_count(8),     // expected-error {{expected 't62' to have a pointer, array, or integer type}}
  union U1 t63  _Byte_count(8),      // expected-error {{expected 't63' to have a pointer, array, or integer type}}
  int* _Single t64  _Byte_count(8),      // expected-error {{bounds declaration not allowed because 't64' has a _Ptr type}}
  void((*t65)(void))  _Byte_count(1),// expected-error {{bounds declaration not allowed because 't65' has a function pointer type}}

  // bounds
  float t70  _Bounds(s1, s1 + 1),          // expected-error {{expected 't70' to have a pointer, array, or integer type}}
  double t71  _Bounds(s1, s1 + 1),         // expected-error {{expected 't71' to have a pointer, array, or integer type}}
  struct S1 t72  _Bounds(s1, s1 + 1),      // expected-error {{expected 't72' to have a pointer, array, or integer type}}
  union U1 t73  _Bounds(s1, s1 + 1),       // expected-error {{expected 't73' to have a pointer, array, or integer type}}
  int* _Single t74  _Bounds(s1, s1 + 1),       // expected-error {{bounds declaration not allowed because 't74' has a _Ptr type}}
  void((*t75)(void))  _Bounds(s1, s1 + 1) // expected-error {{bounds declaration not allowed because 't75' has a function pointer type}}
  )
{
}

extern void anonymous_invalid_param_var_bounds_decl(
  // count
  int s1 checked[16],
  char  _Count(5),           // expected-error {{expected '' to have a pointer or array type}}
  _Bool  _Count(5),          // expected-error {{expected '' to have a pointer or array type}}
  short int   _Count(5),     // expected-error {{expected '' to have a pointer or array type}}
  int  _Count(5),            // expected-error {{expected '' to have a pointer or array type}}
  long int  _Count(5),       // expected-error {{expected '' to have a pointer or array type}}
  unsigned short int  _Count(5), // expected-error {{expected '' to have a pointer or array type}}
  unsigned int  _Count(5),       // expected-error {{expected '' to have a pointer or array type}}
  unsigned long  _Count(5),  // expected-error {{expected '' to have a pointer or array type}}

  float  _Count(5),          // expected-error {{expected '' to have a pointer or array type}}
  double  _Count(5),         // expected-error {{expected '' to have a pointer or array type}}
  struct S1  _Count(5),      // expected-error {{expected '' to have a pointer or array type}}
  union U1  _Count(5),       // expected-error {{expected '' to have a pointer or array type}}
  enum E1  _Count(5),        // expected-error {{expected '' to have a pointer or array type}}
  int* _Single  _Count(1),        // expected-error {{bounds declaration not allowed because '' has a _Ptr type}}
  void* _Array  _Count(1), // expected-error {{expected '' to have a non-void pointer type}}
  void((*)(void))  _Count(1),  // expected-error {{bounds declaration not allowed because '' has a function pointer type}}

  // byte_count
  float  _Byte_count(8),         // expected-error {{expected '' to have a pointer, array, or integer type}}
  double  _Byte_count(8),        // expected-error {{expected '' to have a pointer, array, or integer type}}
  struct S1  _Byte_count(8),     // expected-error {{expected '' to have a pointer, array, or integer type}}
  union U1  _Byte_count(8),      // expected-error {{expected '' to have a pointer, array, or integer type}}
  int* _Single  _Byte_count(8),      // expected-error {{bounds declaration not allowed because '' has a _Ptr type}}
  void((*)(void))  _Byte_count(1),// expected-error {{bounds declaration not allowed because '' has a function pointer type}}

  // bounds
  float  _Bounds(s1, s1 + 1),          // expected-error {{expected '' to have a pointer, array, or integer type}}
  double  _Bounds(s1, s1 + 1),         // expected-error {{expected '' to have a pointer, array, or integer type}}
  struct S1  _Bounds(s1, s1 + 1),      // expected-error {{expected '' to have a pointer, array, or integer type}}
  union U1  _Bounds(s1, s1 + 1),       // expected-error {{expected '' to have a pointer, array, or integer type}}
  int* _Single  _Bounds(s1, s1 + 1),       // expected-error {{bounds declaration not allowed because '' has a _Ptr type}}
  void((*)(void))  _Bounds(s1, s1 + 1) // expected-error {{bounds declaration not allowed because '' has a function pointer type}}
  );


//
// Test member bounds declarations
//

//
// Test valid member bounds declarations
//

// count
struct S3 {
  int* _Array f1  _Count(5);
  int* _Nt_array f1a  _Count(5);
  int *f2  _Count(5);
  int f3 checked[5]  _Count(5);
  int f3a[5]  _Count(5);
  int f3b nt_checked[5]  _Count(4);
};

// byte_count
struct S4 {
  int* _Array f4  _Byte_count(5 * sizeof(int));
  void* _Array f5  _Byte_count(5 * sizeof(int));
  int *f6  _Byte_count(5 * sizeof(int));
  int f7 checked[5]  _Byte_count(5 * sizeof(int));
  int f7a nt_checked[5]  _Byte_count(4 * sizeof(int));
  unsigned int f8 checked[5]  _Byte_count(5 * sizeof(int));
  unsigned int f9[5]  _Byte_count(5 * sizeof(int));
};

// bounds
struct S6 {
  int* _Array f10  _Bounds(f10, f10 + 5);
  // The use of 'f10' is intentional - pointer arithmetic is not
  // allowed on pointers to void types
  void* _Array f11  _Bounds(f10, f10 + 5);
  int *f12  _Bounds(f12, f12 + 5);
  int f13 checked[5]  _Bounds(f13, f13 + 5);
  unsigned int f14 checked[5]  _Bounds(f14, f14 + 5);
  unsigned int f14a nt_checked[5]  _Bounds(f14a, f14a + 5);
  int f15[5]  _Bounds(f15, f15 + 5);
  unsigned int f16[5]  _Bounds(f16, f16 + 5);
};

//
// Test valid member bounds declarations on integer-typed members
//

struct S7 {
  int f1 checked[16];
  short int f20  _Byte_count(5 * sizeof(int));
  int f21  _Byte_count(5 * sizeof(int));
  long int f22  _Byte_count(5 * sizeof(int));
  unsigned long int f23  _Byte_count(5 * sizeof(int));
  enum E1 f24  _Byte_count(8);

  // bounds
  int f25  _Bounds(f1, f1 + 5);
  long int f26  _Bounds(f1, f1 + 5);
  unsigned long int f27  _Bounds(f1, f1 + 5);
  enum E1 f28  _Bounds(f1, f1 + 5);

};

//
// Test invalid member bounds declarations.
//

struct s8 {
  // count
  int s1 checked[16];
  char g41  _Count(5);           // expected-error {{expected 'g41' to have a pointer or array type}}
  _Bool g42  _Count(5);          // expected-error {{expected 'g42' to have a pointer or array type}}
  short int g43  _Count(5);      // expected-error {{expected 'g43' to have a pointer or array type}}
  int g44  _Count(5);            // expected-error {{expected 'g44' to have a pointer or array type}}
  long int g45  _Count(5);       // expected-error {{expected 'g45' to have a pointer or array type}}
  unsigned short int g46  _Count(5); // expected-error {{expected 'g46' to have a pointer or array type}}
  unsigned int g47  _Count(5);       // expected-error {{expected 'g47' to have a pointer or array type}}
  unsigned long int g48  _Count(5);  // expected-error {{expected 'g48' to have a pointer or array type}}

  float g49  _Count(5);           // expected-error {{expected 'g49' to have a pointer or array type}}
  double g50  _Count(5);          // expected-error {{expected 'g50' to have a pointer or array type}}
  struct S1 g51  _Count(5);       // expected-error {{expected 'g51' to have a pointer or array type}}
  union U1 g52  _Count(5);        // expected-error {{expected 'g52' to have a pointer or array type}}
  enum E1 g53  _Count(5);         // expected-error {{expected 'g53' to have a pointer or array type}}
  int* _Single g54 _Count(1);         // expected-error {{bounds declaration not allowed because 'g54' has a _Ptr type}}
  void* _Array g55  _Count(1); // expected-error {{expected 'g55' to have a non-void pointer type}}
  void((*g56)(void))  _Count(1);  // expected-error {{bounds declaration not allowed because 'g56' has a function pointer type}}

  // byte_count
  float g60  _Byte_count(8);      // expected-error {{expected 'g60' to have a pointer, array, or integer type}}
  double g61  _Byte_count(8);     // expected-error {{expected 'g61' to have a pointer, array, or integer type}}
  struct S1 g62  _Byte_count(8);  // expected-error {{expected 'g62' to have a pointer, array, or integer type}}
  union U1 g63  _Byte_count(8);   // expected-error {{expected 'g63' to have a pointer, array, or integer type}}
  int* _Single g64  _Byte_count(8);   // expected-error {{bounds declaration not allowed because 'g64' has a _Ptr type}}
  void((*g65)(void))  _Byte_count(1);    // expected-error {{bounds declaration not allowed because 'g65' has a function pointer type}}

  // bounds
  float g70  _Bounds(s1, s1 + 1);          // expected-error {{expected 'g70' to have a pointer, array, or integer type}}
  double g71  _Bounds(s1, s1 + 1);         // expected-error {{expected 'g71' to have a pointer, array, or integer type}}
  struct S1 g72  _Bounds(s1, s1 + 1);      // expected-error {{expected 'g72' to have a pointer, array, or integer type}}
  union U1 g73  _Bounds(s1, s1 + 1);       // expected-error {{expected 'g73' to have a pointer, array, or integer type}}
  int* _Single g74  _Bounds(s1, s1 + 1);       // expected-error {{bounds declaration not allowed because 'g74' has a _Ptr type}}
  void((*g75)(void))  _Bounds(s1, s1 + 1); // expected-error {{bounds declaration not allowed because 'g75' has a function pointer type}}
};

//
// Test function return bounds declarations.
//

//
// Test valid function return bounds declarations
//

// count
int* _Array fn1(void)  _Count(5) { return 0; }
int* _Nt_array fn1a(void)  _Count(0) { return 0; }
int* fn2(void)  _Count(5) { return 0; }
int *fn3(int len)  _Count(len) { return 0; }

// byte_count
extern int* _Array fn4(void) _Byte_count(5 * sizeof(int));
extern int* _Nt_array fn4a(void)  _Byte_count(5 * sizeof(int));
extern void* _Array fn5(void)  _Byte_count(5 * sizeof(int));
extern int *fn6(void)  _Byte_count(5 * sizeof(int));

// bounds
int* _Array fn10(void)  _Bounds(s1, s1 + 5) { return 0; }
int* _Nt_array fn10a(void)  _Bounds(s1, s1 + 5) { return 0; }
void* _Array fn11(void)  _Bounds(s1, s1 + 5) { return 0; }
int *fn12(void)  _Bounds(s1, s1 + 5) { return 0; }

// Test valid return bounds declarations for integer-typed values
short int fn20(void)  _Byte_count(5 * sizeof(int)) { return (short int) s1; } // expected-warning {{cast to smaller integer type 'short' from '_Array_ptr<int>'}} \
                                                                              // expected-error {{it is not possible to prove that return value bounds imply declared return bounds for 'fn20'}}
int fn21(void)  _Byte_count(5 * sizeof(int)) { return (short int)s1; } // expected-warning {{cast to smaller integer type 'short' from '_Array_ptr<int>'}} \
                                                                       // expected-error {{it is not possible to prove that return value bounds imply declared return bounds for 'fn21'}}
long int fn22(void)  _Byte_count(5 * sizeof(int)) { return (short int)s1; } // expected-warning {{cast to smaller integer type 'short' from '_Array_ptr<int>'}} \
                                                                            // expected-error {{it is not possible to prove that return value bounds imply declared return bounds for 'fn22'}}
unsigned long int fn23(void)  _Byte_count(5 * sizeof(int)) { return (short int)s1; } // expected-warning {{cast to smaller integer type 'short' from '_Array_ptr<int>'}} \
                                                                                     // expected-error {{it is not possible to prove that return value bounds imply declared return bounds for 'fn23'}}
enum E1 fn24(void)  _Byte_count(8) { return (short int)s1; } // expected-warning {{cast to smaller integer type 'short' from '_Array_ptr<int>'}} \
                                                             // expected-error {{it is not possible to prove that return value bounds imply declared return bounds for 'fn24'}}

// bounds
extern int fn25(void)  _Bounds(s1, s1 + 5);
extern long int fn26(void)  _Bounds(s1, s1 + 5);
extern unsigned long int fn27(void)  _Bounds(s1, s1 + 5);
extern enum E1 fn28(void)  _Bounds(s1, s1 + 5);

//
// Test invalid return bounds declarations
//

// count
char fn41(void)  _Count(5);         // expected-error {{count bounds expression only allowed for pointer return type}}
_Bool fn42(void)  _Count(5);        // expected-error {{count bounds expression only allowed for pointer return type}}
short int fn43(void)  _Count(5);    // expected-error {{count bounds expression only allowed for pointer return type}}
int fn44(void)  _Count(5);          // expected-error {{count bounds expression only allowed for pointer return type}}
long int fn45(void)  _Count(5);     // expected-error {{count bounds expression only allowed for pointer return type}}
unsigned short int fn46(void)  _Count(5); // expected-error {{count bounds expression only allowed for pointer return type}}
unsigned int fn47(void)  _Count(5);       // expected-error {{count bounds expression only allowed for pointer return type}}
unsigned long int fn48(void)  _Count(5);  // expected-error {{count bounds expression only allowed for pointer return type}}

float fn49(void)  _Count(5);        // expected-error {{count bounds expression only allowed for pointer return type}}
double fn50(void)  _Count(5);       // expected-error {{count bounds expression only allowed for pointer return type}}
struct S1 fn51(void)  _Count(5);    // expected-error {{count bounds expression only allowed for pointer return type}}
union U1 fn52(void)  _Count(5);     // expected-error {{count bounds expression only allowed for pointer return type}}
enum E1 fn53(void)  _Count(5);      // expected-error {{count bounds expression only allowed for pointer return type}}
int* _Single fn54(void)  _Count(1);     // expected-error {{bounds declaration not allowed for a _Ptr return type}}
void* _Array fn55(void)  _Count(1); // expected-error {{count bounds expression not allowed for a void pointer return type}}
void (*fn56(void)  _Count(1))(int);    // expected-error {{bounds declaration not allowed for a function pointer return type}}
ptr<void(int)> fn57(void)  _Count(1); // expected-error {{bounds declaration not allowed for a _Ptr return type}}

// byte_count
float fn60(void)  _Byte_count(8);     // expected-error {{bounds declaration only allowed for a pointer or integer return type}}
double fn61(void)  _Byte_count(8);    // expected-error {{bounds declaration only allowed for a pointer or integer return type}}
struct S1 fn62(void)  _Byte_count(8); // expected-error {{bounds declaration only allowed for a pointer or integer return type}}
union U1 fn63(void)  _Byte_count(8);  // expected-error {{bounds declaration only allowed for a pointer or integer return type}}
int* _Single fn64(void)  _Byte_count(sizeof(int)); // expected-error {{bounds declaration not allowed for a _Ptr return type}}
void (*fn65(void)  _Byte_count(1))(int);   // expected-error {{bounds declaration not allowed for a function pointer return type}}
ptr<void(int)> fn66(void)  _Byte_count(1); // expected-error {{bounds declaration not allowed for a _Ptr return type}}

// bounds
float fn70(void)  _Bounds(s1, s1 + 1);      // expected-error {{bounds declaration only allowed for a pointer or integer return type}}
double fn71(void)  _Bounds(s1, s1 + 1);     // expected-error {{bounds declaration only allowed for a pointer or integer return type}}
struct S1 fn72(void)  _Bounds(s1, s1 + 1);  // expected-error {{bounds declaration only allowed for a pointer or integer return type}}
union U1 fn73(void)  _Bounds(s1, s1 + 1);   // expected-error {{bounds declaration only allowed for a pointer or integer return type}}
int* _Single fn74(void)  _Bounds(s1, s1 + 1);   // expected-error {{bounds declaration not allowed for a _Ptr return type}}
void (*fn75(void)  _Bounds(s1, s1 + 1))(int);  // expected-error {{bounds declaration not allowed for a function pointer return type}}
ptr<void(int)> fn76(void)  _Bounds(s1, s1 + 1);  // expected-error {{bounds declaration not allowed for a _Ptr return type}}

//
// Test bounds declarations on function parameters
//

// These are numbered so that they correspond to the numbering of
// functions with function pointer parameters in the next section of
// tests.
void fn100(int* _Array p1  _Count(5));
void fn100a(int* _Array p1  _Count(6));
void fn101(int p1  _Count(5));             // expected-error {{expected 'p1' to have a pointer or array type}}
void fn102(void* _Array p1  _Count(5)); // expected-error {{expected 'p1' to have a non-void pointer type}}

int fn103(int* _Array p1  _Byte_count(5 * sizeof(int)));
int fn103a(int* _Array p1  _Byte_count(7 * sizeof(int)));
int fn104(int *p1  _Byte_count(5 * sizeof(int)));
int fn104a(int *p1  _Byte_count(6 * sizeof(int)));
int fn104b(float *p1  _Byte_count(6 * sizeof(float)));
int fn105(int p1  _Byte_count(5 * sizeof(int)));
int fn106(float p1  _Byte_count(5 * sizeof(int))); // expected-error {{expected 'p1' to have a pointer, array, or integer type}}
int fn107(void* _Array p1  _Byte_count(5 * sizeof(int)));

void fn108(int* _Array p1  _Bounds(p1, p1 + 5));
void fn108a(int* _Nt_array p1  _Bounds(p1, p1 + 5));
void fn109(int* _Array p1, int p2  _Bounds(p1, p1 + 5));
void fn110(int* _Array p1, float p2  _Bounds(p1, p1 + 5)); // expected-error {{expected 'p2' to have a pointer, array, or integer type}}
void fn111(void* _Array p1  _Bounds((char *)p1, (char *)p1 + (5 * sizeof(int))));

// A few functions with multiple arguments.
void fn120(int* _Array mid  _Bounds(p1, p1 + 5), int* _Array p1);
void fn122(void* _Array mid  _Bounds((char *)p1, (char *)p1 + (5 * sizeof(int))),
           int* _Array p1);

//
// Test bounds declaration on parameters of function pointer types
//

// These are numbered so that they correspond to the numbering of
// functions in the prior section of tests: fn200 - fn211 take fn100 - fn111 as
//  arguments.  fn220-231 are versions of fn200 - fn211 that take checked
// function pointers.

// Unchecked function pointers
void fn200(void (*fnptr)(int* _Array p1  _Count(5)));
void fn201(void (*fnptr)(int p1  _Count(5)));             // expected-error {{expected 'p1' to have a pointer or array type}}
void fn202(void (*fnptr)(void* _Array p1  _Count(5))); // expected-error {{expected 'p1' to have a non-void pointer type}}

void fn203(int (*fnptr)(int* _Array p1  _Byte_count(5 * sizeof(int))));
void fn204(int (*fnptr)(int *  _Byte_count(5 * sizeof(int))));
void fn205(int (*fnptr)(int p1  _Byte_count(5 * sizeof(int))));
void fn206(int (*fnptr)(float p1  _Byte_count(5 * sizeof(int)))); // expected-error {{expected 'p1' to have a pointer, array, or integer type}}
void fn207(int (*fnptr)(void* _Array p1  _Byte_count(5 * sizeof(int))));

void fn208(void (*fnptr)(int* _Array p1  _Bounds(p1, p1 + 5)));
void fn208a(void(*fnptr)(int* _Nt_array p1  _Bounds(p1, p1 + 5)));
void fn209(void (*fnptr)(int* _Array p1, int p2  _Bounds(p1, p1 + 5)));
void fn210(void (*fnptr)(int* _Array p1, float p2  _Bounds(p1, p1 + 5))); // expected-error {{expected 'p2' to have a pointer, array, or integer type}}
void fn211(void (*fnptr)(void* _Array p1  _Bounds((char *) p1, (char *) p1 + (5 * sizeof(int)))));

// Checked function pointers
void fn220(ptr<void (int* _Array p1  _Count(5))> fnptr);
void fn221(ptr<void (int p1  _Count(5))> fnptr);             // expected-error {{expected 'p1' to have a pointer or array type}}
void fn222(ptr<void (void* _Array p1  _Count(5))> fnptr); // expected-error {{expected 'p1' to have a non-void pointer type}}

void fn223(ptr<int (int* _Array p1  _Byte_count(5 * sizeof(int)))> fnptr);
void fn224(ptr<int (int *  _Byte_count(5 * sizeof(int)))> fnptr);
void fn225(ptr<int (int p1  _Byte_count(5 * sizeof(int)))> fnptr);
void fn226(ptr<int (float p1  _Byte_count(5 * sizeof(int)))> fnptr); // expected-error {{expected 'p1' to have a pointer, array, or integer type}}
void fn227(ptr<int (void* _Array p1  _Byte_count(5 * sizeof(int)))> fnptr);

void fn228(ptr<void (int* _Array p1  _Bounds(p1, p1 + 5))> fnptr);
void fn229(ptr<void (int* _Array p1, int p2  _Bounds(p1, p1 + 5))> fnptr);
void fn230(ptr<void (int* _Array p1, float p2  _Bounds(p1, p1 + 5))> fnptr); // expected-error {{expected 'p2' to have a pointer, array, or integer type}}
void fn231(ptr<void (void* _Array p1  _Bounds((char *)p1, (char *)p1 + (5 * sizeof(int))))> fnptr);

// Function pointers with multiple arguments.
void fn240(ptr<int (int* _Array mid  _Bounds(p1, p1 + 5), int* _Array p1)> fnptr);
void fn241(ptr<int (void* _Array mid  _Bounds((char *)p1, (char *)p1 + (5 * sizeof(int))),
                    int* _Array p1)> fnptr);

//
// Spot check bounds-safe interfaces on parameters of function pointer types
//

void fn250(int(*fnptr)(int *p1  _Count(5)));
void fn251(int(*fnptr)(int *p1  _Byte_count(5 * sizeof(int))));
void fn252(int(*fnptr)(int *p1  _Bounds(p1, p1 + 5)));
//
// Spot check bounds declaration for return values of function pointer types
//

void fn260(int* _Array (*fnptr)(void)  _Count(5));
void fn261(int* _Array(*fnptr)(int i)  _Count(i));
void fn262(int (*fnptr)(void)  _Count(5)); // expected-error {{count bounds expression only allowed for pointer return type}}
void fn263(void* _Array (*fnptr)(void)  _Count(5)); // expected-error {{count bounds expression not allowed for a void pointer return type}}

//
// Test bounds declarations for function pointers
//

void function_pointers(void) {
  // Assignments to function pointers with return bounds on array_ptr types
  int* _Array(*t1)(void)  _Count(5) = fn1;
  int* _Nt_array(*t1a)(void)  _Count(0) = fn1a;
  // Assignment to function pointers with bounds-safe interfaces on
  // unchecked pointer return types.  Unchecked pointers are compatible with
  // unchecked pointers with bounds-safe interfaces.  This extends recursively
  // to parameters and returns of function types.
  int *(*t2)(void) = fn2;
  int *(*t3)(void)  _Count(5) = fn2;
  ptr<int *(void)  _Count(5)> t4 = fn2;

  int *(*t5)(int i) = fn3;
  int *(*t6)(int i)  _Count(i) = fn3;
  ptr<int *(int j)  _Count(j)> t7 = fn3;

  int* _Array(*t8)(void)  _Byte_count(5 * sizeof(int)) = fn4;
  void* _Array(*t9)(void)  _Byte_count(5 * sizeof(int)) = fn5;
  int *(*t10)(void) = fn6;
  int *(*t11)(void)  _Byte_count(5*sizeof(int)) = fn6;
  ptr<int *(void)  _Byte_count(5*sizeof(int))> t12 = fn6;

  int* _Array(*t13)(void)  _Bounds(s1, s1 + 5) = fn10;
  int* _Nt_array(*t13a)(void)  _Bounds(s1, s1 + 5) = fn10a;
  int *(*t14)(void) = fn12;
  int *(*t15)(void)  _Bounds(s1, s1 + 5) = fn12;
  int *(*t16)(void)  _Bounds(s1, s1 + 6) = fn12;    // expected-warning {{incompatible function pointer types}}
  ptr<int *(void)  _Bounds(s1, s1 + 6)> t17 = fn12; // expected-error {{incompatible type}}

  // Unchecked pointer to function assigned to checked pointer to
  // function.
  ptr<int* _Array(void)  _Count(5)> t100 = fn1;
  // The reverse is not allowed
  int* _Array(*t101)(void)  _Count(5) = t100; // expected-error {{incompatible type}}

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
  int* _Array(*t1)(void)  _Count(4) = fn1;  // expected-error {{incompatible type}}
  ptr<int* _Array(void)  _Count(4)> t1a = fn1;  // expected-error {{incompatible type}}
  int* _Array(*t4)(void)  _Byte_count(6 * sizeof(int)) = fn4; // expected-error {{incompatible type}}
  int* _Array(*t10)(void)  _Bounds(s1, s1 + 4) = fn10; // expected-error {{incompatible type}}
}

// Test type checking of return_value

extern int arr[10];
int f300(void)  _Bounds(arr, arr + return_value);
int* _Array f301(void)  _Bounds(return_value, return_value + 10);
int* _Array f302(void)  _Bounds(return_value - 5, return_value + 5);
int* _Array f303(void)  _Count(return_value); // expected-error {{invalid argument type}}
int* _Array f304(void)  _Bounds(arr, arr + (return_value << 5)); // expected-error {{invalid operands to binary expression}}
void* _Array f305(void)  _Bounds(return_value, return_value + 5); // expected-error {{arithmetic on a pointer to void}}
