// RUN: %clang_cc1 -triple wasm32-unknown-unknown -target-feature +reference-types -o - -emit-llvm %s | FileCheck %s

typedef __externref_t externref_t;

externref_t get_null() {
// CHECK:      define {} addrspace(10)* @get_null() #0 {
// CHECK-NEXT: entry:
// CHECK-NEXT:   %0 = call {} addrspace(10)* @llvm.wasm.ref.null.extern()
// CHECK-NEXT:   ret {} addrspace(10)* %0
// CHECK-NEXT: }
  return __builtin_wasm_ref_null_extern();
}

void helper(externref_t);

void handle(externref_t obj) {
// CHECK:      define void @handle({} addrspace(10)* %obj) #0 {
// CHECK-NEXT: entry:
// CHECK-NEXT:   %obj.addr = alloca {} addrspace(10)*, align 1
// CHECK-NEXT:   store {} addrspace(10)* %obj, {} addrspace(10)** %obj.addr, align 1
// CHECK-NEXT:   %0 = load {} addrspace(10)*, {} addrspace(10)** %obj.addr, align 1
// CHECK-NEXT:   call void @helper({} addrspace(10)* %0)
// CHECK-NEXT:   ret void
// CHECK-NEXT: }
  helper(obj);
}
