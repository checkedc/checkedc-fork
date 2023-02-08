#include <stdchecked.h>
// Test type checking of itype generic function calls.
//
// RUN: %clang_cc1 -verify -verify-ignore-unexpected=note %s

//
// Test mismatches between the number of type variables and type arguments.
//
_Itype_for_any(T) void* oneTypeVariable(void* a  _Itype(T* _Single), void* b  _Itype(T* _Single))  _Itype(T* _Single) {
  return a;
}

_Itype_for_any(T, Q) void* manyTypeVariables(void* a  _Itype(T* _Single), void* b  _Itype(Q* _Single))  _Itype(Q* _Single) {
  return b;
}


_Itype_for_any(T)
void* validItypeGenericFunction(int a, void* b  _Itype(T* _Single), void* c  _Itype(T* _Single))  _Itype(T* _Single);

_Itype_for_any(T)
void* validItypeGenericFunction(int a, void* b  _Itype(T* _Single) , void* c  _Itype(T* _Single) )  _Itype(T* _Single) {
  T* _Single m = b;
  return m;
}



void CallItypeGenericFunctions(void) {
  int a = 0, b = 0, c = 0, d = 0;
  int* _Single ap = &a;
  int* _Single bp = &b;
  int* _Single cp = &c;
  int* _Single dp = &d;

  oneTypeVariable<int, int>(ap, bp);  //expected-error {{mismatch between the number of types listed and number of type variables}}
  oneTypeVariable<>(ap, bp);          //expected-error {{mismatch between the number of types listed and number of type variables}}
  manyTypeVariables<int, int>(ap, bp);
  manyTypeVariables<>(ap, bp); //expected-error {{mismatch between the number of types listed and number of type variables}}

  float y = 5.5, z = 0.0;
  float* _Single p2 = &y;
  float* _Single p = &z;
  checked {
    p = validItypeGenericFunction<float>(5, p2, p2);
    //Checked scope expects type arguments
    validItypeGenericFunction(5, 0, 0); //expected-error {{expected a type argument list for a bounds-safe interface call in a checked scope}}
  }
  unchecked{
    void * p3 = (void *) p2;
    //Unchecked scope. Type arguments are not mandated.
    float *p4 = validItypeGenericFunction(5, p3, p3);
    float* _Single p5 = validItypeGenericFunction<float>(5,p2,p2);
  }
}

