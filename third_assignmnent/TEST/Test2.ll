; ModuleID = 'LICM_Test2.cpp'
source_filename = "LICM_Test2.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: mustprogress noinline nounwind uwtable
define dso_local noundef i32 @_Z10licm_test2iii(i32 noundef %0, i32 noundef %1, i32 noundef %2) #0 {
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  %11 = alloca i32, align 4
  store i32 %0, ptr %5, align 4
  store i32 %1, ptr %6, align 4
  store i32 %2, ptr %7, align 4
  %12 = load i32, ptr %7, align 4
  store i32 %12, ptr %8, align 4
  br label %13

13:                                               ; preds = %47, %3
  %14 = load i32, ptr %8, align 4
  %15 = icmp slt i32 %14, 10
  br i1 %15, label %16, label %48

16:                                               ; preds = %13
  %17 = load i32, ptr %5, align 4
  %18 = load i32, ptr %5, align 4
  %19 = mul nsw i32 %17, %18
  %20 = load i32, ptr %6, align 4
  %21 = mul nsw i32 2, %20
  %22 = add nsw i32 %19, %21
  store i32 %22, ptr %9, align 4
  %23 = load i32, ptr %6, align 4
  %24 = sub nsw i32 %23, 4
  %25 = load i32, ptr %6, align 4
  %26 = sub nsw i32 %25, 4
  %27 = mul nsw i32 %24, %26
  store i32 %27, ptr %10, align 4
  %28 = load i32, ptr %9, align 4
  %29 = load i32, ptr %8, align 4
  %30 = add nsw i32 %28, %29
  %31 = load i32, ptr %10, align 4
  %32 = sub nsw i32 %30, %31
  store i32 %32, ptr %11, align 4
  %33 = load i32, ptr %11, align 4
  %34 = and i32 %33, 1
  %35 = icmp ne i32 %34, 0
  br i1 %35, label %36, label %39

36:                                               ; preds = %16
  %37 = load i32, ptr %8, align 4
  %38 = add nsw i32 %37, 1
  store i32 %38, ptr %8, align 4
  br label %47

39:                                               ; preds = %16
  %40 = load i32, ptr %8, align 4
  %41 = add nsw i32 %40, 2
  store i32 %41, ptr %8, align 4
  %42 = load i32, ptr %11, align 4
  %43 = icmp sgt i32 %42, 1000
  br i1 %43, label %44, label %46

44:                                               ; preds = %39
  %45 = load i32, ptr %11, align 4
  store i32 %45, ptr %4, align 4
  br label %50

46:                                               ; preds = %39
  br label %47

47:                                               ; preds = %46, %36
  br label %13, !llvm.loop !6

48:                                               ; preds = %13
  %49 = load i32, ptr %8, align 4
  store i32 %49, ptr %4, align 4
  br label %50

50:                                               ; preds = %48, %44
  %51 = load i32, ptr %4, align 4
  ret i32 %51
}

attributes #0 = { mustprogress noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 19.1.7 (/home/runner/work/llvm-project/llvm-project/clang cd708029e0b2869e80abe31ddb175f7c35361f90)"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
