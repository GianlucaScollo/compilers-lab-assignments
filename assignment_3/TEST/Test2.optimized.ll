; ModuleID = 'TEST/Test2.m2r.ll'
source_filename = "TEST/Test2.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: mustprogress noinline nounwind uwtable
define dso_local noundef i32 @_Z10licm_test2iii(i32 noundef %0, i32 noundef %1, i32 noundef %2) #0 {
  br label %4

4:                                                ; preds = %24, %3
  %.01 = phi i32 [ %2, %3 ], [ %.1, %24 ]
  %5 = icmp slt i32 %.01, 10
  br i1 %5, label %6, label %25

6:                                                ; preds = %4
  %7 = mul nsw i32 %0, %0
  %8 = mul nsw i32 2, %1
  %9 = add nsw i32 %7, %8
  %10 = sub nsw i32 %1, 4
  %11 = sub nsw i32 %1, 4
  %12 = mul nsw i32 %10, %11
  %13 = add nsw i32 %9, %.01
  %14 = sub nsw i32 %13, %12
  %15 = and i32 %14, 1
  %16 = icmp ne i32 %15, 0
  br i1 %16, label %17, label %19

17:                                               ; preds = %6
  %18 = add nsw i32 %.01, 1
  br label %24

19:                                               ; preds = %6
  %20 = add nsw i32 %.01, 2
  %21 = icmp sgt i32 %14, 1000
  br i1 %21, label %22, label %23

22:                                               ; preds = %19
  br label %26

23:                                               ; preds = %19
  br label %24

24:                                               ; preds = %23, %17
  %.1 = phi i32 [ %18, %17 ], [ %20, %23 ]
  br label %4, !llvm.loop !6

25:                                               ; preds = %4
  br label %26

26:                                               ; preds = %25, %22
  %.0 = phi i32 [ %14, %22 ], [ %.01, %25 ]
  ret i32 %.0
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
