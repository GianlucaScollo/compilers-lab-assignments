; ModuleID = './TEST/Edges_TEST/Not_Same_Iteration_Number.cpp'
source_filename = "./TEST/Edges_TEST/Not_Same_Iteration_Number.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

$_ZSt4ceilIiEN9__gnu_cxx11__enable_ifIXsr12__is_integerIT_EE7__valueEdE6__typeES2_ = comdat any

; Function Attrs: mustprogress noinline uwtable
define dso_local noundef i32 @_Z15LoopFusion_basei(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca [64 x i32], align 16
  %4 = alloca [128 x i32], align 16
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  store i32 0, ptr %5, align 4
  br label %9

9:                                                ; preds = %16, %1
  %10 = load i32, ptr %5, align 4
  %11 = icmp slt i32 %10, 64
  br i1 %11, label %12, label %19

12:                                               ; preds = %9
  %13 = load i32, ptr %5, align 4
  %14 = sext i32 %13 to i64
  %15 = getelementptr inbounds [64 x i32], ptr %3, i64 0, i64 %14
  store i32 0, ptr %15, align 4
  br label %16

16:                                               ; preds = %12
  %17 = load i32, ptr %5, align 4
  %18 = add nsw i32 %17, 1
  store i32 %18, ptr %5, align 4
  br label %9, !llvm.loop !6

19:                                               ; preds = %9
  store i32 0, ptr %6, align 4
  br label %20

20:                                               ; preds = %27, %19
  %21 = load i32, ptr %6, align 4
  %22 = icmp slt i32 %21, 128
  br i1 %22, label %23, label %30

23:                                               ; preds = %20
  %24 = load i32, ptr %6, align 4
  %25 = sext i32 %24 to i64
  %26 = getelementptr inbounds [128 x i32], ptr %4, i64 0, i64 %25
  store i32 0, ptr %26, align 4
  br label %27

27:                                               ; preds = %23
  %28 = load i32, ptr %6, align 4
  %29 = add nsw i32 %28, 1
  store i32 %29, ptr %6, align 4
  br label %20, !llvm.loop !8

30:                                               ; preds = %20
  %31 = load i32, ptr %2, align 4
  %32 = icmp sgt i32 %31, 64
  br i1 %32, label %36, label %33

33:                                               ; preds = %30
  %34 = load i32, ptr %2, align 4
  %35 = icmp slt i32 %34, 0
  br i1 %35, label %36, label %37

36:                                               ; preds = %33, %30
  store i32 64, ptr %2, align 4
  br label %37

37:                                               ; preds = %36, %33
  store i32 0, ptr %7, align 4
  br label %38

38:                                               ; preds = %49, %37
  %39 = load i32, ptr %7, align 4
  %40 = load i32, ptr %2, align 4
  %41 = icmp slt i32 %39, %40
  br i1 %41, label %42, label %52

42:                                               ; preds = %38
  %43 = load i32, ptr %7, align 4
  %44 = mul nsw i32 %43, 3
  %45 = add nsw i32 %44, 7
  %46 = load i32, ptr %7, align 4
  %47 = sext i32 %46 to i64
  %48 = getelementptr inbounds [64 x i32], ptr %3, i64 0, i64 %47
  store i32 %45, ptr %48, align 4
  br label %49

49:                                               ; preds = %42
  %50 = load i32, ptr %7, align 4
  %51 = add nsw i32 %50, 1
  store i32 %51, ptr %7, align 4
  br label %38, !llvm.loop !9

52:                                               ; preds = %38
  store i32 0, ptr %8, align 4
  br label %53

53:                                               ; preds = %72, %52
  %54 = load i32, ptr %8, align 4
  %55 = load i32, ptr %2, align 4
  %56 = mul nsw i32 %55, 2
  %57 = icmp slt i32 %54, %56
  br i1 %57, label %58, label %75

58:                                               ; preds = %53
  %59 = load i32, ptr %8, align 4
  %60 = sdiv i32 %59, 2
  %61 = call noundef double @_ZSt4ceilIiEN9__gnu_cxx11__enable_ifIXsr12__is_integerIT_EE7__valueEdE6__typeES2_(i32 noundef %60)
  %62 = fptosi double %61 to i32
  %63 = sext i32 %62 to i64
  %64 = getelementptr inbounds [64 x i32], ptr %3, i64 0, i64 %63
  %65 = load i32, ptr %64, align 4
  %66 = load i32, ptr %8, align 4
  %67 = xor i32 %66, 5
  %68 = add nsw i32 %65, %67
  %69 = load i32, ptr %8, align 4
  %70 = sext i32 %69 to i64
  %71 = getelementptr inbounds [128 x i32], ptr %4, i64 0, i64 %70
  store i32 %68, ptr %71, align 4
  br label %72

72:                                               ; preds = %58
  %73 = load i32, ptr %8, align 4
  %74 = add nsw i32 %73, 1
  store i32 %74, ptr %8, align 4
  br label %53, !llvm.loop !10

75:                                               ; preds = %53
  %76 = load i32, ptr %2, align 4
  %77 = sub nsw i32 %76, 1
  %78 = sext i32 %77 to i64
  %79 = getelementptr inbounds [128 x i32], ptr %4, i64 0, i64 %78
  %80 = load i32, ptr %79, align 4
  ret i32 %80
}

; Function Attrs: mustprogress noinline nounwind uwtable
define linkonce_odr dso_local noundef double @_ZSt4ceilIiEN9__gnu_cxx11__enable_ifIXsr12__is_integerIT_EE7__valueEdE6__typeES2_(i32 noundef %0) #1 comdat {
  %2 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  %3 = load i32, ptr %2, align 4
  %4 = sitofp i32 %3 to double
  %5 = call double @llvm.ceil.f64(double %4)
  ret double %5
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare double @llvm.ceil.f64(double) #2

attributes #0 = { mustprogress noinline uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { mustprogress noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }

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
