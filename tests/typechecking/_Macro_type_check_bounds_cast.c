// Feature tests of parsing new Checked C dynamic and assume bounds
// cast. The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -verify-ignore-unexpected=note %s

#include <stdchecked.h>

extern void f1() {
  int* _Array a : count(1) = 0;
  int i;
  int* _Single c = 0;
  int b[10];
  int p[10];

  int* _Array checkedc_p : bounds(checkedc_p, checkedc_p + 1) = 0;
  a = _Dynamic_bounds_cast<int* _Array>(b, count(10));
  c = _Dynamic_bounds_cast<int>(p); // expected-error {{expected _Ptr or * type}}
}

extern void f2() {
  char p[10];
  int* _Array a : count(1) = 0;
  int b checked[10];
  int* _Array c : count(10) = (int* _Array)a; // expected-error {{declared bounds for 'c' are invalid after initialization}}
  int* _Array d : count(10) = _Dynamic_bounds_cast<int* _Array>(a, count(5)); // expected-error {{declared bounds for 'd' are invalid after initialization}}
  a = _Assume_bounds_cast<int* _Array>(p); // expected-error {{expected _Ptr or * type}}
}

extern void f3() {
  char p[10];
  int* _Single c = 0;
  int* _Array a : count(2) = 0;
  a = _Assume_bounds_cast<int* _Array>(p, count(p + 2)); // expected-error {{invalid argument type 'char *' to count expression}}
  c = _Assume_bounds_cast<int* _Single>(p, count(1)); // expected-error {{expected _Array_ptr type}}
}

extern void f4() {
  int* _Array a : count(2) = 0;
  char* _Array b : count(2) = 0;

  b = _Assume_bounds_cast<char* _Array>(a, count(2));
  a = _Assume_bounds_cast<int* _Array>(b); // expected-error {{expected _Ptr or * type}}
}

struct S1 {
  struct {
    int* _Array lower;
    int* _Array upper;
  } pair;
  int* _Array arr1 : bounds(pair.lower, pair.upper);
  struct {
    int* _Array arr2 : bounds(pair.lower, pair.upper);
  } nested;
};

extern void f5() {
  int *p;
  int* _Single q = 0;
  int* _Array r;
  int* _Array s : bounds(r, r + 5) = 0;
  p = _Assume_bounds_cast<int *>(q);
  p = _Assume_bounds_cast<int *>(s);
}

extern int* _Array h4(void) : count(3) {
  int* _Array p : bounds(p, p + 3) = 0;
  return p;
}

extern void f6() {
  int i[2];
  int* _Array int_array_ptr_lb = i, *_Array int_array_ptr_ub = i + 1;
  int* _Single int_ptr_lb = i, *_Single int_ptr_ub = i + 1;
  int *int_unchecked_ptr_lb = i, *int_unchecked_ptr_ub = i + 1;

  char* _Array char_array_ptr_lb = (char* _Array)i,
               *_Array char_array_ptr_ub = (char* _Array)i + 1;
  char* _Single char_ptr_lb = (char* _Single)i, *_Single char_ptr_ub = (char* _Single)(i + 1);
  char *char_unchecked_ptr_lb = (char *)i,
       *char_unchecked_ptr_ub = (char *)i + 1;

  int* _Array t20 : bounds(int_array_ptr_lb, char_array_ptr_ub) = // expected-error {{pointer type mismatch}}
        _Assume_bounds_cast<int* _Array>(i, bounds(int_array_ptr_lb, char_array_ptr_ub)); // expected-error {{pointer type mismatch}}

  int* _Array t21 : bounds(int_ptr_lb, char_array_ptr_ub) = // expected-error {{pointer type mismatch}}
        _Assume_bounds_cast<int* _Array>(i, bounds(int_ptr_lb, int_array_ptr_ub));
}

extern void f7(void *p) {
  int* _Array r : count(3) = 0;
  int* _Single q = 0;
  r = _Assume_bounds_cast<int* _Array>(p, count(3));
  q = _Dynamic_bounds_cast<int* _Single>(h4());
}

extern void f8() {
  int *p;
  int* _Single q = 0;
  int* _Single r = 0;
  int* *_Single s = 0;
  int* _Single *_Single t = 0;
  r = _Dynamic_bounds_cast<int* _Single>(q);
  p = _Assume_bounds_cast<int *>(q);
  r = _Dynamic_bounds_cast<int* _Single>(q);
  p = _Dynamic_bounds_cast<int *>(q);
  s = _Assume_bounds_cast<int * *_Single>(q);
  t = _Assume_bounds_cast<int* _Single *_Single>(q);
  t = _Dynamic_bounds_cast<int* _Single *_Single>(q);
  r = _Dynamic_bounds_cast<int* _Single>(q);
  p = _Assume_bounds_cast<int *>(q);

}

extern void f9() {
  int* _Array r : count(3) = 0;
  int* _Single q = 0;

  // The declared bounds of h4() use the value of r, but r is overwritten
  // in the assignment. The value of r (used in the declared bounds (r, r + 4))
  // is lost, so the inferred bounds for the cast expression are unknown.
  r = _Assume_bounds_cast<int* _Array>(h4(), bounds(r, r + 4) rel_align(int)); // expected-error {{inferred bounds for 'r' are unknown after assignment}}
  
  // The declared bounds of h4() do not use the value of r, so the bounds of the
  // cast expression are not invalidated.
  r = _Assume_bounds_cast<int* _Array>(h4(), count(4));

  q = _Dynamic_bounds_cast<int* _Single>(h4(), bounds(r, r + 4) rel_align_value(sizeof(int))); // expected-error {{expected _Array_ptr type}}
}

extern void f10() {
  int *p;
  int* _Array q : count(3) = 0;
  int* _Single r = 0;
  int* *_Single s = 0;
  int* _Single *_Single t = 0;
  int* _Array qq : count(4) = 0;
  r = _Dynamic_bounds_cast<int* _Single>(q, bounds(q, q + 4) rel_align(int)); // expected-error {{expected _Array_ptr type}}
  p = _Assume_bounds_cast<int *>(q, bounds(q, q + 4) rel_align_value(sizeof(int))); // expected-error {{expected _Array_ptr type}}

  s = _Assume_bounds_cast<int * *_Single>(q, bounds(q, q + 4) rel_align(int)); // expected-error {{expected _Array_ptr type}}
  t = _Assume_bounds_cast<int* _Single *_Single>(q, bounds(q, q + 4) rel_align_value(sizeof(int))); // expected-error {{expected _Array_ptr type}}

  qq = _Assume_bounds_cast<int* _Array>(q, bounds(q, q + 4)  rel_align(int)) ;
  p = _Assume_bounds_cast<int *>(q, bounds(q, q + 4) rel_align_value(sizeof(int))); // expected-error {{expected _Array_ptr type}}
}

extern void f11() {
  int c, i;
  int *p;
  int len = 5;
  int* _Array q : count(2) = 0;
  int* _Single r = 0;
  char* _Single cq = 0;
  char* _Array rr;
  int* _Array rrr;
  
  r = _Dynamic_bounds_cast<int* _Single>(q, bounds(q, q + 4) rel_align(len)); // expected-error {{unknown type name 'len'}}
  p = _Dynamic_bounds_cast<int *>(q, bounds(q, q + 4) rel_align_value(len)); // expected-error {{expression is not an integer constant expression}} expected-error {{expected _Array_ptr type}}
  p = _Dynamic_bounds_cast<int *>(q, bounds(q, q + 4) rel_align_value(1)); // expected-error {{expected _Array_ptr type}}

  c = _Assume_bounds_cast<int>(cq); // expected-error {{expected _Ptr or * type}}
  p = _Dynamic_bounds_cast<char>(p, bounds(p, p + 2)); // expected-error {{expected _Array_ptr type}}
  p = _Dynamic_bounds_cast<char>(p); // expected-error {{expected _Ptr or * type}}
  r = _Dynamic_bounds_cast<int* _Array>(r); // expected-error{{expected _Ptr or * type}}
  r = _Dynamic_bounds_cast<int* _Array>(q); // expected-error {{expected _Ptr or * type}}
  r = _Dynamic_bounds_cast<int* _Array>(i); // expected-error {{expected _Ptr or * type}}
  r = _Dynamic_bounds_cast<int* _Array>(i); // expected-error {{expected _Ptr or * type}}
  r = _Dynamic_bounds_cast<int* _Array>(p); // expected-error {{expected _Ptr or * type}}
  rr = _Dynamic_bounds_cast<int* _Array>(p); // expected-error {{expected _Ptr or * type}}
  rrr = _Dynamic_bounds_cast<int* _Array>(p); // expected-error {{expected _Ptr or * type}}
  r = _Dynamic_bounds_cast<int* _Array>(p); // expected-error {{expected _Ptr or * type}}
  q = _Dynamic_bounds_cast<int* _Single>(rr, count(1)); // expected-error{{expected _Array_ptr type}}
  q = _Dynamic_bounds_cast<int* _Single>(rr, bounds(rr, rr + 1)); // expected-error{{expected _Array_ptr type}}

  p = _Dynamic_bounds_cast<int *>(q, count(1)); // expected-error {{expected _Array_ptr type}}
  q = _Dynamic_bounds_cast<int* _Single>(q, count(1)); // expected-error {{expected _Array_ptr type}}
  q = _Dynamic_bounds_cast<int* _Single>(i, count(1)); // expected-error {{expected _Array_ptr type}}
  p = _Dynamic_bounds_cast<int *>(i, count(1)); // expected-error {{expected _Array_ptr type}}
  q = _Dynamic_bounds_cast<int* _Single>(i, bounds(i, i + 1)); // expected-error 2 {{expected expression with pointer type}}
  p = _Dynamic_bounds_cast<int *>(i, bounds(i, i + 1)); // expected-error 2 {{expected expression with pointer type}}

  q = _Dynamic_bounds_cast<int* _Single>(p, count(1)); // expected-error {{expected _Array_ptr type}}
  p = _Dynamic_bounds_cast<int *>(p, count(1)); // expected-error {{expected _Array_ptr type}}
  q = _Dynamic_bounds_cast<int* _Single>(p, bounds(p, p + 1)); // expected-error {{expected _Array_ptr type}}
  p = _Dynamic_bounds_cast<int *>(p, bounds(p, p + 1)); // expected-error {{expected _Array_ptr type}}
  
  q = _Dynamic_bounds_cast<int* _Single>(r, count(1)) + 3; // expected-error{{expected _Array_ptr type}}
  c = _Dynamic_bounds_cast<int* _Single>(p, count(4)); // expected-error{{expected _Array_ptr type}}
}

