// Copyright 2024 Research Organization for Information Science and Technology
#ifndef __MYKERNEL_H__
#define __MYKERNEL_H__

#include "vector_oo.h"

/*#define ARRAY_SIZE 10000*/
#define ARRAY_SIZE 1000

void poly1 (vector_oo<double,ARRAY_SIZE> &x ,
            vector_oo<double,ARRAY_SIZE> &c0, vector_oo<double,ARRAY_SIZE> &c1);
void poly4 (vector_oo<double,ARRAY_SIZE> &x ,
            vector_oo<double,ARRAY_SIZE> &c0, vector_oo<double,ARRAY_SIZE> &c1);
void poly8 (vector_oo<double,ARRAY_SIZE> &x ,
            vector_oo<double,ARRAY_SIZE> &c0, vector_oo<double,ARRAY_SIZE> &c1);
void poly16 (vector_oo<double,ARRAY_SIZE> &x ,
            vector_oo<double,ARRAY_SIZE> &c0, vector_oo<double,ARRAY_SIZE> &c1);

#endif // __MYKERNEL_H__
