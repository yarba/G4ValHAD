#!/usr/bin/env bash

dCache=/pnfs/geant4/persistent/g4p/cpu_and_mem_perf/pbs

TOP=${PWD}

TAG=${1}

dirs=( igprof sprof  osshwcsamp osshwcsamp2 osspcsamp ossusertime  )

for dv in ${TAG}_*; do
   if [ -d ${dv} ]; then
      cd ${dv}
      if [ ! -d ${dCache}/${dv} ]; then
         /bin/mkdir ${dCache}/${dv}
      fi
      #
      # plain files
      #
      for f in *.*; do
         /usr/bin/dccp ${f} ${dCache}/${dv}/.
      done
      #
      # igprof directory
      #
      for ((i=0; i<${#dirs[@]}; ++i )) do
      if [ -d ${dirs[$i]} ]; then
         cd ${dirs[$i]}
	 if [ ! -d ${dCache}/${dv}/${dirs[$i]} ]; then
	    /bin/mkdir ${dCache}/${dv}/${dirs[$i]}
	 fi
	 for dd in *; do
	    if [ -d ${dd} ]; then
	       cd ${dd}
	       if [ ! -d ${dCache}/${dv}/${dirs[$i]}/${dd} ]; then
	          /bin/mkdir ${dCache}/${dv}/${dirs[$i]}/${dd}
	       fi
	       for ff in *; do
	          /usr/bin/dccp ${ff} ${dCache}/${dv}/${dirs[$i]}/${dd}/.
	       done 
	       cd ..
	    fi
	 done
         cd ..
      fi
      done
      #
   fi
   cd ${TOP}
done
