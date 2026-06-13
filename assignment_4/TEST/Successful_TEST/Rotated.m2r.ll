; ModuleID = './TEST/Successful_TEST/Rotated.ll'
source_filename = "./TEST/Successful_TEST/Rotated.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: mustprogress noinline nounwind uwtable
define dso_local noundef i32 @_Z15LoopFusion_basei(i32 noundef %0) #0 {
  %2 = alloca [64 x i32], align 16
  %3 = alloca [64 x i32], align 16
  br label %4

4:                                                ; preds = %11, %1
  %.01 = phi i32 [ 0, %1 ], [ %12, %11 ]
  %5 = icmp slt i32 %.01, 64
  br i1 %5, label %6, label %13

6:                                                ; preds = %4
  %7 = sext i32 %.01 to i64
  %8 = getelementptr inbounds [64 x i32], ptr %2, i64 0, i64 %7
  store i32 0, ptr %8, align 4
  %9 = sext i32 %.01 to i64
  %10 = getelementptr inbounds [64 x i32], ptr %3, i64 0, i64 %9
  store i32 0, ptr %10, align 4
  br label %11

11:                                               ; preds = %6
  %12 = add nsw i32 %.01, 1
  br label %4, !llvm.loop !6

13:                                               ; preds = %4
  %14 = icmp sgt i32 %0, 64
  br i1 %14, label %17, label %15

15:                                               ; preds = %13
  %16 = icmp slt i32 %0, 0
  br i1 %16, label %17, label %18

17:                                               ; preds = %15, %13
  br label %18

18:                                               ; preds = %17, %15
  %.0 = phi i32 [ 64, %17 ], [ %0, %15 ]
  br label %19

19:                                               ; preds = %25, %18
  %.02 = phi i32 [ 0, %18 ], [ %24, %25 ]
  %20 = mul nsw i32 %.02, 3
  %21 = add nsw i32 %20, 7
  %22 = sext i32 %.02 to i64
  %23 = getelementptr inbounds [64 x i32], ptr %2, i64 0, i64 %22
  store i32 %21, ptr %23, align 4
  %24 = add nsw i32 %.02, 1
  br label %25

25:                                               ; preds = %19
  %26 = icmp slt i32 %24, %.0
  br i1 %26, label %19, label %27, !llvm.loop !8

27:                                               ; preds = %25
  br label %28

28:                                               ; preds = %37, %27
  %.03 = phi i32 [ 0, %27 ], [ %36, %37 ]
  %29 = sext i32 %.03 to i64
  %30 = getelementptr inbounds [64 x i32], ptr %2, i64 0, i64 %29
  %31 = load i32, ptr %30, align 4
  %32 = xor i32 %.03, 5
  %33 = add nsw i32 %31, %32
  %34 = sext i32 %.03 to i64
  %35 = getelementptr inbounds [64 x i32], ptr %3, i64 0, i64 %34
  store i32 %33, ptr %35, align 4
  %36 = add nsw i32 %.03, 1
  br label %37

37:                                               ; preds = %28
  %38 = icmp slt i32 %36, %.0
  br i1 %38, label %28, label %39, !llvm.loop !9

39:                                               ; preds = %37
  %40 = sub nsw i32 %.0, 1
  %41 = sext i32 %40 to i64
  %42 = getelementptr inbounds [64 x i32], ptr %3, i64 0, i64 %41
  %43 = load i32, ptr %42, align 4
  ret i32 %43
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
!9 = distinct !{!9, !7}
