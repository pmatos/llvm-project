// RUN: %clang_cc1 -triple wasm32-unknown-unknown %s -emit-llvm -disable-llvm-passes -o - | FileCheck %s

typedef int32_t * i32_table_t __attribute__((wasm_table));

