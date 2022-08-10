; ModuleID = './clang/test/CodeGen/WebAssembly/reftypes-end2end.c'
source_filename = "./clang/test/CodeGen/WebAssembly/reftypes-end2end.c"
target datalayout = "e-m:e-p:32:32-p10:8:8-p20:8:8-i64:64-n32:64-S128-ni:1:10:20"
target triple = "wasm32-unknown-unknown"

@table = internal addrspace(1) global [0 x ptr addrspace(10)] zeroinitializer, align 1

; Function Attrs: noinline nounwind optnone
define hidden ptr addrspace(10) @fold_table_all(i32 noundef %n) #0 {
entry:
  %n.addr = alloca i32, align 4
  %eref = alloca ptr addrspace(10), align 1
  store i32 %n, ptr %n.addr, align 4
  %0 = load i32, ptr %n.addr, align 4
  %arrayidx = getelementptr inbounds [0 x ptr addrspace(10)], ptr addrspace(1) @table, i32 0, i32 %0
  %1 = load ptr addrspace(10), ptr addrspace(1) %arrayidx, align 1
  store ptr addrspace(10) %1, ptr %eref, align 1
  %2 = load ptr addrspace(10), ptr %eref, align 1
  ret ptr addrspace(10) %2
}

attributes #0 = { noinline nounwind optnone "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+reference-types" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 15.0.0 (git@github.com:pmatos/llvm-project.git 512928797a30736806955c8de35e62aeb1309473)"}