// RUN: %clang_cc1 -triple wasm32 -target-feature +reference-types -o - -emit-llvm %s | FileCheck %s
// REQUIRES: webassembly-registered-target

// CHECK: @table = internal addrspace(1) global [0 x ptr addrspace(10)] zeroinitializer, align 1
static __externref_t table[0];

void use() {
  // Ensure the table isn't discarded as unused.
  table[0];
}
