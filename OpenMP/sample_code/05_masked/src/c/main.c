/* Copyright 2024 Research Organization for Information Science and Technology */
/* Note: masked construct is introduced in OpenMP 5.1. 
 *       This is regarded as an extension of master construct.          
 *       Please check your OpenMP version and the supported features 
 *       before compiling the program.                 
 *       If your compiler support masked construct, please set 
 *       -DNOT_USE_MAKSED in the compiler's option.                     */ 
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

/* x86 (Intel, AMD, etc.): 64-bytes                                     */
#define CACHE_LINE_DP   8
#define CACHE_LINE_INT 16

/* aarch64: 256 bytes                                                   */
/*#define CACHE_LINE_DP   64
#define CACHE_LINE_INT 128 */

int main (int argc, char **argv)
{
   int *a;
   int nt;

   nt = omp_get_max_threads();

   #pragma omp parallel shared(a)
   {
      int tid = omp_get_thread_num();

#if ! defined(NOT_USE_MASKED)
      #pragma omp masked
      {
         /* To avoid false sharing                                      */ 
         a = (int *) malloc ( sizeof(int)*nt*CACHE_LINE_INT );
      }
#else
      if ( tid == 0 ) {
         /* To avoid false sharing                                      */ 
         a = (int *) malloc ( sizeof(int)*nt*CACHE_LINE_INT );
      }
#endif

      #pragma omp barrier

      int id = tid*CACHE_LINE_INT;

      a[id] = 0;

#if ! defined(NOT_USE_MASKED)
      #pragma omp masked filter(1)
      {
         a[id] = 1;
      }
#else
      if ( tid == 1 ) {
         a[id] = 1;
      }
#endif

      a[id] += 1;
   }

   for ( int i = 0; i < nt; ++i) {
      int id = i*CACHE_LINE_INT;
      fprintf(stdout,"Thread %d: a[%d] = %d\n", i, id, a[id]);
   }

   free( a ); a = NULL;

   return EXIT_SUCCESS;
}
