/* Copyright 2024 Research Organization for Information Science and Technology */
#ifndef __MYTYPE_H__
#define __MYTYPE_H__

#define NSIZE 32000 
#define NREP 100000

typedef struct 
{
  double a;
  double b;
} tcmp;

typedef struct
{
  double a[NSIZE];
  double b[NSIZE];
} tcmpv;

#endif /* __MYTYPE_H__ */
