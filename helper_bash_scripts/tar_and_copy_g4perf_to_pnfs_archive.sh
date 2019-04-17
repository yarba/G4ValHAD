#!/usr/bin/env bash

dCache=/pnfs/geant4/archive/g4p/cpu_memory_profiling/pbs

TAG=${1}

for dv in ${TAG}_*; do
   if [ -d ${dv} ]; then
      echo  "tarball = ${dv}.tar "
      /bin/tar -cf ${dv}.tar ${dv}
# ---> not clear if this buys anything... so just do a plain tar      /bin/gzip ${dv}.tar
      /usr/bin/dccp ${dv}.tar ${dCache}/.
      /bin/rm ${dv}.tar
   fi
done
