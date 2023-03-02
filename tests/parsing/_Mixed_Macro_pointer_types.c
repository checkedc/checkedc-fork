// Feature tests of parsing new Checked C pointer types.
//
// * Test that checked pointer types can be used as parameter types, return
//   types, local variable types, structure member types, in typedefs, and as
//   arguments to operators that take types.
// * Test parsing of array types and function types combined with checked
//   ponter types.
// * Test the case of nested checked types such as ptr<ptr<T>>, where >>
//   could be mistaken for the right-shift operator.
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify %s
// expected-no-diagnostics

#include <stdchecked.h>

//
// parameters have new pointer types
//

extern void f1(int* _Single p, int y) {
   *p = y;
}

extern void f2( int* _Single const p, int y) {
   *p = y;
}

extern void f3(int *_Single const p, int y) {
   y = *p;
}

extern void f4(ptr<int* _Single> p, int y) {
   **p = y;
}

extern void f5(ptr<ptr<int* _Single>>  p, int y) {
   ***p = y;
}

extern void f6(int *_Array p  _Count(1), int y) {
    *p = y;
}

extern void f7(int *_Array p  _Count(1), int y) {
   *p = y;
   f6(p, y);
}

extern void f8(int *_Nt_array p  _Count(1), int y) {
  *p = y;
}

extern void f9(int *_Nt_array p  _Count(1), int y) {
  *p = y;
  f8(p, y);
}

extern void f10(nt_array_ptr<int* _Nt_array>p  _Count(1),
                int *_Nt_array y) {
  *p = y;
}

//
// Second parameter is a new pointer type
//

extern void g1(int y, int* _Single p) {
   *p = y;
}

extern void g2(int y,  int* _Single const p) {
   *p = y;
}

extern void g3(int y, const int *_Single p) {
   y = *p;
}

extern void g4(int y, ptr<int* _Single> p) {
   **p = y;
}

extern void g5(int y, ptr<ptr<int* _Single>>  p) {
   ***p = y;
}


extern void g6(int y, int *_Array p  _Count(1)) {
   *p = y;
   f7(p, y);
}

extern void g7(int y, int *_Nt_array p  _Count(1)) {
  *p = y;
  f9(p, y);
}


//
// returns a new pointer type
//

extern int* _Single h1(int y, int* _Single p) {
   *p = y;
   return p;
}

extern const int* _Single h2(int y,  int* _Single const p) {
   *p = y;
   return p;
}

extern const int *_Single h3(int y, array_ptr<int *const _Single> p  _Count(1)) {
   y = **p;
   return *p;
}

extern ptr<int* _Single> h4(int y, ptr<int* _Single> p) {
   **p = y;
   return p;
}

extern ptr<ptr<int* _Single>>  h5(int y, ptr<ptr<int* _Single>>  p) {
   ***p = y;
   return p;
}

extern int *_Array h6(int y, int *_Array p) {
  return p;
}


extern int *_Single *_Array h7(int y, int *_Single *_Array p  _Count(1)) {
  **p = y;
  return p;
}

extern int *_Nt_array h8(int y, int *_Nt_array p) {
  return p;
}


extern nt_array_ptr<ptr<int>> h9(int y, nt_array_ptr<ptr<int>> p  _Count(1)) {
  **p = y;
  return p;
}

//
// Local variables with pointer types
//

extern void k1(int y)
{
   int v = y;
   int* _Single t1 = &v;
   int *_Array t2  _Count(1) = &v;
   int *_Single *_Array t3   _Count(1) = &t1;
   int *_Nt_array t4 = 0;
   *t1 = 0;
   *t2 = 0;
   *t3 = 0;
}

//
// Struct member with pointer types;
//

struct Vector {
    array_ptr<float> data  _Count(len);
    int len;
};

extern int Multiply(struct Vector vec1, struct Vector vec2) {
    if (vec1.len != vec2.len) {
       return 1;
    }
    for (int i = 0; i < vec1.len; i++) {
        vec1.data[i] *= vec2.data[i];
    }
    return 0;
}

extern int Multiply2(struct Vector* _Single vec1p, struct Vector* _Single vec2p) {
    if (vec1p->len != vec2p->len) {
       return 1;
    }
    for (int i = 0; i < vec1p->len; i++) {
        vec1p->data[i] *= vec2p->data[i];
    }
    return 0;
}

struct StringWrapper {
  char* _Nt_array str  _Count(0);
};

//
// Declaring pointers to functions
//

// An unchecked pointer to a function that takes two integer parameters and returns an integer.
int (*unchecked_ptr_to_func)(int x, int y);
// A ptr to a function that takes two integer parameters and returns an integer
int (*_Single ptrfunc)(int x, int y);
// Not allowed: an array_ptr of a function type:
// array_ptr<int (int x, int y)> array_ptrfunc;
// Function types have no size, so bounds checking does not make sense.
//
// Allowed: An array_ptr to an array of function pointers.
int (*_Single *_Array array_ptr_of_ptrfunc)(int x, int  y) ;
int (*_Single *_Nt_array nullterm_array_ptr_of_ptrfunc)(int x, int  y);

//
// Declaring pointers to arrays and arrays of pointers
//
int (*unchecked_ptr_to_array)[5];
int* _Single ptr_to_array[5];
int* _Array array_ptr_to_array[5];
// not allowed: null terminated array_ptr to array
// nt_array_ptr<int[5]> nullterm_array_ptr_to_array;

int(*unchecked_ptr_to_incomplete_array)[];
int (* _Single ptr_to_incomplete_array)[];
int (* _Array array_ptr_to_incomplete_array)[];
// not allowed: null terminated array_ptr to incomplete array
// nt_array_ptr<int[]> nullterm_array_ptr_to_incomplete_array;

// Declaring arrays of pointers
int *array_of_unchecked_ptrs[5];
int* _Single array_of_ptrs[5];
int *_Array array_of_array_ptrs[5];
int *_Nt_array array_of_nullterm_pointers[5];

// Declaring null-terminated arrays of pointers
int *nullterm_array_of_unchecked_ptrs nt_checked[5];
int* _Single nullterm_array_of_ptrs nt_checked[5];
int *_Array nullterm_array_of_array_ptrs nt_checked[5];
int *_Nt_array nullterm_array_of_nullterm_pointers nt_checked[5];

// Declare an unchecked pointer to arrays of pointers
int *(*uncheckedptr_to_array_of_unchecked_ptrs)[5];
int* _Single(*unchecked_ptr_to_array_of_ptrs)[5];
int *_Array(*unchecked_ptr_to_array_of_array_ptrs)[5];
int *_Nt_array(*unchecked_ptr_to_array_of_null_term_array_ptrs)[5];

int *(*uncheckedptr_to_nullterm_array_of_unchecked_ptrs) nt_checked[5];
int* _Single(*unchecked_ptr_to_nullterm_array_of_ptrs) nt_checked[5];
int *_Array(*unchecked_ptr_to_null_termarray_of_array_ptrs) nt_checked[5];
int *_Nt_array(*unchecked_ptr_to_null_term_array_of_null_term_array_ptrs)nt_checked[5];

// Declare ptr to arrays of pointers
int * *_Single ptr_to_array_of_unchecked_ptrs[5];
int* _Single *_Single ptr_to_array_of_ptrs[5];
int *_Array *_Single ptr_to_array_of_array_ptrs[5];
int *_Nt_array *_Single ptr_to_array_of_nullterm_array_ptrs[5];

// Declare ptr to nullterm arrays of pointers
int * *_Single ptr_to_nullterm_array_of_unchecked_ptrs nt_checked[5];
int* _Single ptr_to_nullterm_array_of_ptrs nt_checked[5];
int *_Array *_Single  ptr_to_nullterm_array_of_array_ptrs nt_checked[5];
int *_Nt_array *_Single ptr_to_nullterm_array_of_nullterm_array_ptrs nt_checked[5];


// Declare ptr to an array of function pointers
ptr<int (*[5])(int x, int y)> ptr_to_array_of_unchecked_func_ptrs;
int (*_Single *_Single ptr_to_array_of_checked_func_ptrs[5])(int x, int y);
// Make parameter and return types be ptrs too.
ptr<ptr<int* _Single (int* _Single x, int* _Single y)>[5]> ptr_to_array_of_checked_func_ptrs_with_ptr_parameters;

//
// Typedefs using checked pointer types
//

typedef int* _Single t_ptr_int;
typedef int (*_Single t_ptr_func)(int x, int y);
typedef int *_Array t_array_ptr_int;
typedef int *_Single *_Array t_array_ptr_ptr_int;
typedef int *_Nt_array t_nullterm_array_ptr_int;
typedef int *_Single *_Nt_array t_nullterm_array_ptr_ptr_int;

//
// Operators that take types
//

void parse_operators_with_types(void) {
    int s1 = sizeof(int* _Single);
    int s2 = sizeof(int *_Array);
    int s3 = sizeof(int *_Nt_array);
    int s4 = sizeof(int (*_Single)[5]);
    int s5 = sizeof(int (*_Array)[5]);
    // not allowed: sizeof(nt_array_ptr<int[5]>);
    int s6 = sizeof(int* _Single[5]);
    int s7 = sizeof(int *_Array[5]);
    int s8 = sizeof(int *_Nt_array[5]);
    // C11 spec says sizeof function types is illegal, but clang accepts it.
    int s9 = sizeof(int(int x, int y));
    int s10 = sizeof(int* _Single(int x, int y));
    // These are OK
    int s11 = sizeof(int (*_Single)(int x, int y));
    int s12 = sizeof(int(*)(int x, int y));

    int s20 = _Alignof(int* _Single);
    int s21 = _Alignof(int *_Array);
    int s22 = _Alignof(int *_Nt_array);
    int s23 = _Alignof(int (*_Single)[5]);
    int s24 = _Alignof(int (*_Array)[5]);
    // not allowed: _Alignof(nt_array_ptr<int[5]>);
    int s25 = _Alignof(int* _Single[5]);
    int s26 = _Alignof(int *_Array[5]);
    int s27 = _Alignof(int *_Nt_array[5]);
    // C11 spec says _Alignof function types is illegal, but clang accepts it.
    int s28 = _Alignof(int(int x, int y));
    int s29 = _Alignof(int* _Single(int x, int y));
    // These are OK
    int s30 = _Alignof(int (*_Single)(int x, int y));
    int s31 = _Alignof(int(*)(int x, int y));

    // Test parsing of some cast operations that should pass checking
    // of bounds declarations.
    int x = 0;
    int arr[5];
    int* _Single px = (int* _Single) &x;
    int *_Array pax = (int *_Array) &x;
    int nt_arr nt_checked[5];
    int *_Nt_array nt_pax = (int *_Nt_array) nt_arr;
    // ptr to array type
    int (*_Single parr)[5] = 0;
    parr = &arr;
    // ptr to function type
    int (*_Single pfunc)(int x, int y) = (int (*_Single) (int x, int y)) 0;
    int (*_Single (*_Single ptr_to_pfunc_arr)[5])(int x, int y)= (int (*_Single (*_Single)[5])(int x, int y)) 0;
}
