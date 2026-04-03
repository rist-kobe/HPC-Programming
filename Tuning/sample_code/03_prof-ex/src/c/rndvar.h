/* Copyright 2024 Research Organization for Information Science and Technology */
#ifndef __RNDVAR_H__
#define __RNDVAR_H__

/* MT method */
#include "mt19937ar.h"

void set_rndseed ( int *irnd );
void get_rnd_array( int *n, double rd[] );
void get_rnd_array_for_BM ( int *n, double rdx[], double rdy[] );
double boxmueller( int *irnd, double *rdx, double *rdy, 
		double *sigam );

#endif /* __RNDBAR_H__ */
