// RUN: %clang_cc1 -fcxx-exceptions -fsyntax-only -verify -std=gnu++11 -triple wasm32 -Wno-unused-value -target-feature +reference-types %s

// This file tests C++ specific constructs with WebAssembly references and
// tables. See wasm-refs-and-tables.c for C constructs.

__externref_t ref;
__externref_t &ref_ref1 = ref; // expected-error {{references to WebAssembly reference types are illegal}}
__externref_t &ref_ref2(ref);  // expected-error {{references to WebAssembly reference types are illegal}}

static __externref_t table[0];                    // expected-note 2 {{'table' declared here}}
static __externref_t (&ref_to_table1)[0] = table; // expected-error {{references to WebAssembly tables are illegal}}
static __externref_t (&ref_to_table2)[0](table);  // expected-error {{references to WebAssembly tables are illegal}}

void illegal_argument_1(__externref_t &r); // expected-error {{references to WebAssembly reference types are illegal}}
void illegal_argument_2(__externref_t (&t)[0]); // expected-error {{references to WebAssembly tables are illegal}}

__externref_t &illegal_return_1(); // expected-error {{references to WebAssembly reference types are illegal}}
__externref_t (&illegal_return_2())[0]; // expected-error {{references to WebAssembly tables are illegal}}

void illegal_throw1() throw(__externref_t);   // expected-error {{sizeless type '__externref_t' is not allowed in exception specification}}
void illegal_throw2() throw(__externref_t *); // expected-error {{pointers to WebAssembly reference types are illegal}}
void illegal_throw3() throw(__externref_t &); // expected-error {{references to WebAssembly reference types are illegal}}
void illegal_throw4() throw(__externref_t[0]); // expected-error {{WebAssembly table not allowed in exception sepcification}}

class RefClass {
  __externref_t f1;       // expected-error {{field has sizeless type '__externref_t'}}
  __externref_t f2[0];    // expected-error {{field has sizeless type '__externref_t'}}
  __externref_t f3[];     // expected-error {{field has sizeless type '__externref_t'}}
  __externref_t f4[0][0]; // expected-error {{multi-dimensional arrays of WebAssembly references are illegal}}
  __externref_t *f5;      // expected-error {{pointers to WebAssembly reference types are illegal}}
  __externref_t ****f6;   // expected-error {{pointers to WebAssembly reference types are illegal}}
  __externref_t (*f7)[0]; // expected-error {{pointers to WebAssembly tables are illegal}}
};

struct AStruct {};

template <typename T>
struct TemplatedStruct {
  T f;         // expected-error 2 {{field has sizeless type '__externref_t'}}
  void foo(T); // expected-error {{cannot use WebAssembly table as a function argument}}
  T bar(void); // expected-error {{function cannot return array type '__externref_t[0]'}}
  T arr[0];    // expected-error {{field has sizeless type '__externref_t'}} expected-error {{multi-dimensional arrays of WebAssembly references are illegal}}
  T *ptr;      // expected-error {{pointers to WebAssembly reference types are illegal}} expected-error {{pointers to WebAssembly tables are illegal}}
};

void func() {
  int foo = 40;
  static_cast<__externref_t>(foo);      // expected-error {{static_cast from 'int' to '__externref_t' is not allowed}}
  static_cast<__externref_t *>(&foo);   // expected-error {{pointers to WebAssembly reference types are illegal}}
  static_cast<int>(ref);                // expected-error {{static_cast from '__externref_t' to 'int' is not allowed}}
  __externref_t(10);                    // expected-error {{functional-style cast from 'int' to '__externref_t' is not allowed}}
  int i(ref);                           // expected-error {{cannot initialize a variable of type 'int' with an lvalue of type '__externref_t'}}
  int j(table);                         // expected-error {{cannot initialize a variable of type 'int' with an lvalue of type '__attribute__((address_space(1))) __externref_t[0]'}}
  const_cast<__externref_t[0]>(table);  // expected-error {{const_cast to '__externref_t[0]', which is not a reference, pointer-to-object, or pointer-to-data-member}}
  const_cast<__externref_t *>(table);   // expected-error {{pointers to WebAssembly reference types are illegal}}
  reinterpret_cast<__externref_t>(foo); // expected-error {{reinterpret_cast from 'int' to '__externref_t' is not allowed}}
  reinterpret_cast<int>(ref);           // expected-error {{reinterpret_cast from '__externref_t' to 'int' is not allowed}}
  reinterpret_cast<int *>(table);       // expected-error {{reinterpret_cast from '__attribute__((address_space(1))) __externref_t *' to 'int *' is not allowed}}
  int iarr[0];
  reinterpret_cast<__externref_t[0]>(iarr); // expected-error {{reinterpret_cast from 'int *' to '__externref_t[0]' is not allowed}}
  reinterpret_cast<__externref_t *>(iarr);  // expected-error {{pointers to WebAssembly reference types are illegal}}
  dynamic_cast<__externref_t>(foo);         // expected-error {{invalid target type '__externref_t' for dynamic_cast; target type must be a reference or pointer type to a defined class}}
  dynamic_cast<__externref_t *>(&foo);      // expected-error {{pointers to WebAssembly reference types are illegal}}
  dynamic_cast<AStruct *>(table);           // expected-error {{'__externref_t' is not a class type}}

  for (auto x : table) { // expected-error {{cannot assign WebAssembly table to a variable}}
  }

  TemplatedStruct<__externref_t> ts1;    // expected-note {{in instantiation}}
  TemplatedStruct<__externref_t *> ts2;  // expected-error {{pointers to WebAssembly reference types are illegal}}
  TemplatedStruct<__externref_t &> ts3;  // expected-error {{references to WebAssembly reference types are illegal}}
  TemplatedStruct<__externref_t[0]> ts4; // expected-note {{in instantiation}}

  auto auto_table = table; // expected-error {{cannot assign WebAssembly table to a variable}}
  auto auto_ref = ref;

  auto fn1 = [](__externref_t x) { return x; };
  auto fn2 = [](__externref_t *x) { return x; };   // expected-error {{pointers to WebAssembly reference types are illegal}}
  auto fn3 = [](__externref_t &x) { return x; };   // expected-error {{references to WebAssembly reference types are illegal}}
  auto fn4 = [](__externref_t x[0]) { return x; }; // expected-error {{cannot use WebAssembly table as a function argument}}
  auto fn5 = [](void) { return table; };           // expected-error {{cannot return a WebAssembly table}}
  auto fn6 = [&auto_ref](void) { return true; };   // expected-error {{cannot capture WebAssembly reference}}
  auto fn7 = [auto_ref](void) { return true; };    // expected-error {{cannot capture WebAssembly reference}}
  auto fn8 = [&](void) { auto_ref; return true; };                        // expected-error {{cannot capture WebAssembly reference}}
  auto fn9 = [=](void) { auto_ref; return true; };                        // expected-error {{cannot capture WebAssembly reference}}
  auto fn10 = [table](void) { return true; };      // expected-error {{'table' cannot be captured because it does not have automatic storage duration}}
  auto fn11 = [&table](void) { return true; };     // expected-error {{'table' cannot be captured because it does not have automatic storage duration}}

  alignof(__externref_t);    // expected-error {{invalid application of 'alignof' to sizeless type '__externref_t'}}
  alignof(ref);              // expected-warning {{'alignof' applied to an expression is a GNU extension}} expected-error {{invalid application of 'alignof' to sizeless type '__externref_t'}}
  alignof(__externref_t[0]); // expected-error {{invalid application of 'alignof' to sizeless type '__externref_t'}}
  alignof(table);            // expected-warning {{'alignof' applied to an expression is a GNU extension}} expected-error {{invalid application of 'alignof' to WebAssembly table}}

  throw ref;  // expected-error {{cannot throw object of sizeless type '__externref_t'}}
  throw &ref; // expected-error {{cannot take address of WebAssembly reference}}
  throw table;  // expected-error {{cannot throw a WebAssembly table}}
  throw &table; // expected-error {{cannot take address of WebAssembly table}}

  try {
  } catch (__externref_t) { // expected-error {{cannot catch sizeless type '__externref_t'}}
  }
  try {
  } catch (__externref_t *) { // expected-error {{pointers to WebAssembly reference types are illegal}}
  }
  try {
  } catch (__externref_t &) { // expected-error {{references to WebAssembly reference types are illegal}}
  }
  try {
  } catch (__externref_t[0]) { // expected-error {{cannot catch WebAssembly table}}
  }

  new __externref_t;    // expected-error {{allocation of sizeless type '__externref_t'}}
  new __externref_t[0]; // expected-error {{allocation of sizeless type '__externref_t'}}

  delete ref;     // expected-error {{cannot delete expression of type '__externref_t'}}
  delete[] table; // expected-error {{annot delete expression of type '__attribute__((address_space(1))) __externref_t[0]'}}
}
