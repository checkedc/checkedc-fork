// Feature tests of parsing new Checked C member bounds declarations.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify %s

#include <stdchecked.h>

struct S1 {
  int* _Array arr  _Count(5);
};

struct S2 {
  int* _Array arr  _Count(3 + 2);
};

struct S3 {
  int len;
  int* _Array arr  _Count(len);
};

struct S4 {
  int* _Array arr  _Count(len);
  int len;
};

struct S5 {
  int* _Array arr  _Byte_count(20);
};

struct S6 {
  int* _Array arr  _Byte_count(5 * sizeof(int));
};

struct S7 {
  int* _Array arr  _Bounds(arr, arr + 5);
};

// Structure with a variable length array at the end
struct S8 {
  int len;
  int arr checked[]  _Count(len);
};

// Pointer into the middle of an array
struct S9 {
  int start;
  int* _Array arr  _Bounds(arr - start, arr - start + 5);
};

struct S10 {
  int* _Array arr  _Bounds(unknown);
};

// Count is a contextual keyword.  It is only a keyword when it immediately
// folows the ':' in a bounds declaration.  Otherwise it can be used as an
// identifer.
struct S11 {
  int count;
  int* _Array arr  _Bounds(unknown);
};

struct S12 {
  int count;
  int* _Array arr  _Count(count);
};

struct S13 {
  // 'unknown' is a contextual keyword.  It is only a keyword when it
  // is the sole argument to a 'bounds' expression.
  // not a keyword
  int unknown;
  // a keyword
  int* _Array arr1  _Bounds(unknown);
  // not a keyword
  int* _Array arr2  _Count(unknown);
  // a keyword
  int* _Array arr3  _Bounds(unknown + arr2, unknown + arr2 + 5); // expected-error {{expected ')'}} \
                                                              // expected-note {{to match this '('}}
  // not a keyword
  int* _Array arr4  _Bounds(arr3, arr3 + unknown); 
};

struct S14 {
  // 'bounds' is a contextual keyword.  It is only a keyword when it follows
  // the ':' in a bounds declaration.

  // not a keyword
  int bounds;   
  // not a keyword as an argument
  int* _Array arr1  _Bounds(bounds + arr1, bounds + arr1 + 2);
  // a keyword
  int* _Array arr2  _Bounds(unknown);
  // not a keyword as an argument.
  int* _Array arr3  _Bounds(bounds + arr2, bounds + arr2 + 5);
  // not a keyword
  int* _Array arr4  _Bounds(arr3, arr3 + bounds);
};

//
// Parsing of more complicated structure member declarations with bounds
// declarations
//

struct S15 {
  // Members that are array_ptrs to ptrs
  int* _Single *_Array arr1  _Count(5);
  int* _Single *_Array arr2  _Byte_count(5 * sizeof(int *_Single));
  int* _Single *_Array arr3  _Bounds(arr3, arr3 + 5);
  // pointer into middle of arr3
  int* _Single *_Array mid1  _Bounds(arr3, arr3 + 5);
  // Members that are array_ptrs to unchecked arrays
  int *_Array arr4[10]  _Count(5);
  int* _Array arr5[10]  _Byte_count(5 * sizeof(int *_Single));
  int* _Array arr6[10]  _Bounds(arr6, arr6 + 5);
  // pointer into middle of arr6
  int* _Array mid2[10]  _Bounds(arr6, arr6 + 5);
  // Members that are array_ptrs to checked arrays
  int* _Array arr7 checked[10]  _Count(5);
  int* _Array arr8 checked[10]  _Byte_count(5 * sizeof(int *_Single));
  int* _Array arr9 checked[10]  _Bounds(arr9, arr9 + 9);
  // Members that are unchecked pointers with bounds.  These will
  // be used for interoperation.
  int *arr10  _Count(5);
  int *arr11  _Byte_count(5 * sizeof(int));
  int *arr12  _Bounds(arr12, arr12 + 9);
  // Members that are arrays and that have bounds specified for them.
  // Legal but not too interestting.
  int arr13[10]  _Count(9);
  int arr14[10]  _Byte_count(9 * sizeof(int));
  int arr15[10]  _Bounds(arr15, arr15 + 9);
};

// Members that are pointers to functions that have bounds declarations on
// return values
extern void S16(void) {
  // Checked pointer to a function that returns an array_ptr to 5 integers.
  ptr<int* _Array(void)  _Count(5)> p1 = 0;
  // Checked pointer to a function that returns an array_ptr to n integers,
  // where n is n argument.
  ptr<int* _Array(int n)  _Count(n)> p2 = 0;
  // Use 'byte_count; instead of 'count'
  ptr<int* _Array(void)  _Byte_count(5 * sizeof(int))> q1 = 0;
  ptr<int(int arg)  _Byte_count(5 * sizeof(int))> q2 = 0;
  ptr<int(int n, int arg)  _Byte_count(n * sizeof(int))> q3 = 0;
  // Use 'bounds' instead of 'count'.
  ptr<int* _Array(int* _Array arg  _Count(5))  _Bounds(arg, arg + 5)>
    r1 = 0; 
  ptr<int(int* _Array arg  _Count(n), int n)  _Bounds(arg, arg + n)> r2 = 0;
  // Unchecked pointers to functions.
  int(*s1)(int* _Array  _Count(5));
  int(*s2)(int* _Array arg  _Count(5));
  int(*s3)(int n, int* _Array arg  _Count(n));
  int(*s4)(int* _Array arg  _Count(n), int n);
  // Use 'byte_count' instead of 'count'.
  int(*t1)(int* _Array  _Byte_count(5 * sizeof(int)));
  int(*t2)(int* _Array arg  _Count(5 * sizeof(int)));
  int(*t3)(int n, int* _Array arg  _Count(n * sizeof(int)));
  int(*t4)(int* _Array arg  _Count(n * sizeof(int)), int n);
}

struct S17 {
  // Members that are pointers to functions that have bounds on
  // arguments or return values.
  int (*f1)(int len, int* _Array arr  _Count(len));
  int* _Array (*f2)(int len, int* _Array arr  _Count(len))  _Count(len);
  // same as f1, but checked ptr
  ptr<int (int len, int* _Array arr  _Count(len))> f3; 
  // same as f2, but checked ptr
  ptr<int* _Array (int len, int* _Array arr  _Count(len))  _Count(len)> f4;
  // Use count as a parameter variable name.
  int(*f5)(int count, int* _Array arr  _Count(count));
  // Unchecked pointer to a function that returns a ptr to a function that
  // takes in a length and returns an array_ptr of that length.
  ptr<int* _Array(int len)  _Count(len)> (*f6)(int len);
  // Checked pointer to a function instead.
  ptr<ptr<int* _Array(int len)  _Count(len)> (int len)> f7;
};

// Bounds distributed across multiple nested members
struct S18 {
  struct S19 {
     int* _Array lower;
     int* _Array upper;
  } pair;
  int* _Array arr1  _Bounds(pair.lower, pair.upper);
  struct S20 {
    int* _Array arr2  _Bounds(pair.lower, pair.upper);
  } nested;
};

// Anonymous struct version
struct S21 {
  struct {
    int* _Array lower;
    int* _Array upper;
  } pair;
  int* _Array arr1  _Bounds(pair.lower, pair.upper);
  struct {
    int* _Array arr2  _Bounds(pair.lower, pair.upper);
  } nested;
};

//
// Errors in declaring structure members with bounds
//

struct S22 {
  int* _Array arr  _Bounds(arr, unknown_id); // expected-error {{use of undeclared member}}
};

struct S23 {
  int* _Array arr : 6 + 6; // expected-error {{expected bounds expression}}
};

// Misspell bounds to cause an semantic checking error.
// clang will parse this as a constant-expression that specifies a bit field
// and generate several errors.
struct S24 {
  int len;
  int* _Array arr : boounds(arr, arr + 5);  // expected-error 2 {{use of undeclared identifier 'arr'}} \
                                               // expected-warning {{implicit declaration of function 'boounds'}}
};

// Misspell count to cause an semantic checking error.
// clang will parse this as a constant-expression that specifies a bit field
// and generate several errors.
struct S25 {
  int len;
  int* _Array arr : coount(5); // expected-error {{expected bounds expression}} \
                                  // expected-warning {{implicit declaration of function 'coount'}}
};

// Omit an argument to bounds to cause a parsing error
struct S26 {
  int len;
  int* _Array arr  _Bounds(arr); // expected-error {{expected ','}}
};

// Omit both arguments to bounds to cause a parsing error
struct S27 {
  int len;
  int* _Array arr  _Bounds(); //expected-error {{expected expression}}
};

// Omit the argument to count to cause a parsing error.
struct S28 {
  int len;
  int* _Array arr  _Count(); //expected-error {{expected expression}}
};

//
// Member bounds cannot refer to variables.
//
int* _Array global_bound;

struct S29 {
  int len;
  int* _Array arr  _Bounds(global_bound, global_bound + len); // expected-error 2 {{use of undeclared member 'global_bound'}}
};


int f1(void) {
  int buffer checked[100];
  struct S30 {
     int len;
     int* _Array arr  _Bounds(buffer, buffer + len); // expected-error 2 {{use of undeclared member 'buffer'}}
  };
}

int f2(void) {
  const int bounds = 4;
  struct S31 {
    // This should be parsed as an incorrect bounds expression+-
    int x : bounds; // expected-error {{expected '(' after 'bounds'}}
  };
}

int f3(void) {
  enum E {
    bounds = 4
  };
  struct S30 {
    // This should be parsed as an incorrect bounds expression+-
    int x : bounds; // expected-error {{expected '(' after 'bounds'}}
  };
}

//
// Union members
//

// Union that is bounds-safe because its only members are scalar types
// and arrays of scalar types.
union U1 {
  int i;
  float f;
  char arr checked[sizeof(int)];
};

// Union that is bounds safe because all members point to objects
// with the same size that are arrays of scalars.
union U2 {
  int* _Array ip  _Count(4);
  char* _Array cp  _Count(4 * sizeof(int));
};

// Unions where dynamic tags are used to ensure correct use of members.
// There still needs to be a design that connects a particular tag value
// with the validity of a specific member.
union U3 {
  enum E {
    Array,
    Ptr,
    Integer
  } tag;

  // TODO: enum identifiers should be accessible in
  // member bounds expressions
  int* _Array f _Count((tag == 0) ? 5 : 0);
  int *_Single p;
  int i;
};

// Union of pointers where lengths depend on a tag in an enclosing structure
struct S32 {
  int len : 31;
  int tag : 1;
  union {
    char* _Array cp  _Count(tag ? len : 0);
    int* _Array ip  _Count(!tag ? 0 : len);
  };
};

// Unions without tags that would depend on program invariants for bounds safety.
union U5 {
  _Bool isInteger;
  int* _Single ip;
  float* _Single fp;
};

union U6 {
  int* _Array f  _Count(5);
  int *_Single p;
  int i;
};
