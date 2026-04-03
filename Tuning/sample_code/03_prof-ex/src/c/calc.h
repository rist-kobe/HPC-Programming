/* Copyright 2024 Research Organization for Information Science and Technology */
#ifndef __CALC_H__
#define __CALC_H__

void calc_mass ( InputData *IDp, AtomData *ADp );
int calc_iniposit (int nn, InputData *IDp, AtomData *ADp);
void calc_inivel(int nn, InputData *IDp, AtomData *ADp);
void calc_force ( InputData *IDp, AtomData *ADp, 
		double x[], double y[], double z[]);
void calc_force_diffuse ( InputData *IDp, AtomData *ADp, 
		double x[], double y[], double z[]);
void calc_positr1 ( InputData *IDp, AtomData *ADp,
		double x[], double y[], double z[],
		double vx[], double vy[], double vz[] );
void calc_positr1_diffuse ( InputData *IDp, AtomData *ADp,
		double x[], double y[], double z[],
		double vx[], double vy[], double vz[] );
void calc_positr ( InputData *IDp, AtomData *ADp,
		double rbx[], double rby[], double rbz[],
		double rtx[], double rty[], double rtz[]);
void calc_positr_diffuse ( InputData *IDp, AtomData *ADp,
		double rbx[], double rby[], double rbz[],
		double rtx[], double rty[], double rtz[]);

#endif  /* __CALC_H__ */
