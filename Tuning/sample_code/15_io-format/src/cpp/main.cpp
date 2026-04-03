// Copyright 2024 Research Organization for Information Science and Technology
#include <iostream>
#include <fstream>
#include <ios>
#include <iomanip>
#include <cstdlib>
#include "memory.h"
#include "timer.h"

int main(int argc, char **argv)
{
  const int nx=1024; // 2^10
  const int ny=1024; // 2^10
  int nz;
  int i, j, k;
  int ifmt;
  int aasize;

  double elp, cpu, elp0;
  double aamemsize;

  if ( argc != 3 ) {
     std::cout << "[usage] ./run.x (arg1) (arg2)                 \n"
               << "   (arg1): Integer (0-1)                      \n"
	       << "           0: write w/ format                 \n"
	       << "           1: write w/o format                \n"
	       << "   (arg2): Integer (<200) to change array size\n";
     return EXIT_SUCCESS;
  }

  ifmt = atoi(argv[1]);
  nz = atoi(argv[2]);

  if ( nz > 200 ) {
     std::cout << "[Error] 2nd arg must be < 200." << std::endl;
     return EXIT_FAILURE;
  }

  int ***aa;
  create_3d_array( aa, nx, ny, nz);

  for ( i=0; i<nx; ++i ) {
  for ( j=0; j<ny; ++j ) {
  for ( k=0; k<nz; ++k ) {
      aa[i][j][k] = k + (j + i*ny)*nz;
  }}}

  aasize = nx*ny*nz;
  aamemsize = 4.0*nx*ny*nz / 1048576; // MiB

  //-----------------------------------------------------------------
  //  write with format
  //-----------------------------------------------------------------
  if ( ifmt == 0 ) {

    std::ofstream ofs1("data1.out");
    elp0 = get_elp_time();
    for ( i=0; i<nx; ++i ) {
    for ( j=0; j<ny; ++j ) {
    for ( k=0; k<nz; ++k ) {
       ofs1 << std::dec 
            << aa[i][j][k] << std::endl;
    }}}
    elp = get_elp_time() - elp0;
    ofs1.close();

    std::ofstream ofs2("data2.out");
    elp0 = get_cpu_time();
    for ( i=0; i<nx; ++i ) {
    for ( j=0; j<ny; ++j ) {
    for ( k=0; k<nz; ++k ) {
       ofs2 << std::dec 
            << aa[i][j][k] << std::endl;
    }}}
    cpu = get_cpu_time() - elp0;
    ofs2.close();

  //-----------------------------------------------------------------
  // write without format
  //-----------------------------------------------------------------
  } else if ( ifmt == 1 ) {

    std::ofstream ofs1("data1.out",std::ios_base::binary);
    elp0 = get_elp_time();
    ofs1.write(reinterpret_cast<const char*>(&aa[0][0][0]), aasize*sizeof(int));
    elp = get_elp_time() - elp0;
    ofs1.close();

    std::ofstream ofs2("data2.out",std::ios_base::binary);
    elp0 = get_cpu_time();
    ofs2.write(reinterpret_cast<const char*>(&aa[0][0][0]), aasize*sizeof(int));
    cpu = get_cpu_time() - elp0;
    ofs2.close();

  } else {
    std::cout <<"[Error] No routine. Check command-line options" << std::endl;
    release_3d_array(aa);
    return EXIT_FAILURE;
  }

  std::cout << "format memory_MiB elapsed_sec cpu_time_sec diff\n"
            << std::dec 
            << std::fixed
	    << std::setw(3)
	    << ifmt << " "
	    << std::setw(10)
	    << std::setprecision(3) 
	    << aamemsize << " "
	    << elp << " "
	    << cpu << " " 
	    << elp - cpu << "\n";

  release_3d_array(aa);

  return EXIT_SUCCESS;
}
