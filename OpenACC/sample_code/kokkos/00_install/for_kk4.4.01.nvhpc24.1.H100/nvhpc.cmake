set(CMAKE_CXX_COMPILER "nvc++" CACHE FILEPATH "" FORCE)

# CUDA (for GPU package)
# -ccbin is set by nvc++ (default is CMAKE_C_COMPILER in CMake)
set(CUDA_HOST_COMPILER "nvc++" CACHE FILEPATH "" FORCE)

# Other compilers
set(CMAKE_C_COMPILER "nvc" CACHE STRING "" FORCE)
set(CMAKE_Fortran_COMPILER "nvfortran" CACHE STRING "" FORCE)
set(OpenMP_CXX "nvc++" CACHE STRING "" FORCE)
set(OpenMP_CXX_FLAGS "-mp=multicore" CACHE STRING "" FORCE)
set(OpenMP_C "nvc" CACHE STRING "" FORCE)
set(OpenMP_C_FLAGS "-mp=multicore" CACHE STRING "" FORCE)
set(OpenMP_Fortran "nvfortran" CACHE STRING "" FORCE)
set(OpenMP_Fortran_FLAGS "-mp=multicore" CACHE STRING "" FORCE)
unset(HAVE_OMP_H_INCLUDE CACHE)

# Flags
set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "" FORCE)
set(CMAKE_CXX_STANDARD 17)
#set(MINFO_VALUES "-Minfo=loop -Minfo=opt -Minfo=vect -Minfo=accel")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -fPIC -O3 -mp=multicore -tp=zen3" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-g -fPIC -c11 -O3 -mp=multicore -tp=zen3" CACHE STRING "" FORCE)
set(CMAKE_Fortran_FLAGS_RELWITHDEBINFO "-g -fPIC -O3 -mp=multicore -tp=zen3" CACHE STRING "" FORCE)
set(CMAKE_SHARED_LINKER_FLAGS "-g -fPIC -cuda -fortranlibs" CACHE STRING "" FORCE)
#set(CMAKE_SHARED_LINKER_FLAGS "-fPIC -Wl,-rpath,:" CACHE STRING "" FORCE)
