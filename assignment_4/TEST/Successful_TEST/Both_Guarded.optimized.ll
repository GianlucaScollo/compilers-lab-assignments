; ModuleID = 'TEST/Successful_TEST/Both_Guarded.m2r.ll'
source_filename = "TEST/Successful_TEST/Both_Guarded.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: mustprogress noinline nounwind uwtable
define dso_local noundef i32 @_Z15LoopFusion_basei(i32 noundef %0) #0 {
  %2 = alloca [64 x i32], align 16
  %3 = alloca [64 x i32], align 16
  %4 = icmp slt i32 100, %0
  br i1 %4, label %5, label %15

5:                                                ; preds = %1
  br label %6

6:                                                ; preds = %12, %5
  %.01 = phi i32 [ 0, %5 ], [ %11, %12 ]
  %7 = mul nsw i32 %.01, 3
  %8 = add nsw i32 %7, 7
  %9 = sext i32 %.01 to i64
  %10 = getelementptr inbounds [64 x i32], ptr %2, i64 0, i64 %9
  store i32 %8, ptr %10, align 4
  %11 = add nsw i32 %.01, 1
  br label %12

12:                                               ; preds = %6
  %13 = icmp slt i32 %11, %0
  br i1 %13, label %6, label %14, !llvm.loop !6

14:                                               ; preds = %12
  br label %15

15:                                               ; preds = %14, %1
  %16 = icmp sgt i32 100, %0
  br i1 %16, label %17, label %30

17:                                               ; preds = %15
  br label %18

18:                                               ; preds = %27, %17
  %.0 = phi i32 [ 0, %17 ], [ %26, %27 ]
  %19 = sext i32 %.0 to i64
  %20 = getelementptr inbounds [64 x i32], ptr %2, i64 0, i64 %19
  %21 = load i32, ptr %20, align 4
  %22 = xor i32 %.0, 5
  %23 = add nsw i32 %21, %22
  %24 = sext i32 %.0 to i64
  %25 = getelementptr inbounds [64 x i32], ptr %3, i64 0, i64 %24
  store i32 %23, ptr %25, align 4
  %26 = add nsw i32 %.0, 1
  br label %27

27:                                               ; preds = %18
  %28 = icmp slt i32 %26, %0
  br i1 %28, label %18, label %29, !llvm.loop !8

29:                                               ; preds = %27
  br label %30

30:                                               ; preds = %29, %15
  %31 = sub nsw i32 %0, 1
  %32 = sext i32 %31 to i64
  %33 = getelementptr inbounds [64 x i32], ptr %3, i64 0, i64 %32
  %34 = load i32, ptr %33, align 4
  ret i32 %34
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
!8 = distinct !{!8, !7}
