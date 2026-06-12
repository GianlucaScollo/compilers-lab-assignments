; ModuleID = './TEST/Edges_TEST/L2_Has_More_Exit_Blocks.cpp'
source_filename = "./TEST/Edges_TEST/L2_Has_More_Exit_Blocks.cpp"
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
  br label %8

8:                                                ; preds = %18, %1
  %9 = load i32, ptr %5, align 4
  %10 = icmp slt i32 %9, 64
  br i1 %10, label %11, label %21

11:                                               ; preds = %8
  %12 = load i32, ptr %5, align 4
  %13 = sext i32 %12 to i64
  %14 = getelementptr inbounds [64 x i32], ptr %3, i64 0, i64 %13
  store i32 0, ptr %14, align 4
  %15 = load i32, ptr %5, align 4
  %16 = sext i32 %15 to i64
  %17 = getelementptr inbounds [64 x i32], ptr %4, i64 0, i64 %16
  store i32 0, ptr %17, align 4
  br label %18

18:                                               ; preds = %11
  %19 = load i32, ptr %5, align 4
  %20 = add nsw i32 %19, 1
  store i32 %20, ptr %5, align 4
  br label %8, !llvm.loop !6

21:                                               ; preds = %8
  %22 = load i32, ptr %2, align 4
  %23 = icmp sgt i32 %22, 64
  br i1 %23, label %27, label %24

24:                                               ; preds = %21
  %25 = load i32, ptr %2, align 4
  %26 = icmp slt i32 %25, 0
  br i1 %26, label %27, label %28

27:                                               ; preds = %24, %21
  store i32 64, ptr %2, align 4
  br label %28

28:                                               ; preds = %27, %24
  store i32 0, ptr %6, align 4
  store i32 0, ptr %7, align 4
  br label %29

29:                                               ; preds = %38, %28
  %30 = load i32, ptr %6, align 4
  %31 = mul nsw i32 %30, 3
  %32 = add nsw i32 %31, 7
  %33 = load i32, ptr %6, align 4
  %34 = sext i32 %33 to i64
  %35 = getelementptr inbounds [64 x i32], ptr %3, i64 0, i64 %34
  store i32 %32, ptr %35, align 4
  %36 = load i32, ptr %6, align 4
  %37 = add nsw i32 %36, 1
  store i32 %37, ptr %6, align 4
  br label %38

38:                                               ; preds = %29
  %39 = load i32, ptr %6, align 4
  %40 = load i32, ptr %2, align 4
  %41 = icmp slt i32 %39, %40
  br i1 %41, label %29, label %42, !llvm.loop !8

42:                                               ; preds = %38
  br label %43

43:                                               ; preds = %60, %42
  %44 = load i32, ptr %7, align 4
  %45 = icmp eq i32 %44, 7
  br i1 %45, label %46, label %47

46:                                               ; preds = %43
  br label %64

47:                                               ; preds = %43
  %48 = load i32, ptr %7, align 4
  %49 = sext i32 %48 to i64
  %50 = getelementptr inbounds [64 x i32], ptr %3, i64 0, i64 %49
  %51 = load i32, ptr %50, align 4
  %52 = load i32, ptr %7, align 4
  %53 = xor i32 %52, 5
  %54 = add nsw i32 %51, %53
  %55 = load i32, ptr %7, align 4
  %56 = sext i32 %55 to i64
  %57 = getelementptr inbounds [64 x i32], ptr %4, i64 0, i64 %56
  store i32 %54, ptr %57, align 4
  %58 = load i32, ptr %7, align 4
  %59 = add nsw i32 %58, 1
  store i32 %59, ptr %7, align 4
  br label %60

60:                                               ; preds = %47
  %61 = load i32, ptr %7, align 4
  %62 = load i32, ptr %2, align 4
  %63 = icmp slt i32 %61, %62
  br i1 %63, label %43, label %64, !llvm.loop !9

64:                                               ; preds = %60, %46
  %65 = load i32, ptr %2, align 4
  %66 = sub nsw i32 %65, 1
  %67 = sext i32 %66 to i64
  %68 = getelementptr inbounds [64 x i32], ptr %4, i64 0, i64 %67
  %69 = load i32, ptr %68, align 4
  ret i32 %69
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
