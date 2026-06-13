; ModuleID = 'TEST/Edges_TEST/Not_Adjacent.cpp'
source_filename = "TEST/Edges_TEST/Not_Adjacent.cpp"
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
  %8 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  store i32 0, ptr %5, align 4
  br label %9

9:                                                ; preds = %19, %1
  %10 = load i32, ptr %5, align 4
  %11 = icmp slt i32 %10, 64
  br i1 %11, label %12, label %22

12:                                               ; preds = %9
  %13 = load i32, ptr %5, align 4
  %14 = sext i32 %13 to i64
  %15 = getelementptr inbounds [64 x i32], ptr %3, i64 0, i64 %14
  store i32 0, ptr %15, align 4
  %16 = load i32, ptr %5, align 4
  %17 = sext i32 %16 to i64
  %18 = getelementptr inbounds [64 x i32], ptr %4, i64 0, i64 %17
  store i32 0, ptr %18, align 4
  br label %19

19:                                               ; preds = %12
  %20 = load i32, ptr %5, align 4
  %21 = add nsw i32 %20, 1
  store i32 %21, ptr %5, align 4
  br label %9, !llvm.loop !6

22:                                               ; preds = %9
  %23 = load i32, ptr %2, align 4
  %24 = icmp sgt i32 %23, 64
  br i1 %24, label %28, label %25

25:                                               ; preds = %22
  %26 = load i32, ptr %2, align 4
  %27 = icmp slt i32 %26, 0
  br i1 %27, label %28, label %29

28:                                               ; preds = %25, %22
  store i32 64, ptr %2, align 4
  br label %29

29:                                               ; preds = %28, %25
  store i32 0, ptr %6, align 4
  br label %30

30:                                               ; preds = %41, %29
  %31 = load i32, ptr %6, align 4
  %32 = load i32, ptr %2, align 4
  %33 = icmp slt i32 %31, %32
  br i1 %33, label %34, label %44

34:                                               ; preds = %30
  %35 = load i32, ptr %6, align 4
  %36 = mul nsw i32 %35, 3
  %37 = add nsw i32 %36, 7
  %38 = load i32, ptr %6, align 4
  %39 = sext i32 %38 to i64
  %40 = getelementptr inbounds [64 x i32], ptr %3, i64 0, i64 %39
  store i32 %37, ptr %40, align 4
  br label %41

41:                                               ; preds = %34
  %42 = load i32, ptr %6, align 4
  %43 = add nsw i32 %42, 1
  store i32 %43, ptr %6, align 4
  br label %30, !llvm.loop !8

44:                                               ; preds = %30
  store i32 0, ptr %7, align 4
  %45 = load i32, ptr %2, align 4
  %46 = icmp sgt i32 %45, 14
  br i1 %46, label %47, label %48

47:                                               ; preds = %44
  store i32 44, ptr %7, align 4
  br label %49

48:                                               ; preds = %44
  store i32 2, ptr %7, align 4
  br label %49

49:                                               ; preds = %48, %47
  store i32 0, ptr %8, align 4
  br label %50

50:                                               ; preds = %65, %49
  %51 = load i32, ptr %8, align 4
  %52 = load i32, ptr %2, align 4
  %53 = icmp slt i32 %51, %52
  br i1 %53, label %54, label %68

54:                                               ; preds = %50
  %55 = load i32, ptr %8, align 4
  %56 = sext i32 %55 to i64
  %57 = getelementptr inbounds [64 x i32], ptr %3, i64 0, i64 %56
  %58 = load i32, ptr %57, align 4
  %59 = load i32, ptr %8, align 4
  %60 = xor i32 %59, 5
  %61 = add nsw i32 %58, %60
  %62 = load i32, ptr %8, align 4
  %63 = sext i32 %62 to i64
  %64 = getelementptr inbounds [64 x i32], ptr %4, i64 0, i64 %63
  store i32 %61, ptr %64, align 4
  br label %65

65:                                               ; preds = %54
  %66 = load i32, ptr %8, align 4
  %67 = add nsw i32 %66, 1
  store i32 %67, ptr %8, align 4
  br label %50, !llvm.loop !9

68:                                               ; preds = %50
  %69 = load i32, ptr %2, align 4
  %70 = sub nsw i32 %69, 1
  %71 = sext i32 %70 to i64
  %72 = getelementptr inbounds [64 x i32], ptr %4, i64 0, i64 %71
  %73 = load i32, ptr %72, align 4
  %74 = load i32, ptr %7, align 4
  %75 = add nsw i32 %73, %74
  ret i32 %75
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
