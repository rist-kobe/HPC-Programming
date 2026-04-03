// Copyright 2024 Research Organization for Information Science and Technology
#ifndef __MYKERNEL_H__
#define __MYKERNEL_H__

#if __CLANG_FUJITSU
#define _restrict_cpp __restrict__
#elif __FUJITSU
#define _restrict_cpp __restrict__
#elif __INTEL_LLVM_COMPILER
#define _restrict_cpp __restrict
#elif __GNUC__
#define _restrict_cpp __restrict__
#endif

int dummy (double * _restrict_cpp, int, double);
void poly1 (double * _restrict_cpp, double * _restrict_cpp, double * _restrict_cpp, const int);
void poly4 (double * _restrict_cpp, double * _restrict_cpp, double * _restrict_cpp, const int);
void poly8 (double * _restrict_cpp, double * _restrict_cpp, double * _restrict_cpp, const int);
void poly16 (double * _restrict_cpp, double * _restrict_cpp, double * _restrict_cpp, const int);
void poly32 (double * _restrict_cpp, double * _restrict_cpp, double * _restrict_cpp, const int);

#endif // __MYKERNEL_H__
