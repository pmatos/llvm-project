// RUN: %clang_cc1 -triple wasm32 -target-feature +reference-types -o - -emit-llvm %s | FileCheck %s

typedef __funcref_t funcref_t;

funcref_t get_null() {
// CHECK:      define i8 addrspace(20)* @get_null() #0 {
// CHECK-NEXT: entry:
// CHECK-NEXT:   %0 = call i8 addrspace(20)* @llvm.wasm.ref.null.func()
// CHECK-NEXT:   ret i8 addrspace(20)* %0
// CHECK-NEXT: }
  return __builtin_wasm_ref_null_func();
}

void helper(funcref_t);

void handle(funcref_t fn) {
// CHECK:      define void @handle(i8 addrspace(20)* %fn) #0 {
// CHECK-NEXT: entry:
// CHECK-NEXT:   %fn.addr = alloca i8 addrspace(20)*, align 1
// CHECK-NEXT:   store i8 addrspace(20)* %fn, i8 addrspace(20)** %fn.addr, align 1
// CHECK-NEXT:   %0 = load i8 addrspace(20)*, i8 addrspace(20)** %fn.addr, align 1
// CHECK-NEXT:   call void @helper(i8 addrspace(20)* %0)
// CHECK-NEXT:   ret void
// CHECK-NEXT: }
  helper(fn);
}

typedef int (*fn_t)(int);

// What happens when we move a function pointer into a funcref and then call it?
funcref_t get_ref(fn_t fnptr) {
  return (funcref_t)fnptr;
}

int call_fn(funcref_t ref, int x) {
  fn_t fnptr = (fn_t) ref;
  return fnptr(x);
}
