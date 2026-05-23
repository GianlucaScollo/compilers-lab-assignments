; ModuleID = 'LICM_Test1.cpp'
source_filename = "LICM_Test1.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable
define dso_local void @_Z10licm_test1ii(i32 noundef %0, i32 noundef %1) local_unnamed_addr #0 {
  %3 = shl nsw i32 %0, 1
  br label %4

4:                                                ; preds = %2, %64
  %5 = phi i32 [ %6, %64 ], [ %1, %2 ]
  %6 = add nsw i32 %5, -3
  %7 = icmp slt i32 %5, 8
  br i1 %7, label %8, label %64

8:                                                ; preds = %4, %12
  %9 = phi i32 [ %10, %12 ], [ %6, %4 ]
  %10 = add nsw i32 %9, -3
  %11 = icmp slt i32 %9, 8
  br i1 %11, label %15, label %12

12:                                               ; preds = %8
  %13 = sub nsw i32 %3, %10
  %14 = icmp sgt i32 %13, -31
  br i1 %14, label %8, label %67, !llvm.loop !5

15:                                               ; preds = %8, %19
  %16 = phi i32 [ %17, %19 ], [ %10, %8 ]
  %17 = add nsw i32 %16, -3
  %18 = icmp slt i32 %16, 8
  br i1 %18, label %22, label %19

19:                                               ; preds = %15
  %20 = sub nsw i32 %3, %17
  %21 = icmp sgt i32 %20, -31
  br i1 %21, label %15, label %67, !llvm.loop !5

22:                                               ; preds = %15, %26
  %23 = phi i32 [ %24, %26 ], [ %17, %15 ]
  %24 = add nsw i32 %23, -3
  %25 = icmp slt i32 %23, 8
  br i1 %25, label %29, label %26

26:                                               ; preds = %22
  %27 = sub nsw i32 %3, %24
  %28 = icmp sgt i32 %27, -31
  br i1 %28, label %22, label %67, !llvm.loop !5

29:                                               ; preds = %22, %33
  %30 = phi i32 [ %31, %33 ], [ %24, %22 ]
  %31 = add nsw i32 %30, -3
  %32 = icmp slt i32 %30, 8
  br i1 %32, label %36, label %33

33:                                               ; preds = %29
  %34 = sub nsw i32 %3, %31
  %35 = icmp sgt i32 %34, -31
  br i1 %35, label %29, label %67, !llvm.loop !5

36:                                               ; preds = %29, %40
  %37 = phi i32 [ %38, %40 ], [ %31, %29 ]
  %38 = add nsw i32 %37, -3
  %39 = icmp slt i32 %37, 8
  br i1 %39, label %43, label %40

40:                                               ; preds = %36
  %41 = sub nsw i32 %3, %38
  %42 = icmp sgt i32 %41, -31
  br i1 %42, label %36, label %67, !llvm.loop !5

43:                                               ; preds = %36, %47
  %44 = phi i32 [ %45, %47 ], [ %38, %36 ]
  %45 = add nsw i32 %44, -3
  %46 = icmp slt i32 %44, 8
  br i1 %46, label %50, label %47

47:                                               ; preds = %43
  %48 = sub nsw i32 %3, %45
  %49 = icmp sgt i32 %48, -31
  br i1 %49, label %43, label %67, !llvm.loop !5

50:                                               ; preds = %43, %54
  %51 = phi i32 [ %52, %54 ], [ %45, %43 ]
  %52 = add nsw i32 %51, -3
  %53 = icmp slt i32 %51, 8
  br i1 %53, label %57, label %54

54:                                               ; preds = %50
  %55 = sub nsw i32 %3, %52
  %56 = icmp sgt i32 %55, -31
  br i1 %56, label %50, label %67, !llvm.loop !5

57:                                               ; preds = %50, %60
  %58 = phi i32 [ %61, %60 ], [ %52, %50 ]
  %59 = icmp slt i32 %58, 8
  br i1 %59, label %67, label %60

60:                                               ; preds = %57
  %61 = add nsw i32 %58, -3
  %62 = sub nsw i32 %3, %61
  %63 = icmp sgt i32 %62, -31
  br i1 %63, label %57, label %67, !llvm.loop !5

64:                                               ; preds = %4
  %65 = sub nsw i32 %3, %6
  %66 = icmp sgt i32 %65, -31
  br i1 %66, label %4, label %67, !llvm.loop !5

67:                                               ; preds = %64, %12, %19, %26, %33, %40, %47, %54, %60, %57
  ret void
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{!"clang version 19.1.7 (/home/runner/work/llvm-project/llvm-project/clang cd708029e0b2869e80abe31ddb175f7c35361f90)"}
!5 = distinct !{!5, !6}
!6 = !{!"llvm.loop.mustprogress"}
