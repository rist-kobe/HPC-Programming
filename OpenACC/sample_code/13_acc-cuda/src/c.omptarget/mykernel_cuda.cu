// Copyright 2024 Research Organization for Information Science and Technology 
#include <cstdio>
__global__ void mmp_cuda_kernel(int ns, double * mc, double * ma, double * mb)
{
   int i = threadIdx.y + blockIdx.y*blockDim.y;
   int j = threadIdx.x + blockIdx.x*blockDim.x;

   if ( j < ns && i < ns ) {
      double tmp = 0.0;
      for (int k = 0; k < ns; ++k ) {
          tmp += ma[k+i*ns]*mb[j+k*ns];
      }
      mc[j+i*ns] = tmp;
   }
}

extern "C" void mmp_cuda_wrapper(int ns, double * mc, double * ma, double * mb)
{
   dim3 block, grid; 

   block = dim3(16,16);
   grid  = dim3((ns+block.x-1)/block.x, (ns+block.y-1)/block.y);

#if 0
   printf("block: %d %d\n", block.x, block.y);
   printf("grid: %d %d\n", grid.x, grid.y);
#endif

   mmp_cuda_kernel<<<grid,block>>>(ns, mc, ma, mb);

   const cudaError_t error = cudaDeviceSynchronize();
   if ( error != cudaSuccess) {
      exit(1);
   }
}
