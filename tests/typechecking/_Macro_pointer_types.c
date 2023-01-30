// Unit tests for typechecking new Checked C pointer types.
//
// The following line is for the LLVM test harness:
// RUN: %clang_cc1 -Wno-unused-value -Wno-pointer-bool-conversion -verify -verify-ignore-unexpected=note %s
//

#include <stdchecked.h>

extern void check_indirection_unsafe_ptr(int *p, const int *const_p, int y) {
	*p = y;
	y = *p;
	*const_p = y; // expected-error {{read-only variable is not assignable}}
	y = *const_p;
}

extern void check_indirection_ptr(int* _Single p, const int *_Single const_p, int y) {
	*p = y;
	y = *p;
	*const_p = y; // expected-error {{read-only variable is not assignable}}
	y = *const_p;
}

extern void check_indirection_array_ptr(int* _Array p : count(1),
                                        const int * _Array const_p : count(1),
                                        int y) {
	*p = y;
	y = *p;
	*const_p = y; // expected-error {{read-only variable is not assignable}}
	y = *const_p;
}

extern void check_indirection_nt_array_ptr(int* _Nt_array p : count(1),
                                           const int *_Nt_array const_p : count(1),
                                           int y) {
  *p = y;
  y = *p;
  *const_p = y; // expected-error {{read-only variable is not assignable}}
  y = *const_p;
}

extern void check_subscript_unsafe_ptr(int *p, int y) {
    p[0] = y;
    y = p[0]; 
    0[p] = y;
    y = 0[p];
}

extern void check_subscript_ptr(int* _Single p, const int *_Single p_const, int y) {
   p[0] = y;  // expected-error {{subscript of '_Ptr<int>'}}
   y = p[0];  // expected-error {{subscript of '_Ptr<int>'}}
   0[p] = y;  // expected-error {{subscript of '_Ptr<int>'}}
   y = 0[p];  // expected-error {{subscript of '_Ptr<int>'}}
   p_const[0] = y;  // expected-error {{subscript of '_Ptr<const int>'}}
   y = p_const[0];  // expected-error {{subscript of '_Ptr<const int>'}}
   0[p_const] = y;  // expected-error {{subscript of '_Ptr<const int>'}}
   y = 0[p_const];  // expected-error {{subscript of '_Ptr<const int>'}}
}

extern void check_subscript_array_ptr(int* _Array p : count(1),
                                      const int * _Array p_const : count(1),
                                      int y) {
   p[0] = y;  // OK
   y = p[0];  // OK
   0[p] = y;  // OK
   y = 0[p];  // OK
   p_const[0] = y;  // expected-error {{read-only variable is not assignable}}
   y = p_const[0];  // OK
   0[p_const] = y;  // expected-error {{read-only variable is not assignable}}
   y = 0[p_const];  // OK
}

extern void check_subscript_nt_array_ptr(int* _Nt_array p : count(1),
                                         const int *_Nt_array p_const : count(1),
                                         int y) {
  p[0] = y;  // OK
  y = p[0];  // OK
  0[p] = y;  // OK
  y = 0[p];  // OK
  p_const[0] = y;  // expected-error {{read-only variable is not assignable}}
  y = p_const[0];  // OK
  0[p_const] = y;  // expected-error {{read-only variable is not assignable}}
  y = 0[p_const];  // OK
}

// Test restrictions on null-terminated pointer types.
void check_nullterm_restrictions(void) {
  int* _Nt_array t1 = 0;              // integer types are OK.
  int* _Single *_Nt_array t2 = 0;         // pointer types are OK.
  enum E { Null, Blue, White };
  enum E* _Nt_array t3 = 0;           // enum types are OK

  void* _Nt_array t10 = 0;            // expected-error {{only integer and pointer types are allowed}}
  float* _Nt_array t11 = 0;           // expected-error {{only integer and pointer types are allowed}}
  double* _Nt_array t12 = 0;          // expected-error {{only integer and pointer types are allowed}}
  int* _Nt_array t13 checked[5] = 0;  // expected-error {{array initializer must be an initializer list}}
  struct S { int i; };
  struct S* _Nt_array  t14 = 0;        // expected-error {{only integer and pointer types are allowed}}
}


// Test assignments between different kinds of pointers, excluding
// void pointers and pointers with constant/volatile attributes.
extern void check_assign(int val, int *p, int* _Single q, int* _Array r,
                         float *s, float* _Single t, float* _Array u,
                         int* _Nt_array v : count(1), short* _Nt_array x) {
    int *t1 = p;              // T *  = T * OK
    int* _Single t2 = &val;       // ptr<T> = T * OK when T * has known bounds
    int* _Single t3 = q;          // ptr<T> = ptr<T> OK
    int* _Array t4 = &val; // array_ptr<T> = T * OK when T * has known bounds
    int* _Array t5 = r;    // array_ptr<T> = array_ptr<T> OK
    int* _Nt_array t5a = v;// nt_array_ptr<T> = nt_array_ptr<T> OK.
    int* _Nt_array t5b = &val; // expected-error {{incompatible type}}
                                  // nt_array_ptr<T> = T * not OK, even when T
                                  // has known bounds.
    int *t6 = q;              // expected-error {{incompatible type}}
                              // T * = ptr<T> not OK;
    int *t7 = r;              // expected-error {{incompatible type}}
                              // T * = array_ptr<T> not OK
    int *t7a = v;             // // expected-error {{incompatible type}}
                              // T * = nt_array_ptr<T> not OK
    int* _Single t8 = r;          // expected-error {{expression has unknown bounds}}
                              // ptr<T> = array_ptr<T> OK
    int* _Single t8a = v;         // ptr<T> = nt_array_ptr<T> OK.
    int* _Array t9 = q;    // array_ptr<T> = ptr<T> OK
    int* _Array t10a = v;  // array_ptr<T> = nt_array_ptr<T> OK.
    int* _Nt_array t10b = q; // expected-error {{incompatible type}}
                               // nt_array_ptr<T> = ptr<T> not OK.
    int* _Nt_array t10ca = r; // expected-error {{incompatible type}}
                                // nt_array_ptr<T> = array_ptr<T> not OK.

    // check assigning different kinds of pointers with different referent
    // types

    // right hand referent type is int and left hand referent type is float
    int *t11 = t;             // expected-error {{incompatible type}}
                              // T * = ptr<S> not OK
    int *t12 = u;             // expected-error {{incompatible type}}
                              // T * = array_ptr<S> not OK;
    int* _Single t13 = s;         // expected-error {{incompatible type}}
                              // ptr<T> = S * not OK
    int* _Single t14 = t;         // expected-error {{incompatible type}}
                              // ptr<T> = ptr<S> not OK
    int* _Single t15 = u;         // expected-error {{incompatible type}}
                              // ptr<T> = array_ptr<S> not OK
    int* _Array t16 = s;   // expected-error {{incompatible type}}
                              // array_ptr<T> = S * not OK;
    int* _Array t17 = t;   // expected-error {{incompatible type}}
                              // array_ptr<T> = ptr<S> not OK;
    int* _Array t18 = u;   // expected-error {{incompatible type}}
                              // array_ptr<T> = array_ptr<S> not OK
    int* _Nt_array t18a = s; // expected-error {{incompatible type}}
                                // nt_array_ptr<T> = S * not OK;
    int* _Nt_array t18b = t; // expected-error {{incompatible type}}
                                // nt_array_ptr<T> = ptr<S> not OK;
    int* _Nt_array t18c = u; // expected-error {{incompatible type}}
                                // nt_array_ptr<T> = array_ptr<S> not OK
    int* _Nt_array t18d = x; // expected-error {{incompatible type}}
                                // nt_array_ptr<T> = nt_array_ptr<S> not OK

    // left hand referent type is float and right hand  referent type is int
    float *t19 = q;           // expected-error {{incompatible type}}
                              // T * = ptr<S> not OK
    float *t20 = r;           // expected-error {{incompatible type}}
                              // T * = array_ptr<S> not OK;
    float* _Single t21 = p;       // expected-error {{incompatible type}}
                              // ptr<T> = S * not OK
    float* _Single t22 = q;       // expected-error {{incompatible type}}
                              // ptr<T> = ptr<S> not OK
    float* _Single t23 = r;       // expected-error {{incompatible type}}
                              // ptr<T> = array_ptr<S> not OK
    float* _Array t24 = p; // expected-error {{incompatible type}}
                              // array_ptr<T> = S * not OK;
    float* _Array t25 = q; // expected-error {{incompatible type}}
                              // array_ptr<T> = ptr<S> not OK;
    float* _Array t26 = r; // expected-error {{incompatible type}}
                              // array_ptr<T> = array_ptr<S> not OK

    // C compilers enforcing C99 conversion rules allow implicit 
    // integer to pointer conversions. These are errors for the new safe
    // pointer types.
    int* _Single t27 = val;       // expected-error {{incompatible type}}
                              // ptr<T> = int not OK
    float* _Single t28 = val;     // expected-error {{incompatible type}}
                              // ptr<T> = int not OK;
    int t29 = q;              // expected-error {{incompatible type}}
                              // int = ptr<T> not OK;
    int t30 = t;              // expected-error {{incompatible type}}
                              // int = ptr<T> not OK
    int* _Array t32 = val; // expected-error {{incompatible type}}
                              // array_ptr<T> = int not OK
    int* _Nt_array t32a = val; // expected-error {{incompatible type}}
                                  // nt_array_ptr<T> = int not OK
    int t33 = q;              // expected-error {{incompatible type}}
                              // int = int* _Array not OK.
    int t33a = v;              // expected-error {{incompatible type}}
                               // int = int* _Array not OK.

    // spot check converting a pointer to a floating point type
    float t31 = q;            // expected-error {{incompatible type}}
                              // float = ptr<T> not OK

    // Implicit conversion of a safe pointer type to _Bool is OK.
    _Bool t34 = q;
    _Bool t35 = r;
    _Bool t36 = t;
    _Bool t37 = u;
    _Bool t37a = v;

    // _Bool to safe pointer is not OK.
    int* _Single t38 = (_Bool)(1);   // expected-error {{incompatible type}}
    float* _Single t39 = (_Bool)(1); // expected-error {{incompatible type}}
    int* _Array t40 = (_Bool)(1);   // expected-error {{incompatible type}}
    float* _Array t41 = (_Bool)(1); // expected-error {{incompatible type}}
    int* _Nt_array t41a = (_Bool)(1); // expected-error {{incompatible type}}

    // Implicit conversion of 0 to a safe pointer type is OK.
    int* _Single t42 = 0;
    int* _Array t43 = 0;
    float* _Single t44 = 0;
    float* _Array t45 = 0;
    int* _Nt_array t46 = 0;

    // Check assignments of pointers to pointers.
    //
    // There are language extensions where only warnings are issued when pointer
    // referent types are mismatched or have different levels of indirection. Except
    // for conversions of unchecked pointer to T to checked pointers to T, these
    // should be errors for checked pointers.  Otherwise the integrity of bounds checking 
    // can be compromised accidentally.

    int **unchecked_ptr_to_unchecked_ptr = &p;
    int* _Single *unchecked_ptr_to_checked_ptr= &q;
    int **_Single checked_ptr_to_unchecked_ptr = &p;
    int *_Single *_Single checked_ptr_to_checked_ptr = &q;
    array_ptr<ptr<int>> array_ptr_to_checked_ptr = &q;
    array_ptr<int *> array_ptr_to_unchecked_ptr = &p;
    // First check pointers with different levels of indirection.
    int* _Single t50 = unchecked_ptr_to_checked_ptr;  // expected-error {{incompatible type}}
    int* _Single t51 = checked_ptr_to_checked_ptr;    // expected-error {{incompatible type}}
    int* _Single t52 = array_ptr_to_checked_ptr;      // expected-error {{incompatible type}}

    int* _Array t53 = unchecked_ptr_to_checked_ptr;  // expected-error {{incompatible type}}
    int* _Array t54 = checked_ptr_to_checked_ptr;    // expected-error {{incompatible type}}
    int* _Array t56 = array_ptr_to_checked_ptr;      // expected-error {{incompatible type}}

    unchecked_ptr_to_checked_ptr = q;      // expected-error {{incompatible type}}
    checked_ptr_to_checked_ptr = p;        // expected-error {{incompatible type}}
    checked_ptr_to_checked_ptr = q;        // expected-error {{incompatible type}}
    array_ptr_to_checked_ptr = p;          // expected-error {{incompatible type}}
    array_ptr_to_checked_ptr = q;          // expected-error {{incompatible type}}

    // Check assignments of pointers to pointers with the same level of indirection
    // but different checked qualities.

    unchecked_ptr_to_unchecked_ptr = unchecked_ptr_to_unchecked_ptr; // OK
    unchecked_ptr_to_unchecked_ptr = unchecked_ptr_to_checked_ptr;   // expected-error {{incompatible type}}
    unchecked_ptr_to_unchecked_ptr = checked_ptr_to_unchecked_ptr;   // expected-error {{incompatible type}}
    unchecked_ptr_to_unchecked_ptr = checked_ptr_to_checked_ptr;     // expected-error {{incompatible type}}

    unchecked_ptr_to_checked_ptr = unchecked_ptr_to_unchecked_ptr; // expected-error {{incompatible type}}
    unchecked_ptr_to_checked_ptr = unchecked_ptr_to_checked_ptr;   // OK
    unchecked_ptr_to_checked_ptr = checked_ptr_to_unchecked_ptr;   // expected-error {{incompatible type}}
    unchecked_ptr_to_checked_ptr = checked_ptr_to_checked_ptr;     // expected-error {{incompatible type}}

    checked_ptr_to_checked_ptr = unchecked_ptr_to_unchecked_ptr; // expected-error {{incompatible type}}
    // checked_ptr_to_checked_ptr = unchecked_ptr_to_checked_ptr;    not allowed: right-hand side has unknown bounds.
    checked_ptr_to_checked_ptr = checked_ptr_to_unchecked_ptr;   // expected-error {{incompatible type}}
    checked_ptr_to_checked_ptr = checked_ptr_to_checked_ptr;     // OK

    array_ptr_to_checked_ptr = unchecked_ptr_to_unchecked_ptr; // expected-error {{incompatible type}}
    // array_ptr_to_checked_ptr = unchecked_ptr_to_checked_ptr;  not allowed: right-hand side has unknown bounds.
    array_ptr_to_checked_ptr = checked_ptr_to_unchecked_ptr;   // expected-error {{incompatible type}}
    array_ptr_to_checked_ptr = checked_ptr_to_checked_ptr;     // OK, array_ptr<T> = ptr<T>

    array_ptr_to_unchecked_ptr = unchecked_ptr_to_unchecked_ptr; // OK
    array_ptr_to_unchecked_ptr = unchecked_ptr_to_checked_ptr;   // expected-error {{incompatible type}}
    array_ptr_to_unchecked_ptr = checked_ptr_to_unchecked_ptr;   // OK , array_ptr<T> = ptr<T>
    array_ptr_to_unchecked_ptr = checked_ptr_to_checked_ptr;     // expected-error {{incompatible type}}
}

// Test assignments between different kinds of pointers where the
// the source and/or destination pointers are pointers to void.

// Unchecked scope
extern void
check_assign_void_unchecked(int val, int *p, int* _Single q,
                            int* _Array r : count(1),
                            void *s, void* _Single t,
                            void* _Array u : byte_count(sizeof(int)),
                            int* _Nt_array v : count(1)) {

    // pointer to void = pointer to integer for the different kinds of pointers
    void *t1 = p;            // void *  = T * OK;
    void* _Single t2 = &val;     // void* _Single = T * OK when T * has known bounds
    void* _Single t3 = q;        // void* _Single = ptr<T> OK
    void* _Single t4 = &val;     // void* _Single = T * OK provided T * has known bounds.
    void* _Single t4a = r;       // void* _Single = array_ptr<T> OK.
    void* _Single t4b = v;       // void* _Single = nt_array_ptr<T> OK.
    void* _Array t5 = p;  // void* _Array = T * OK
    void* _Array t6 = r;  // void* _Array = array_ptr<T> OK
    void* _Array t6a = v; // void* _Array = nt_array_ptr<T> OK
    // nt_array_ptr<void> is not legal, so we can skip testing it as the
    // left-hand side of an assignment.
    nt_array_ptr<void> t6b = 0; // expected-error {{only integer and pointer types are allowed}}

    // pointer to void = pointer to void for the different kinds of pointers
    void *t7 = s;            // void * = void * OK.
    void *t8 = t;            // expected-error {{incompatible type}}
                             // void * = void* _Single not OK
    void *t9 = u;            // expected-error {{incompatible type}}
                             // void * = void* _Array not OK.
    void* _Single t10 = (void *) &val; // void* _Single = void * OK provided void * has known bounds
    void* _Single t11 = t;       // void* _Single = void* _Single OK
    void* _Single t12 = u;
                             // void* _Single = void* _Array OK.
    void* _Array t13 = u; // void* _Array = void * OK when array_ptr has no
                             // bounds
    void* _Array t14 = t; // void* _Array = void* _Single OK.
    void* _Array t15 = u; // void* _Array = void* _Array OK

    // pointer to integer = pointer to void for the different kinds of pointers.
    int *t17 = s;            // T * = void * OK
    int *t18 = t;            // expected-error {{incompatible type}}
                             // int * = void* _Single not OK.
    int *t19 = u;            // expected-error {{incompatible type}}
                             // int * = void* _Array not OK.
    int* _Single t20 = (void *) &val; // int* _Single = void * OK provided void * has known bounds
    int* _Single t21 = t;        // expected-error {{incompatible type}}
                             // int* _Single = void* _Single not OK.
    int* _Single t22 = u;
    int* _Array t23 = s;  // int* _Array = void * OK when int* _Array has no
                             // bounds.
    int* _Array t24 = t;  // expected-error {{incompatible type}}
                             // int* _Array = void* _Single not OK.
    int* _Array t25 = u;
    int* _Nt_array t25a = (void *)&val; // expected-error {{incompatible type}}
                                           // int* _Nt_array = void * not OK.
    int* _Nt_array t25b = s;  // expected-error {{incompatible type}}
                                 // int* _Nt_array = void * not OK, even with obunds
    int* _Nt_array t25c = t;  // expected-error {{incompatible type}}
                                 // int* _Nt_array = void* _Single not OK.
    int* _Nt_array t25d = u;  // expected-error {{incompatible type}}
                                 // int* _Nt_array = void* _Array not OK.
    // conversions between integers and safe void pointers.
    int t26 = t;             // expected-error {{incompatible type}}
                             // int = void* _Single not OK;
    int t27 = u;             // expected-error {{incompatible type}}
                             // int = void* _Array not OK;
    void* _Single t28 = val;     // expected-error {{incompatible type}}
                             // void* _Array = int not OK
    void* _Array t29 = val; // expected-error {{incompatible type}}
                               // void* _Array = int not OK

    // spot check converting a pointer to a float
    float t30 = t;           // expected-error {{incompatible type}}
                             // float = ptr<T> not OK
    float t31 = u;           // expected-error {{incompatible type}}
                             // float = array_ptr<T> not OK

    // Implicit conversion of a safe void pointer type to _Bool is OK.
    _Bool t32 = t;
    _Bool t33 = u;

    // _Bool to safe void pointer is not OK.
    void* _Single t34 = (_Bool)(1);   // expected-error {{incompatible type}}
    void* _Array t35 = (_Bool)(1);   // expected-error {{incompatible type}}

    // Implicit conversion of 0 to a safe void pointer type is OK.
    void* _Single t37 = 0;
    void* _Array t38 = 0;
}

// Test assignments between different kinds of pointers where the
// the source and/or destination pointers are pointers to void.

// Checked scope
struct CheckedData1 {
   int len;
   int* _Array p : count(len);
};

extern void
check_assign_void_checked(int val, int* _Single p,
                          int* _Array q : count(1),
                          void* _Array r : byte_count(sizeof(int)),
                          void* _Array r2 : byte_count(sizeof(struct CheckedData1)),
                          void* _Array r3 : byte_count(sizeof(int* _Single)),
                          int* _Nt_array s : count(1),
                          ptr<struct CheckedData1> t,
                          ptr<ptr<int>> u,
                          array_ptr<int* _Single checked[5]> v : count(1)) checked {
    // pointer to void = pointer to integer for the different kinds of pointers
    // allowed in checked scopes
    void* _Array t1 = p;  // void* _Array = int* _Single OK
    void* _Array t2 = q;  // void* _Array = int* _Array OK
    void* _Array t3 = s;  // void* _Array = int* _Nt_array OK
    void* _Array t4 = t;  // expected-error {{not allowed in a checked scope}}
                             // array_ptr<void = array_ptr<T> where T contains
                             // a checked pointer
    void* _Array t5 = u;  // expected-error {{not allowed in a checked scope}}
                             // array_ptr<void = array_ptr<T> where T contains
                             // a checked pointer
    void* _Array t6 = v;  // expected-error {{not allowed in a checked scope}}
                             // array_ptr<void = array_ptr<T> where T contains
                             // a checked pointer
    // nt_array_ptr<void> is not legal, so we can skip testing it as the
    // left-hand side of an assignment.
    nt_array_ptr<void> t7 = 0; // expected-error {{only integer and pointer types are allowed}}

    // pointer to void = pointer to void for the different kinds of pointers
    void* _Array t8 = r; // void* _Array = void * OK when array_ptr has no
                             // bounds
    void* _Array t9 = r; // void* _Array = void* _Array OK

    int* _Single t20 = r;
    int* _Array t21 = r;
    int* _Nt_array t22 = r;  // expected-error {{incompatible type}}
                                 // int* _Nt_array = void* _Array not OK.
    array_ptr<struct CheckedData1> t23 = r2;  // expected-error {{not allowed in a checked scope}}
                                          // array_ptr<T> = void* _Array where T contains
                                          // a checked pointer
    int* _Single *_Single t24 = r3; // expected-error {{not allowed in a checked scope}}
                            // array_ptr<T> = void* _Array where T contains
                            // a checked pointer
    int* _Single *_Single t25 checked[1]= r3;  // expected-error{{array initializer must be an initializer list}}
                                         // array_ptr<T> = void* _Array where T contains
                                         // a checked pointer
    // conversions between integers and safe void pointers.
    int t26 = r;             // expected-error {{incompatible type}}
                             // int = void* _Array not OK;
    void* _Array t27 = val; // expected-error {{incompatible type}}
                               // void* _Array = int not OK

    // spot check converting a pointer to a float
    float t31 = r;           // expected-error {{incompatible type}}
                             // float = array_ptr<T> not OK

    // Implicit conversion of a safe void pointer type to _Bool is OK.
    _Bool t32 = r;

    // _Bool to safe void pointer is not OK.
    void* _Array t33 = (_Bool)(1);   // expected-error {{incompatible type}}

    // Implicit conversion of 0 to a safe void pointer type is OK.
    void* _Array t34 = 0;
}

extern void
check_assign_void_checked_bounds_only(int val, int* _Single p,
                          int* _Array q : count(1),
                          void* _Array r : byte_count(sizeof(int)),
                          void* _Array r2 : byte_count(sizeof(struct CheckedData1)),
                          void* _Array r3 : byte_count(sizeof(int* _Single)),
                          int* _Nt_array s : count(1),
                          ptr<struct CheckedData1> t,
                          ptr<ptr<int>> u) checked _Bounds_only {
    // pointer to void = pointer to integer for the different kinds of pointers
    // allowed in checked scopes
    void* _Array t1 = p;  // void* _Array = int* _Single OK
    void* _Array t2 = q;  // void* _Array = int* _Array OK
    void* _Array t3 = s;  // void* _Array = int* _Nt_array OK
    void* _Array t4 = t;  // array_ptr<void = array_ptr<T> OK where T contains
                             // a checked pointer
    void* _Array t5 = u;  // array_ptr<void = array_ptr<T> OK where T contains
                             // a checked pointer
    // nt_array_ptr<void> is not legal, so we can skip testing it as the
    // left-hand side of an assignment.
    nt_array_ptr<void> t6 = 0; // expected-error {{only integer and pointer types are allowed}}

    // pointer to void = pointer to void for the different kinds of pointers
    void* _Array t7 = r; // void* _Array = void * OK when array_ptr has no
                             // bounds
    void* _Array t8 = r; // void* _Array = void* _Array OK
    int* _Single t20 = r;
    int* _Array t21 = r;
    int* _Nt_array t22 = r;  // expected-error {{incompatible type}}
                                 // int* _Nt_array = void* _Array not OK.
    struct CheckedData1 * _Array t23 = r2;  // array_ptr<T> = void* _Array OK where T contains
                                              // a checked pointer
    int *_Single *_Single t24 = r3; // array_ptr<T> = void* _Array OK where T contains
                            // a checked pointer
    // conversions between integers and safe void pointers.
    int t25 = r;             // expected-error {{incompatible type}}
                             // int = void* _Array not OK;
    void* _Array t26 = val; // expected-error {{incompatible type}}
                               // void* _Array = int not OK

    // spot check converting a pointer to a float
    float t31 = r;           // expected-error {{incompatible type}}
                             // float = array_ptr<T> not OK

    // Implicit conversion of a safe void pointer type to _Bool is OK.
    _Bool t32 = r;

    // _Bool to safe void pointer is not OK.
    void* _Array t33 = (_Bool)(1);   // expected-error {{incompatible type}}

    // Implicit conversion of 0 to a safe void pointer type is OK.
    void* _Array t34 = 0;
}


// Test assignments between pointers of different kinds with const/volatile
// attributes on referent types
extern void check_assign_cv(void) {
    int val = 0;
    const int const_val = 0;
    volatile int volatile_val = 0;
    int *p = 0;
    const int *p_const = 0;
    volatile int *p_volatile = 0;
    int* _Single q = 0;
    const int *_Single q_const = 0;
    volatile int * _Single q_volatile = 0;
    int* _Array r = 0;
    const int * _Array r_const = 0;
    volatile int *_Array r_volatile = 0;
    int* _Nt_array s = 0;
    const int *_Nt_array s_const = 0;
    volatile int *_Nt_array s_volatile = 0;

    p_const = p;    // unsafe pointer to const assigned unsafe pointer to non-const OK.
    q_const = q;    // ptr to const assigned ptr to non-const OK.
    r_const = r;    // array_ptr to const assigned array_ptr to non-const OK.
    q_const = &val; // ptr to const assigned unsafe pointer OK, provided unsafe pointer
                    // has known bounds.
    r_const = p_const; // array_ptr to const assigned unsafe pointer to const OK,
                       // provided array_ptr has unknown bounds.
    r_const = &val; // array_ptr to const assigned unsafe pointer OK, provided array_ptr
                    // has unknown bounds.
    s_const = s;    //  nt_array_ptr to const assigned non-const OK.

    p = p_const;    // expected-warning {{discards qualifiers}}
    q = q_const;    // expected-warning {{discards qualifiers}}
                    // ptr assigned to ptr to const int
    r = r_const;    // expected-warning {{discards qualifiers}}
    q = &const_val; // expected-warning {{discards qualifiers}}
    r = &const_val; // expected-warning {{discards qualifiers}}
    r = p_const;    // expected-warning {{discards qualifiers}}
    s = s_const;    // expected-warning {{discards qualifiers}}

    p_volatile = p; // unsafe pointer to volatile assigned unsafe pointer to non-volatile OK.
    q_volatile = q; // ptr to volatile assigned ptr to non-volatile OK.
    r_volatile = r; // array_ptr to volatile assigned array_ptr to non-volatile OK.
    s_volatile = s; // nt_array_ptr to volatile assigned array_ptr to non-volatile OK.

    q_volatile = &val; // ptr to volatile assigned unsafe pointer OK, provided unsafe pointer
                       // has known bounds.
    r_volatile = p_volatile; // array_ptr to volatile assigned unsafe pointer to volatile OK,
                             // provided array_ptr has unknown bounds.
    r_volatile = &val; // array_ptr to volatile assigned unsafe pointer OK, provided array_ptr
                       // has unknown bounds.
    p = p_volatile;    // expected-warning {{discards qualifiers}}
    q = q_volatile;    // expected-warning {{discards qualifiers}}
                       // ptr assigned to ptr to volatile int
    r = r_volatile;    // expected-warning {{discards qualifiers}}
    q = &volatile_val; // expected-warning {{discards qualifiers}}
    r = &volatile_val; // expected-warning {{discards qualifiers}}
    r = p_volatile;    // expected-warning {{discards qualifiers}}
    s = s_volatile;    // expected-warning {{discards qualifiers}}
}

// Test conditional expressions where arms have different kinds of
// pointer types, excluding pointers to void.
extern void check_condexpr(int val, int *p, int* _Single q, int* _Array r,
                           float *s, float* _Single t, float* _Array u,
                           int* _Nt_array v) {
   float fval;
   int *t1 = val ? p : p;            // T * and T * OK;
   int *t2 = val ? &val : q;         // expected-error {{initializing 'int *' with an expression of incompatible type '_Ptr<int>'}}
                                     // T * and ptr<T> produce ptr<T>
   int *t3 = val ? q : &val;         // expected-error {{initializing 'int *' with an expression of incompatible type '_Ptr<int>'}}
                                     // T * and ptr<T> produce ptr<T>
   int* _Single t4 = val ? &val : q;     // T * and ptr<T> OK when T has known bounds
   int* _Single t5 = val ? q : &val;     // ptr<T> and T * OK when T has known bounds
   int *t6 = val ? p : r;            // expected-error {{initializing 'int *' with an expression of incompatible type '_Array_ptr<int>'}}
                                     // T * and array_ptr<T> produce array_ptr<T>
   int *t7 = val ? r : p;            // expected-error {{initializing 'int *' with an expression of incompatible type '_Array_ptr<int>'}}
                                     // array_ptr<T> and T * produce array_ptr<T>
   int* _Array t8 = val ? p : r;  // T * and array_ptr<T> OK when array_ptr<T> has unknown bounds
   int* _Array t9 = val ? r : p;  // array_ptr<T> and T * OK when array_ptr<T> has unknown bounds
   int* _Single t10 = val ? q : q;       // ptr<T> and ptr<T> OK
   int* _Array t11 = val ? r : r;  // array_ptr<T> and array_ptr<T> OK
   int* _Array t12 = val ? v : v; // nt_array_ptr<T> and nt_array_ptr<T> OK

   // omit assignment because type of expression is not valid when there is an error.
   val ? &fval : q; // expected-error {{pointer type mismatch}}
                    // S * and ptr<T> not OK
   val ? q : &fval; // expected-error {{pointer type mismatch}}
                    // ptr<T> and S * not OK
   val ? &fval : r; // expected-error {{pointer type mismatch}}
                    // S * and array_ptr<T> not OK;
   val ? r : &fval; // expected-error {{pointer type mismatch}}
                    // array_ptr<T> and S * not OK;
   val ? &fval : v; // expected-error {{pointer type mismatch}}
                    // nt_array_ptr<T> and S * not OK;
   val ? v : &fval; // expected-error {{pointer type mismatch}}
                    // nt_array_ptr<T> and S * not OK;
   val ? q : t;     // expected-error {{pointer type mismatch}}
                    // ptr<T> and ptr<S> not OK
   val ? t : q;     // expected-error {{pointer type mismatch}}
                    // ptr<S> and ptr<T> not OK
   val ? q : r;     // expected-error {{pointer type mismatch}}
                    // ptr<T> and array_ptr<T> not OK
   val ? r : q;     // expected-error {{pointer type mismatch}}
                    // array_ptr<T> and ptr<T> not OK
   val ? q : v;     // expected-error {{pointer type mismatch}}
                    // ptr<T> and nt_array_ptr<T> not OK
   val ? v : q;     // expected-error {{pointer type mismatch}}
                    // nt_array_ptr<T> and ptr<T> not OK
   val ? u : q;     // expected-error {{pointer type mismatch}}
                    // array_ptr<T> and ptr<S> not OK;
   val ? q : u;     // expected-error {{pointer type mismatch}}
                    // ptr<T> and array_ptr<S> not OK;
   val ? r : u;     // expected-error {{pointer type mismatch}}
                    // array_ptr<T> and array_ptr<S> not OK
   val ? u : r;     // expected-error {{pointer type mismatch}}
                    // array_ptr<S> and array_ptr<T> not OK

   // Some C compilers have allowed implicit integer to pointer conversions.
   // These are errors for the new safe pointer types.
   val ? q : val;   // expected-error {{incompatible operand types}}
                    // ptr<T> and int not OK
   val ? val : q;   // expected-error {{incompatible operand types}}
                    // int and ptr<T> not OK
   val ? r : val;   // expected-error {{incompatible operand types}}
                    // array_ptr<T> and int not OK
   val ? val : r;   // expected-error {{incompatible operand types}}
                    // int and array_ptr<T> not OK
   val ? t : val;   // expected-error {{incompatible operand types}}
                    // ptr<T> and int not OK
   val ? val : t;   // expected-error {{incompatible operand types}}
                    // int and ptr<T> not OK
   val ? u : val;   // expected-error {{incompatible operand types}}
                    // array_ptr<T> and int not OK
   val ? val : u;   // expected-error {{incompatible operand types}}
                    // int and array_ptr<T> not OK
   val ? v : val;   // expected-error {{incompatible operand types}}
                    // nt_array_ptr<T> and int not OK
   val ? val : v;   // expected-error {{incompatible operand types}}
                    // int and nt_array_ptr<T> not OK

  // Implicit conversion of 0 to a safe pointer type is OK.
   int* _Single t30 = val ? q : 0;
   int* _Single t31 = val ? 0 : q;
   int* _Array t32 = val ? r : 0;
   int* _Array t33 = val ? 0 : r;
   int* _Nt_array t34 = val ? v : 0; // expected-warning {{cannot prove declared bounds for 't34' are valid after initialization}}
   int* _Nt_array t35 = val ? 0 : v; // expected-warning {{cannot prove declared bounds for 't35' are valid after initialization}}
   float* _Single t36 = val ? t : 0;
   float* _Single t37 = val ? 0 : t;
   float* _Array t38 = val ? u : 0;
   float* _Array t39 = val ? 0 : u;
}

// Test conditional expressions where arms have different kinds of
// pointer types and one or both of the types of the arms is a
// pointer to void.
extern void check_condexpr_void(int val, int *p, int* _Single q, int* _Array r,
                                void *s : byte_count(sizeof(struct CheckedData1)),
                                void* _Single t,
                                void* _Array u, int* _Nt_array v,
                                ptr<struct CheckedData1> w,
                                array_ptr<struct CheckedData1> x,
                                struct CheckedData1 *y) {

    // valid combinations of void pointers for the arms of the expressions.
    void *t1 = val ? s : s;           // void * and void * OK
    void* _Single t2 = val ? s : t;       // void * and void* _Single  OK
    void* _Single t2a = val ? t : s;      // void* _Single and void * OK
    void* _Single t3 = val ? t : t;       // void* _Single and void* _Single OK
    void* _Single t4 = val ? t : (void *) &val;  // void* _Single and void * OK
    void* _Array t5 = val ? u : s; // void* _Array and void * OK
    void* _Array t6 = val ? s : u; // void * and void* _Array OK
    void* _Array t7 = val ? u : u; // void* _Array and void* _Array OK

    // valid combinations of void pointer and int pointers for the arms of the expression
    void *t8 = val ? s : p;            // void * and int * OK
    void *t9 = val ? p : s;            // int * and void * OK
    void *t10 = val ? y : s;           // struct CheckedData1 * and void * OK, where CheckedData1
                                       // contains a checked pointer.
    void *t11 = val ? s : y;           // void * struct CheckedData1 * OK.
    void* _Single t14 = val ? t : &val;    // void* _Single and int * OK when int * has bounds of at least 1 byte
    void* _Single t15 = val ? &val : t;    // int * and void* _Single OK when int * has bounds of at least 1 byte
    void* _Array t17 = val ? u : p; // void* _Array and int * OK when array_ptr has unknown bounds
    void* _Array t18 = val ? p : u; // int * and void* _Array OK when array_ptr has unknown bounds
    void* _Array t19 = val ? u : y; // void* _Array and struct CheckedData1 * OK when
                                       // array_ptr has unknown bounds.
    void* _Array t20 = val ? y : u; // struct CheckedData1 * and void* _Array OK when
                                       // array_ptr has unknown bounds.
    void* _Single t21 = val ? t : q;       // void* _Single and int* _Single OK
    void* _Single t22 = val ? q : t;       // int* _Single and void* _Single OK
    void* _Single t23 = val ? w : t;       // ptr<struct CheckedData1> and void* _Single OK.
    void* _Single t24 = val ? t : w;       // void* _Single and ptr<struct CheckedData1> OK.
    void* _Array t25 = val ? u : r; // void* _Array and int* _Array OK
    void* _Array t26 = val ? r : u; // int* _Array and void* _Array OK
    void* _Array t27 = val ? u : x; // array_ptr<struct CheckedData1> and int* _Array OK
    void* _Array t28 = val ? x : u; // void* _Array and array_ptr<struct CheckedData1> OK

    // omit assignment because type of expression is not valid when there is an error

    // invalid combinations of void pointer types
    val ? t : u;   // expected-error {{pointer type mismatch}}
                   // void* _Single and void* _Array not OK.
    val ? u : t;   // expected-error {{pointer type mismatch}}
                   // void* _Array and void* _Single not OK.

    // Invalid combinations of pointer to int and void pointer types

    // According to the C11 standard, section 6.5.15 (conditional
    // operator), if one arm of a conditional expression is a pointer
    // to void and the other arm is another pointer type, the type of
    // the entire expression is pointer to void.  That implies that
    // the other arm is implicitly cast to the void pointer type.
    // Checked C only allows implicit conversions of safe types to
    // unsafe types at bounds-safe interfaces.  If one arm is void *
    // and the other is a safe pointer type, this is a typechecking error.
    //
    // Checked C also requires that for implicit conversions between
    // safe types and safe pointers to void, the pointer kind must
    // match.
    val ? (void *) &val : q;   // expected-error {{pointer type mismatch}}
                   // void * and int* _Single not OK.
    val ? q : (void *) &val;   // expected-error {{pointer type mismatch}}
                   // int* _Single and void * not OK.
    val ? s : r;   // expected-error {{pointer type mismatch}}
                   // void * and int* _Array not OK
    val ? r : s;   // expected-error {{pointer type mismatch}}
                   // int* _Array and void * not OK.
    val ? s : v;   // expected-error {{pointer type mismatch}}
                   // void * and int* _Nt_array not OK
    val ? v : s;   // expected-error {{pointer type mismatch}}
                   // int* _Nt_array and void * not OK.
    val ? t : r;   // expected-error {{pointer type mismatch}}
                   // void* _Single and int* _Array not OK
    val ? r : t;   // expected-error {{pointer type mismatch}}
                   // int* _Array and void* _Single not OK
    val ? t : v;   // expected-error {{pointer type mismatch}}
                   // void* _Single and int* _Nt_array not OK
    val ? v : t;   // expected-error {{pointer type mismatch}}
                   // int* _Nt_array and void* _Single not OK
    val ? u : q;   // expected-error {{pointer type mismatch}}
                   // void* _Array and int* _Single not OK
    val ? q : u;   // expected-error {{pointer type mismatch}}
                   // int* _Single and void* _Array not OK

     // Spot check invalid combinations of pointer to struct and
     // void pointer data types
     val ? s : w;   // expected-error {{pointer type mismatch}}
                    // void * and ptr<struct CheckedData1> not OK.
     val ? x : s;   // expected-error {{pointer type mismatch}}
                    // array_ptr<struct CheckedData1> and void * not OK.

    // Some C compilers have allowed implicit integer to pointer conversions.
    // These are errors for new safe pointer types to void
    val ? t : val;   // expected-error {{incompatible operand types}}
                     // void* _Single and int not OK
    val ? val : t;   // expected-error {{incompatible operand types}}
                     // int and void* _Single not OK
    val ? u : val;   // expected-error {{incompatible operand types}}
                     // void* _Array and int not OK
    val ? val : u;   // expected-error {{incompatible operand types}}
                     // int  and void* _Array not OK

    // Implicit conversion of 0 to a void safe pointer type is OK.
    void* _Single t40 = val ? t : 0;
    void* _Single t41 = val ? 0 : t;
    void* _Array t42 = val ? u : 0;
    void* _Array t43 = val ? 0 : u;
}

// Test conditional expressions where arms have different kinds of
// pointer types and one or both of the types of the arms is a
// pointer to void, a checked bounds_only scope.
checked bounds_only extern void
check_condexpr_void_bounds_only(int val, int* _Single q, int* _Array r,
                                void* _Single t,
                                void* _Array u, int* _Nt_array v,
                                ptr<struct CheckedData1> w,
                                array_ptr<struct CheckedData1> x) {

    // valid combinations of void pointers for the arms of the expressions.
    void* _Single t3 = val ? t : t;       // void* _Single and void* _Single OK
    void* _Array t7 = val ? u : u; // void* _Array and void* _Array OK

    // valid combinations of void pointer and int pointers for the arms of the expression
    void* _Single t21 = val ? t : q;       // void* _Single and int* _Single OK
    void* _Single t22 = val ? q : t;       // int* _Single and void* _Single OK
    void* _Single t23 = val ? w : t;       // ptr<struct CheckedData1> and void* _Single OK.
    void* _Single t24 = val ? t : w;       // void* _Single and ptr<struct CheckedData1> OK.
    void* _Array t25 = val ? u : r; // void* _Array and int* _Array OK
    void* _Array t26 = val ? r : u; // int* _Array and void* _Array OK
    void* _Array t27 = val ? u : x; // array_ptr<struct CheckedData1> and int* _Array OK
    void* _Array t28 = val ? x : u; // void* _Array and array_ptr<struct CheckedData1> OK

    // omit assignment because type of expression is not valid when there is an error

    // invalid combinations of void pointer types
    val ? t : u;   // expected-error {{pointer type mismatch}}
                   // void* _Single and void* _Array not OK.
    val ? u : t;   // expected-error {{pointer type mismatch}}
                   // void* _Array and void* _Single not OK.

    // Invalid combinations of pointer to int and void pointer types

    // According to the C11 standard, section 6.5.15 (conditional
    // operator), if one arm of a conditional expression is a pointer
    // to void and the other arm is another pointer type, the type of
    // the entire expression is pointer to void.  That implies that
    // the other arm is implicitly cast to the void pointer type.
    // Checked C only allows implicit conversions of safe types to
    // unsafe types at bounds-safe interfaces.  If one arm is void *
    // and the other is a safe pointer type, this is a typechecking error.
    //
    // Checked C also requires that for implicit conversions between
    // safe types and safe pointers to void, the pointer kind must
    // match.
    val ? t : r;   // expected-error {{pointer type mismatch}}
                   // void* _Single and int* _Array not OK
    val ? r : t;   // expected-error {{pointer type mismatch}}
                   // int* _Array and void* _Single not OK
    val ? t : v;   // expected-error {{pointer type mismatch}}
                   // void* _Single and int* _Nt_array not OK
    val ? v : t;   // expected-error {{pointer type mismatch}}
                   // int* _Nt_array and void* _Single not OK
    val ? u : q;   // expected-error {{pointer type mismatch}}
                   // void* _Array and int* _Single not OK
    val ? q : u;   // expected-error {{pointer type mismatch}}
                   // int* _Single and void* _Array not OK

    // Some C compilers have allowed implicit integer to pointer conversions.
    // These are errors for new safe pointer types to void
    val ? t : val;   // expected-error {{incompatible operand types}}
                     // void* _Single and int not OK
    val ? val : t;   // expected-error {{incompatible operand types}}
                     // int and void* _Single not OK
    val ? u : val;   // expected-error {{incompatible operand types}}
                     // void* _Array and int not OK
    val ? val : u;   // expected-error {{incompatible operand types}}
                     // int  and void* _Array not OK

    // Implicit conversion of 0 to a void safe pointer type is OK.
    void* _Single t40 = val ? t : 0;
    void* _Single t41 = val ? 0 : t;
    void* _Array t42 = val ? u : 0;
    void* _Array t43 = val ? 0 : u;
}

// Test conditional expressions where arms have different kinds of
// pointer types and one or both of the types of the arms is a
// pointer to void, in a checked scope.
checked extern void
check_condexpr_void_checked(int val, int* _Single q, int* _Array r,
                            void* _Single t,
                            void* _Array u, int* _Nt_array v,
                            ptr<struct CheckedData1> w,
                            array_ptr<struct CheckedData1> x) {

    // valid combinations of void pointers for the arms of the expressions.
    void* _Single t3 = val ? t : t;       // void* _Single and void* _Single OK
    void* _Array t7 = val ? u : u; // void* _Array and void* _Array OK

    // combinations of void pointer and int pointers for the arms of the expression
    // that are valid in some kinds of checked scopes.
    void* _Single t21 = val ? t : q;       // void* _Single and int* _Single OK
    void* _Single t22 = val ? q : t;       // int* _Single and void* _Single OK
    void* _Single t23 = val ? w : t;       // expected-error {{not allowed in a checked scope}}
                                       // ptr<struct CheckedData1> and void* _Single not OK
                                       // because CheckedData1 has a checked pointer in it.
    void* _Single t24 = val ? t : w;       // expected-error {{not allowed in a checked scope}}
                                       // void* _Single and ptr<struct CheckedData1> not OK
                                       // because CheckedData1 has a checked pointer in it.
    void* _Array t25 = val ? u : r; // void* _Array and int* _Array OK
    void* _Array t26 = val ? r : u; // int* _Array and void* _Array OK
    void* _Array t27 = val ? u : x; // expected-error {{not allowed in a checked scope}}
                                       // array_ptr<struct CheckedData1> and void* _Array not OK
                                       // because CheckedData1 has a checked pointer in it.
    void* _Array t28 = val ? x : u; // expected-error {{not allowed in a checked scope}}
                                       // void* _Array and array_ptr<struct CheckedData1> not OK

    // omit assignment because type of expression is not valid when there is an error

    // invalid combinations of void pointer types
    val ? t : u;   // expected-error {{pointer type mismatch}}
                   // void* _Single and void* _Array not OK.
    val ? u : t;   // expected-error {{pointer type mismatch}}
                   // void* _Array and void* _Single not OK.

    // Invalid combinations of pointer to int and void pointer types

    // According to the C11 standard, section 6.5.15 (conditional
    // operator), if one arm of a conditional expression is a pointer
    // to void and the other arm is another pointer type, the type of
    // the entire expression is pointer to void.  That implies that
    // the other arm is implicitly cast to the void pointer type.
    // Checked C only allows implicit conversions of safe types to
    // unsafe types at bounds-safe interfaces.  If one arm is void *
    // and the other is a safe pointer type, this is a typechecking error.
    //
    // Checked C also requires that for implicit conversions between
    // safe types and safe pointers to void, the pointer kind must
    // match.
    val ? t : r;   // expected-error {{pointer type mismatch}}
                   // void* _Single and int* _Array not OK
    val ? r : t;   // expected-error {{pointer type mismatch}}
                   // int* _Array and void* _Single not OK
    val ? t : v;   // expected-error {{pointer type mismatch}}
                   // void* _Single and int* _Nt_array not OK
    val ? v : t;   // expected-error {{pointer type mismatch}}
                   // int* _Nt_array and void* _Single not OK
    val ? u : q;   // expected-error {{pointer type mismatch}}
                   // void* _Array and int* _Single not OK
    val ? q : u;   // expected-error {{pointer type mismatch}}
                   // int* _Single and void* _Array not OK

    // Some C compilers have allowed implicit integer to pointer conversions.
    // These are errors for new safe pointer types to void
    val ? t : val;   // expected-error {{incompatible operand types}}
                     // void* _Single and int not OK
    val ? val : t;   // expected-error {{incompatible operand types}}
                     // int and void* _Single not OK
    val ? u : val;   // expected-error {{incompatible operand types}}
                     // void* _Array and int not OK
    val ? val : u;   // expected-error {{incompatible operand types}}
                     // int  and void* _Array not OK

    // Implicit conversion of 0 to a void safe pointer type is OK.
    void* _Single t40 = val ? t : 0;
    void* _Single t41 = val ? 0 : t;
    void* _Array t42 = val ? u : 0;
    void* _Array t43 = val ? 0 : u;
}

// Test conditional expressions where arms have different kinds of
// pointer types and const/volatile modifiers.
extern void check_condexpr_cv(void)
{
  int val = 0;
  const int const_val = 0;
  volatile int volatile_val = 0;
  int *p = 0;
  const int *p_const = 0;
  volatile int *p_volatile = 0;
  int* _Single q = 0;
  const int *_Single q_const = 0;
  volatile int * _Single q_volatile = 0;
  int* _Array r = 0;
  const int * _Array r_const = 0;
  volatile int *_Array r_volatile = 0;
  int* _Nt_array s = 0;
  const int *_Nt_array s_const = 0;
  volatile int *_Nt_array s_volatile = 0;

  // test different kinds of pointers with const modifiers
  const int *t1 = val ? p : p_const;       // int * and const int * OK
  const int *t2 = val ? p_const : p;       // const int * and int * OK
  const int *t3 = val ? p_const : p_const; // const int * and const int * OK

  int *t4 = val ? p : p_const;             // expected-warning {{discards qualifiers}}
                                           // int * and const int * produce const int *
  int *t5 = val ? p_const : p;             // expected-warning {{discards qualifiers}}
                                           // const int * and int * produce const int *
  int *t6 = val ? p_const : p_const;       // expected-warning {{discards qualifiers}}
                                           // const int * and const int * produce const int *

  const int *_Single t7 = val ? &val : q_const;   // int * and const int *_Single OK
  const int *_Single t8 = val ? q_const : &val;   // const int *_Single and int * OK
  const int *_Single t9 = val ? &const_val : q;   // const int * and int* _Single OK
  const int *_Single t10 = val ? q : &const_val;  // int* _Single and const int * OK
  const int *_Single t11 = val ? &const_val : q_const;  // const int * and const int *_Single OK
  const int *_Single t12 = val ? q_const : &const_val;  // const int *_Single and const int * OK
  const int *_Single t13 = val ? q : q_const;        // int* _Single and const int *_Single OK
  const int *_Single t14 = val ? q_const : q;  // const int *_Single and int* _Single OK
  const int *_Single t15 = val ? q_const : q_const;  // const int *_Single and const int *_Single OK

  int* _Single t16 = val ? &val : q_const;     // expected-warning {{discards qualifiers}}
                                           // int * and const int *_Single produce const int *_Single
  int* _Single t17 = val ? q_const : &val;     // expected-warning {{discards qualifiers}}
                                           // const int *_Single and int * produce const int *_Single
  int* _Single t18 = val ? &const_val : q;     // expected-warning {{discards qualifiers}}
                                           // const int * and int* _Single produce const int *_Single
  int* _Single t19 = val ? q : &const_val;     // expected-warning {{discards qualifiers}}
                                           // int* _Single and const int * produce const int *_Single
  int* _Single t20 = val ? &const_val : q_const;   // expected-warning {{discards qualifiers}}
                                           // const int * and const int *_Single produce const int *_Single
  int* _Single t21 = val ? q_const : &const_val;   // expected-warning {{discards qualifiers}}
                                           // const int *_Single and const int * produce const int *_Single
  int* _Single t22 = val ? q : q_const;        // expected-warning {{discards qualifiers}}
                                           // int* _Single and const int *_Single produce const int *_Single
  int* _Single t23 = val ? q_const : q;        // expected-warning {{discards qualifiers}}
                                           // const int *_Single and int* _Single produce const int *_Single
  int* _Single t24 = val ? q_const : q_const;        // expected-warning {{discards qualifiers}}
                                           // const int *_Single and const int *_Single produce const int *_Single

  const int * _Array t25 = val ? p : r_const;       // int * and const int * _Array OK
  const int * _Array t26 = val ? r_const : p;       // const int * _Array and int * OK
  const int * _Array t27 = val ? p_const : r;       // const int * and int* _Array OK
  const int * _Array t28 = val ? r : p_const;       // int* _Array and const int * OK
  const int * _Array t29 = val ? p_const : r_const; // const int * and const int * _Array OK
  const int * _Array t30 = val ? r_const : p_const; // const int * _Array and const int * OK
  const int * _Array t31 = val ? r : r_const;       // int* _Array and const int * _Array OK
  const int * _Array t32 = val ? r_const : r;       // const int * _Array and int* _Array OK
  const int * _Array t33 = val ? r_const : r_const; // const int * _Array and const int * _Array OK
                                                          // int* _Nt_array and const int *_Nt_array OK
  const int *_Nt_array t31a = val ? s : s_const;       // expected-error {{inferred bounds for 't31a' are unknown after initialization}}
                                                          // const int *_Nt_array and int* _Nt_array OK
  const int *_Nt_array t32a = val ? s_const : s;       // expected-error {{inferred bounds for 't32a' are unknown after initialization}}
  const int *_Nt_array t33a = val ? s_const : s_const; // const int *_Nt_array and const int *_Nt_array OK

  int* _Array t34 = val ? p : r_const;   // expected-warning {{discards qualifiers}}
                                            // int * and const int * _Array produce const int * _Array
  int* _Array t35 = val ? r_const : p;   // expected-warning {{discards qualifiers}}
                                            // const int * _Array and int * produce const int * _Array
  int* _Array t36 = val ? p_const : r;   // expected-warning {{discards qualifiers}}
                                            // const int * and int* _Array produce const int * _Array
  int* _Array t37 = val ? r : p_const;   // expected-warning {{discards qualifiers}}
                                            // int* _Array and const int * produce const int * _Array
  int* _Array t38 = val ? p_const : r_const;   // expected-warning {{discards qualifiers}}
                                            // const int * and const int * _Array produce const int * _Array
  int* _Array t39 = val ? r_const : p_const;   // expected-warning {{discards qualifiers}}
                                            // const int * _Array and const int * produce const int * _Array
  int* _Array t40 = val ? r : r_const;   // expected-warning {{discards qualifiers}}
                                            // int* _Array and const int * _Array produce const int * _Array
  int* _Array t41 = val ? r_const : r;   // expected-warning {{discards qualifiers}}
                                            // const int * _Array and int* _Array produce const int * _Array
  int* _Array t42 = val ? r_const : r_const;   // expected-warning {{discards qualifiers}}
                                            // const int * _Array and const int * _Array produce const int * _Array
  int* _Array t40a = val ? s : s_const;  // expected-warning {{discards qualifiers}}
                                            // int* _Nt_array and const int *_Nt_array produce const int *_Nt_array
  int* _Array t41a = val ? s_const : s;  // expected-warning {{discards qualifiers}}
                                            // const int *_Nt_array and int* _Nt_array produce const int * _Array
  int* _Array t42a = val ? r_const : r_const;   // expected-warning {{discards qualifiers}}
                                                   // const int *_Nt_array and const int *_Nt_array produce const int * _Array

  // test different kinds of pointers with volatile modifers
  volatile int *t50 = val ? p : p_volatile;          // int * and volatile int * OK
  volatile int *t51 = val ? p_volatile : p;          // volatile int * and int * OK
  volatile int *t52 = val ? p_volatile : p_volatile; // volatile int * and volatile int * OK

  int *t53 = val ? p : p_volatile;                   // expected-warning {{discards qualifiers}}
                                                     // int * and volatile int * produce volatile int *
  int *t54 = val ? p_volatile : p;                   // expected-warning {{discards qualifiers}}
                                                     // volatile int * and int * produce volatile int *
  int *t55 = val ? p_volatile : p_volatile;          // expected-warning {{discards qualifiers}}
                                                     // volatile int * and volatile int * produce volatile int *

  volatile int * _Single t56 = val ? &val : q_volatile;       // int * and volatile int * _Single OK
  volatile int * _Single t57 = val ? q_volatile : &val;       // volatile int * _Single and int * OK
  volatile int * _Single t58 = val ? &volatile_val : q;       // volatile int * and int* _Single OK
  volatile int * _Single t59 = val ? q : &volatile_val;       // int* _Single and volatile int * OK
  volatile int * _Single t60 = val ? &volatile_val : q_volatile; // volatile int * and volatile int * _Single OK
  volatile int * _Single t61 = val ? q_volatile : &volatile_val; // volatile int * _Single and volatile int * OK
  volatile int * _Single t62 = val ? q : q_volatile;          // int* _Single and volatile int * _Single OK
  volatile int * _Single t63 = val ? q_volatile : q;          // volatile int * _Single and int* _Single OK
  volatile int * _Single t64 = val ? q_volatile : q_volatile; // volatile int * _Single and volatile int * _Single OK

  int* _Single t65 = val ? &val : q_volatile;          // expected-warning {{discards qualifiers}}
                                                   // int * and volatile int * _Single produce volatile int * _Single
  int* _Single t66 = val ? q_volatile : &volatile_val; // expected-warning {{discards qualifiers}}
                                                   // volatile int * _Single and int * produce volatile int * _Single
  int* _Single t67 = val ? &volatile_val : q;          // expected-warning {{discards qualifiers}}
                                                   // volatile int * and int* _Single produce volatile int * _Single
  int* _Single t68 = val ? q : &volatile_val;          // expected-warning {{discards qualifiers}}
                                                   // int* _Single and volatile int * produce volatile int * _Single
  int* _Single t69 = val ? &volatile_val : q_volatile; // expected-warning {{discards qualifiers}}
                                                   // volatile int * and volatile int * _Single produce volatile int * _Single
  int* _Single t70 = val ? q_volatile : &volatile_val; // expected-warning {{discards qualifiers}}
                                                   // volatile int * _Single and volatile int * produce volatile int * _Single
  int* _Single t71 = val ? q : q_volatile;             // expected-warning {{discards qualifiers}}
                                                   // int* _Single and volatile int * _Single produce volatile int * _Single
  int* _Single t72 = val ? q_volatile : q;             // expected-warning {{discards qualifiers}}
                                                   // volatile int * _Single and int* _Single produce volatile int * _Single
  int* _Single t73 = val ? q_volatile : q_volatile;    // expected-warning {{discards qualifiers}}
                                                   // volatile int * _Single and volatile int * _Single produce volatile int * _Single

  volatile int *_Array t74 = val ? p : r_volatile;          // int * and volatile int *_Array OK
  volatile int *_Array t75 = val ? r_volatile : p;          // volatile int *_Array and int * OK
  volatile int *_Array t76 = val ? p_volatile : r;          // volatile int * and int* _Array OK
  volatile int *_Array t77 = val ? r : p_volatile;          // int* _Array and volatile int * OK
  volatile int *_Array t78 = val ? p_volatile : r_volatile; // volatile int * and volatile int *_Array OK
  volatile int *_Array t79 = val ? r_volatile : p_volatile; // volatile int *_Array and volatile int * OK
  volatile int *_Array t80 = val ? r : r_volatile;          // int* _Array and volatile int *_Array OK
  volatile int *_Array t81 = val ? r_volatile : r;          // volatile int *_Array and int* _Array OK
  volatile int *_Array t82 = val ? r_volatile : r_volatile; // volatile int *_Array and volatile int *_Array OK
                                                                   // int* _Nt_array and volatile int *_Nt_array OK
  volatile int *_Nt_array t80a = val ? s : s_volatile;          // expected-error {{inferred bounds for 't80a' are unknown after initialization}}
                                                                   // volatile int *_Nt_array and int* _Nt_array OK
  volatile int *_Nt_array t81a = val ? s_volatile : s;          // expected-error {{inferred bounds for 't81a' are unknown after initialization}}
  volatile int *_Nt_array t82b = val ? s_volatile : s_volatile; // volatile int *_Nt_array and volatile int *_Nt_array OK

  int* _Array t83 = val ? p : r_volatile;          // expected-warning {{discards qualifiers}}
                                                      // int * and volatile int *_Array produce volatile int *_Array
  int* _Array t84 = val ? r_volatile : p;          // expected-warning {{discards qualifiers}}
                                                      // volatile int *_Array and int * produce volatile int *_Array
  int* _Array t85 = val ? p_volatile : r;          // expected-warning {{discards qualifiers}}
                                                      // volatile int * and int* _Array produce volatile int *_Array
  int* _Array t86 = val ? r : p_volatile;          // expected-warning {{discards qualifiers}}
                                                      // int* _Array and volatile int * produce volatile int *_Array
  int* _Array t87 = val ? p_volatile : r_volatile; // expected-warning {{discards qualifiers}}
                                                      // volatile int * and volatile int *_Array produce volatile int *_Array
  int* _Array t88 = val ? r_volatile : p_volatile; // expected-warning {{discards qualifiers}}
                                                      // volatile int *_Array and volatile int * produce volatile int *_Array
  int* _Array t89 = val ? r : r_volatile;          // expected-warning {{discards qualifiers}}
                                                      // int* _Array and volatile int *_Array produce volatile int *_Array
  int* _Array t90 = val ? r_volatile : r;          // expected-warning {{discards qualifiers}}
                                                      // volatile int *_Array and int* _Array produce volatile int *_Array
  int* _Array t92 = val ? r_volatile : r_volatile;  // expected-warning {{discards qualifiers}}
                                                       // volatile int *_Array and volatile int *_Array produce volatile int *_Array
  int* _Nt_array t89a = val ? s : s_volatile;      // expected-error {{inferred bounds for 't89a' are unknown after initialization}} \
                                                      // expected-warning {{discards qualifiers}}
                                                      // int* _Nt_array and volatile int *_Nt_array produce volatile int *_Nt_array
  int* _Nt_array t90a = val ? s_volatile : s;      // expected-error {{inferred bounds for 't90a' are unknown after initialization}} \
                                                      // expected-warning {{discards qualifiers}}
                                                      // volatile int *_Nt_array and int* _Nt_array produce volatile int *_Nt_array
  int* _Nt_array t92a = val ? s_volatile : s_volatile;  // expected-warning {{discards qualifiers}}
                                                       // volatile int *_Nt_array and volatile int *_Nt_array produce volatile int *_Nt_array
}

// Define functions used to test typechecking of call expressions.

extern void f1(int *p, int y) {
    *p = y;
}

extern void f2(int* _Single p, int y) {
    *p = y;
}

extern void f3(int* _Array p, int y) {
    // can't dereference p because is has unknown bounds
    // just use p in a compare.
     p != 0;
}

extern void f3a(int* _Nt_array p, int y) {
  p != 0;
}

extern void f4(_Bool p, int y) {
}

extern void f5(ptr<struct CheckedData1> p, int y) {
}

extern void f6(array_ptr<struct CheckedData1> p : count(1), int y) {
}

extern void f1_void(void *p, int y) {
}

extern void f2_void(void* _Single p, int y) {
}

extern void f3_void(void* _Array p, int y) {
}

extern void f1_const(const int *p, int y) {
}

extern void f2_const(const int *_Single p, int y) {
}

extern void f3_const(const int * _Array p, int y) {
}

extern void f3a_const(const int * _Array p, int y) {
}
// Second parameter is a new pointer type
//

extern void g1(int y, int *p) {
    *p = y;
}

extern void g2(int y, int* _Single p) {
    *p = y;
}

extern void g3(int y, int* _Array p) {
    // can't dereference p because is has unknown bounds
    // just use p in a compare.
    p != 0;
}

extern void g3a(int y, int* _Nt_array p) {
  p != 0;
}

extern void g4(int y, _Bool p) {
}

//
// returns a new pointer type
//

extern int *h1(void) : itype(int* _Single) {
    return 0;
}

extern int* _Single h2(void) {
   return 0;
}

extern int* _Array h3(void) : count(1) {
   return 0;
}

extern int* _Nt_array h3a(void) : count(1) {
  return 0;
}

extern void check_call(void) {
    int val = 0;
    float fval = 0.0;
    int *p = 0;
    int* _Single q = 0;
    int* _Array r : count(1) = 0;


    float *s = 0;
    float* _Single t = 0;
    float* _Array u = 0;

    int* _Nt_array v : count(1) = 0;
    nt_array_ptr<ptr<int>> w = 0;

    // Test different kinds of pointers where the referent type matches.
    // Type of first parameter is a pointer type.
    f1(q, 0);      // expected-error {{incompatible type}}
                   // param int *, arg int* _Single not OK
    f1(r, 0);      // expected-error {{incompatible type}}
                   // param int *, arg int* _Array not OK.
    f1(v, 0);      // expected-error {{incompatible type}}
                   // param int *, arg int* _Nt_array not OK.
    f2(q, val);    // param int* _Single, arg int* _Single OK.
    f2(&val, 0);   // param int* _Single, arg int * OK, provided that arg has known bounds.
    f2(r, 0);      // param int* _Single, arg int* _Array OK, provided that arg has known bounds.
    f2(v, 0);      // param int* _Single, arg int* _Nt_array OK, provided that arg has known bounds.
    f3(r, val);    // param int* _Array, arg int* _Array OK.
    f3(p, 0);      // param int* _Array, arg int * OK, provided that param has unknown bounds.
    f3(q, 0);      // param int* _Array, arg int* _Single OK
    f3(&val, 0);   // param int* _Array, arg int * OK, when param has unknown bounds and arg has known bounds
    f3(v, 0);      // param int* _Array, arg int* _Nt_array OK, when param has unknown bounds and arg has known bounds

    f3a(p, val);   // expected-error {{incompatible type}}
                   // param int* _Nt_array, arg int * not OK
    f3a(q, val);   // expected-error {{incompatible type}}
                   // param int* _Nt_array, arg int* _Single not OK
    f3a(r, val);   // expected-error {{incompatible type}}
                   // param int* _Nt_array, arg int* _Array not OK
    f3a(v, 0);     // param int* _Nt_array, arg int* _Nt_array OK

    // Test different kinds of pointers where the referent type differs.  These are all
    // expected to fail to typecheck.
    f1(t, val);    // expected-error {{incompatible type}}
                   // param int *, arg float* _Single not OK.
    f1(u, val);    // expected-error {{incompatible type}}
                   // param int *, arg float* _Array not OK.
    f2(t, val);    // expected-error {{incompatible type}}
                   // param int* _Single, arg float* _Single not OK.
    f2(u, val);    // expected-error {{incompatible type}}
                   // param int* _Single, arg float* _Array not OK.
    f2(&fval, 0);  // expected-error {{incompatible type}}
                   // param int* _Single, arg float * not OK.
    f3(u, val);    // expected-error {{incompatible type}}
                   // param int* _Array, arg float* _Array not OK.
    f3(s, 0);      // expected-error {{incompatible type}}
                   // param int* _Array, arg float * not OK.
    f3(&fval, 0);  // expected-error {{incompatible type}}

    // Second parameter is a new pointer type.  Spot check a few cases to
    // make sure the parameter order doesn't matter.
    g2(val, q);    // param int* _Single, arg int* _Single OK.
    g2(0, &val);   // param int* _Single, arg int * OK, provided that arg has known bounds.
    g3(val, r);    // param int* _Array, arg int* _Array OK.
    g3(0, p);      // param int* _Array, arg int * OK, provided that param has unknown bounds.
    g3(0, &val);   // param int* _Array, arg int * OK, when param has unknown bounds and arg has known bounds
    g3a(0, p);     // expected-error {{incompatible type}}
                   // param int* _Array, arg int * not OK.
    g3a(0, &val);  // expected-error {{incompatible type}}
                   // param int* _Array, arg int * not OK.
    g1(val, t);    // expected-error {{incompatible type}}
                   // param int *, arg float* _Single not OK.
    g1(val, u);    // expected-error {{incompatible type}}
                   // param int *, arg float* _Array not OK.
    g2(val, t);    // expected-error {{incompatible type}}
                   // param int* _Single, arg float* _Single not OK.

    // Null pointer OK
    f1(0, val);
    f2(0, val);
    f3(0, val);
    f3a(0, val);
    g1(val, 0);
    g2(val, 0);
    g3(val, 0);
    g3a(val, 0);

    // Other integers not OK for safe pointers.
    f2(val, val);  // expected-error {{incompatible type}}
    f3(val, val);  // expected-error {{incompatible type}}
    f3a(val, val);  // expected-error {{incompatible type}}
    g2(val, val);  // expected-error {{incompatible type}}
    g3(val, val);  // expected-error {{incompatible type}}
    g3a(val, val);  // expected-error {{incompatible type}}

    // Pointers OK for _Bool arguments
    f4(p, val);
    f4(q, val);
    f4(v, val);
    g4(val, p);
    g4(val, q);
    g4(val, v);

    //
    // Check return values
    //
    int *t1 = h1();
    int* _Single t2 = h1();
    int* _Array t3 = h1();  // OK, provided that t3 has unknown bounds.
    int* _Single t4 = h2();
    int* _Array t5 = h3();
    int* _Nt_array t5a = h3a();

    // Conversion of a pointer to a Boolean.
    _Bool t6 = h1();
    _Bool t7 = h2();
    _Bool t8 = h3();
    _Bool t8a = h3a();

    // expected to fail to typecheck
    int *t9 = h2();            // expected-error {{incompatible type}}
                               // int * = int* _Single not OK
    int *t10 = h3();           // expected-error {{incompatible type}}
                               // int * = int* _Array not OK
    int *t10a = h3a();         // expected-error {{incompatible type}}
                               // int * = int* _Nt_array not OK
    int* _Single t11 = h3();       // int* _Single = int* _Array OK
    int* _Single t11a = h3a();     // int* _Single = int* _Array OK
    int* _Array t12 = h2(); // int* _Array = int* _Single OK
    int* _Nt_array t12a = h2(); // expected-error {{incompatible type}}
                                   // int* _Array = int* _Single OK

    int t13 = h2();            // expected-error {{incompatible type}}
                               // int = int* _Single
    int t14 = h3();            // expected-error {{incompatible type}}
                               // int = int* _Array
    int t14a = h3a();          // expected-error {{incompatible type}}
                               // int = int* _Array
}

extern void check_call_void(void) {
    int val = 0;
    float fval = 0.0;
    int *p = 0;
    int* _Single q = 0;
    int* _Array r : count(1) = 0;

    // TODO: s will need bounds information
    void *s = 0;
    void* _Single t = 0;
    void* _Array u : byte_count(sizeof(int)) = 0;
    int* _Nt_array v : count(1) = 0;

    // Test different kinds of pointers where the parameter type is a pointer to void and
    // the referent type is not a void pointer.

    // Type of first parameter is a pointer type.
    // Expected to typecheck
    f2_void(q, val);    // param void* _Single, arg int* _Single OK.
    f2_void(&val, val); // param void* _Single, arg int * OK, provided that arg has known bounds.
    f3_void(r, val);    // param void* _Array, arg int* _Array OK.
    f3_void(p, val);    // param void* _Array, arg int * OK, provided that param has unknown bounds.
    f3_void(&val, val); // param void* _Array, arg int * OK, when param has unknown bounds and arg has known bounds

    // Expected to not typecheck
    f1_void(q, val);   // expected-error {{incompatible type}}
                        // param void *, arg int* _Single not OK
    f1_void(r, val);    // expected-error {{incompatible type}}
                        // param void *, arg int* _Array not OK
    f1_void(v, val);    // expected-error {{incompatible type}}
                        // param void *, arg int* _Nt_array not OK
    f2_void(r, val);    // param void* _Single, arg int* _Array OK
    f3_void(q, val);    // param void* _Array, arg int* _Single OK
    f2_void(v, val);    // param void* _Single, arg int* _Nt_array OK
    f3_void(v, val);    // param void* _Array, arg int* _Nt_array OK
  

    // Test different kinds of pointers where the parameter type is a pointer to void and the
    // referent type is a pointer to void
    f1_void(s, val);  // param void *, arg void * OK
    f2_void((void *) &val, val);  // param void* _Single, arg void * OK
    f2_void(t, val);  // param void* _Single, arg ptr<void OK
    f3_void(s, val);  // param void* _Array, arg void * OK.
    f3_void(u, val);  // param void* _Array arg void* _Array OK.

    f1_void(t, val);  // expected-error {{incompatible type}}
    f1_void(u, val);  // expected-error {{incompatible type}}
    f2_void(u, val);  // param void* _Single, arg void* _Array, OK
    f3_void(t, val);  // param void* _Array, arg void* _Single, OK

    // Test parameter types that are pointer to integers and argument types that are pointers to void
    f1(s, val);       // param int *, arg void * OK
    f1(t, val);       // expected-error {{incompatible type}}
                      // param int *, arg void* _Single
    f1(u, val);       // expected-error {{incompatible type}}
                      // param int *, arg void* _Array
    f2((void *) &val, val);       // param int* _Single, arg void * OK
    f2(t, val);       // expected-error {{incompatible type}}
                      // param int* _Single, arg void* _Single
    f2(u, val);       // param int* _Single, arg void* _Array OK
    f3(s, val);       // param int* _Array, arg void * OK
    f3(t, val);       // expected-error {{incompatible type}}
                      // param int* _Array, arg void* _Single
    f3(u, val);       // param int* _Array, arg void* _Array OK.
    f3a(s, val);      // expected-error {{incompatible type}}
                      // param int* _Nt_array, arg void * not OK
    f3a(t, val);      // expected-error {{incompatible type}}
                      // param int* _Nt_array, arg void* _Single
    f3a(u, val);      // expected-error {{incompatible type}}
                      // param int* _Nt_array, arg void* _Array not OK.

   // Test parameters that are integers and argument types that are safe pointers to void
    f1(0, t);         // expected-error {{incompatible type}}
    f1(0, u);         // expected-error {{incompatible type}}
    f1(0, v);         // expected-error {{incompatible type}}

   // Test parameters that are safe pointers to void and argument types that are integers
    f2_void(5, val);  // expected-error {{incompatible type}}
    f3_void(5, val);  // expected-error {{incompatible type}}

    // Pass safe pointers to void to _Bool parameters
    f4(s, val);       // OK
    f4(t, val);       // OK
    f4(u, val);       // OK

    // Pass _Bool to safe pointers to void
    f2_void((_Bool) 1, val);   // expected-error {{incompatible type}}
    f3_void((_Bool) 1, val);   // expected-error {{incompatible type}}

    // Null pointers passed to safe pointers to void .
    f2_void(0, val);
    f3_void(0, val);
}

extern void check_call_void_checked(void) checked {
    int val = 0;
    float fval = 0.0;
    int* _Single q = 0;
    int* _Array r : count(1) = 0;
    ptr<struct CheckedData1> s = 0;
    void* _Single t = 0;
    void* _Array u : byte_count(sizeof(struct CheckedData1)) = 0;
    int* _Nt_array v : count(1) = 0;

    // Test different kinds of pointers where the parameter type is a pointer to void and
    // the referent type is not a void pointer.

    // Type of first parameter is a pointer type.
    // Expected to typecheck

    f2_void(q, val);    // param void* _Single, arg int* _Single OK.
    f3_void(r, val);    // param void* _Array, arg int* _Array OK.
    f2_void(r, val);    // param void* _Single, arg int* _Array OK
    f3_void(q, val);    // param void* _Array, arg int* _Single OK
    f2_void(v, val);    // param void* _Single, arg int* _Nt_array OK
    f3_void(v, val);    // param void* _Array, arg int* _Nt_array OK

    f3_void(s, val);    // expected-error {{not allowed in a checked scope}}
                        // param void* _Array, arg pointer to struct containing
                        // a checked pointer not OK.

    // Test different kinds of pointers where the parameter type is a pointer to void and the
    // referent type is a pointer to void
    f2_void(t, val);  // param void* _Single, arg ptr<void OK
    f3_void(u, val);  // param void* _Array arg void* _Array OK.

    f2_void(u, val);  // param void* _Single, arg void* _Array, OK
    f3_void(t, val);  // param void* _Array, arg void* _Single, OK

    // Test parameter types that are pointer to integers and argument types that are pointers to void
    f2(t, val);       // expected-error {{incompatible type}}
                      // param int* _Single, arg void* _Single
    f2(u, val);       // param int* _Single, arg void* _Array OK
    f3(t, val);       // expected-error {{incompatible type}}
                      // param int* _Array, arg void* _Single
    f3(u, val);       // param int* _Array, arg void* _Array OK.
    f3a(t, val);      // expected-error {{incompatible type}}
                      // param int* _Nt_array, arg void* _Single
    f3a(u, val);      // expected-error {{incompatible type}}
                      // param int* _Nt_array, arg void* _Array not OK.
    f5(u, val);       // expected-error {{not allowed in a checked scope}}
                      // param ptr<T> where T contains a checked pointer, arg
                      // void* _Array not OK.
    f6(u, val);       // expected-error {{not allowed in a checked scope}}
                      // param array_ptr<T> where T contains a checked pointer, arg
                      // void* _Array not OK.

   // Test parameters that are integers and argument types that are safe pointers to void
    f1(0, t);         // expected-error {{incompatible type}}
    f1(0, u);         // expected-error {{incompatible type}}
    f1(0, v);         // expected-error {{incompatible type}}

   // Test parameters that are safe pointers to void and argument types that are integers
    f2_void(5, val);  // expected-error {{incompatible type}}
    f3_void(5, val);  // expected-error {{incompatible type}}

    // Pass safe pointers to void to _Bool parameters
    f4(t, val);       // OK
    f4(u, val);       // OK

    // Pass _Bool to safe pointers to void
    f2_void((_Bool) 1, val);   // expected-error {{incompatible type}}
    f3_void((_Bool) 1, val);   // expected-error {{incompatible type}}

    // Null pointers passed to safe pointers to void .
    f2_void(0, val);
    f3_void(0, val);
}


extern void check_call_void_checked_bounds_only(void) checked bounds_only {
    int val = 0;
    float fval = 0.0;
    int* _Single q = 0;
    int* _Array r : count(1) = 0;
    ptr<struct CheckedData1> s = 0;
    void* _Single t = 0;
    void* _Array u : byte_count(sizeof(struct CheckedData1)) = 0;
    int* _Nt_array v : count(1) = 0;

    // Test different kinds of pointers where the parameter type is a pointer to void and
    // the referent type is not a void pointer.

    // Type of first parameter is a pointer type.
    // Expected to typecheck

    f2_void(q, val);    // param void* _Single, arg int* _Single OK.
    f3_void(r, val);    // param void* _Array, arg int* _Array OK.
    f2_void(r, val);    // param void* _Single, arg int* _Array OK
    f3_void(q, val);    // param void* _Array, arg int* _Single OK
    f2_void(v, val);    // param void* _Single, arg int* _Nt_array OK
    f3_void(v, val);    // param void* _Array, arg int* _Nt_array OK
    f3_void(s, val);    // param void* _Array, arg pointer to struct containing
                        // a checked pointer OK.

    // Test different kinds of pointers where the parameter type is a pointer to void and the
    // referent type is a pointer to void
    f2_void(t, val);  // param void* _Single, arg ptr<void OK
    f3_void(u, val);  // param void* _Array arg void* _Array OK.

    f2_void(u, val);  // param void* _Single, arg void* _Array, OK
    f3_void(t, val);  // param void* _Array, arg void* _Single, OK

    // Test parameter types that are pointer to integers and argument types that are pointers to void
    f2(t, val);       // expected-error {{incompatible type}}
                      // param int* _Single, arg void* _Single
    f2(u, val);       // param int* _Single, arg void* _Array OK
    f3(t, val);       // expected-error {{incompatible type}}
                      // param int* _Array, arg void* _Single
    f3(u, val);       // param int* _Array, arg void* _Array OK.
    f3a(t, val);      // expected-error {{incompatible type}}
                      // param int* _Nt_array, arg void* _Single
    f3a(u, val);      // expected-error {{incompatible type}}
                      // param int* _Nt_array, arg void* _Array not OK.
    f5(u, val);       // param ptr<T> where T contains a checked pointer, arg
                      // void* _Array OK.
    f6(u, val);       // param array_ptr<T> where T contains a checked pointer, arg
                      // void* _Array OK.

   // Test parameters that are integers and argument types that are safe pointers to void
    f1(0, t);         // expected-error {{incompatible type}}
    f1(0, u);         // expected-error {{incompatible type}}
    f1(0, v);         // expected-error {{incompatible type}}

   // Test parameters that are safe pointers to void and argument types that are integers
    f2_void(5, val);  // expected-error {{incompatible type}}
    f3_void(5, val);  // expected-error {{incompatible type}}

    // Pass safe pointers to void to _Bool parameters
    f4(t, val);       // OK
    f4(u, val);       // OK

    // Pass _Bool to safe pointers to void
    f2_void((_Bool) 1, val);   // expected-error {{incompatible type}}
    f3_void((_Bool) 1, val);   // expected-error {{incompatible type}}

    // Null pointers passed to safe pointers to void .
    f2_void(0, val);
    f3_void(0, val);
}

void check_call_cv(void) {
    int val = 0;
    const int const_val = 0;
    int *p = 0;
    const int *p_const = 0;
    int* _Single q = 0;
    const int *_Single q_const = 0;
    int* _Array r = 0;
    const int * _Array r_const = 0;
    int* _Nt_array s = 0;
    const int *_Nt_array s_const = 0;

    // Parameters that are pointers to constants being passed pointers to non-const & const values.
    f1_const(p, val);           // param const int *, arg int * OK
    f1_const(p_const, val);     // param const int *, arg const int * OK
    f2_const(&val, val);        // param const int *_Single, arg int * OK, provided int * has bounds.
    f2_const(&const_val, val);  // param const int *_Single, arg const int * OK, provided int * has bounds
    f2_const(q, val);           // param const int *_Single, arg int* _Single OK
    f2_const(q_const, val);     // param const int *_Single arg const int *_Single OK
    f3_const(&val, val);        // param const int * _Array, arg int * OK, provided int * has bounds.
    f3_const(&const_val, val);  // param const int * _Array, arg const int * OK, provided int * has bounds
    f3_const(r, val);           // param const int * _Array, arg int* _Array OK
    f3_const(r_const, val);     // param const int * _Array arg const int * _Array OK
    f3a_const(s, val);           // param const int * _Array, arg int* _Array OK
    f3a_const(s_const, val);     // param const int * _Array arg const int * _Array OK

    // Parameters that are not pointers to constant, arguments that are pointers to constants.
    f1(p_const, val);     // expected-warning {{discards qualifiers}}
                          // param int *, arg const int * not OK
    f2(&const_val, val);  // expected-warning {{discards qualifiers}}
                          // param int* _Single, arg const int * not OK.
    f2(q_const, val);     // expected-warning {{discards qualifiers}}
                          // param int* _Single arg const int *_Single not OK
    f3(&const_val, val);  // expected-warning {{discards qualifiers}}
                          // param int* _Array, arg const int * not OK
    f3(r_const, val);     // expected-warning {{discards qualifiers}}
                          // param int* _Array arg const int * _Array not OK
    f3a(s_const, val);    // expected-warning {{discards qualifiers}}
                          // param int* _Nt_array arg const int *_Nt_array not OK
}

//
// Check pointer return types with different types of values.
//

// Check 'int *' return types.
int *check_return1(int *p) {
  return p;
}

int *check_return2(float *p) {
  return p;  // expected-warning {{incompatible pointer types}}
}

int *check_return3(void) {
  return 5; // expected-warning {{incompatible integer to pointer conversion}}
}

int *check_return4(int* _Single p) {
  return p; // expected-error {{incompatible result type}}
}

int *check_return5(int* _Array p : count(1)) {
  return p; // expected-error {{incompatible result type}}
}

int *check_return6(int* _Nt_array p : count(1)) {
  return p; // expected-error {{incompatible result type}}
}

// Check int* _Single return types.

int* _Single check_return11(int *p) {
  return p; // expected-error {{expression has unknown bounds}}
}

int* _Single check_return12(float *p) {
  return p;  // expected-error {{incompatible result type}}
}

int* _Single check_return13(void) {
  return 5; // expected-error {{incompatible result type}}
}

int* _Single check_return14(int* _Single p) {
  return p;
}

int* _Single checked_return15(float* _Single p) {
  return p;  // expected-error {{incompatible result type}}
}

int* _Single check_return16(int* _Array p : count(1)) {
  return p;
}

int* _Single check_return17(int* _Nt_array p : count(1)) {
  return p;
}

// Check int* _Array return types.

int* _Array check_return21(int *p) {
  return p;
}

int* _Array check_return21a(int *p) : count(1) {
  return p; // expected-error {{return value has unknown bounds, bounds expected because the function 'check_return21a' has bounds}}
}

int* _Array check_return22(float *p) {
  return p;  // expected-error {{incompatible result type}}
}

int* _Array check_return23(void) {
  return 5; // expected-error {{incompatible result type}}
}

int* _Array check_return24(int* _Single p) {
  return p;
}

int* _Array checked_return25(float* _Single p) {
  return p;  // expected-error {{incompatible result type}}
}

int* _Array check_return26(int* _Array p : count(1)) {
  return p;
}

int* _Array check_return27(int* _Nt_array p : count(1)) {
  return p;
}


//
// Check pointer return types with void pointer return values
//

// Unchecked scopes

int *check_voidptr_val_return1(void *p) {
  return p;
}

int *check_voidptr_val_return2(void* _Array p : byte_count(sizeof(int))) {
  return p; // expected-error {{incompatible result type}}
}

int* _Single check_voidptr_val_return11(void *p) {
  return p;  // expected-error {{expression has unknown bounds}}
}

int* _Single check_voidptr_val_return12(void* _Array p : byte_count(sizeof(int))) {
  return p;
}

int* _Array check_voidptr_val_return21(void *p) {
  return p;
}

int* _Array check_voidptr_val_return22(void* _Array p : byte_count(sizeof(int)))   {
  return p;
}

array_ptr<struct CheckedData1> check_voidptr_val_return23(void* _Array p : byte_count(sizeof(int)))   {
  return p;
}

// Checked scopes

checked int* _Single check_voidptr_val_return31(void* _Array p : byte_count(sizeof(int))) {
  return p;
}

checked int* _Array check_voidptr_val_return32(void* _Array p : byte_count(sizeof(int)))   {
  return p;
}

checked array_ptr<struct CheckedData1> check_voidptr_val_return23_checked(void* _Array p : byte_count(sizeof(int)))   {
  return p; // expected-error {{not allowed in a checked scope}}
}

// Checked bounds_only scopes

checked bounds_only
int* _Single check_voidptr_val_return41(void* _Array p : byte_count(sizeof(int))) {
  return p;
}

checked bounds_only
int* _Array check_voidptr_val_return42(void* _Array p : byte_count(sizeof(int)))   {
  return p;
}

checked bounds_only
array_ptr<struct CheckedData1> check_voidptr_val_return43(void* _Array p : byte_count(sizeof(int)))   {
  return p;
}

//
// Check void pointer return types with different types of values
// in unchecked scopes
//

// Check 'void *' return types.
void *check_voidptr_return1(int *p) {
  return p;
}

void *check_voidptr_return2(float *p) {
  return p;
}

void *check_voidptr_return3(void) {
  return 5; // expected-warning {{incompatible integer to pointer conversion}}
}

void *check_voidptr_return4(int* _Single p) {
  return p; // expected-error {{incompatible result type}}
}

void *check_voidptr_return5(int* _Array p : count(1)) {
  return p; // expected-error {{incompatible result type}}
}

void *check_voidptr_return6(int* _Nt_array p : count(1)) {
  return p; // expected-error {{incompatible result type}}
}

void *check_voidptr_return7(ptr<struct CheckedData1> p) {
  return p; // expected-error {{incompatible result type}}
}


// Check void* _Array return types.

void* _Array check_voidptr_return21(int *p) {
  return p;
}

void* _Array check_voidptr_return21a(int *p) : byte_count(sizeof(int)) {
  return p; // expected-error {{return value has unknown bounds, bounds expected because the function 'check_voidptr_return21a' has bounds}}
}

void* _Array check_voidptr_return21b(void *p) {
  return p;
}

void* _Array check_voidptr_return21c(void *p) : byte_count(sizeof(int)) {
  return p; // expected-error {{return value has unknown bounds, bounds expected because the function 'check_voidptr_return21c' has bounds}}
}


void* _Array check_voidptr_return22(float *p) {
  return p;
}

void* _Array check_voidptr_return23(void) {
  return 5; // expected-error {{incompatible result type}}
}

void* _Array check_voidptr_return24(int* _Single p) {
  return p;
}

void* _Array checked_voidptr_return25(float* _Single p) {
  return p;
}

void* _Array check_voidptr_return26(int* _Array p : count(1)) {
  return p;
}

void* _Array check_voidptr_return27(ptr<struct CheckedData1> p) {
  return p;
}


void* _Array check_voidptr_return28(int* _Nt_array p : count(1)) {
  return p;
}

void* _Array check_voidptr_return29(ptr<struct CheckedData1> p) {
  return p;
}

//
// Check void pointer return types with different types of values
// in checked scopes
//

checked void *check_voidptr_return_checked21(int* _Single p) {  // expected-error {{return in a checked scope must have a checked type}}
  return p; // expected-error {{incompatible result type}}
}

checked void* _Array check_voidptr_return31(int* _Single p) {
  return p;
}

checked void* _Array check_voidptr_return31a(int* _Array p) : byte_count(sizeof(int)) {
  return p; // expected-error {{return value has unknown bounds, bounds expected because the function 'check_voidptr_return31a' has bounds}}
}

checked void* _Array check_voidptr_return31b(void* _Array p) {
  return p;
}

checked void* _Array check_voidptr_return32(void) {
  return 5; // expected-error {{incompatible result type}}
}

checked void* _Array check_voidptr_return33(int* _Single p) {
  return p;
}

checked void* _Array checked_voidptr_return34(float* _Single p) {
  return p;
}

checked void* _Array check_voidptr_return35(int* _Array p : count(1)) {
  return p;
}

_Checked void* _Array check_voidptr_return36(ptr<struct CheckedData1> p) {
  return p;  // expected-error {{not allowed in a checked scope}}
}

checked void* _Array check_voidptr_return39(int* _Nt_array p : count(1)) {
  return p;
}

checked bounds_only void *check_voidptr_return41(int* _Single p) {  // expected-error {{return in a checked scope must have a checked type}}
  return p; // expected-error {{incompatible result type}}
}

checked bounds_only
void* _Array check_voidptr_return42(int* _Single p) {
  return p;
}

checked bounds_only
void* _Array check_voidptr_return42a(int* _Array p) : byte_count(sizeof(int)) {
  return p; // expected-error {{return value has unknown bounds, bounds expected because the function 'check_voidptr_return42a' has bounds}}
}

checked bounds_only
void* _Array check_voidptr_return42b(void* _Array p) {
  return p;
}

checked bounds_only
void* _Array check_voidptr_return43(void) {
  return 5; // expected-error {{incompatible result type}}
}

checked bounds_only
void* _Array check_voidptr_return44(int* _Single p) {
  return p;
}

checked bounds_only
void* _Array checked_voidptr_return45(float* _Single p) {
  return p;
}

checked bounds_only
void* _Array check_voidptr_return46(int* _Array p : count(1)) {
  return p;
}

checked bounds_only
void* _Array check_voidptr_return47(ptr<struct CheckedData1> p) {
  return p;
}

checked bounds_only
void* _Array check_voidptr_return48(int* _Nt_array p : count(1)) {
  return p;
}



void check_pointer_arithmetic(void)
{
   int val[5];
   int *p = val;
   int* _Single q = &val[0];
   void* _Single q_void = &val[0];
   int* _Array r = val;
   void* _Array r_void = val;
   int* _Nt_array s = 0;
   // nt_array_ptr<void> is not allowed, so we don't have to test it.
   int *p_tmp;
   int* _Array r_tmp;
   // By default, s_tmp has bounds of count(0).  We don't want to test checking
   // of bounds declarations at assignments to s_tmp below, so set the bounds to
   // unknown.
   int* _Nt_array s_tmp : bounds(unknown) = 0;

   p_tmp = p + 5;
   p_tmp = 5 + p;
   p_tmp = p_tmp - 2;
   p_tmp = 2 - p_tmp;  // expected-error {{invalid operands to binary expression}}
   p_tmp = p++;
   p_tmp = p--;
   p_tmp = ++p;
   p_tmp = --p;
   p_tmp = (p += 1);
   p_tmp = (p -= 1);

   // 0 interpreted as an integer, not null
   p_tmp = p + 0;
   p_tmp = 0 + p;  
   p_tmp = p - 0;
   p_tmp = 0 - p;  // expected-error {{invalid operands to binary expression}}

   q + 5;  // expected-error {{arithmetic on _Ptr type}}
   5 + q;  // expected-error {{arithmetic on _Ptr type}}
   q++;    // expected-error {{arithmetic on _Ptr type}}
   q--;    // expected-error {{arithmetic on _Ptr type}}
   ++q;    // expected-error {{arithmetic on _Ptr type}}
   --q;    // expected-error {{arithmetic on _Ptr type}}
   q += 1; // expected-error {{arithmetic on _Ptr type}}
   q -= 1; // expected-error {{arithmetic on _Ptr type}}

   q_void + 5;  // expected-error {{arithmetic on _Ptr type}}
   5 + q_void;  // expected-error {{arithmetic on _Ptr type}}
   q_void++;    // expected-error {{arithmetic on _Ptr type}}
   q_void--;    // expected-error {{arithmetic on _Ptr type}}
   ++q_void;    // expected-error {{arithmetic on _Ptr type}}
   --q_void;    // expected-error {{arithmetic on _Ptr type}}
   q_void += 1; // expected-error {{arithmetic on _Ptr type}}
   q_void -= 1; // expected-error {{arithmetic on _Ptr type}}

   r_tmp = r + 5;
   r_tmp = 5 + r;
   r_tmp = r_tmp - 2; 
   r_tmp = 2 - r_tmp; // expected-error {{invalid operands to binary expression}}
   r_tmp = r++;
   r_tmp = r--;
   r_tmp = ++r;
   r_tmp = --r;
   r_tmp = (r += 1);
   r_tmp = (r -= 1);
   // 0 interpreted as an integer, not null
   r_tmp = r + 0;
   r_tmp = 0 + r;
   r_tmp = r - 0;
   r_tmp = 0 - r; // expected-error {{invalid operands to binary expression}}

   s_tmp = s + 5;
   s_tmp = 5 + s;
   s_tmp = s_tmp - 2;
   s_tmp = 2 - s_tmp; // expected-error {{invalid operands to binary expression}}
   s_tmp = s++; // expected-warning {{cannot prove declared bounds for 's' are valid after increment}}
   s_tmp = s--; // expected-warning {{cannot prove declared bounds for 's' are valid after decrement}}
   s_tmp = ++s; // expected-warning {{cannot prove declared bounds for 's' are valid after increment}}
   s_tmp = --s; // expected-warning {{cannot prove declared bounds for 's' are valid after decrement}}
   s_tmp = (s += 1); // expected-warning {{cannot prove declared bounds for 's' are valid after assignment}}
   s_tmp = (s -= 1); // expected-warning {{cannot prove declared bounds for 's' are valid after assignment}}
   // 0 interpreted as an integer, not null
   s_tmp = s + 0;
   s_tmp = 0 + s;
   s_tmp = s - 0;
   s_tmp = 0 - s; // expected-error {{invalid operands to binary expression}}

   // GCC allows arithmetic on void pointers, not allowed for safe pointer types

   r_void + 5;  // expected-error {{arithmetic on a pointer to void}}
   5 + r_void;  // expected-error {{arithmetic on a pointer to void}}
   r_void++;    // expected-error {{arithmetic on a pointer to void}}
   r_void--;    // expected-error {{arithmetic on a pointer to void}}
   ++r_void;    // expected-error {{arithmetic on a pointer to void}} 
   --r_void;    // expected-error {{arithmetic on a pointer to void}}
   r_void += 1; // expected-error {{arithmetic on a pointer to void}}
   r_void -= 1; // expected-error {{arithmetic on a pointer to void}}

   // adding two pointers is not allowed
   q + q; // expected-error {{invalid operands}}
   r + r; // expected-error {{invalid operands}}
   s + s; // expected-error {{invalid operands}}
}

void check_pointer_difference(void)
{
    int count;
    int val_int[5];
    float val_float[5];

    float *p_float = val_float;
    int *p_int = val_int;
    void *p_void = val_int;

    float* _Single q_float = &val_float[0];
    int* _Single q_int = &val_int[0];
    void* _Single q_void = &val_int[0];

    float* _Array r_float = val_float;
    int* _Array r_int = val_int;
    void* _Array r_void = val_int;

    // nt_array_ptr<float> and nt_array_ptr<void> are not allowed.
    int* _Nt_array s_int = 0;
    nt_array_ptr<char> s_char = 0;

    // check pointer difference using different kinds of pointers to float
    count = p_float - p_float; // float * - float * OK
    count = p_float - q_float; // float * - float* _Single OK
    count = p_float - r_float; // float * - float* _Array OK

    count = q_float - p_float; // float* _Single - float * OK
    count = q_float - q_float; // float* _Single - float* _Single OK
    count = q_float - r_float; // float* _Single - float* _Array OK.

    count = r_float - p_float; // float* _Array - float * OK.
    count = r_float - q_float; // float* _Array - float* _Single OK.

   // check pointer difference using different kinds of pointers to int
    count = p_int - p_int; // int * - int * OK
    count = p_int - q_int; // int * - int* _Single OK
    count = p_int - r_int; // int * - int* _Array OK
    count = p_int - s_int; // int * - int* _Nt_array OK 

    count = q_int - p_int; // int* _Single - int * OK
    count = q_int - q_int; // int* _Single - int* _Single OK
    count = q_int - r_int; // int* _Single - int* _Array OK.
    count = q_int - s_int; // int* _Single - int* _Nt_array OK

    count = r_int - p_int; // int* _Array - int * OK.
    count = r_int - q_int; // int* _Array - int* _Single OK.
    count = r_int - r_int; // int* _Array - int* _Array OK.
    count = r_int - s_int; // int* _Array - int* _Nt_array OK.

    count = s_int - p_int; // int* _Nt_array - int * OK.
    count = s_int - q_int; // int* _Nt_array - int* _Single OK.
    count = s_int - r_int; // int* _Nt_array - int* _Array OK.
    count = s_int - s_int; // int* _Nt_array - int* _Nt_array OK.

    // invalid pointer differences
    // differences involving safe pointers to different referent types

    // pointers to int and pointers to float (using pointer to char for nt_array_char)
    count = p_int - p_float; // expected-error {{not pointers to compatible types}}
                             // int * - float * not OK
    count = p_int - q_float; // expected-error {{not pointers to compatible types}}
                             // int * - float* _Single not OK
    count = p_int - r_float; // expected-error {{not pointers to compatible types}}
                             // int * - float* _Array not OK
    count = p_int - s_char;  // expected-error {{not pointers to compatible types}}
                             // int * - nt_array_ptr<char> not OK

    count = q_int - p_float; // expected-error {{not pointers to compatible types}}
                             // int* _Single - float * not OK
    count = q_int - q_float; // expected-error {{not pointers to compatible types}}
                             // int* _Single - float* _Single not OK
    count = q_int - r_float; // expected-error {{not pointers to compatible types}}
                             // int* _Single - float* _Array not OK.
    count = q_int - s_char;  // expected-error {{not pointers to compatible types}}
                             // int* _Single - nt_array_ptr<char> not OK.

    count = r_int - p_float; // expected-error {{not pointers to compatible types}}
                             // int* _Array - float * not OK.
    count = r_int - q_float; // expected-error {{not pointers to compatible types}}
                             // int* _Array - float* _Single not OK.
    count = r_int - r_float; // expected-error {{not pointers to compatible types}}
                             // int* _Array - float* _Array not OK.
    count = r_int - s_char; // expected-error {{not pointers to compatible types}}
                            // int* _Array - nt_array_ptr<char> not OK.
   
    // pointers to int and nt_array_ptr<char>

    count = s_int - p_float; // expected-error {{not pointers to compatible types}}
                             // int* _Nt_array - float * not OK.
    count = s_int - q_float; // expected-error {{not pointers to compatible types}}
                             // int* _Nt_array - float* _Single not OK.
    count = s_int - r_float; // expected-error {{not pointers to compatible types}}
                             // int* _Nt_array - float* _Array not OK.
    count = s_int - s_char; // expected-error {{not pointers to compatible types}}
                             // int* _Nt_array - nt_array_ptr<char> not OK.

    // pointers to float and pointers to int
    count = p_float - p_int; // expected-error {{not pointers to compatible types}}
                             // float * - int * OK
    count = p_float - q_int; // expected-error {{not pointers to compatible types}}
                             // float * - int* _Single OK
    count = p_float - r_int; // expected-error {{not pointers to compatible types}}
                             // float * - int* _Array OK

    count = q_float - p_int; // expected-error {{not pointers to compatible types}}
                             // float* _Single - int * OK
    count = q_float - q_int; // expected-error {{not pointers to compatible types}}
                             // float* _Single - int* _Single OK
    count = q_float - r_int; // expected-error {{not pointers to compatible types}}
                             // float* _Single - int* _Array OK.

    count = r_float - p_int; // expected-error {{not pointers to compatible types}}
                             // float* _Array - int * OK.
    count = r_float - q_int; // expected-error {{not pointers to compatible types}}
                             // float* _Array - int* _Single OK.
    count = r_float - r_int; // expected-error {{not pointers to compatible types}}
                             // float* _Array - int* _Array OK.

    // differences involving safe pointers to void
    count = p_void - q_void; // expected-error {{arithmetic on pointers to void}}
                             // void * - void* _Single not OK
    count = p_void - r_void; // expected-error {{arithmetic on pointers to void}}
                             // void * - void* _Array not OK
    count = q_void - p_void; // expected-error {{arithmetic on pointers to void}}
                             // void* _Single - void * not OK
    count = q_void - q_void; // expected-error {{arithmetic on pointers to void}}
                             // void* _Single - void* _Single not OK
    count = q_void - r_void; // expected-error {{arithmetic on pointers to void}}
                             // void* _Single - void* _Array not  OK.
    count = r_void - p_void; // expected-error {{arithmetic on pointers to void}}
                             // void* _Array - void * not OK.
    count = r_void - q_void; // expected-error {{arithmetic on pointers to void}}
                             // void* _Array - void* _Single not OK.
    count = r_void - r_void; // expected-error {{arithmetic on pointers to void}}
                             // void* _Array - void* _Array not OK.

    // spot check differences involving pointers to void
	q_int - p_void;          // expected-error {{not pointers to compatible types}}
							 // int* _Single - void * not OK
    q_int - q_void;          // expected-error {{not pointers to compatible types}}
	                         // int* _Single - void* _Single not OK
	q_int - r_void;          // expected-error {{not pointers to compatible types}}
							 // int* _Single - void* _Array not OK
	r_int - p_void;          // expected-error {{not pointers to compatible types}}
	                         // int* _Array - void * not OK.
	r_int - q_void;          // expected-error {{not pointers to compatible types}}
							 // int* _Array - void* _Single not OK
	r_int - r_void;          // expected-error {{not pointers to compatible types}}
							 // int* _Array - void* _Array not OK.   
    s_int - p_void;          // expected-error {{not pointers to compatible types}}
                             // int* _Nt_array - void * not OK.
    s_int - q_void;          // expected-error {{not pointers to compatible types}}
                             // int* _Nt_array - void* _Single not OK
    s_int - r_void;          // expected-error {{not pointers to compatible types}}
                             // int* _Nt_array - void* _Array not OK.

	p_void - q_int;          // expected-error {{not pointers to compatible types}}
							 // void* _Single - int * not OK
	p_void - r_int;          // expected-error {{not pointers to compatible types}}
							 // void* _Single - int* _Array not OK
    p_void - s_int;          // expected-error {{not pointers to compatible types}}
                             // void* _Single - int* _Nt_array not OK
	q_void - p_int;          // expected-error {{not pointers to compatible types}}
							 // void* _Single - int * not OK
	q_void - q_int;          // expected-error {{not pointers to compatible types}}
							 // void* _Single - int* _Single not OK
	q_void - r_int;          // expected-error {{not pointers to compatible types}}
							 // void* _Single - int* _Array not OK.
    q_void - s_int;          // expected-error {{not pointers to compatible types}}
                             // void* _Single - int* _Nt_array not OK.
	r_void - p_int;          // expected-error {{not pointers to compatible types}}
							 // void* _Array - int * not OK
	r_void - q_int;          // expected-error {{not pointers to compatible types}}
							 // void* _Array - int* _Single not OK
	r_void - r_int;          // expected-error {{not pointers to compatible types}}
							 // void* _Array - int* _Array not OK.
    r_void - s_int;          // expected-error {{not pointers to compatible types}}
                             // void* _Array - int* _Nt_array not OK.
}

void check_pointer_relational_compare(void)
{
    int result;
    int val_int[5];
    float val_float[5];

    float *p_float = val_float;
    float *p2_float = val_float;
    int *p_int = val_int;
    int *p2_int = val_int;
    void *p_void = val_int;

    float* _Single q_float = &val_float[0];
    float* _Single q2_float = &val_float[0];
    int* _Single q_int = &val_int[0];
    int* _Single q2_int = &val_int[0];
    void* _Single q_void = &val_int[0];
    void* _Single q2_void = &val_int[0];

    float* _Array r_float = val_float;
    float* _Array r2_float = val_float;
    int* _Array r_int = val_int;
    int* _Array r2_int = val_int;
    void* _Array r_void = val_int;
    void* _Array r2_void = val_int;
    int* _Array s_int = 0;
    int* _Array s2_int = s_int;

    // relational comparisons using different kinds of pointers to float
    result = p_float < p2_float; // float * < float * OK
    result = p_float <= q_float; // float * <= float* _Single OK
    result = p_float > r_float;  // float * > float* _Array OK

    result = q_float >= p_float; // float* _Single >= float * OK
    result = q_float < q2_float; // float* _Single < float* _Single OK
    result = q_float <= r_float; // float* _Single <= float* _Array OK.

    result = r_float < p_float;  // float* _Array < float * OK.
    result = r_float <= p_float; // float* _Array <= float* _Single OK.
    result = r_float > r2_float; // float* _Array > float* _Array OK.

    // relational comparisons using different kinds of pointers to int
    result = p_int >= p2_int; // int * >= int * OK
    result = p_int < q_int;   // int * < int* _Single OK
    result = p_int <= r_int;  // int * <= int* _Array OK
    result = p_int <= s_int;  // int * <= int* _Nt_array OK

    result = q_int > p_int;   // int* _Single > int * OK
    result = q_int >= q2_int; // int* _Single >= int* _Single OK
    result = q_int < r_int;   // int* _Single < int* _Array OK.
    result = q_int < s_int;   // int* _Single < int* _Nt_array OK.

    result = r_int <= p_int;  // int* _Array <= int * OK.
    result = r_int > q_int;   // int* _Array > int* _Single OK.
    result = r_int >= r2_int; // int* _Array >= int* _Array OK.
    result = r_int >= s_int;  // int* _Array >= int* _Nt_array OK.

    result = s_int <= p_int;  // int* _Nt_array <= int * OK.
    result = s_int > q_int;   // int* _Nt_array > int* _Single OK.
    result = s_int >= r_int;  // int* _Nt_array >= int* _Array OK.
    result = s_int >= s2_int; // int* _Nt_array >= int* _Nt_array OK.

    // relational comparisons involving safe pointers to void
    result = p_void < q_void;  // void  * < void* _Single OK
    result = p_void <= r_void; // void * <= void* _Array OK
    result = q_void > p_void;  // void* _Single > void * OK
    result = q_void >= q2_void;  // void* _Single >= void* _Single OK
    result = q_void < r_void; // void* _Single < void* _Array  OK.
    result = r_void <= p_void; // void* _Array <= void * OK.
    result = r_void > q_void; // void* _Array > void* _Single OK.
    result = r_void >= r2_void;  // void* _Array >= void* _Array OK.

    // invalid relational comparisons

    // comparisons involving safe pointers to different referent types
    // pointers to int and pointers to float
    result = p_int < p_float;  // expected-warning {{comparison of distinct pointer types}}
                               // int * < float * not OK
    result = p_int <= q_float; // expected-warning {{comparison of distinct pointer types}}
                               // int * <= float* _Single not OK
    result = p_int > r_float;  // expected-warning {{comparison of distinct pointer types}}
                               // int * > float* _Array not OK
    result = q_int >= p_float; // expected-warning {{comparison of distinct pointer types}}
                               // int* _Single >= float * not OK
    result = q_int < q_float; // expected-warning {{comparison of distinct pointer types}}
                               // int* _Single < float* _Single not OK
    result = q_int <= r_float; // expected-warning {{comparison of distinct pointer types}}
                               // int* _Single <= float* _Array not OK.

    result = r_int > p_float;  // expected-warning {{comparison of distinct pointer types}}
                               // int* _Array > float * not OK.
    result = r_int >= q_float; // expected-warning {{comparison of distinct pointer types}}
                               // int* _Array >= float* _Single not OK.
    result = r_int < r_float;  // expected-warning {{comparison of distinct pointer types}}
                               // int* _Array < float* _Array not OK.

    result = s_int > p_float;  // expected-warning {{comparison of distinct pointer types}}
                               // int* _Nt_array > float * not OK.
    result = s_int >= q_float; // expected-warning {{comparison of distinct pointer types}}
                               // int* _Nt_array >= float* _Single not OK.
    result = s_int < r_float;  // expected-warning {{comparison of distinct pointer types}}
                               // int* _Nt_array < float* _Array not OK.

    // pointers to float and pointers to int
    result = p_float <= p_int; // expected-warning {{comparison of distinct pointer types}}
                               // float * <= int * not OK
    result = p_float > q_int;  // expected-warning {{comparison of distinct pointer types}}
                               // float * > int* _Single not OK
    result = p_float >= r_int; // expected-warning {{comparison of distinct pointer types}}
                               // float * >= int* _Array not OK
    result = p_float >= s_int; // expected-warning {{comparison of distinct pointer types}}
                               // float * >= int* _Nt_array not OK

    result = q_float < p_int;  // expected-warning {{comparison of distinct pointer types}}
                               // float* _Single < int * not OK
    result = q_float <= q_int; // expected-warning {{comparison of distinct pointer types}}
                               // float* _Single <= int* _Single not OK
    result = q_float > r_int;  // expected-warning {{comparison of distinct pointer types}}
                               // float* _Single > int* _Array not OK.
    result = q_float > s_int;  // expected-warning {{comparison of distinct pointer types}}
                               // float* _Single > int* _Nt_array not OK.

    result = r_float >= p_int; // expected-warning {{comparison of distinct pointer types}}
                               // float* _Array >= int * not OK.
    result = r_float < q_int;  // expected-warning {{comparison of distinct pointer types}}
                               // float* _Array < int* _Single not OK.
    result = r_float <= r_int; // expected-warning {{comparison of distinct pointer types}}
                               // float* _Array <= int* _Array not OK.
    result = r_float <= s_int; // expected-warning {{comparison of distinct pointer types}}
                               // float* _Array <= int* _Nt_array not OK.

    // pointers to void compared to pointers to int
    result = p_void < p_int;  // expected-warning {{comparison of distinct pointer types}}
                              // void  * < int * not OK
    result = p_void <= q_int; // expected-warning {{comparison of distinct pointer types}}
                              // void  * <= int* _Single not OK
    result = p_void > r_int;  // expected-warning {{comparison of distinct pointer types}}
                              // void * > int* _Array not OK
    result = p_void > s_int;  // expected-warning {{comparison of distinct pointer types}}
                              // void * > int* _Nt_array not OK
    result = q_void >= p_int; // expected-warning {{comparison of distinct pointer types}}
                              // void* _Single >= int * not OK
    result = q_void < q_int;  // expected-warning {{comparison of distinct pointer types}}
                              // void* _Single < int* _Single not OK
    result = q_void <= r_int; // expected-warning {{comparison of distinct pointer types}}
                              // void* _Single <= int* _Array not OK.
    result = q_void <= s_int; // expected-warning {{comparison of distinct pointer types}}
                              // void* _Single <= int* _Nt_array not OK.
    result = r_void > p_int;  // expected-warning {{comparison of distinct pointer types}}
                              // void* _Array > int * not OK.
    result = r_void >= q_int; // expected-warning {{comparison of distinct pointer types}}
                              // void* _Array >= int* _Single not OK.
    result = r_void < r_int;  // expected-warning {{comparison of distinct pointer types}}
                              // void* _Array < int* _Array not OK.
    result = r_void < s_int;  // expected-warning {{comparison of distinct pointer types}}
                              // void* _Array < int* _Array not OK.

    // pointers to int compared to pointers to void
    result = p_int < p_void;  // expected-warning {{comparison of distinct pointer types}}
                              // int  * < void * not OK
    result = p_int <= q_void; // expected-warning {{comparison of distinct pointer types}}
                              // int  * <= void* _Single not OK
    result = p_int > r_void;  // expected-warning {{comparison of distinct pointer types}}
                              // int * > void* _Array not OK
    result = q_int >= p_void; // expected-warning {{comparison of distinct pointer types}}
                              // int* _Single >= void * not OK
    result = q_int < q_void;  // expected-warning {{comparison of distinct pointer types}}
                              // int* _Single < void* _Single not OK
    result = q_int <= r_void; // expected-warning {{comparison of distinct pointer types}}
                              // int* _Single <= void* _Array not OK.
    result = r_int > p_void;  // expected-warning {{comparison of distinct pointer types}}
                              // int* _Array > void * not OK.
    result = r_int >= q_void; // expected-warning {{comparison of distinct pointer types}}
                              // int* _Array >= void* _Single not OK.
    result = r_int < r_void;  // expected-warning {{comparison of distinct pointer types}}
                              // int* _Array < void* _Array not OK.
    result = s_int > p_void;  // expected-warning {{comparison of distinct pointer types}}
                              // int* _Nt_array > void * not OK.
    result = s_int >= q_void; // expected-warning {{comparison of distinct pointer types}}
                              // int* _Nt_array >= void* _Single not OK.
    result = s_int < r_void;  // expected-warning {{comparison of distinct pointer types}}
                              // mt+int* _Array < void* _Array not OK.

    // Relational comparisons involving 0
    // The C11 specification technically doesn't allow this to typecheck for unsafe pointers.  
    // clang allows it anyway, so it is allowed for Checked C.  Note that according to the
    // Checked C spec, this has well-defined behavior.  According to the C11 spec, the behavior
    // is undefined because 0 doesn't point into the object array.
    result = 0 < p_void; // 0 < void * OK
    result = p_int <= 0; // int  * <= 0 OK
    result = p_int > 0;  // int * > 0  OK
    result = q_int >= 0; // int* _Single >= 0 OK
    result = 0 < q_void; // 0 < void* _Single OK
    result = q_int <= 0; // int* _Single <= 0 OK.
    result = r_int > 0;  // int* _Array > 0 OK.
    result = 0 > r_int;  // 0 < int* _Array OK.
    result = 0 < r_void; // 0 < void* _Array OK.
    result = s_int > 0;  // int* _Array > 0 OK.
    result = 0 > s_int;  // 0 < int* _Array OK.
}

void check_pointer_equality_compare(void)
{
    int result;
    int val_int[5];
    float val_float[5];

    float *p_float = val_float;
    float *p2_float = val_float;
    int *p_int = val_int;
    int *p2_int = val_int;
    void *p_void = val_int;

    float* _Single q_float = &val_float[0];
    float* _Single q2_float = &val_float[0];
    int* _Single q_int = &val_int[0];
    int* _Single q2_int = &val_int[0];
    void* _Single q_void = &val_int[0];
    void* _Single q2_void = &val_int[0];

    float* _Array r_float = val_float;
    float* _Array r2_float = val_float;
    int* _Array r_int = val_int;
    int* _Array r2_int = val_int;
    void* _Array r_void = val_int;
    void* _Array r2_void = val_int;

    int* _Nt_array s_int = 0;
    int* _Nt_array s2_int = 0;
    nt_array_ptr<char> s_char = 0;

    // equality/inequality comparisons using different kinds of pointers to float
    result = p_float == p2_float; // float * == float * OK
    result = p_float != q_float;  // float * != float* _Single OK
    result = p_float == r_float;  // float * == float* _Array OK

    result = q_float != p_float; // float* _Single != float * OK
    result = q_float == q2_float; // float* _Single == float* _Single OK
    result = q_float != r_float; // float* _Single != float* _Array OK.

    result = r_float == p_float;  // float* _Array == float * OK.
    result = r_float != p_float; // float* _Array != float* _Single OK.
    result = r_float == r2_float; // float* _Array == float* _Array OK.

    // equality/inquality comparisons using different kinds of pointers to int
    result = p_int != p2_int; // int * != int * OK
    result = p_int == q_int;  // int * == int* _Single OK
    result = p_int != r_int;  // int * != int* _Array OK
    result = p_int != s_int;  // int * != int* _Nt_array OK

    result = q_int == p_int;   // int* _Single == int * OK
    result = q_int != q2_int; // int* _Single != int* _Single OK
    result = q_int == r_int;   // int* _Single == int* _Array OK.
    result = q_int == s_int;   // int* _Single == int* _Nt_array OK.

    result = r_int != p_int;  // int* _Array != int * OK.
    result = r_int == q_int;  // int* _Array == int* _Single OK.
    result = r_int == r2_int; // int* _Array == int* _Array OK.
    result = r_int == s_int;  // int* _Array == int* _Nt_array OK.

    result = s_int != p_int;  // int* _Nt_array != int * OK.
    result = s_int == q_int;  // int* _Nt_array == int* _Single OK.
    result = s_int == r_int;  // int* _Nt_array == int* _Array OK.
    result = s_int == s2_int; // int* _Nt_array == int* _Nt_array OK.

    // equality/inequality comparisons involving safe pointers to void

    result = p_void != q_void;  // void  * != void* _Single OK
    result = p_void == r_void; // void * == void* _Array OK
    result = q_void != p_void;  // void* _Single != void * OK
    result = q_void == q2_void;  // void* _Single == void* _Single OK
    result = q_void != r_void; // void* _Single != void* _Array  OK.
    result = r_void == p_void; // void* _Array == void * OK.
    result = r_void != q_void; // void* _Array != void* _Single OK.
    result = r_void == r2_void;  // void* _Array == void* _Array OK.

    // pointers to void compared to pointers to int
    result = p_void != p_int; // void  * != int * OK
    result = p_void == q_int; // void  * == int* _Single OK
    result = p_void == r_int; // void * == int* _Array OK
    result = p_void == s_int; // void * == int* _Nt_array OK
    result = q_void != p_int; // void* _Single != int * OK
    result = q_void == q_int; // void* _Single == int* _Single OK
    result = q_void != r_int; // void* _Single != int* _Array  OK.
    result = q_void != s_int; // void* _Single != int* _Nt_array  OK.
    result = r_void == p_int; // void* _Array == int * OK.
    result = r_void != q_int; // void* _Array != int* _Single OK.
    result = r_void == r_int; // void* _Array == int* _Array OK.
    result = r_void == s_int; // void* _Array == int* _Nt_array OK.

    // pointers to int compared to pointers to void
    result = p_int != p_void; // int  * != void * OK
    result = p_int == q_void; // int  * == void* _Single OK
    result = p_int != r_void; // int * != void* _Array OK
    result = q_int == p_void; // int* _Single == void * OK
    result = q_int != q_void; // int* _Single != void* _Single OK
    result = q_int == r_void; // int* _Single == void* _Array  OK.
    result = r_int != p_void; // int* _Array != void * OK.
    result = r_int == q_void; // int* _Array == void* _Single OK.
    result = r_int != r_void; // int* _Array != void* _Array OK.
    result = s_int != p_void; // int* _Array != void * OK.
    result = s_int == q_void; // int* _Array == void* _Single OK.
    result = s_int != r_void; // int* _Array != void* _Array OK.

    // equality/inequality comparisons using different kinds of pointers to float and 0
    result = p_float == 0; // float * == 0 OK
    result = p_float != 0; // float * != float* _Single OK

    result = q_float == 0; // float* _Single == 0 OK
    result = q_float != 0; // float* _Single != 0 OK

    result = r_float == 0; // float* _Array == 0 OK.
    result = r_float != 0; // float* _Array != 0 OK.

    result = 0 != p_float; // 0 != float * OK
    result = 0 == p_float; // 0 == float* _Single OK

    result = 0 != q_float; // 0 != float* _Single OK
    result = 0 == q_float; // 0  == float* _Single OK

    result = 0 == r_float; // 0 == float* _Array OK.
    result = 0 != r_float; // 0 != float* _Array OK.

    // spot check pointers to int and pointers to void

    result = p_void == 0; // void * == 0 OK
    result = p_int != 0;  // int * != 0 OK

    result = q_int == 0;  // int* _Single == 0 OK
    result = q_void != 0; // ptr<fvoid> == 0 OK

    result = r_void == 0; // void* _Array == 0 OK.
    result = r_int != 0;  // int* _Array != 0 OK.

    result = s_int != 0;  // int* _Nt_array != 0 OK.

    result = 0 != p_int;  // 0 != int * OK
    result = 0 == p_void; // 0 == void * OK

    result = 0 != q_int;  // 0 == int* _Single OK
    result = 0 == q_void; // 0 == void* _Single OK

    result = 0 != r_void; // 0 != void* _Array
    result = 0 == r_int;  // 0  == int* _Array.

    result = 0 == s_int;  // 0  == int* _Array.

    // invalid equality/inquality comparisons

    // comparisons involving safe pointers to different referent types

    // pointers to int and pointers to float (for nt_array_ptr, use char instead
    // of float).
    result = p_int != p_float; // expected-warning {{comparison of distinct pointer types}}
                               // int * != float * not OK
    result = p_int == q_float; // expected-warning {{comparison of distinct pointer types}}
                               // int * == float* _Single not OK
    result = p_int != r_float; // expected-warning {{comparison of distinct pointer types}}
                               // int * != float* _Array not OK
    result = p_int != s_char;  // expected-warning {{comparison of distinct pointer types}}
                               // int * != nt_array_ptr<char> not OK

    result = q_int == p_float; // expected-warning {{comparison of distinct pointer types}}
                               // int* _Single == float * not OK
    result = q_int != q_float; // expected-warning {{comparison of distinct pointer types}}
                               // int* _Single != float* _Single not OK
    result = q_int == r_float; // expected-warning {{comparison of distinct pointer types}}
                               // int* _Single <= float* _Array not OK.
    result = q_int == s_char;  // expected-warning {{comparison of distinct pointer types}}
                               // int* _Single <= nt_array_ptr<char> not OK.

    result = r_int != p_float; // expected-warning {{comparison of distinct pointer types}}
                               // int* _Array != float * not OK.
    result = r_int == q_float; // expected-warning {{comparison of distinct pointer types}}
                               // int* _Array == float* _Single not OK.
    result = r_int != r_float; // expected-warning {{comparison of distinct pointer types}}
                               // int* _Array != float* _Array not OK.
    result = r_int != s_char;  // expected-warning {{comparison of distinct pointer types}}
                               // int* _Array != nt_array_ptrchar> not OK.

    result = s_int != p_float; // expected-warning {{comparison of distinct pointer types}}
                               // int* _Nt_array != float * not OK.
    result = s_int == q_float; // expected-warning {{comparison of distinct pointer types}}
                               // int* _Nt_array == float* _Single not OK.
    result = s_int != r_float; // expected-warning {{comparison of distinct pointer types}}
                               // int* _Nt_array != float* _Array not OK.
    result = s_int != s_char;  // expected-warning {{comparison of distinct pointer types}}
                               // int* _Nt_array != nt_array_ptrchar> not OK.

   // pointers to float and pointers to int
    result = p_float == p_int; // expected-warning {{comparison of distinct pointer types}}
                               // float * == int * not OK
    result = p_float != q_int; // expected-warning {{comparison of distinct pointer types}}
                               // float * != int* _Single not OK
    result = p_float == r_int; // expected-warning {{comparison of distinct pointer types}}
                               // float * == int* _Array not OK
    result = p_float == s_int; // expected-warning {{comparison of distinct pointer types}}
                               // float * == int* _Nt_array not OK

    result = q_float != p_int; // expected-warning {{comparison of distinct pointer types}}
                               // float* _Single != int * not OK
    result = q_float == q_int; // expected-warning {{comparison of distinct pointer types}}
                               // float* _Single == int* _Single not OK
    result = q_float != r_int; // expected-warning {{comparison of distinct pointer types}}
                               // float* _Single != int* _Array not OK.
    result = q_float != s_int; // expected-warning {{comparison of distinct pointer types}}
                               // float* _Single != int* _Array not OK.

    result = r_float == p_int; // expected-warning {{comparison of distinct pointer types}}
                               // float* _Array == int * not OK.
    result = r_float != q_int; // expected-warning {{comparison of distinct pointer types}}
                               // float* _Array != int* _Single not OK.
    result = r_float == r_int; // expected-warning {{comparison of distinct pointer types}}
                               // float* _Array == int* _Array not OK.
    result = r_float == s_int; // expected-warning {{comparison of distinct pointer types}}
                               // float* _Array == mt+int* _Array not OK.

    result = s_char == p_int; // expected-warning {{comparison of distinct pointer types}}
                               // float* _Array == int * not OK.
    result = s_char != q_int; // expected-warning {{comparison of distinct pointer types}}
                               // float* _Array != int* _Single not OK.
    result = s_char == r_int; // expected-warning {{comparison of distinct pointer types}}
                               // float* _Array == int* _Array not OK.
    result = s_char == s_int; // expected-warning {{comparison of distinct pointer types}}
                               // float* _Array == int* _Array not OK.
}

void check_logical_operators(void)
{
    int val[5];
    int *p = val;
    int* _Single q = &val[0];
    void* _Single q_void = &val[0];
    int* _Array r = 0;
    void* _Array r_void = val;
    int* _Nt_array s = 0;
    _Bool b;

    b = !p;
    b = !q;
    b = !q_void;
    b = !r;
    b = !r_void;
    b = !s;

    b = p || b;
    b = q || b;
    b = q_void || b;
    b = r || b;
    b = r_void || b;
    b = s || b;
    b = b || p;
    b = b || q;
    b = b || q_void;
    b = b || r;
    b = b || r_void;
    b = b || s;

    b = p || q;
    b = q || p;
    b = q_void || r_void;
    b = r || r_void;
    b = r_void || p;
    b = r || p;
    b = r || q;
    b = r_void || q_void;
    b = s || p;
    b = s || q;
    b = p || r;
    b = p || r_void;
    b = p || s;

    b = p && b;
    b = q && b;
    b = q_void && b;
    b = r && b;
    b = r_void && b;
    b = s && b;
    b = b && p;
    b = b && q;
    b = b && q_void;
    b = b && r;
    b = b && r_void;
    b = b && s;

    b = p && q;
    b = q && p;
    b = q_void && r_void;
    b = r && r_void;
    b = r_void && p;
    b = r && p;
    b = r && q;
    b = s && q;
    b = r_void && q_void;
    b = p && r;
    b = p && r_void;
    b = p && s;
}

// spot check operators that aren't supposed to be used with pointer types:
//   *, /, %, <<, >>, |, &, ^, ~, unary -, and unary +
void check_illegal_operators(void)
{
    int val[5];
    int *p = val;
    int* _Single q = &val[0];
    int* _Array r = val;
    int* _Nt_array s = 0;

    p * 5;  // expected-error {{invalid operands to binary expression}}
    5 * p;  // expected-error {{invalid operands to binary expression}}
    p *= 5; // expected-error {{invalid operands to binary expression}}

    q * 5;  // expected-error {{invalid operands to binary expression}}
    5 * q;  // expected-error {{invalid operands to binary expression}}
    q *= 5; // expected-error {{invalid operands to binary expression}}

    r * 5;  // expected-error {{invalid operands to binary expression}}
    5 * r;  // expected-error {{invalid operands to binary expression}}
    r *= 5; // expected-error {{invalid operands to binary expression}}

    s * 5;  // expected-error {{invalid operands to binary expression}}
    5 * s;  // expected-error {{invalid operands to binary expression}}
    s *= 5; // expected-error {{invalid operands to binary expression}}

    p * p;  // expected-error {{invalid operands to binary expression}}
    p *= p; // expected-error {{invalid operands to binary expression}}

    q * q;  // expected-error {{invalid operands to binary expression}}
    q *= q; // expected-error {{invalid operands to binary expression}}

    r * r;  // expected-error {{invalid operands to binary expression}}
    r *= r; // expected-error {{invalid operands to binary expression}}

    s * s;  // expected-error {{invalid operands to binary expression}}
    s *= s; // expected-error {{invalid operands to binary expression}}

    //
    // Test /
    //

    p / 5;  // expected-error {{invalid operands to binary expression}}
    5 / p;  // expected-error {{invalid operands to binary expression}}
    p /= 5; // expected-error {{invalid operands to binary expression}}

    q / 5;  // expected-error {{invalid operands to binary expression}}
    5 / q;  // expected-error {{invalid operands to binary expression}}
    q /= 5; // expected-error {{invalid operands to binary expression}}

    r / 5;  // expected-error {{invalid operands to binary expression}}
    5 / r;  // expected-error {{invalid operands to binary expression}}
    r /= 5; // expected-error {{invalid operands to binary expression}}

    s / 5;  // expected-error {{invalid operands to binary expression}}
    5 / s;  // expected-error {{invalid operands to binary expression}}
    s /= 5; // expected-error {{invalid operands to binary expression}}

    p / p;  // expected-error {{invalid operands to binary expression}}
    p /= p; // expected-error {{invalid operands to binary expression}}

    q / q;  // expected-error {{invalid operands to binary expression}}
    q /= q; // expected-error {{invalid operands to binary expression}}

    r / r;  // expected-error {{invalid operands to binary expression}}
    r /= r; // expected-error {{invalid operands to binary expression}}

    s / s;  // expected-error {{invalid operands to binary expression}}
    s /= s; // expected-error {{invalid operands to binary expression}}

    //
    // Test %
    //

    p % 5;  // expected-error {{invalid operands to binary expression}}
    5 % p;  // expected-error {{invalid operands to binary expression}}
    p %= 5; // expected-error {{invalid operands to binary expression}}

    q % 5;  // expected-error {{invalid operands to binary expression}}
    5 % q;  // expected-error {{invalid operands to binary expression}}
    q %= 5; // expected-error {{invalid operands to binary expression}}

    r % 5;  // expected-error {{invalid operands to binary expression}}
    5 % r;  // expected-error {{invalid operands to binary expression}}
    r %= 5; // expected-error {{invalid operands to binary expression}}

    s % 5;  // expected-error {{invalid operands to binary expression}}
    5 % s;  // expected-error {{invalid operands to binary expression}}
    s %= 5; // expected-error {{invalid operands to binary expression}}

    p % p;  // expected-error {{invalid operands to binary expression}}
    p %= p; // expected-error {{invalid operands to binary expression}}

    q % q;  // expected-error {{invalid operands to binary expression}}
    q %= q; // expected-error {{invalid operands to binary expression}}

    r % r;  // expected-error {{invalid operands to binary expression}}
    r %= r; // expected-error {{invalid operands to binary expression}}

    s % s;  // expected-error {{invalid operands to binary expression}}
    s %= s; // expected-error {{invalid operands to binary expression}}

    //
    // Test <<
    //

    p << 5;  // expected-error {{invalid operands to binary expression}}
    5 << p;  // expected-error {{invalid operands to binary expression}}
    p <<= 5; // expected-error {{invalid operands to binary expression}}

    q << 5;  // expected-error {{invalid operands to binary expression}}
    5 << q;  // expected-error {{invalid operands to binary expression}}
    q <<= 5; // expected-error {{invalid operands to binary expression}}

    r << 5;  // expected-error {{invalid operands to binary expression}}
    5 << r;  // expected-error {{invalid operands to binary expression}}
    r <<= 5; // expected-error {{invalid operands to binary expression}}

    s << 5;  // expected-error {{invalid operands to binary expression}}
    5 << s;  // expected-error {{invalid operands to binary expression}}
    s <<= 5; // expected-error {{invalid operands to binary expression}}

    p << p;  // expected-error {{invalid operands to binary expression}}
    p <<= p; // expected-error {{invalid operands to binary expression}}

    q << q;  // expected-error {{invalid operands to binary expression}}
    q <<= q; // expected-error {{invalid operands to binary expression}}

    r << r;  // expected-error {{invalid operands to binary expression}}
    r <<= r; // expected-error {{invalid operands to binary expression}}

    s << s;  // expected-error {{invalid operands to binary expression}}
    s <<= s; // expected-error {{invalid operands to binary expression}}

    //
    // Test >>
    //
    p >> 5;  // expected-error {{invalid operands to binary expression}}
    5 >> p;  // expected-error {{invalid operands to binary expression}}
    p >>= 5; // expected-error {{invalid operands to binary expression}}

    q >> 5;  // expected-error {{invalid operands to binary expression}}
    5 >> q;  // expected-error {{invalid operands to binary expression}}
    q >>= 5; // expected-error {{invalid operands to binary expression}}

    r >> 5;  // expected-error {{invalid operands to binary expression}}
    5 >> r;  // expected-error {{invalid operands to binary expression}}
    r >>= 5; // expected-error {{invalid operands to binary expression}}

    s >> 5;  // expected-error {{invalid operands to binary expression}}
    5 >> s;  // expected-error {{invalid operands to binary expression}}
    s >>= 5; // expected-error {{invalid operands to binary expression}}

    p >> p;  // expected-error {{invalid operands to binary expression}}
    p >>= p; // expected-error {{invalid operands to binary expression}}

    q >> q;  // expected-error {{invalid operands to binary expression}}
    q >>= q; // expected-error {{invalid operands to binary expression}}

    r >> r;  // expected-error {{invalid operands to binary expression}}
    r >>= r; // expected-error {{invalid operands to binary expression}}

    s >> s;  // expected-error {{invalid operands to binary expression}}
    s >>= s; // expected-error {{invalid operands to binary expression}}

    //
    // Test |
    //

    p | 5;  // expected-error {{invalid operands to binary expression}}
    5 | p;  // expected-error {{invalid operands to binary expression}}
    p |= 5; // expected-error {{invalid operands to binary expression}}

    q | 5;  // expected-error {{invalid operands to binary expression}}
    5 | q;  // expected-error {{invalid operands to binary expression}}
    q |= 5; // expected-error {{invalid operands to binary expression}}

    r | 5;  // expected-error {{invalid operands to binary expression}}
    5 | r;  // expected-error {{invalid operands to binary expression}}
    r |= 5; // expected-error {{invalid operands to binary expression}}

    s | 5;  // expected-error {{invalid operands to binary expression}}
    5 | s;  // expected-error {{invalid operands to binary expression}}
    s |= 5; // expected-error {{invalid operands to binary expression}}

    p | p;  // expected-error {{invalid operands to binary expression}}
    p |= p; // expected-error {{invalid operands to binary expression}}

    q | q;  // expected-error {{invalid operands to binary expression}}
    q |= q; // expected-error {{invalid operands to binary expression}}

    r | r;  // expected-error {{invalid operands to binary expression}}
    r |= r; // expected-error {{invalid operands to binary expression}}

    s | s;  // expected-error {{invalid operands to binary expression}}
    s |= s; // expected-error {{invalid operands to binary expression}}

    //
    // Test &
    //

    p & 5;  // expected-error {{invalid operands to binary expression}}
    5 & p;  // expected-error {{invalid operands to binary expression}}
    p &= 5; // expected-error {{invalid operands to binary expression}}

    q & 5;  // expected-error {{invalid operands to binary expression}}
    5 & q;  // expected-error {{invalid operands to binary expression}}
    q &= 5; // expected-error {{invalid operands to binary expression}}

    r & 5;  // expected-error {{invalid operands to binary expression}}
    5 & r;  // expected-error {{invalid operands to binary expression}}
    r &= 5; // expected-error {{invalid operands to binary expression}}

    s & 5;  // expected-error {{invalid operands to binary expression}}
    5 & s;  // expected-error {{invalid operands to binary expression}}
    s &= 5; // expected-error {{invalid operands to binary expression}}

    p & p;  // expected-error {{invalid operands to binary expression}}
    p &= p; // expected-error {{invalid operands to binary expression}}

    q & q;  // expected-error {{invalid operands to binary expression}}
    q &= q; // expected-error {{invalid operands to binary expression}}

    r & r;  // expected-error {{invalid operands to binary expression}}
    r &= r; // expected-error {{invalid operands to binary expression}}

    s & s;  // expected-error {{invalid operands to binary expression}}
    s &= s; // expected-error {{invalid operands to binary expression}}

    //
    // Test ^
    //

    p ^ 5;  // expected-error {{invalid operands to binary expression}}
    5 ^ p;  // expected-error {{invalid operands to binary expression}}
    p ^= 5; // expected-error {{invalid operands to binary expression}}

    q ^ 5;  // expected-error {{invalid operands to binary expression}}
    5 ^ q;  // expected-error {{invalid operands to binary expression}}
    q ^= 5; // expected-error {{invalid operands to binary expression}}

    r ^ 5;  // expected-error {{invalid operands to binary expression}}
    5 ^ r;  // expected-error {{invalid operands to binary expression}}
    r ^= 5; // expected-error {{invalid operands to binary expression}}

    s ^ 5;  // expected-error {{invalid operands to binary expression}}
    5 ^ s;  // expected-error {{invalid operands to binary expression}}
    s ^= 5; // expected-error {{invalid operands to binary expression}}

    p ^ p;  // expected-error {{invalid operands to binary expression}}
    p ^= p; // expected-error {{invalid operands to binary expression}}

    q ^ q;  // expected-error {{invalid operands to binary expression}}
    q ^= q; // expected-error {{invalid operands to binary expression}}

    r ^ r;  // expected-error {{invalid operands to binary expression}}
    r ^= r; // expected-error {{invalid operands to binary expression}}

    s ^ s;  // expected-error {{invalid operands to binary expression}}
    s ^= s; // expected-error {{invalid operands to binary expression}}

    //
    // Test ~
    //
    ~p;  // expected-error {{invalid argument type}}
    ~q;  // expected-error {{invalid argument type}}
    ~r;  // expected-error {{invalid argument type}}
    ~s;  // expected-error {{invalid argument type}}

    //
    // Test unary -
    //
    -p;  // expected-error {{invalid argument type}}
    -q;  // expected-error {{invalid argument type}}
    -r;  // expected-error {{invalid argument type}}
    -s;  // expected-error {{invalid argument type}}

    //
    // Test unary +
    //
    +p;  // expected-error {{invalid argument type}}
    +q;  // expected-error {{invalid argument type}}
    +r;  // expected-error {{invalid argument type}}
    +s;  // expected-error {{invalid argument type}}
}

//
// Test that address-of dereference and array subscript expressions
// have the correct types.
//

extern void test_sprintf(char *s);

void check_address_of_types(char s[10],
                            char *c : itype(char _Checked[10]),
                            char buf _Nt_checked[],
                            char* _Nt_array str,
                            char* _Array arr : count(10),
                            char* _Single p) {
  _Unchecked {
    test_sprintf(s);
    test_sprintf(&*s);
    test_sprintf(&*(s + 2));
    test_sprintf(&s[2]);
    test_sprintf(&2[s]);

    test_sprintf(c);
    test_sprintf(&*c);
    test_sprintf(&*(c + 3));
    test_sprintf(&c[3]);
    test_sprintf(&3[c]);

    test_sprintf(buf); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&*buf); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&*(buf + 0)); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&buf[0]); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&0[buf]); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}

    test_sprintf(str); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&*str); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&*(str + 0)); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&str[0]); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&0[str]); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}

    test_sprintf(arr); // expected-error {{passing '_Array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&*arr); // expected-error {{passing '_Array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&*(arr + 1)); // expected-error {{passing '_Array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&arr[1]); // expected-error {{passing '_Array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&1[arr]); // expected-error {{passing '_Array_ptr<char>' to parameter of incompatible type 'char *'}}

    test_sprintf(p); // expected-error {{passing '_Ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&*p); // expected-error {{passing '_Ptr<char>' to parameter of incompatible type 'char *'}}
  }

  _Checked {
    test_sprintf(c); // expected-error {{passing '_Array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&*c); // expected-error {{passing '_Array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&*(c + 3)); // expected-error {{passing '_Array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&c[3]); // expected-error {{passing '_Array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&3[c]); // expected-error {{passing '_Array_ptr<char>' to parameter of incompatible type 'char *'}}

    test_sprintf(buf); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&*buf); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&*(buf + 0)); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&buf[0]); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&0[buf]); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}

    test_sprintf(str); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&*str); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&*(str + 0)); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&str[0]); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&0[str]); // expected-error {{passing '_Nt_array_ptr<char>' to parameter of incompatible type 'char *'}}

    test_sprintf(arr); // expected-error {{passing '_Array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&*arr); // expected-error {{passing '_Array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&*(arr + 1)); // expected-error {{passing '_Array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&arr[1]); // expected-error {{passing '_Array_ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&1[arr]); // expected-error {{passing '_Array_ptr<char>' to parameter of incompatible type 'char *'}}

    test_sprintf(p); // expected-error {{passing '_Ptr<char>' to parameter of incompatible type 'char *'}}
    test_sprintf(&*p); // expected-error {{passing '_Ptr<char>' to parameter of incompatible type 'char *'}}
  }
}
