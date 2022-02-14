// RUN: %clang_cc1 -triple wasm32-unknown-unknown -target-feature +reference-types -o - -emit-llvm %s | FileCheck %s

// Assume these exist, to get a free slot into a global table.
typedef unsigned int Handle;
typedef unsigned int size_t;
extern int handles_available(void);
extern Handle acquire_handle(void);
extern void release_handle(Handle h);
extern void out_of_memory() __attribute__((noreturn));

typedef __externref_t externref_t;

// Declare and define a table.
static externref_t objects[0];

static void expand_table(void) {
  size_t old_size = __builtin_wasm_table_size(objects);
  size_t grow = (old_size >> 1) + 1;
  if (__builtin_wasm_table_grow(objects,
                                __builtin_wasm_ref_null_extern(),
                                grow) == -1) {
    out_of_memory();
    __builtin_trap();
  }
  size_t end = __builtin_wasm_table_size(objects);
  while (end != old_size) {
    release_handle(--end);
  }
}

static Handle intern(externref_t obj) {
  if (!handles_available()) expand_table();
  Handle ret = acquire_handle();
  objects[ret] = obj;
  return ret;
}

static externref_t handle_value(Handle h) {
  return h == -1
    ? __builtin_wasm_ref_null_extern()
    : objects[h];
}
