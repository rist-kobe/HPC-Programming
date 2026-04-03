// Copyright 2024 Research Organization for Information Science and Technology 
#ifndef __MYCLASS_H__
#define __MYCLASS_H__

#define MAX(a,b) ((a) > (b) ? (a) : (b) ) 
#define MIN(a,b) ((a) < (b) ? (a) : (b) ) 

class MyClass 
{
  public:
  void decomp_size (const int, int, int, int &, int &);
  int func (int *, int, int);

  private:
  int u[2];
};
#endif // __MYCASS_H__
