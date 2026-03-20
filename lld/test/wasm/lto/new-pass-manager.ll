; RUN: llvm-as -o %t.bc %s
; RUN: wasm-ld --lto-debug-pass-manager -o /dev/null %t.bc 2>&1 | FileCheck %s

; CHECK: Running pass: GlobalOptPass

target datalayout = "e-m:e-p:32:32-i64:64-n32:64-S128-ni:1"
target triple = "wasm32-unknown-unknown"

define void @_start() local_unnamed_addr {
entry:
  ret void
}
