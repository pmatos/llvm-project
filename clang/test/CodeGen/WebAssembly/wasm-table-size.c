// RUN: %clang_cc1 -triple wasm32 -target-feature +reference-types -o - -emit-llvm %s | FileCheck %s

// CHECK: @tb = internal addrspace(1) global [0 x {} addrspace(10)*] zeroinitializer, align 1

typedef __externref_t externref_t;
__attribute__((address_space(1))) static externref_t tb[0];

int get_tb_size() {
// CHECK:      define i32 @get_tb_size() #0 {
// CHECK-NEXT: entry:
// CHECK-NEXT:   %0 = call i32 @llvm.wasm.table.size(i8 addrspace(1)* bitcast ([0 x {} addrspace(10)*] addrspace(1)* @tb to i8 addrspace(1)*))
// CHECK-NEXT:   ret i32 %0
// CHECK-NEXT: }
  return __builtin_wasm_table_size(tb);
}
