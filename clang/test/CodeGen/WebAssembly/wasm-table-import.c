// RUN: %clang_cc1 -triple wasm32 -target-feature +reference-types -disable-O0-optnone -emit-llvm %s -o - | opt -S -passes=mem2reg | FileCheck %s
// REQUIRES: webassembly-registered-target

// Table import
extern __externref_t my_table[0] __attribute__((import_module("my_env"), import_name("mt")));

// CHECK: .tabletype my_table, externref
// CHECK: .import_module my_table, my_env
// CHECK: .import_name my_table, mt