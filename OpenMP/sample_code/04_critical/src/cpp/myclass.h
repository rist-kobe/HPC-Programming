// Copyright 2024 Research Organization for Information Science and Technology 
#ifndef MYCLASS_H_
#define MYCLASS_H_

class MyClass 
{
  public:
  void decomp_size (const int, const int, const int, int &, int &);
  int func (const int *, const int, const int);
  int func_local (const int *, const int, const int) const;

  private:
  // ATTENTION: The work array u is a class member. When a single
  // MyClass instance is used from an OpenMP parallel region, u is
  // SHARED among the threads and concurrent calls to func race on it.
  int u[2];
};
#endif // MYCLASS_H_
