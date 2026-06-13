; ModuleID = './TEST/Edges_TEST/Not_Same_Iteration_Number.m2r.ll'
source_filename = "./TEST/Edges_TEST/Not_Same_Iteration_Number.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

$_ZSt4ceilIiEN9__gnu_cxx11__enable_ifIXsr12__is_integerIT_EE7__valueEdE6__typeES2_ = comdat any

; Function Attrs: mustprogress noinline uwtable
define dso_local noundef i32 @_Z15LoopFusion_basei(i32 noundef %0) #0 {
  %2 = alloca [64 x i32], align 16
  %3 = alloca [128 x i32], align 16
  br label %4

4:                                                ; preds = %9, %1
  %.01 = phi i32 [ 0, %1 ], [ %10, %9 ]
  %5 = icmp slt i32 %.01, 64
  br i1 %5, label %6, label %11

6:                                                ; preds = %4
  %7 = sext i32 %.01 to i64
  %8 = getelementptr inbounds [64 x i32], ptr %2, i64 0, i64 %7
  store i32 0, ptr %8, align 4
  br label %9

9:                                                ; preds = %6
  %10 = add nsw i32 %.01, 1
  br label %4, !llvm.loop !6

11:                                               ; preds = %4
  br label %12

12:                                               ; preds = %17, %11
  %.02 = phi i32 [ 0, %11 ], [ %18, %17 ]
  %13 = icmp slt i32 %.02, 128
  br i1 %13, label %14, label %19

14:                                               ; preds = %12
  %15 = sext i32 %.02 to i64
  %16 = getelementptr inbounds [128 x i32], ptr %3, i64 0, i64 %15
  store i32 0, ptr %16, align 4
  br label %17

17:                                               ; preds = %14
  %18 = add nsw i32 %.02, 1
  br label %12, !llvm.loop !8

19:                                               ; preds = %12
  %20 = icmp sgt i32 %0, 64
  br i1 %20, label %23, label %21

21:                                               ; preds = %19
  %22 = icmp slt i32 %0, 0
  br i1 %22, label %23, label %24

23:                                               ; preds = %21, %19
  br label %24

24:                                               ; preds = %23, %21
  %.0 = phi i32 [ 64, %23 ], [ %0, %21 ]
  br label %25

25:                                               ; preds = %32, %24
  %.03 = phi i32 [ 0, %24 ], [ %33, %32 ]
  %26 = icmp slt i32 %.03, %.0
  br i1 %26, label %27, label %34

27:                                               ; preds = %25
  %28 = mul nsw i32 %.03, 3
  %29 = add nsw i32 %28, 7
  %30 = sext i32 %.03 to i64
  %31 = getelementptr inbounds [64 x i32], ptr %2, i64 0, i64 %30
  store i32 %29, ptr %31, align 4
  br label %32

32:                                               ; preds = %27
  %33 = add nsw i32 %.03, 1
  br label %25, !llvm.loop !9

34:                                               ; preds = %25
  br label %35

35:                                               ; preds = %49, %34
  %.04 = phi i32 [ 0, %34 ], [ %50, %49 ]
  %36 = mul nsw i32 %.0, 2
  %37 = icmp slt i32 %.04, %36
  br i1 %37, label %38, label %51

38:                                               ; preds = %35
  %39 = sdiv i32 %.04, 2
  %40 = call noundef double @_ZSt4ceilIiEN9__gnu_cxx11__enable_ifIXsr12__is_integerIT_EE7__valueEdE6__typeES2_(i32 noundef %39)
  %41 = fptosi double %40 to i32
  %42 = sext i32 %41 to i64
  %43 = getelementptr inbounds [64 x i32], ptr %2, i64 0, i64 %42
  %44 = load i32, ptr %43, align 4
  %45 = xor i32 %.04, 5
  %46 = add nsw i32 %44, %45
  %47 = sext i32 %.04 to i64
  %48 = getelementptr inbounds [128 x i32], ptr %3, i64 0, i64 %47
  store i32 %46, ptr %48, align 4
  br label %49

49:                                               ; preds = %38
  %50 = add nsw i32 %.04, 1
  br label %35, !llvm.loop !10

51:                                               ; preds = %35
  %52 = sub nsw i32 %.0, 1
  %53 = sext i32 %52 to i64
  %54 = getelementptr inbounds [128 x i32], ptr %3, i64 0, i64 %53
  %55 = load i32, ptr %54, align 4
  ret i32 %55
}

; Function Attrs: mustprogress noinline nounwind uwtable
define linkonce_odr dso_local noundef double @_ZSt4ceilIiEN9__gnu_cxx11__enable_ifIXsr12__is_integerIT_EE7__valueEdE6__typeES2_(i32 noundef %0) #1 comdat {
  %2 = sitofp i32 %0 to double
  %3 = call double @llvm.ceil.f64(double %2)
  ret double %3
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
