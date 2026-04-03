// Copyright 2023 Research Organization for Information Science and Technology
#ifndef MYKERNEL_H
#define MYKERNEL_H

#if __CLANG_FUJITSU
#define _restrict_cpp __restrict__
#elif __FUJITSU
#define _restrict_cpp __restrict__
#elif __GNUC__
#define _restrict_cpp __restrict__
#elif
#define _restrict_cpp 
#endif

//#define MAXINUM 2000
#define MAXINUM 50000

class MyKernel {
 public:
  int * il;
  double *x;
  double *y;
  double *z;
  int * d_il;
  double *d_x;
  double *d_y;
  double *d_z;
  void device_memory(const int);
  void memory_copy_to_device(const int);
  void do_kernel (const int, const int, const int, double, double *, double &, double &);
  void count_true (int, double, double &);

 private:
  double xt;
  double yt;
  double zt;
  inline void setup();
  void calc1(int, double, double *, double &);
  void calc2(int, double, double *, double &);
  void calc2_alt(int, double, double * _restrict_cpp, double &f);
  void calc2_mod(int, double, double * _restrict_cpp, double &f);
};

#endif //MYKERNEL_H
