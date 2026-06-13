; ModuleID = 'TEST/Successful_TEST/Both_Guarded.cpp'
source_filename = "TEST/Successful_TEST/Both_Guarded.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: mustprogress noinline nounwind uwtable
define dso_local noundef i32 @_Z15LoopFusion_basei(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca [64 x i32], align 16
  %4 = alloca [64 x i32], align 16
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  store i32 0, ptr %5, align 4
  store i32 0, ptr %6, align 4
  store i32 100, ptr %7, align 4
  %8 = load i32, ptr %7, align 4
  %9 = load i32, ptr %2, align 4
  %10 = icmp slt i32 %8, %9
  br i1 %10, label %11, label %26

11:                                               ; preds = %1
  br label %12

12:                                               ; preds = %21, %11
  %13 = load i32, ptr %5, align 4
  %14 = mul nsw i32 %13, 3
  %15 = add nsw i32 %14, 7
  %16 = load i32, ptr %5, align 4
  %17 = sext i32 %16 to i64
  %18 = getelementptr inbounds [64 x i32], ptr %3, i64 0, i64 %17
  store i32 %15, ptr %18, align 4
  %19 = load i32, ptr %5, align 4
  %20 = add nsw i32 %19, 1
  store i32 %20, ptr %5, align 4
  br label %21

21:                                               ; preds = %12
  %22 = load i32, ptr %5, align 4
  %23 = load i32, ptr %2, align 4
  %24 = icmp slt i32 %22, %23
  br i1 %24, label %12, label %25, !llvm.loop !6

25:                                               ; preds = %21
  br label %26

26:                                               ; preds = %25, %1
  %27 = load i32, ptr %7, align 4
  %28 = load i32, ptr %2, align 4
  %29 = icmp sgt i32 %27, %28
  br i1 %29, label %30, label %49

30:                                               ; preds = %26
  br label %31

31:                                               ; preds = %44, %30
  %32 = load i32, ptr %6, align 4
  %33 = sext i32 %32 to i64
  %34 = getelementptr inbounds [64 x i32], ptr %3, i64 0, i64 %33
  %35 = load i32, ptr %34, align 4
  %36 = load i32, ptr %6, align 4
  %37 = xor i32 %36, 5
  %38 = add nsw i32 %35, %37
  %39 = load i32, ptr %6, align 4
  %40 = sext i32 %39 to i64
  %41 = getelementptr inbounds [64 x i32], ptr %4, i64 0, i64 %40
  store i32 %38, ptr %41, align 4
  %42 = load i32, ptr %6, align 4
  %43 = add nsw i32 %42, 1
  store i32 %43, ptr %6, align 4
  br label %44

44:                                               ; preds = %31
  %45 = load i32, ptr %6, align 4
  %46 = load i32, ptr %2, align 4
  %47 = icmp slt i32 %45, %46
  br i1 %47, label %31, label %48, !llvm.loop !8

48:                                               ; preds = %44
  br label %49

49:                                               ; preds = %48, %26
  %50 = load i32, ptr %2, align 4
  %51 = sub nsw i32 %50, 1
  %52 = sext i32 %51 to i64
  %53 = getelementptr inbounds [64 x i32], ptr %4, i64 0, i64 %52
  %54 = load i32, ptr %53, align 4
  ret i32 %54
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
