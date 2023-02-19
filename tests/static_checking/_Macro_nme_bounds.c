// Tests for Non-Modifying Expressions with Checked C Bounds expressions
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang_cc1 -verify -verify-ignore-unexpected=note %s

#include <stdchecked.h>

int f0int(void);
int* f0ptr(void);

struct S1 {
  int m1;
  int* m2;
};

union U1 {
  int m1;
  int* m2;
};

// Expressions explicitly banned by spec within Non-Modifying Expressions
void f1(int i, int* loc) {
  volatile int j;

  int* _Array as0a  _Count(i = 1) = 0;         // expected-error {{assignment expression not allowed in count expression}}
  int* _Array as0b  _Byte_count(i = 1) = 0;    // expected-error {{assignment expression not allowed in byte count expression}}
  int* _Array as0c  _Bounds(loc = 0, loc) = 0; // expected-error {{assignment expression not allowed in bounds expression}}
  int* _Array as0d  _Bounds(loc, loc = 0) = 0; // expected-error {{assignment expression not allowed in bounds expression}}

  int* _Array as1a  _Count(i += 1) = 0;         // expected-error {{assignment expression not allowed in count expression}}
  int* _Array as1b  _Byte_count(i += 1) = 0;    // expected-error {{assignment expression not allowed in byte count expression}}
  int* _Array as1c  _Bounds(loc += 1, loc) = 0; // expected-error {{assignment expression not allowed in bounds expression}}
  int* _Array as1d  _Bounds(loc, loc += 1) = 0; // expected-error {{assignment expression not allowed in bounds expression}}

  int* _Array as2a  _Count(i -= 1) = 0;         // expected-error {{assignment expression not allowed in count expression}}
  int* _Array as2b  _Byte_count(i -= 1) = 0;    // expected-error {{assignment expression not allowed in byte count expression}}
  int* _Array as2c  _Bounds(loc -= 1, loc) = 0; // expected-error {{assignment expression not allowed in bounds expression}}
  int* _Array as2d  _Bounds(loc, loc -= 1) = 0; // expected-error {{assignment expression not allowed in bounds expression}}

  int* _Array as3a  _Count(i *= 1) = 0;         // expected-error {{assignment expression not allowed in count expression}}
  int* _Array as3b  _Byte_count(i *= 1) = 0;    // expected-error {{assignment expression not allowed in byte count expression}}
  int* _Array as3c  _Bounds(loc + (i *= 1), loc) = 0; // expected-error {{assignment expression not allowed in bounds expression}}
  int* _Array as3d  _Bounds(loc, loc + (i *= 1)) = 0; // expected-error {{assignment expression not allowed in bounds expression}}

  int* _Array as4a  _Count(i /= 1) = 0;         // expected-error {{assignment expression not allowed in count expression}}
  int* _Array as4b  _Byte_count(i /= 1) = 0;    // expected-error {{assignment expression not allowed in byte count expression}}
  int* _Array as4c  _Bounds(loc + (i /= 1), loc) = 0; // expected-error {{assignment expression not allowed in bounds expression}}
  int* _Array as4d  _Bounds(loc, loc + (i /= 1)) = 0; // expected-error {{assignment expression not allowed in bounds expression}}

  int* _Array as5a  _Count(i %= 1) = 0;         // expected-error {{assignment expression not allowed in count expression}}
  int* _Array as5b  _Byte_count(i %= 1) = 0;    // expected-error {{assignment expression not allowed in byte count expression}}
  int* _Array as5c  _Bounds(loc + (i %= 1), loc) = 0; // expected-error {{assignment expression not allowed in bounds expression}}
  int* _Array as5d  _Bounds(loc, loc + (i %= 1)) = 0; // expected-error {{assignment expression not allowed in bounds expression}}

  int* _Array as6a  _Count(i <<= 1) = 0;         // expected-error {{assignment expression not allowed in count expression}}
  int* _Array as6b  _Byte_count(i <<= 1) = 0;    // expected-error {{assignment expression not allowed in byte count expression}}
  int* _Array as6c  _Bounds(loc + (i <<= 1), loc) = 0; // expected-error {{assignment expression not allowed in bounds expression}}
  int* _Array as6d  _Bounds(loc, loc + (i <<= 1)) = 0; // expected-error {{assignment expression not allowed in bounds expression}}

  int* _Array as7a  _Count(i >>= 1) = 0;         // expected-error {{assignment expression not allowed in count expression}}
  int* _Array as7b  _Byte_count(i >>= 1) = 0;    // expected-error {{assignment expression not allowed in byte count expression}}
  int* _Array as7c  _Bounds(loc + (i >>= 1), loc) = 0; // expected-error {{assignment expression not allowed in bounds expression}}
  int* _Array as7d  _Bounds(loc, loc + (i >>= 1)) = 0; // expected-error {{assignment expression not allowed in bounds expression}}

  int* _Array as8a  _Count(i &= 1) = 0;         // expected-error {{assignment expression not allowed in count expression}}
  int* _Array as8b  _Byte_count(i &= 1) = 0;    // expected-error {{assignment expression not allowed in byte count expression}}
  int* _Array as8c  _Bounds(loc + (i &= 1), loc) = 0; // expected-error {{assignment expression not allowed in bounds expression}}
  int* _Array as8d  _Bounds(loc, loc + (i &= 1)) = 0; // expected-error {{assignment expression not allowed in bounds expression}}

  int* _Array as9a  _Count(i ^= 1) = 0;         // expected-error {{assignment expression not allowed in count expression}}
  int* _Array as9b  _Byte_count(i ^= 1) = 0;    // expected-error {{assignment expression not allowed in byte count expression}}
  int* _Array as9c  _Bounds(loc + (i ^= 1), loc) = 0; // expected-error {{assignment expression not allowed in bounds expression}}
  int* _Array as9d  _Bounds(loc, loc + (i ^= 1)) = 0; // expected-error {{assignment expression not allowed in bounds expression}}

  int* _Array as10a  _Count(i |= 1) = 0;         // expected-error {{assignment expression not allowed in count expression}}
  int* _Array as10b  _Byte_count(i |= 1) = 0;    // expected-error {{assignment expression not allowed in byte count expression}}
  int* _Array as10c  _Bounds(loc + (i |= 1), loc) = 0; // expected-error {{assignment expression not allowed in bounds expression}}
  int* _Array as10d  _Bounds(loc, loc + (i|= 1)) = 0; // expected-error {{assignment expression not allowed in bounds expression}}

  int* _Array as11a  _Count(i++) = 0;         // expected-error {{increment expression not allowed in count expression}}
  int* _Array as11b  _Byte_count(i++) = 0;    // expected-error {{increment expression not allowed in byte count expression}}
  int* _Array as11c  _Bounds(loc++, loc) = 0; // expected-error {{increment expression not allowed in bounds expression}}
  int* _Array as11d  _Bounds(loc, loc++) = 0; // expected-error {{increment expression not allowed in bounds expression}}

  int* _Array as12a  _Count(++i) = 0;         // expected-error {{increment expression not allowed in count expression}}
  int* _Array as12b  _Byte_count(++i) = 0;    // expected-error {{increment expression not allowed in byte count expression}}
  int* _Array as12c  _Bounds(++loc, loc) = 0; // expected-error {{increment expression not allowed in bounds expression}}
  int* _Array as12d  _Bounds(loc, ++loc) = 0; // expected-error {{increment expression not allowed in bounds expression}}

  int* _Array as13a  _Count(i--) = 0;         // expected-error {{decrement expression not allowed in count expression}}
  int* _Array as13b  _Byte_count(i--) = 0;    // expected-error {{decrement expression not allowed in byte count expression}}
  int* _Array as13c  _Bounds(loc--, loc) = 0; // expected-error {{decrement expression not allowed in bounds expression}}
  int* _Array as13d  _Bounds(loc, loc--) = 0; // expected-error {{decrement expression not allowed in bounds expression}}

  int* _Array as14a  _Count(--i) = 0;         // expected-error {{decrement expression not allowed in count expression}}
  int* _Array as14b  _Byte_count(--i) = 0;    // expected-error {{decrement expression not allowed in byte count expression}}
  int* _Array as14c  _Bounds(--loc, loc) = 0; // expected-error {{decrement expression not allowed in bounds expression}}
  int* _Array as14d  _Bounds(loc, --loc) = 0; // expected-error {{decrement expression not allowed in bounds expression}}

  int* _Array as15a  _Count(f0int()) = 0;         // expected-error {{call expression not allowed in count expression}}
  int* _Array as15b  _Byte_count(f0int()) = 0;    // expected-error {{call expression not allowed in byte count expression}}
  int* _Array as15c  _Bounds(f0ptr(), loc) = 0; // expected-error {{call expression not allowed in bounds expression}}
  int* _Array as15d  _Bounds(loc, f0ptr()) = 0; // expected-error {{call expression not allowed in bounds expression}}

  int* _Array as16a  _Count(f0int() + 1) = 0;         // expected-error {{call expression not allowed in count expression}}
  int* _Array as16b  _Byte_count(f0int() + 1) = 0;    // expected-error {{call expression not allowed in byte count expression}}
  int* _Array as16c  _Bounds(f0ptr() + 1, loc) = 0; // expected-error {{call expression not allowed in bounds expression}}
  int* _Array as16d  _Bounds(loc, f0ptr() + 1) = 0; // expected-error {{call expression not allowed in bounds expression}}

  int* _Array as17a  _Count(j) = 0;         // expected-error {{volatile expression not allowed in count expression}}
  int* _Array as17b  _Byte_count(j) = 0;    // expected-error {{volatile expression not allowed in byte count expression}}
  int* _Array as17c  _Bounds(loc + j, loc) = 0; // expected-error {{volatile expression not allowed in bounds expression}}
  int* _Array as17d  _Bounds(loc, loc + j) = 0; // expected-error {{volatile expression not allowed in bounds expression}}

  int* _Array as18a  _Count(i + j) = 0;         // expected-error {{volatile expression not allowed in count expression}}
  int* _Array as18b  _Byte_count(i + j) = 0;    // expected-error {{volatile expression not allowed in byte count expression}}
  int* _Array as18c  _Bounds(loc + (i + j), loc) = 0; // expected-error {{volatile expression not allowed in bounds expression}}
  int* _Array as18d  _Bounds(loc, loc + (i + j)) = 0; // expected-error {{volatile expression not allowed in bounds expression}}
}

// Expressions explicitly allowed by spec within Non-Modifying Expressions
void f2(int i, int* loc) {
  int j;
  int* loc2;
  int* pi;
  int** ploc;
  union U1 u1;
  struct S1 s1;
  struct S1 *ps1;

  int* _Array as0a  _Count(i) = 0;
  int* _Array as0b  _Byte_count(i) = 0;
  int* _Array as0c  _Bounds(loc, loc) = 0;
  int* _Array as0d  _Bounds(loc, loc) = 0;

  int* _Array as1a  _Count(j) = 0;
  int* _Array as1b  _Byte_count(j) = 0;
  int* _Array as1c  _Bounds(loc2, loc) = 0;
  int* _Array as1d  _Bounds(loc, loc2) = 0;

  int* _Array as2a  _Count(i + j) = 0;
  int* _Array as2b  _Byte_count(i + j) = 0;
  int* _Array as2c  _Bounds(loc + j, loc) = 0;
  int* _Array as2d  _Bounds(loc, loc + j) = 0;

  int* _Array as3a  _Count(0) = 0;
  int* _Array as3b  _Byte_count(0) = 0;

  int* _Array as4a  _Count(&i == &j) = 0;
  int* _Array as4b  _Byte_count(&i == &j) = 0;
  int* _Array as4c  _Bounds(&i, loc) = 0;
  int* _Array as4d  _Bounds(loc, &i) = 0;

  int* _Array as5a  _Count(*pi) = 0;
  int* _Array as5b  _Byte_count(*pi) = 0;
  int* _Array as5c  _Bounds(*ploc, loc) = 0;
  int* _Array as5d  _Bounds(loc, *ploc) = 0;

  int* _Array as6a  _Count((int)'a') = 0;
  int* _Array as6b  _Byte_count((int)'a') = 0;
  int* _Array as6c  _Bounds((int*)0, loc) = 0;
  int* _Array as6d  _Bounds(loc, (int*)0) = 0;

  int* _Array as7a  _Count(+i) = 0;
  int* _Array as7b  _Byte_count(+i) = 0;
  int* _Array as7c  _Bounds(loc + (+i), loc) = 0;
  int* _Array as7d  _Bounds(loc, loc + (+i)) = 0;

  int* _Array as8a  _Count(-i) = 0;
  int* _Array as8b  _Byte_count(-i) = 0;
  int* _Array as8c  _Bounds(loc + -i, loc) = 0;
  int* _Array as8d  _Bounds(loc, loc + -i) = 0;

  int* _Array as9a  _Count(~i) = 0;
  int* _Array as9b  _Byte_count(~i) = 0;
  int* _Array as9c  _Bounds(loc + ~i, loc) = 0;
  int* _Array as9d  _Bounds(loc, loc + ~i) = 0;

  int* _Array as10a  _Count(!i) = 0;
  int* _Array as10b  _Byte_count(!i) = 0;
  int* _Array as10c  _Bounds(loc + !i, loc) = 0;
  int* _Array as10d  _Bounds(loc, loc + !i) = 0;

  int* _Array as11a  _Count(sizeof(i)) = 0;
  int* _Array as11b  _Byte_count(sizeof(i)) = 0;
  int* _Array as11c  _Bounds(loc + sizeof(loc), loc) = 0;
  int* _Array as11d  _Bounds(loc, loc + sizeof(loc)) = 0;

  int* _Array as12a  _Count(i * 1 / 1 % 1) = 0;
  int* _Array as12b  _Byte_count(i * 1 / 1 % 1) = 0;
  int* _Array as12c  _Bounds(loc + (i * 1 / 1 % 1), loc) = 0;
  int* _Array as12d  _Bounds(loc, loc + (i * 1 / 1 % 1)) = 0;

  int* _Array as13a  _Count(i + 1 - 1) = 0;
  int* _Array as13b  _Byte_count(i + 1 - 1) = 0;
  int* _Array as13c  _Bounds(loc + 1 - 1, loc) = 0;
  int* _Array as13d  _Bounds(loc, loc + 1 - 1) = 0;

  int* _Array as14a  _Count(i << 1 >> 1) = 0;
  int* _Array as14b  _Byte_count(i << 1 >> 1) = 0;
  int* _Array as14c  _Bounds(loc + (i << 1 >> 1), loc) = 0;
  int* _Array as14d  _Bounds(loc, loc + (i << 1 >> 1)) = 0;

  int* _Array as15a  _Count(i < j) = 0;
  int* _Array as15b  _Byte_count(i < j) = 0;

  int* _Array as16a  _Count(i > j) = 0;
  int* _Array as16b  _Byte_count(i > j) = 0;

  int* _Array as17a  _Count(i <= j) = 0;
  int* _Array as17b  _Byte_count(i <= j) = 0;

  int* _Array as18a  _Count(i >= j) = 0;
  int* _Array as18b  _Byte_count(i >= j) = 0;

  int* _Array as19a  _Count(i == j) = 0;
  int* _Array as19b  _Byte_count(i == j) = 0;
  int* _Array as19c  _Bounds(loc + (loc == loc2), loc) = 0;
  int* _Array as19d  _Bounds(loc, loc + (loc == loc2)) = 0;

  int* _Array as20a  _Count(i & j) = 0;
  int* _Array as20b  _Byte_count(i & j) = 0;

  int* _Array as21a  _Count(i ^ j) = 0;
  int* _Array as21b  _Byte_count(i ^ j) = 0;

  int* _Array as22a  _Count(i | j) = 0;
  int* _Array as22b  _Byte_count(i | j) = 0;

  int* _Array as23a  _Count(i && j) = 0;
  int* _Array as23b  _Byte_count(i && j) = 0;

  int* _Array as24a  _Count(i || j) = 0;
  int* _Array as24b  _Byte_count(i || j) = 0;

  int* _Array as25a  _Count(i ? j : 0) = 0;
  int* _Array as25b  _Byte_count(i ? j : 0) = 0;
  int* _Array as25c  _Bounds(i ? loc : loc2, loc) = 0;
  int* _Array as25d  _Bounds(loc, i ? loc : loc2) = 0;

  int* _Array as26a  _Count(u1.m1) = 0;
  int* _Array as26b  _Byte_count(u1.m1) = 0;
  int* _Array as26c  _Bounds(u1.m2, loc) = 0;
  int* _Array as26d  _Bounds(loc, u1.m2) = 0;

  int* _Array as27a  _Count(s1.m1) = 0;
  int* _Array as27b  _Byte_count(s1.m1) = 0;
  int* _Array as27c  _Bounds(s1.m2, loc) = 0;
  int* _Array as27d  _Bounds(loc, s1.m2) = 0;

  int* _Array as28a  _Count(ps1->m1) = 0;
  int* _Array as28b  _Byte_count(ps1->m1) = 0;
  int* _Array as28c  _Bounds(ps1->m2, loc) = 0;
  int* _Array as28d  _Bounds(loc, ps1->m2) = 0;
}


// Expressions that result in inferred bounds containing modifying expressions.

struct S2 {
  int* _Array p  _Count(5);
};

void f3_helper(int* _Array p  _Count(5));

void f3(void) {
  struct S2 arr[10] = { 0 };
  int i = 0;
  // TODO: this will eventually be allowed when we add support for current_expr_value.
  (arr[i++]).p = 0;                         // expected-error {{contain a modifying expression}}
  // TODO: this will eventually be allowed when we add support for current_expr_value.
  int* _Array a  _Count(5) = arr[i++].p; // expected-error {{contain a modifying expression}}
  // TODO: this will eventually be allowed when we add support for current_expr_value.
  f3_helper(arr[i++].p);                    // expected-error {{contain a modifying expression}}
}

struct S3 {
  int* _Array p  _Count(len);
  int len;
};

void f4(void) {
  struct S3 arr[10] = { 0 };
  int i = 0;
  int j = arr[i++].p[0];   // expected-error {{contain a modifying expression}}
                           // There is no way to write down the bounds for arr[i++].p.  The bounds of
                           // arr[i++].p need to refer to a temporary holding the value of the subexpression
                           // arr[i++].  We have no way of describing this.
}

void f4_helper(int* _Array p  _Count(1));

void f5(void) {
  int i = 0;
  int* _Single p = &i;
  int* _Single *_Array r  _Count(1) = &p;
  int* _Single *_Array s  _Count(1) = &p;
// TODO: this will eventually be allowed when we add support for current_expr_value.
  int*_Array t  _Count(1) = *(r = s);  // expected-error {{contain a modifying expression}}
// TODO: this will eventually be allowed when we add support for current_expr_value.
  f4_helper(*(r = s));     // expected-error {{contain a modifying expression}}
}
