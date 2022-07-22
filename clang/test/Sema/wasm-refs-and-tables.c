// RUN: %clang_cc1 -fsyntax-only -verify -triple wasm32 -Wno-unused-value -target-feature +reference-types %s

// Note: As WebAssembly references are sizeless types, we don't exhaustively
// test for cases covered by sizeless-1.c and similar tests.

// Unlike standard sizeless types, reftype globals are supported.
__externref_t r1;
extern __externref_t r2;
static __externref_t r3;

__externref_t *t1;               // expected-error {{pointers to WebAssembly reference types are illegal}}
__externref_t **t2;              // expected-error {{pointers to WebAssembly reference types are illegal}}
__externref_t ******t3;          // expected-error {{pointers to WebAssembly reference types are illegal}}
static __externref_t t4[3];      // expected-error {{only zero-length WebAssembly tables are currently supported}}
static __externref_t t5[];       // expected-error {{only zero-length WebAssembly tables are currently supported}}
static __externref_t t6[] = {0}; // expected-error {{only zero-length WebAssembly tables are currently supported}}
__externref_t t7[0];             // expected-error {{WebAssembly table must be static}}
static __externref_t t8[0][0];   // expected-error {{multi-dimensional arrays of WebAssembly references are illegal}}
static __externref_t (*t9)[0];   // expected-error {{pointers to WebAssembly tables are illegal}}

static __externref_t table[0];
static __externref_t other_table[0] = {};

struct s {
  __externref_t f1;       // expected-error {{field has sizeless type '__externref_t'}}
  __externref_t f2[0];    // expected-error {{field has sizeless type '__externref_t'}}
  __externref_t f3[];     // expected-error {{field has sizeless type '__externref_t'}}
  __externref_t f4[0][0]; // expected-error {{multi-dimensional arrays of WebAssembly references are illegal}}
  __externref_t *f5;      // expected-error {{pointers to WebAssembly reference types are illegal}}
  __externref_t ****f6;   // expected-error {{pointers to WebAssembly reference types are illegal}}
  __externref_t (*f7)[0]; // expected-error {{pointers to WebAssembly tables are illegal}}
};

union u {
  __externref_t f1;       // expected-error {{field has sizeless type '__externref_t'}}
  __externref_t f2[0];    // expected-error {{field has sizeless type '__externref_t'}}
  __externref_t f3[];     // expected-error {{field has sizeless type '__externref_t'}}
  __externref_t f4[0][0]; // expected-error {{multi-dimensional arrays of WebAssembly references are illegal}}
  __externref_t *f5;      // expected-error {{pointers to WebAssembly reference types are illegal}}
  __externref_t ****f6;   // expected-error {{pointers to WebAssembly reference types are illegal}}
  __externref_t (*f7)[0]; // expected-error {{pointers to WebAssembly tables are illegal}}
};

void illegal_argument_1(__externref_t table[]);     // expected-error {{cannot use WebAssembly table as a function argument}}
void illegal_argument_2(__externref_t table[0][0]); // expected-error {{multi-dimensional arrays of WebAssembly references are illegal}}
void illegal_argument_3(__externref_t *table);      // expected-error {{pointers to WebAssembly reference types are illegal}}
void illegal_argument_4(__externref_t ***table);    // expected-error {{pointers to WebAssembly reference types are illegal}}
void illegal_argument_5(__externref_t (*table)[0]); // expected-error {{pointers to WebAssembly tables are illegal}}

__externref_t *illegal_return_1();   // expected-error {{pointers to WebAssembly reference types are illegal}}
__externref_t ***illegal_return_2(); // expected-error {{pointers to WebAssembly reference types are illegal}}
__externref_t (*illegal_return_3())[0]; // expected-error {{pointers to WebAssembly tables are illegal}}

void varargs(int, ...);

__externref_t func(__externref_t ref) {
  &ref; // expected-error {{cannot take address of WebAssembly reference}}
  int foo = 40;
  (__externref_t *)(&foo);     // expected-error {{pointers to WebAssembly reference types are illegal}}
  (__externref_t ****)(&foo);  // expected-error {{pointers to WebAssembly reference types are illegal}}
  sizeof(ref);                 // expected-error {{invalid application of 'sizeof' to sizeless type '__externref_t'}}
  sizeof(__externref_t);       // expected-error {{invalid application of 'sizeof' to sizeless type '__externref_t'}}
  sizeof(__externref_t[0]);    // expected-error {{invalid application of 'sizeof' to WebAssembly table}}
  sizeof(table);               // expected-error {{invalid application of 'sizeof' to WebAssembly table}}
  sizeof(__externref_t[0][0]); // expected-error {{multi-dimensional arrays of WebAssembly references are illegal}}
  sizeof(__externref_t *);     // expected-error {{pointers to WebAssembly reference types are illegal}}
  sizeof(__externref_t ***);   // expected-error {{pointers to WebAssembly reference types are illegal}};
  // expected-warning@+1 {{'_Alignof' applied to an expression is a GNU extension}}
  _Alignof(ref);                 // expected-error {{invalid application of 'alignof' to sizeless type '__externref_t'}}
  _Alignof(__externref_t);       // expected-error {{invalid application of 'alignof' to sizeless type '__externref_t'}}
  _Alignof(__externref_t[]);     // expected-error {{invalid application of 'alignof' to sizeless type '__externref_t'}}
  _Alignof(__externref_t[0]);    // expected-error {{invalid application of 'alignof' to sizeless type '__externref_t'}}
  _Alignof(table);               // expected-warning {{'_Alignof' applied to an expression is a GNU extension}} expected-error {{invalid application of 'alignof' to WebAssembly table}}
  _Alignof(__externref_t[0][0]); // expected-error {{multi-dimensional arrays of WebAssembly references are illegal}}
  _Alignof(__externref_t *);     // expected-error {{pointers to WebAssembly reference types are illegal}}
  _Alignof(__externref_t ***);   // expected-error {{pointers to WebAssembly reference types are illegal}};
  varargs(1, ref);               // expected-error {{cannot pass expression of type '__externref_t' to variadic function}}

  __externref_t lt1[0];           // expected-error {{WebAssembly table cannot be declared within a function}}
  static __externref_t lt2[0];    // expected-error {{WebAssembly table cannot be declared within a function}}
  static __externref_t lt3[0][0]; // expected-error {{multi-dimensional arrays of WebAssembly references are illegal}}
  static __externref_t(*lt4)[0];  // expected-error {{pointers to WebAssembly tables are illegal}}
  illegal_argument_1(table);      // expected-error {{cannot use WebAssembly table as a function argument}}
  varargs(1, table);              // expected-error {{cannot use WebAssembly table as a function argument}}
  table == 1;                     // expected-error {{invalid operands to binary expression ('__attribute__((address_space(1))) __externref_t[0]' and 'int')}}
  1 >= table;                     // expected-error {{invalid operands to binary expression ('int' and '__attribute__((address_space(1))) __externref_t[0]')}}
  !table;                         // expected-error {{invalid argument type '__attribute__((address_space(1))) __externref_t *' to unary expression}}
  1 && table;                     // expected-error {{invalid operands to binary expression ('int' and '__attribute__((address_space(1))) __externref_t[0]')}}
  table || 1;                     // expected-error {{invalid operands to binary expression ('__attribute__((address_space(1))) __externref_t[0]' and 'int')}}
  1 ? table : table;              // expected-error {{cannot use WebAssembly tables as the 2nd or 3rd operands of a conditional expression}}
  (void *)table;                  // expected-error {{cannot cast WebAssembly table}}
  void *u;
  u = table;       // expected-error {{cannot assign WebAssembly table to a variable}}
  void *v = table; // expected-error {{cannot assign WebAssembly table to a variable}}
  &table;          // expected-error {{cannot take address of WebAssembly table}}

  table[0];
  table[0] = ref;
  return ref;
}

void *ret_void_ptr() {
  return table; // expected-error {{cannot return a WebAssembly table}}
}
