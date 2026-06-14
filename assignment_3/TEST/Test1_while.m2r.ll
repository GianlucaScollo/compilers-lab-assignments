; ModuleID = 'TEST/Test1_while.ll'
source_filename = "TEST/Test1_while.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: mustprogress noinline nounwind uwtable
define dso_local void @_Z10licm_test1ii(i32 noundef %0, i32 noundef %1) #0 {
  br label %3

3:                                                ; preds = %16, %2
  %.01 = phi i32 [ 0, %2 ], [ %.1, %16 ]
  %.0 = phi i32 [ %1, %2 ], [ %7, %16 ]
  %4 = icmp slt i32 %.01, 20
  br i1 %4, label %5, label %17

5:                                                ; preds = %3
  %6 = mul nsw i32 %0, 2
  %7 = sub nsw i32 %.0, 3
  %8 = icmp slt i32 %7, 5
  br i1 %8, label %9, label %11

9:                                                ; preds = %5
  %10 = add nsw i32 %.01, 2
  br label %16

11:                                               ; preds = %5
  %12 = sub nsw i32 %6, %7
  %13 = icmp slt i32 %12, -30
  br i1 %13, label %14, label %15

14:                                               ; preds = %11
  br label %18

15:                                               ; preds = %11
  br label %16

16:                                               ; preds = %15, %9
  %.1 = phi i32 [ %10, %9 ], [ %.01, %15 ]
  br label %3, !llvm.loop !6

17:                                               ; preds = %3
  br label %18

18:                                               ; preds = %17, %14
  ret void
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
