/* Copyright 2024 Research Organization for Information Science and Technology */
#include <stdio.h>
#include <stdlib.h>

#if defined(USE_NVTX)
#include <nvtx3/nvToolsExt.h>
#endif

#define NGSIZE 2048
#define NCSIZE 200 
#define NREP 1000

#define IDX(I,J,NJ) ( (J) + (I)*(NJ) )

int main (int argc, char **argv)
{  
   float *a, *b, *c;
   float *a2, *b2, *c2;

   a  = (float *)malloc( sizeof(float)*NGSIZE*NGSIZE );
   b  = (float *)malloc( sizeof(float)*NGSIZE*NGSIZE );
   c  = (float *)malloc( sizeof(float)*NGSIZE*NGSIZE );
   a2 = (float *)malloc( sizeof(float)*NCSIZE*NCSIZE );
   b2 = (float *)malloc( sizeof(float)*NCSIZE*NCSIZE );
   c2 = (float *)malloc( sizeof(float)*NCSIZE*NCSIZE );

   for (int i = 0; i < NGSIZE; ++i) {
   for (int j = 0; j < NGSIZE; ++j) {
      a[IDX(i,j,NGSIZE)] =0.01f;
      b[IDX(i,j,NGSIZE)] =0.02f;
   }}

   for (int i = 0; i < NCSIZE; ++i) {
   for (int j = 0; j < NCSIZE; ++j) {
      a2[IDX(i,j,NCSIZE)] =0.01f;
      b2[IDX(i,j,NCSIZE)] =0.02f;
   }}   

   /* main loop */
#if defined(USE_NVTX)
   nvtxRangeId_t id1 = nvtxRangeStartA("total");
#endif
   #pragma acc data copyin(a[0:NGSIZE*NGSIZE],b[0:NGSIZE*NGSIZE]) copyout(c[0:NGSIZE*NGSIZE])
   {
   for (int itr = 0; itr < NREP; ++itr) {

#if defined(USE_NVTX)
     nvtxRangeId_t id2 = nvtxRangeStartA("gpu_acc");
#endif
     #pragma acc kernels
     {
     #pragma acc loop independent collapse(2)
     for (int i = 0; i < NGSIZE; ++i) {
     for (int j = 0; j < NGSIZE; ++j) {
        c[IDX(i,j,NGSIZE)] = 0.0f;
     }}

     #pragma acc loop independent collapse(2) 
     for (int i = 0; i < NGSIZE; ++i) {
     for (int j = 0; j < NGSIZE; ++j) {
        float tmp = 0.0f;
        #pragma acc loop seq
	for (int k = 0; k <  NGSIZE; ++k ){
           tmp += a[IDX(i,k,NGSIZE)]*b[IDX(k,j,NGSIZE)]; 
	}
        c[IDX(i,j,NGSIZE)] = tmp;
     }}
     } /* acc kernel */
#if defined(USE_NVTX)
     nvtxRangeEnd(id2);
#endif

#if defined(USE_NVTX)
     nvtxRangeId_t id3 = nvtxRangeStartA("cpu_serial");
#endif
     for (int i = 0; i < NCSIZE; ++i) {
     for (int j = 0; j < NCSIZE; ++j) {
        c2[IDX(i,j,NCSIZE)] = 0.0f;
     }}

     for (int i = 0; i < NCSIZE; ++i) {
     for (int j = 0; j < NCSIZE; ++j) {
        float tmp = 0.0f;
	for (int k = 0; k <  NCSIZE; ++k ){
           tmp += a2[IDX(i,k,NCSIZE)]*b2[IDX(k,j,NCSIZE)]; 
	}
        c2[IDX(i,j,NCSIZE)] = tmp;
     }}
#if defined(USE_NVTX)
     nvtxRangeEnd(id3);
#endif
   } /* itr-loop */
   } /* acc data */
#if defined(USE_NVTX)
   nvtxRangeEnd(id1);
#endif

   /* dummy output */
   printf("%15.6f %15.6f\n",c[0], c2[0]);

   /* finalize */
   free( a );
   free( b );
   free( c );
   free( a2 );
   free( b2 );
   free( c2 );

   return EXIT_SUCCESS;
}
