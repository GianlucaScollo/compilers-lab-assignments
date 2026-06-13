; ModuleID = 'TEST/Successful_TEST/Nested.m2r.ll'
source_filename = "TEST/Successful_TEST/Nested.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: mustprogress noinline nounwind uwtable
define dso_local noundef i32 @_Z17LoopFusion_nestedi(i32 noundef %0) #0 {
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

19:                                               ; preds = %43, %18
  br label %20

20:                                               ; preds = %40, %19
  %.02 = phi i32 [ 0, %19 ], [ %28, %40 ]
  %21 = icmp slt i32 %.02, %.0
  br i1 %21, label %22, label %42

22:                                               ; preds = %20
  %23 = mul nsw i32 %.02, 3
  %24 = add nsw i32 %23, 7
  %25 = sext i32 %.02 to i64
  %26 = getelementptr inbounds [64 x i32], ptr %2, i64 0, i64 %25
  store i32 %24, ptr %26, align 4
  br label %27

27:                                               ; preds = %22
  %28 = add nsw i32 %.02, 1
  br label %32, !llvm.loop !8

29:                                               ; No predecessors!
  br label %30

30:                                               ; preds = %29
  %31 = icmp slt i32 %.02, %.0
  br i1 %31, label %32, label %42

32:                                               ; preds = %27, %30
  %33 = sext i32 %.02 to i64
  %34 = getelementptr inbounds [64 x i32], ptr %2, i64 0, i64 %33
  %35 = load i32, ptr %34, align 4
  %36 = xor i32 %.02, 5
  %37 = add nsw i32 %35, %36
  %38 = sext i32 %.02 to i64
  %39 = getelementptr inbounds [64 x i32], ptr %3, i64 0, i64 %38
  store i32 %37, ptr %39, align 4
  br label %40

40:                                               ; preds = %32
  %41 = add nsw i32 %.02, 1
  br label %20, !llvm.loop !9

42:                                               ; preds = %20, %30
  br label %43

43:                                               ; preds = %42
  %44 = icmp slt i32 %.0, 0
  br i1 %44, label %19, label %45, !llvm.loop !10

45:                                               ; preds = %43
  %46 = sub nsw i32 %.0, 1
  %47 = sext i32 %46 to i64
  %48 = getelementptr inbounds [64 x i32], ptr %3, i64 0, i64 %47
  %49 = load i32, ptr %48, align 4
  ret i32 %49
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
!10 = distinct !{!10, !7}
