// Tests for calling functions declared in checkedc_extensions.h
//
// The following lines are for the LLVM test harness:
//
// RUN: %clang -o %t.exe %s %checkedc_target_flags
// RUN: %checkedc_rununder %t.exe

// expected-no-diagnostics

#pragma CHECKED_SCOPE on

#include <checkedc_extensions.h>
#include <stddef.h>
#include <stdio.h>

// A function with an interface similar to snprintf but without the variable
// number of arguments. The purpose is to test such a call interface in checked
// scope.
void iface(char * restrict s  _Itype( char* _Nt_array restrict) count(n-1),
           size_t n _Where_M( n > 0),
           const char * restrict src  _Itype( const char* _Nt_array restrict)) {
  return;
}


void iface_array_ptr(char * restrict s  _Itype( char* _Array restrict) count(n),
           size_t n,
           const char * restrict src  _Itype( const char* _Array restrict)) {
  // Enclosing the call to iface in an unchecked block to be able to pass an
  // argument of type char* _Array and bounds count(n) whose corresponding
  // parameter has the interface type char* _Nt_array and bounds count(n-1).
  // This avoids the compiler error: passing 'char* _Array restrict' to
  // parameter of incompatible type 'char* _Nt_array'.
  _Unchecked{ iface(s, n, src); }
}



void iface_test1(char* _Array p _Count(len), size_t len) {

  char buf _Checked[50];
  iface_array_ptr(buf, 50, "Hello world");
  iface_array_ptr(buf, 0, "Hello world");

  iface_array_ptr(p, len, "Hello world");
  iface_array_ptr(p, 0, "Hello world");
}

void iface_test2(char* _Nt_array p _Count(len), size_t len) {
  char buf _Nt_checked[50];
  iface(buf, 50, "Hello world");

  iface(p, len + 1, "Hello world");
}

void test3(char* _Array buf _Count(len), size_t len) {
  _Unchecked{ snprintf_array_ptr(buf, len, "Hello world - 3"); }
}

int main(void) {
  char buf _Checked[50];
  test3(buf, 50) ;
  char buf1 _Checked[50];
  test3(buf1, 50) ;
  int res = strncmp_array_ptr(buf, buf1, 50);
  _Unchecked {printf("res: %d  buf: %s  buf1: %s\n", res, buf, buf1);}
}
