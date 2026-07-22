/* Copyright 2024 Research Organization for Information Science and Technology */
/* Note: masked construct is introduced in OpenMP 5.1. 
 *       This is regarded as an extension of master construct: masked
 *       without a filter clause behaves like filter(0), i.e., only the
 *       primary thread executes the region. It is the OpenMP 5.1
 *       replacement for the deprecated master construct.
 *       No implied barrier exists at entry to or exit from a masked
 *       region; that is why an explicit barrier is placed after the
 *       allocation below.
 *       Please check your OpenMP version and the supported features 
 *       before compiling the program.                 
 *       If your compiler does NOT support masked construct, please set 
 *       -DNOT_USE_MASKED in the compiler's option.                     */ 
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

/* Number of array elements per cache line. CACHE_LINE_INT is used for
 * the int work array below; CACHE_LINE_DP (double) is provided for the
 * exercises.                                                           */
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
         /* To avoid false sharing: each thread works on its own cache
          * line. The allocation must also be cache-line ALIGNED, not
          * just cache-line strided; plain malloc only guarantees
          * alignment for the largest fundamental type, so the first
          * element could share a cache line with unrelated data.      */ 
         a = (int *) aligned_alloc ( sizeof(int)*CACHE_LINE_INT,
                                     sizeof(int)*nt*CACHE_LINE_INT );
      }
#else
      if ( tid == 0 ) {
         /* To avoid false sharing: each thread works on its own cache
          * line. The allocation must also be cache-line ALIGNED, not
          * just cache-line strided; plain malloc only guarantees
          * alignment for the largest fundamental type, so the first
          * element could share a cache line with unrelated data.      */ 
         a = (int *) aligned_alloc ( sizeof(int)*CACHE_LINE_INT,
                                     sizeof(int)*nt*CACHE_LINE_INT );
      }
#endif

      /* masked has no implied barrier: all threads must wait here
       * until the allocation by thread 0 is visible.                  */
      #pragma omp barrier

      int id = tid*CACHE_LINE_INT;

      a[id] = 0;

#if ! defined(NOT_USE_MASKED)
      /* Only the thread whose number equals the filter integer runs
       * this region. With a single thread no thread matches filter(1)
       * and the region is simply skipped.                             */
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
