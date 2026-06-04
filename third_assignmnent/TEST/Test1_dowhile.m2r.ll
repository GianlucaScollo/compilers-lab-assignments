; ModuleID = 'LICM_Test1_dowhile.ll'
source_filename = "LICM_Test1_dowhile.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: mustprogress noinline nounwind uwtable
define dso_local void @_Z10licm_test1ii(i32 noundef %0, i32 noundef %1) #0 {
  br label %3

3:                                                ; preds = %15, %2
  %.01 = phi i32 [ 0, %2 ], [ %.1, %15 ]
  %.0 = phi i32 [ %1, %2 ], [ %5, %15 ]
  %4 = mul nsw i32 %0, 2
  %5 = sub nsw i32 %.0, 3
  %6 = icmp slt i32 %5, 5
  br i1 %6, label %7, label %9

7:                                                ; preds = %3
  %8 = add nsw i32 %.01, 2
  br label %14

9:                                                ; preds = %3
  %10 = sub nsw i32 %4, %5
  %11 = icmp slt i32 %10, -30
  br i1 %11, label %12, label %13

12:                                               ; preds = %9
  br label %18

13:                                               ; preds = %9
  br label %14

14:                                               ; preds = %13, %7
  %.1 = phi i32 [ %8, %7 ], [ %.01, %13 ]
  br label %15

15:                                               ; preds = %14
  %16 = icmp slt i32 %.1, 20
  br i1 %16, label %3, label %17, !llvm.loop !6

17:                                               ; preds = %15
  br label %18

18:                                               ; preds = %17, %12
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
