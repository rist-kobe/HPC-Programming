#!/bin/bash
function blklst () {
cat <<EOF
32
64
128
192
256
1024
EOF
}

uname -a
nvidia-smi -L

EXE=../../../src/cuda/cuda-stream/stream
for bl in $(blklst); do
   time ${EXE} -b ${bl} &> out.${bl}
done
