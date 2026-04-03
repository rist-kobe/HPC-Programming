// Copyright 2025 Research Organization for Information Science and Technology
// Reference: 
// * John Cheng, Max Grossman, and Ty McKercher, "Professional CUDA C Programming"
//   (2014) Chap.2.
#include <cstdio>
#include <cstdlib>
#include <cuda_runtime.h>

#define CHECK(call)                                                                 \
{                                                                                   \
   cudaError_t error = call;                                                        \
   if ( error != cudaSuccess )                                                      \
   {                                                                                \
      fprintf(stderr,"Error: %s:%d ", __FILE__, __LINE__ );                         \
      fprintf(stderr,"returned %d -> %s\n", (int)error, cudaGetErrorString(error)); \
      exit(EXIT_FAILURE);                                                           \
   }                                                                                \
}

int main (int argc, char **argv)
{
   int deviceCount = 0;
   CHECK(cudaGetDeviceCount(&deviceCount));

   if ( deviceCount == 0 ) {
      fprintf(stdout,"There is no available device.\n");
      return EXIT_SUCCESS;
   } else {
      fprintf(stdout,"%d CUDA Capable devices are detected.\n");
   }

   int dev = 0;
   int driverVersion = 0;
   int runtimeVersion = 0;
   cudaDeviceProp deviceProp;

   CHECK(cudaSetDevice(dev));
   CHECK(cudaGetDeviceProperties(&deviceProp, dev));
   fprintf(stdout,"Device %d: %s\n", dev, deviceProp.name);

   CHECK(cudaDriverGetVersion(&driverVersion));
   CHECK(cudaRuntimeGetVersion(&runtimeVersion));
   fprintf(stdout,"  CUDA Driver Version / Runtime Version          %d.%d / %d.%d\n",
           driverVersion / 1000, (driverVersion % 100) / 10,
           runtimeVersion / 1000, (runtimeVersion % 100) / 10);
   fprintf(stdout,"  CUDA Capability Major/Minor version number:    %d.%d\n",
           deviceProp.major, deviceProp.minor);
   fprintf(stdout,"  Total amount of global memory:                 %.2f Mbytes\n",
           static_cast<double>(deviceProp.totalGlobalMem) / (1048576.0));
   fprintf(stdout,"  GPU Clock rate:                                %.0f MHz "
           "(%0.2f GHz)\n", deviceProp.clockRate * 1.0e-3f,
           deviceProp.clockRate * 1.0e-6f);
   fprintf(stdout,"  Memory Clock rate:                             %.0f MHz\n",
           deviceProp.memoryClockRate * 1.0e-3f);
   fprintf(stdout,"  Memory Bus Width:                              %d-bit\n",
           deviceProp.memoryBusWidth);

   if ( deviceProp.l2CacheSize)
   {
   fprintf(stdout,"  L2 Cache Size:                                 %d bytes\n",
           deviceProp.l2CacheSize);
   }

   fprintf(stdout,"  Max Texture Dimension Size (x,y,z)             1D=(%d), "
           "2D=(%d,%d), 3D=(%d,%d,%d)\n", deviceProp.maxTexture1D,
           deviceProp.maxTexture2D[0], deviceProp.maxTexture2D[1],
           deviceProp.maxTexture3D[0], deviceProp.maxTexture3D[1],
           deviceProp.maxTexture3D[2]);
   fprintf(stdout,"  Max Layered Texture Size (dmim) x layers       1D=(%d) x %d, "
           "2D=(%d,%d) x %d\n", deviceProp.maxTexture1DLayered[0],
           deviceProp.maxTexture1DLayered[1], deviceProp.maxTexture2DLayered[0],
           deviceProp.maxTexture2DLayered[1],
           deviceProp.maxTexture2DLayered[2]);
   fprintf(stdout,"  Total amount of constant memory:               %lu bytes\n",
           deviceProp.totalConstMem);
   fprintf(stdout,"  Total amount of shared memory per block:       %lu bytes\n",
           deviceProp.sharedMemPerBlock);
   fprintf(stdout,"  Total number of registers available per block: %d\n",
           deviceProp.regsPerBlock);
   fprintf(stdout,"  Warp size:                                     %d\n",
           deviceProp.warpSize);
   fprintf(stdout,"  Maximum number of threads per multiprocessor:  %d\n",
           deviceProp.maxThreadsPerMultiProcessor);
   fprintf(stdout,"  Maximum number of threads per block:           %d\n",
           deviceProp.maxThreadsPerBlock);
   fprintf(stdout,"  Maximum sizes of each dimension of a block:    %d x %d x %d\n",
           deviceProp.maxThreadsDim[0],
           deviceProp.maxThreadsDim[1],
           deviceProp.maxThreadsDim[2]);
   fprintf(stdout,"  Maximum sizes of each dimension of a grid:     %d x %d x %d\n",
           deviceProp.maxGridSize[0],
           deviceProp.maxGridSize[1],
           deviceProp.maxGridSize[2]);
   fprintf(stdout,"  Maximum memory pitch:                          %lu bytes\n",
           deviceProp.memPitch);

   return EXIT_SUCCESS;
}
