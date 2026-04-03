/* Copyright 2024 Research Organization for Information Science and Technology */
#include "mykernel.h"

/* ==================================================================== 
 * Core part of Modified Gram-Schmidt (MGS) method
 *
 * Note: Be careful. This is NOT a full implementation of MGS.
 *
 * Reference:
 * [1] D. S. Watkins, "The Matrix Eigenvalue Problem" 
 *     (SIAM, 2007) Chap.3.
 * [2] H. Sabukawa, K. Fujino, T. Nagashima, and D. Takahashi, 
 *     "HPC programming" (2009) Chap.3 [In Japanese].
 * [3] A. Imakura and Y. Yamamoto, "Efficient implementations of 
 *     the modified Gram-Schmidt orthogonalization with a non-standard 
 *     inner product", Jpn. J. Ind. Appl. Math. 36, 619-641 (2019).
 * ==================================================================*/
void coremgs(double * restrict a, double * restrict q, 
            const int N, const int M)
{
   for (int i = 0; i < M; ++i) {

      double r = 0.0;
      for (int j = 0; j < N; ++j) {
         int ij = j + i*N;
         r += q[ij]*a[j]; 
      }

      for (int j = 0; j < N; ++j) {
         int ij = j + i*N;
         a[j] += -r*q[ij]; 
      }

   }
}

/* ==================================================================== 
 * Core part of Modified Gram-Schmidt (MGS) method:
 * Application of software pipelining[2]
 *
 * Note: Be careful. This is NOT a full implementation of MGS.
 *
 * Reference:
 * [1] D. S. Watkins, "The Matrix Eigenvalue Problem" 
 *     (SIAM, 2007) Chap.3.
 * [2] H. Sabukawa, K. Fujino, T. Nagashima, and D. Takahashi, 
 *     "HPC programming" (2009) Chap.3 [In Japanese].
 * [3] A. Imakura and Y. Yamamoto, "Efficient implementations of 
 *     the modified Gram-Schmidt orthogonalization with a non-standard 
 *     inner product", Jpn. J. Ind. Appl. Math. 36, 619-641 (2019).
 * ==================================================================*/
void coremgs_swp(double * restrict a, double * restrict q, 
            const int N, const int M)
{
   double s;

   /* loop peeling: prologue */
   s = 0.0; 
   for (int j = 0; j < N; ++j) {
      int ij = j + 0*N;
      s += q[ij]*a[j];      /* i=0   */
   }

   /* main body  */
   for (int i = 1; i < M-1; ++i) {

      double r = 0.0;
      for (int j = 0; j < N; ++j) {
         int ij1 = j + (i-1)*N;
         int ij  = j + i*N;
         a[j] += -s*q[ij1]; /* i-1   */ 
         r += q[ij]*a[i];   /* i     */
      }

      s = r;

   }

   /* loop peeling: epilogue */
   for (int j = 0; j < N; ++j) {
      int ij = j + (M-1)*N;
      a[j] += -s*q[ij];     /* i=M-1 */ 
   }
}

int dummy(double * restrict a, const int N)
{
  int u = 0;
  return u;
}
