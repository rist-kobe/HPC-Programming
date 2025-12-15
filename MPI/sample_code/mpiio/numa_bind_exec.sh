export PATH=${PATH}:${2}
NUMA_NODE_NUM=$((${PMI_RANK}%${1}))
numactl --cpunodebind=${NUMA_NODE_NUM} --membind=${NUMA_NODE_NUM} ${3}
