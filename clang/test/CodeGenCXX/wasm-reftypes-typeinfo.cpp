// REQUIRES: webassembly-registered-target
// RUN: %clang_cc1 %s -triple wasm32-unknown-unknown -target-feature +reference-types -emit-llvm -o - -std=c++11 | FileCheck %s
// RUN: %clang_cc1 %s -triple wasm64-unknown-unknown -target-feature +reference-types -emit-llvm -o - -std=c++11 | FileCheck %s

namespace std { class type_info; };

auto &externref = typeid(__externref_t);
auto &funcref = typeid(__funcref_t);

// CHECK-DAG: @_ZTS11externref_t = {{.*}} c"11externref_t\00"
// CHECK-DAG: @_ZTI11externref_t = {{.*}} @_ZTVN10__cxxabiv123__fundamental_type_infoE, {{.*}} @_ZTS11externref_t

// CHECK-DAG: @_ZTS9funcref_t = {{.*}} c"9funcref_t\00"
// CHECK-DAG: @_ZTI9funcref_t = {{.*}} @_ZTVN10__cxxabiv123__fundamental_type_infoE, {{.*}} @_ZTS9funcref_t
