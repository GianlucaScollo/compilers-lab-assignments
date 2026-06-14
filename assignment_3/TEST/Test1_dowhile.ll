; ModuleID = 'TEST/Test1_dowhile.cpp'
source_filename = "TEST/Test1_dowhile.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: mustprogress noinline nounwind uwtable
define dso_local void @_Z10licm_test1ii(i32 noundef %0, i32 noundef %1) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  store i32 %0, ptr %3, align 4
  store i32 %1, ptr %4, align 4
  store i32 0, ptr %5, align 4
  br label %7

7:                                                ; preds = %26, %2
  %8 = load i32, ptr %3, align 4
  %9 = mul nsw i32 %8, 2
  store i32 %9, ptr %6, align 4
  %10 = load i32, ptr %4, align 4
  %11 = sub nsw i32 %10, 3
  store i32 %11, ptr %4, align 4
  %12 = load i32, ptr %4, align 4
  %13 = icmp slt i32 %12, 5
  br i1 %13, label %14, label %17

14:                                               ; preds = %7
  %15 = load i32, ptr %5, align 4
  %16 = add nsw i32 %15, 2
  store i32 %16, ptr %5, align 4
  br label %25

17:                                               ; preds = %7
  %18 = load i32, ptr %6, align 4
  %19 = load i32, ptr %4, align 4
  %20 = sub nsw i32 %18, %19
  store i32 %20, ptr %6, align 4
  %21 = load i32, ptr %6, align 4
  %22 = icmp slt i32 %21, -30
  br i1 %22, label %23, label %24

23:                                               ; preds = %17
  br label %30

24:                                               ; preds = %17
  br label %25

25:                                               ; preds = %24, %14
  br label %26

26:                                               ; preds = %25
  %27 = load i32, ptr %5, align 4
  %28 = icmp slt i32 %27, 20
  br i1 %28, label %7, label %29, !llvm.loop !6

29:                                               ; preds = %26
  br label %30

30:                                               ; preds = %29, %23
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
