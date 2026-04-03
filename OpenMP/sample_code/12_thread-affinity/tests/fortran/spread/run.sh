#!/bin/bash
# Copyright 2026 Research Organization for Information Science and Technology 
# Input environment variables
#NTHREADS: Number of threads
#TAFF: Thread affinity



# Executable
EXE=../../../src/stream.v5/stream_f.exe
LOGFILE=omp${NTHREADS}

# OpenMP
export OMP_NUM_THREADS=${NTHREADS}
export OMP_STACKSIZE=512M

# Thread affinity and memory bind
if [ ${TAFF} = "close" ] ; then
   export OMP_PLACES=cores 
   #export OMP_PLACES=threads # if hardware threads are active
   export OMP_PROC_BIND=close
   #NMAC=$(echo "numactl --localalloc ") 
elif [ ${TAFF} = "spread" ] ; then
   export OMP_PLACES=cores 
   #export OMP_PLACES=threads # if hardware threads are active
   export OMP_PROC_BIND=spread
   #NMAC=$(echo "numactl --interleave=all ")
elif [ ${TAFF} = "customize" ] ; then
   # Example: We consider the following CPU.
   # * 2 NUMA nodes
   # * The number of physical cores per NUMA node is 3.
   # * The number of hardware threads per physical core is 2.
   # * (Logical) Core IDs in NUMA node 0:  0,  1,  2,  6,  7,  8.   
   # * (Logical) Core IDs in NUMA node 1:  3,  4,  5,  9, 10, 11.   
   # * The number of threads is 16 with a scatter setting:
   #   * Threads 0 (1) bind to either 0 or 6 in logical core IDs. 
   #   * Threads 2 (3) bind to either 3 or 9 in logical core IDs. 
   #   * Threads 4 (5) bind to either 1 or 7 in logical core IDs. 
   #   * Threads 6 (7) bind to either 4 or 10 in logical core IDs. 
   #   * Threads 8 (9) bind to either 2 or 8 in logical core IDs. 
   #   * Threads 10 (11) bind to either 5 or 11 in logical core IDs. 
   export OMP_PLACES="{0,6},{3,9},{1,7},{4,10},{2,8},{5,11}"
   #export OMP_PLACES="{0:2:6},{3:2:6},{1:2:6},{4:2:6},{2:2:6},{5:2:6}" # the same as the above OMP_PLACES.
   export OMP_PROC_BIND=true
   #NMAC=$(echo "numactl --interleave=all ")
else
  # default
  export OMP_PLACES=threads
  export OMP_PROC_BIND=close
fi

# Obtain affinity setting. (>= OpenMP v5.0)
export OMP_DISPLAY_AFFINITY=TRUE
export OMP_AFFINITY_FORMAT= "thread_num=%5n thread_affinity=%5A"

ulimit -s unlimited

${NMAC} ${EXE} > ${LOGFILE}
