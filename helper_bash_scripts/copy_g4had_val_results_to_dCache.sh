#!/usr/bin/env bash

dCache=/pnfs/geant4/persistent/g4p/g4-had-validation/results

TOP=${PWD}

for dv in *; do
   if [ -d ${dv} ]; then
      cd ${dv}
      if [ ! -d ${dCache}/${dv} ]; then
         /bin/mkdir ${dCache}/${dv}
      fi
      # echo ${PWD}
      # /bin/ls -alF
      for dt in *; do
         if [ -d ${dt} ]; then
	    cd ${dt}
	    if [ ! -d ${dCache}/${dv}/${dt} ]; then
	       /bin/mkdir ${dCache}/${dv}/${dt}
	    fi
	    if [ -d models ]; then
	       cd models
	       if [ ! -d ${dCache}/${dv}/${dt}/models ]; then
	          /bin/mkdir ${dCache}/${dv}/${dt}/models
	       fi
	       for f in *.gif; do
	          /usr/bin/dccp $f ${dCache}/${dv}/${dt}/models/.
	       done
	       cd .. 
	    fi
	    if [ -d regre ]; then
	       cd regre
	       if [ ! -d ${dCache}/${dv}/${dt}/regre ]; then
	          /bin/mkdir ${dCache}/${dv}/${dt}/regre
	       fi
	       for f in *.gif; do
	          /usr/bin/dccp $f ${dCache}/${dv}/${dt}/regre/.
	       done
	       cd ..
	    fi
	    if [ -d sasm6e-models ]; then
	       cd sasm6e-models
	       if [ ! -d ${dCache}/${dv}/${dt}/sasm6e-models ]; then
	          /bin/mkdir ${dCache}/${dv}/${dt}/sasm6e-models
	       fi
	       for f in *.gif; do
	          /usr/bin/dccp $f ${dCache}/${dv}/${dt}/sasm6e-models/.
	       done
	       cd ..
	    fi
	    if [ -d sasm6e-regre ]; then
	       cd sasm6e-regre
	       if [ ! -d ${dCache}/${dv}/${dt}/sasm6e-regre ]; then
	          /bin/mkdir ${dCache}/${dv}/${dt}/sasm6e-regre
	       fi
	       for f in *.gif; do
	          /usr/bin/dccp $f ${dCache}/${dv}/${dt}/sasm6e-regre/.
	       done
	       cd ..
	    fi
	    if [ -d pbar-prod-models ]; then
	       cd pbar-prod-models
	       if [ ! -d ${dCache}/${dv}/${dt}/pbar-prod-models ]; then
	          /bin/mkdir ${dCache}/${dv}/${dt}/pbar-prod-models
	       fi
	       for f in *.gif; do
	          /usr/bin/dccp $f ${dCache}/${dv}/${dt}/pbar-prod-models/.
	       done
	       cd ..
	    fi
	    if [ -d pbar-prod-regre ]; then
	       cd pbar-prod-regre
	       if [ ! -d ${dCache}/${dv}/${dt}/pbar-prod-regre ]; then
	          /bin/mkdir ${dCache}/${dv}/${dt}/pbar-prod-regre
	       fi
	       for f in *.gif; do
	          /usr/bin/dccp $f ${dCache}/${dv}/${dt}/pbar-prod-regre/.
	       done
	       cd ..
	    fi	       
	    cd ..
	 fi
      done
   fi
   cd ${TOP}
done
