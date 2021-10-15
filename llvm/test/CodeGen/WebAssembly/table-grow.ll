; RUN: llc --mtriple=wasm32-unknown-unknown -asm-verbose=false -mattr=+reference-types < %s | FileCheck %s

%extern = type opaque
%externref = type %extern addrspace(10)* ;; addrspace 10 is nonintegral

@externref_table = local_unnamed_addr addrspace(1) global [0 x %externref] undef

declare i32 @llvm.wasm.table.grow.externref([0 x %externref], i32, %externref) nounwind

define void @table_grow(i32 sz) {
  call i32 @llvm.wasm.table.grow.externref()
  ret void
}
