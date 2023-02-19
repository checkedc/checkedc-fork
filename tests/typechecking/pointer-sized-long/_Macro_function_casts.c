// These are some platform-specific tests to go with
// the tests in ../function_casts.c
//
// The following line is for the LLVM test harness:
// RUN: %clang_cc1 -verify %s
//

#include <stdchecked.h>

int f0(int a) {
  return a;
}

void local_convert(int(*f1)(int), int (*_Single f2)(int) ) {
  // There's no good reason to do this to any function pointers
  // and it's definitely not safe.
  int(*_Single local_weird_unsafe1)(int)  = (int (*_Single )(int))~(long)f1; // expected-error {{can only cast function names or null pointers to checked function pointer type '_Ptr<int (int)>'}}
  int (*_Single local_weird_unsafe2)(int)  = (int (*_Single )(int))~(long)f2; // expected-error {{can only cast function names or null pointers to checked function pointer type '_Ptr<int (int)>'}}
  int (*_Single local_weird_unsafe3)(int)  = (int (*_Single )(int))~(long)f0; // expected-error {{can only cast function names or null pointers to checked function pointer type '_Ptr<int (int)>'}}
  int (*_Single local_weird_unsafe4)(int)  = (int (*_Single )(int))(long)!(long)f1; // expected-error {{can only cast function names or null pointers to checked function pointer type '_Ptr<int (int)>'}}
  int (*_Single local_weird_unsafe5)(int)  = (int (*_Single )(int))(long)!(long)f2; // expected-error {{can only cast function names or null pointers to checked function pointer type '_Ptr<int (int)>'}}
  int (*_Single local_weird_unsafe6)(int)  = (int (*_Single )(int))(long)!(long)f0; // expected-error {{can only cast function names or null pointers to checked function pointer type '_Ptr<int (int)>'}}
  int (*_Single local_weird_unsafe7)(int)  = (int (*_Single )(int)) + (long)f1; // expected-error {{can only cast function names or null pointers to checked function pointer type '_Ptr<int (int)>'}}
  int (*_Single local_weird_unsafe8)(int)  = (int (*_Single )(int)) + (long)f2; // expected-error {{can only cast function names or null pointers to checked function pointer type '_Ptr<int (int)>'}}
  int (*_Single local_weird_unsafe9)(int)  = (int (*_Single )(int)) + (long)f0; // expected-error {{can only cast function names or null pointers to checked function pointer type '_Ptr<int (int)>'}}
  int (*_Single local_weird_unsafe10)(int)  = (int (*_Single )(int)) - (long)f1; // expected-error {{can only cast function names or null pointers to checked function pointer type '_Ptr<int (int)>'}}
  int (*_Single local_weird_unsafe11)(int)  = (int (*_Single )(int)) - (long)f2; // expected-error {{can only cast function names or null pointers to checked function pointer type '_Ptr<int (int)>'}}
  int (*_Single local_weird_unsafe12)(int)  = (int (*_Single )(int)) - (long)f0; // expected-error {{can only cast function names or null pointers to checked function pointer type '_Ptr<int (int)>'}}

}