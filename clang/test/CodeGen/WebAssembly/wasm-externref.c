// RUN: %clang_cc1 -triple wasm32-unknown-unknown -target-feature +reference-types -o - -emit-llvm %s | FileCheck %s

typedef __externref_t externref_t;

externref_t get_null() {
  // CHECK:      define ptr addrspace(10) @get_null() #0 {
  // CHECK-NEXT: entry:
  // CHECK-NEXT:   %0 = call ptr addrspace(10) @llvm.wasm.ref.null.extern()
  // CHECK-NEXT:   ret ptr addrspace(10) %0
  // CHECK-NEXT: }
  return __builtin_wasm_ref_null_extern();
}

void helper(externref_t);

void handle(externref_t obj) {
  // CHECK:      define void @handle(ptr addrspace(10) %obj) #0 {
  // CHECK-NEXT: entry:
  // CHECK-NEXT:   %obj.addr = alloca ptr addrspace(10), align 1
  // CHECK-NEXT:   store ptr addrspace(10) %obj, ptr %obj.addr, align 1
  // CHECK-NEXT:   %0 = load ptr addrspace(10), ptr %obj.addr, align 1
  // CHECK-NEXT:   call void @helper(ptr addrspace(10) %0)
  // CHECK-NEXT:   ret void
  // CHECK-NEXT: }
  helper(obj);
}
