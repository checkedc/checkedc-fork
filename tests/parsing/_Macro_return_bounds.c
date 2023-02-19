// Feature tests of parsing new Checked C bounds declarations for
// function return values
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -verify-ignore-unexpected=note %s

#include <stdchecked.h>

// Parsing of function declarations
extern void* _Array alloc(unsigned size)  _Byte_count(size);
extern int* _Array f2(int* _Array arr  _Count(5))  _Count(3 + 2);
extern int* _Array f3(int len, int* _Array arr  _Count(len)) 
                       _Count(len);
extern int* _Array f4(int* _Array arr  _Byte_count(20))
                       _Byte_count(20);
extern int* _Array f5(int* _Array arr  _Byte_count(5 * sizeof(int)))
                       _Byte_count(5 * sizeof(int));
extern int* _Array f6(int* _Array arr  _Bounds(arr, arr + 5))
                       _Bounds(arr, arr + 5);
extern int* _Array f7(int start,
                         int* _Array arr  _Bounds(arr - start, arr - start + 5))
                       _Bounds(arr - start, arr - start + 5);
extern char* _Array f8(void)  _Bounds(unknown);

// Parsing of return bounds expressions containing return_value.  Test this by
// expanding count/byte_count expressions into bounds expressions.
extern void* _Array alloc2(unsigned size) :
   bounds(return_value, (char* _Array) return_value + size);

extern int* _Array f3a(int len, int* _Array arr  _Count(len))
                       _Bounds(return_value, return_value + len);

// count, bounds, and unknown are contextual keywords.  They are treated as keyword
// only when they immediately follow a ':';
extern char* _Array f9(int count)  _Count(count);
extern char* _Array f10(int unknown)  _Count(unknown);
extern int* _Array f11(int bounds, int* _Array arr  _Count(bounds))
                       _Bounds(arr, arr + bounds);

// Parsing function definitions.
extern int* _Array f1(int* _Array arr  _Count(5))  _Count(5) {
  return arr;
}

extern int* _Array f2(int* _Array arr  _Count(5))  _Count(3 + 2) {
  return arr;
}

extern int* _Array f3(int len,
                         int* _Array arr  _Count(len))
                       _Count(len) {
  return arr;
}

extern int* _Array f4(int* _Array arr  _Byte_count(20))
                       _Byte_count(20) {
  return arr;
}

extern int* _Array f5(int* _Array arr  _Byte_count(5 * sizeof(int)))
                       _Byte_count(5 * sizeof(int)) {
  return arr;
}

extern int* _Array f6(int* _Array arr  _Bounds(arr, arr + 5))
                       _Bounds(arr, arr + 5) {
  return arr;
}

extern int* _Array f7(int start,
                         int* _Array arr  _Bounds(arr - start, arr - start + 5))
                       _Bounds(arr - start, arr - start + 5) {
   return arr;
}

extern char* _Array f8(void)  _Bounds(unknown) {
  return 0;
}

// 'count' is a contextual keyword.  It is only a keyword when it
// immediately follows the ':' in a bounds declaration.
extern char* _Array f9(int count)  _Count(count) {
  return 0;
}

// 'unknown' is a contextual keyword.  It is only a keyword when it
// immediately follows the ':' in a bounds declaration.
extern char* _Array f10(int unknown)  _Count(unknown) {
  return 0;
}

// 'bounds' is a contextual keyword.  It is only a keyword when it
// immediately follows the ':' in a bounds declaration.
extern int* _Array f11(int bounds, int* _Array arr  _Count(bounds))
                       _Bounds(arr, arr + bounds) {
  return arr;
}

//
// Parsing of more complicated function declarations with bounds declarations
//

// Functions that return array_ptrs to ptrs

extern int *_Single *_Array f12(int *_Single *_Array arr  _Count(5))  _Count(5) {
  return arr;
}

extern int *_Single *_Array f13(int *_Single *_Array arr  _Count(5))
                            _Byte_count(5 * sizeof(int*_Single)) {
  return arr;
}

extern int *_Single *_Array f14(int *_Single *_Array arr  _Count(5))
                            _Bounds(arr, arr + 5) {
  return arr;
}

// Functions that return array_ptrs to arrays
typedef  int (* _Array arrayptr10)[10];
extern arrayptr10 f15(arrayptr10 arr  _Count(5))  _Count(3) {
  return arr;
}

extern arrayptr10 f16(arrayptr10 arr  _Count(5))
                           _Byte_count(3 * sizeof(int[10])) {
  return arr;
}

extern arrayptr10 f17(arrayptr10 arr  _Count(5))
                           _Bounds(arr, arr + 3) {
  return arr;
}

// Functions that return unchecked pointers to arrays, but that declare bounds
// on the unchecked pointers.  These will be used for interoperation.

extern int (*f18(int (*arr)[10]  _Count(5))  _Count(3))[10] {
  return arr;
}

// Functions that return pointers to functions.  Note that array_ptrs to
// function types are not allowed, but array_ptr to pointers to functions type
// are allowed.

// Function that returns a ptr to a function that takes in a length and returns
// an array_ptr of that length.
typedef int* _Array f19_temp(int len) _Count(len);
extern f19_temp* _Single f19(int len) {
  return 0;
}

// Check that return_value can be used.  Expand count to a bounds expression.
typedef int* _Array f19a_temp(int len) _Bounds(return_value, return_value + len);
extern f19a_temp* _Single f19a(int len) {
  return 0;
}
// Like the prior function, but returns an unchecked pointer instead. The
// unchecked pointer points to a function that takes in a length and returns an
// array_ptr of that length.
//
// Parsing C declarators with * and () (for function declarators) intermixed
// can be confusing. To read this declaration, start from the innermost
// declarator and work your way out.  f19 is a function that takes an integer
// argument and returns a pointer.  Function declarator parens take precedence
//  over *.  The pointer points to function that takes an integer argument and
// returns an int* _Array.
extern int* _Array (*f20(int arg))(int len)  _Count(len) {
  return 0;
}

// Function that returns an array pointer to ptrs to functions that take in a
// length and return int* _Arrays of that length.
typedef int* _Array f21_temp(int len) _Count(len);
typedef f21_temp* _Single f21_temp2;
extern f21_temp2* _Array f21(int arg)  _Count(arg) {
  return 0;
}

// Check that return_value can be used.  Expand count to a bounds expression.
typedef int* _Array f21a_temp(int len) _Bounds(return_value, return_value + len);
typedef f21a_temp* _Single f21a_temp2;
extern  f21a_temp2* _Array f21a(int arg) _Bounds(return_value, return_value + arg) {
  return 0;
}


// Use unchecked pointers instead. This is a function that returns a pointer to
// a pointer to a function that take in a length and return int* _Arrays of
// that length.
extern int* _Array (**f22(int arg)  _Count(arg))(int len)  _Count(len) {
  return 0;
}

// Re-use len in the function type returned by the function and as an argument.
typedef int* _Array f23_temp(int len) _Count(len);
typedef f23_temp* _Single f23_temp2;
extern f23_temp2* _Array f23(int len)  _Count(len) {
  return 0;
}

// Pointers to functions that have bounds declarations on return values
extern void f24(void) {
  // Checked pointer to a function that returns an array_ptr to 5 integers.
  int* _Array (*_Single p1)(void)  _Count(5) = 0;
  // Checked pointer to a function that returns an array_ptr to n integers,
  // where n is n argument.
  int* _Array (*_Single p2)(int n)  _Count(n) = 0;
  // Use 'byte_count; instead of 'count'
  int* _Array (*_Single q1)(void)  _Byte_count(5 * sizeof(int)) = 0;
  int (*_Single q2)(int arg)  _Byte_count(5 * sizeof(int)) = 0;
  int (*_Single q3)(int n, int arg)  _Byte_count(n * sizeof(int)) = 0;
  // Use 'bounds' instead of 'count'.
  int* _Array (*_Single r1)(int* _Array arg  _Count(5))  _Bounds(arg, arg + 5) = 0;
  int (*_Single r2)(int* _Array arg  _Count(n), int n)  _Bounds(arg, arg + n) = 0;
  // Unchecked pointers to functions.
  int(*s1)(int* _Array  _Count(5)) = 0;
  int(*s2)(int* _Array arg  _Count(5)) = 0;
  int(*s3)(int n, int* _Array arg  _Count(n)) = 0;
  int(*s4)(int* _Array arg  _Count(n), int n) = 0;
  // Use 'byte_count' instead of 'count'.
  int(*t1)(int* _Array  _Byte_count(5 * sizeof(int))) = 0;
  int(*t2)(int* _Array arg  _Count(5 * sizeof(int))) = 0;
  int(*t3)(int n, int* _Array arg  _Count(n * sizeof(int))) = 0;
  int(*t4)(int* _Array arg  _Count(n * sizeof(int)), int n) = 0;
}

typedef int* _Array func1(int len)  _Count(len);
typedef int func2(int* _Array arr  _Count(len), int len)
             _Bounds(arr, arr + len);
static func1 *func1_ptr1;
static func2 *func2_ptr1;
func1 *func1_ptr2;

extern void f25(void) : 6 + 6 { // expected-error {{expected bounds expression}}
}

extern char* _Array f26(void)  _Count(len) { // expected-error {{use of undeclared identifier 'len'}}
  return 0;
}

// Misspell bounds to cause a parsing error.
extern int* _Array f27(int len, int* _Array arr  _Count(len)) : boounds(arr, arr + len)) { // expected-error {{expected bounds expression}}
  return 0;
}

// Misspell count to cause a parsing error.
extern int* _Array f28(int len) : coount(len) { // expected-error {{expected bounds expression}}
  return 5;  // expected-error {{incompatible result type}}
}

// Omit an argument to bounds to cause a parsing error
extern int* _Array f29(int len, int* _Array arr  _Count(len))  _Bounds(arr) { // expected-error {{expected ','}}
  return 0;
}

// Omit both arguments to bounds to cause a parsing error
extern int* _Array f30(int len, int* _Array arr  _Count(len))  _Bounds() { // expected-error {{expected expression}}
  return 0;
}

// Omit the argument to count to cause a parsing error.
extern int* _Array f31(int len)  _Count() { // expected-error {{expected expression}}
  return 5;  // expected-error {{incompatible result type}}
}

// Misplace a return bounds expression for a function with a complex
// declarator.

// f32 is a function that returns a pointer to an array of 10 integers.  The
// return bounds expression must be part of the function declarator and
// should not follow the array declarator.
int(*(f32(int arg[10][10])))[10]  _Count(10); // expected-error {{unexpected bounds expression after declarator}}

int(*(f32(int arg[10][10])))[10]  _Count(10) { // expected-error {{unexpected bounds expression after declarator}}
  return arg;
}

// A return bounds expression cannot follow a parenthesized function declarator
int *(f33(int i))  _Count(10); // expected-error {{unexpected bounds expression after declarator}}
int *(f33(int i))  _Count(10) { // expected-error {{unexpected bounds expression after declarator}}
  return 0;
}

//
// _Return value can only be used in a return bounds expression
//

int f40(int* _Array a  _Bounds(return_value, return_value + 10)); // expected-error 2 {{_Return_value can be used only in a return bounds expression}}

int f41(void) {
  return_value = 0;  // expected-error {{_Return_value can be used only in a return bounds expression}}
}

// Make sure that presence of a return bounds expression doesn't cause
// return_value to be allowed in the body of a function.
int* _Array f42(void)  _Bounds(return_value, 5 + return_value) {
   return_value = 0; // expected-error {{_Return_value can be used only in a return bounds expression}}
}

// Make sure return_value is not allowed at top-level.
int x = return_value; // expected-error {{_Return_value can be used only in a return bounds expression}}
