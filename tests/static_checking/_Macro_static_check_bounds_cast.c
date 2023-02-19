// Feature tests of static checking of Pointer Bounds Cast
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -verify-ignore-unexpected=note  %s

#include <stdchecked.h>

extern void f1() {
  int* _Array a  _Count(1) = 0;
  int i;
  int* _Single c = 0;
  int b[10];

  int *p = 0;
  int* _Array checkedc_p : bounds(checkedc_p, checkedc_p + 1) = 0;
  c = _Dynamic_bounds_cast<int* _Single>(p); // expected-error {{expression has unknown bounds}}
  c = _Dynamic_bounds_cast<int* _Single>(p); // expected-error {{expression has unknown bounds}}
  a = _Assume_bounds_cast<int* _Array>(p, count(4));
  checkedc_p = _Assume_bounds_cast<int* _Array>(p, bounds(p, p + 1));
  checkedc_p = _Dynamic_bounds_cast<int* _Array>(p, bounds(p, p + 1)); // expected-error {{expression has unknown bounds}}
  a = _Assume_bounds_cast<int* _Array>(p, count(1));
  a = _Assume_bounds_cast<int* _Array>(p, bounds(p, p + 1));
  int* _Array d = _Assume_bounds_cast<int* _Array>(p, count(4));
  c = _Dynamic_bounds_cast<int* _Single>(p); // expected-error {{expression has unknown bounds}}
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

extern void f2() {
  int* _Array a  _Count(2) = 0;
  struct S1 *p = 0;
  a = _Dynamic_bounds_cast_M(int* _Array, p, count(2)); // expected-error {{expression has unknown bounds}}
}

extern void f3() {
  int *p = 0;
  int* _Single q = 0;
  int* _Array r = 0;
  int* _Array s1 = 0;
  int* _Array s2 : bounds(r, r + 5) = 0;
  int* _Array t  _Count(5) = 0;
  p = _Assume_bounds_cast_M_N(int *, r);
  p = _Dynamic_bounds_cast_M_N(int *, r); // expected-error {{expression has unknown bounds}}
  q = _Assume_bounds_cast_M_N(int* _Single, p);
  q = _Dynamic_bounds_cast_M_N(int* _Single, p); // expected-error {{expression has unknown bounds}}
  q = _Dynamic_bounds_cast_M_N(int* _Single, r); // expected-error {{expression has unknown bounds}}
  q = _Dynamic_bounds_cast_M_N(int* _Single, r) + 3; // expected-error{{arithmetic on _Ptr type}}

  *(_Assume_bounds_cast<int* _Single>(r) + 2) = 4; // expected-error{{arithmetic on _Ptr type}}
  // For the statement below, the compiler figures out that r + 2 is out of bounds r  _Count(1).
  // r  _Count(1) normals to bounds(r, r + 1), and r + 2 is out of that range.
  *(_Dynamic_bounds_cast<int* _Array>(r, count(1)) + 2) = 4; // expected-error {{expression has unknown bounds}} expected-error {{out-of-bounds memory access}}
  // For the statement below, the compiler figures out that t[3] is out of bounds t  _Count(3).
  // t  _Count(3) normals to bounds(t, t + 3), and t[3] is out of that range.
  int n = (_Dynamic_bounds_cast<int* _Array>(t, count(3)))[3]; // expected-error {{out-of-bounds memory access}}
  s1 = _Dynamic_bounds_cast<int* _Array>(p, count(5)); // expected-error {{expression has unknown bounds}}
  s2 = _Assume_bounds_cast<int* _Array>(r, count(5));
}

extern int* _Single f4(int arr checked[]) {
  return _Dynamic_bounds_cast<int* _Single>(arr); // expected-error{{expression has unknown bounds}}
}

checked int *f5(int *p, int* _Single q, int* _Array r, int* _Array s _Count(2)) unchecked { // expected-error {{return in a checked scope must have a checked type or a bounds-safe interface}} \
     expected-error {{parameter in a checked scope must have a checked type or a bounds-safe interface}}
  int* _Single t8 = 0;
  t8 = _Assume_bounds_cast<int* _Single>(r, count(1)); // expected-error{{expected _Array_ptr type}}
  int a = 5;
  checked {
    *q = 2;
    *(_Dynamic_bounds_cast<int* _Single>(r, count(1))) = 3; // expected-error{{expected _Array_ptr type}}
    *s = 4;
    unchecked {
      int b checked[5][5];
      for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
          b[i][j] += *q + *(_Dynamic_bounds_cast<int* _Array>(r, count(1))); // expected-error {{expression has unknown bounds}}
        }
      }
    }
  }
  return 0;
}

extern int* _Array h5(void) {
  int* _Array k = 0;
  return k;
}

extern void f6() {
  int *p;
  p = _Dynamic_bounds_cast<int *>(h5); // expected-error{{expected h5 to have a pointer, array, or integer type}}
}

extern int *f7(int arr checked[]) {
  int k;
  return _Dynamic_bounds_cast<int *>(k); // expected-error{{expression has unknown bounds}}
}

extern void f18(int i) {
  int c;
  int *p = 0;
  char *cp = 0;
  int* _Single q = 0;
  ptr<char> cq = 0;
  int* _Array r  _Count(5) = 0;
  array_ptr<char> cr = 0;

  p = _Dynamic_bounds_cast<int *>(p); // expected-error{{expression has unknown bounds}}
  p = _Dynamic_bounds_cast<char *>(p);   // expected-warning {{incompatible pointer type}} expected-error {{expression has unknown bounds}}

  p = _Dynamic_bounds_cast<int *>(i); // expected-error {{expression has unknown bounds}}
  p = _Dynamic_bounds_cast<char *>(i); // expected-error {{expression has unknown bounds}} expected-warning {{incompatible pointer}}

  p = _Dynamic_bounds_cast<int *>(q);

  p = _Dynamic_bounds_cast<int *>(q, bounds(q, q + 1)); // expected-error {{arithmetic on _Ptr}}

  p = _Dynamic_bounds_cast<int *>(r);
  p = _Dynamic_bounds_cast<int *>(r, count(1)); // expected-error {{expected _Array_ptr type}}
  p = _Dynamic_bounds_cast<int *>(r, bounds(r, r + 1)); // expected-error {{expected _Array_ptr type}}

  q = _Dynamic_bounds_cast<int* _Single>(p); // expected-error {{expression has unknown bounds}}
  q = _Dynamic_bounds_cast<ptr<char>>(p); // expected-error {{assigning to '_Ptr<int>'}}

  q = _Dynamic_bounds_cast<int* _Single>(i); // expected-error {{expression has unknown bounds}}
  q = _Dynamic_bounds_cast<ptr<char>>(i); // expected-error{{assigning to '_Ptr<int>'}}

  q = _Dynamic_bounds_cast<int* _Single>(q);
  q = _Dynamic_bounds_cast<int* _Single>(q, bounds(q, q + 1)); // expected-error {{arithmetic on _Ptr}}

  q = _Dynamic_bounds_cast<int* _Single>(r);

  r = _Dynamic_bounds_cast<int* _Array>(p, count(1)); // expected-error {{expression has unknown bounds}} expected-error {{declared bounds for 'r' are invalid after assignment}}
  r = _Dynamic_bounds_cast<int* _Array>(p, bounds(p, p + 1)); // expected-error {{expression has unknown bounds}} expected-error {{declared bounds for 'r' are invalid after assignment}}

  r = _Dynamic_bounds_cast<int* _Array>(i, count(5)); // expected-error {{expression has unknown bounds}}
  r = _Dynamic_bounds_cast<int* _Array>(i, bounds(i, i + 1)); // expected-error 2 {{expected expression with pointer}}

  int len;

  r = _Dynamic_bounds_cast<int* _Array>(q, count(len));       // expected-error {{it is not possible to prove that the inferred bounds of 'r' imply the declared bounds of 'r' after assignment}}
  r = _Dynamic_bounds_cast<int* _Array>(q, bounds(q, q + 1)); // expected-error {{arithmetic on _Ptr type}}

  r = _Dynamic_bounds_cast<int* _Array>(r, count(1));        // expected-error {{declared bounds for 'r' are invalid after assignment}}
  r = _Dynamic_bounds_cast<int* _Array>(r, bounds(r, r + 1)); // expected-error {{declared bounds for 'r' are invalid after assignment}}

  p = _Dynamic_bounds_cast<char *>(p); // expected-warning{{incompatible pointer types assigning}} expected-error{{expression has unknown bounds}}

  p = _Assume_bounds_cast<int *>(q);
  p = _Assume_bounds_cast<int *>(cq);
  p = _Assume_bounds_cast<int *>(cr);
  p = _Dynamic_bounds_cast<int *>(cr); // expected-error{{expression has unknown bounds}}
  cp = _Dynamic_bounds_cast<char *>(p); // expected-error{{expression has unknown bounds}}
  q = _Dynamic_bounds_cast<int* _Single>(p); // expected-error {{expression has unknown bounds}}
  p = _Assume_bounds_cast<int *>(r);
}

extern float h6(void) {
  float k = 0;
  return k;
}

extern void f19(){
  int p[10];
  int a : bounds(p, p+1) = 0;
  float b;
  int* _Array x  _Count(10) = 0;

  p = _Dynamic_bounds_cast<int *>(h6); // expected-error {{to have a pointer}}
  p = _Dynamic_bounds_cast<int *>(h6, count(2)); // expected-error {{to have a pointer}}
  p = _Dynamic_bounds_cast<int *>(h6, bounds(p, p + 1)); // expected-error {{to have a pointer}}
  x = _Dynamic_bounds_cast<int* _Array>(a, count(10));
  x = _Dynamic_bounds_cast<int *>(b); // expected-error {{to have a pointer}}
  x = _Dynamic_bounds_cast<int* _Array>(b, count(1)); // expected-error {{to have a pointer}}
  x = _Dynamic_bounds_cast<int* _Array>(b, bounds(p, p + 1)); // expected-error {{to have a pointer}}

  x = _Dynamic_bounds_cast<int* _Array>(p, count(b)); // expected-error {{invalid argument type}}
  x = _Dynamic_bounds_cast<int* _Array>(p, bounds(p, 1)); // expected-error {{expected expression with}}
  x = _Dynamic_bounds_cast<int* _Array>(p, bounds(p, p + 1)); // expected-error {{declared bounds for 'x' are invalid after assignment}}
}

extern int* _Array h7(void)  _Count(3) {
  int* _Array p : bounds(p, p + 3) = 0;
  return p;
}

extern array_ptr<char> h8(void)  _Count(8) {
  array_ptr<char> buf  _Count(8) = 0;
  return buf;
}

extern void f20(void *p) {
  int* _Array intbuf  _Count(3) = 0;
  intbuf = _Assume_bounds_cast<int* _Array>(h7(), count(3));
  int i = intbuf[2];
}

extern void f21(array_ptr<char> buf  _Count(len), int len) {
  int* _Array intbuf  _Count(12) = _Dynamic_bounds_cast<int* _Array>(buf, bounds(intbuf, intbuf + 12));
  int i = intbuf[12]; // expected-error {{out-of-bounds memory access}} \
                      // expected-note {{accesses memory at or above the upper bound}} \
                      // expected-note {{(expanded) inferred bounds are 'bounds(intbuf, intbuf + 12)'}}
}

extern void f22() {
  int* _Array intbuf  _Count(2) = _Dynamic_bounds_cast<int* _Array>(h8(), count(2));
  int i = intbuf[2]; // expected-error {{out-of-bounds memory access}} \
                     // expected-note {{accesses memory at or above the upper bound}} \
                     // expected-note {{(expanded) inferred bounds are 'bounds(intbuf, intbuf + 2)'}}
}

extern void f23() {
  array_ptr<char> buf  _Count(10) = _Assume_bounds_cast<array_ptr<char>>(h7(), count(10));
  char c = buf[10]; // expected-error {{out-of-bounds memory access}} \
                    // expected-note {{accesses memory at or above the upper bound}} \
                    // expected-note {{(expanded) inferred bounds are 'bounds(buf, buf + 10)'}}
}

extern void f24() {
  array_ptr<char> buf  _Count(3) = "abc";

  // The declared bounds of h7() use the value of buf, but buf is overwritten
  // in the assignment. The value of buf (used in the declared bounds (buf, buf + 3))
  // is lost, so the inferred bounds for the cast expression are unknown.
  buf = _Dynamic_bounds_cast<array_ptr<char>>(h7(), bounds(buf, buf + 3)); // expected-error {{inferred bounds for 'buf' are unknown after assignment}}

  // The declared bounds of h7() do not use the value of buf, so the bounds of the
  // cast expression are not invalidated.
  buf = _Dynamic_bounds_cast<array_ptr<char>>(h7(), count(3));

  char c = buf[3]; // expected-error {{out-of-bounds memory access}} \
                   // expected-note {{accesses memory at or above the upper bound}} \
                   // expected-note {{(expanded) inferred bounds are 'bounds(buf, buf + 3)'}}
}

extern void f25(array_ptr<char> buf  _Count(len), int len) {
  int* _Array intbuf  _Count(6) = _Dynamic_bounds_cast<int* _Array>(buf + 5, count(6));
  int i = intbuf[6]; // expected-error {{out-of-bounds memory access}} \
                     // expected-note {{accesses memory at or above the upper bound}} \
                     // expected-note {{(expanded) inferred bounds are 'bounds(intbuf, intbuf + 6)'}}
}

