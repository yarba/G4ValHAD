#!/usr/bin/env bash
#

#if [ "x" == "x$target" ] ; then
# bail out if NO TARGET !!!
#fi
#if [ "x" == "x$momentum" ] ; then
## bail out if NO MOMENTUM !!!
#fi

source /g4/g4p/pbs/g4-had-validation/build-scripts/g4_set_prod.sh

# setup G4 datasets
#
source /g4/g4p/pbs/g4-had-validation/env-setup/g4-datasets-setup-${G4RELEASE}.sh

# setup workdir
#
if [ "x" == "x$G4WORKDIR" ] ; then
# ---> PBS/obsolete ---> G4WORKDIR=${PBS_O_WORKDIR}/.. 
G4WORKDIR=${SLURM_SUBMIT_DIR}/..
else
    echo "Variable says: $G4WORKDIR"
    # ---> PBS/obsolete ---> echo "Variable PBS_O_WORKDIR says: $PBS_O_WORKDIR"
    echo "Variable SLURM_SUBMIT_DIR says: $SLURM_SUBMIT_DIR"
fi

cd ${G4WORKDIR}

# ---> leftover from using pbs_multiCore_master.sh; 
# ---> now passed in as an explicit env.var. ---> nevents=${1}

# JobID=1
# JobID=${PBS_ARRAYID}
# ---> leftover from using pbs_multiCore_master.sh ---> JobID=${2}
JobID=${SLURM_ARRAY_TASK_ID}
seed=$((1234+${JobID}))

config=piplus.${target}.${momentum}.HARP.${physlist}

if [ "x" == "x$JobID" ]; then
   echo "Process entire statisticas in one job"
else
config=piplus.${target}.${momentum}.HARP.${physlist}.${JobID}
fi

if [ -f ${G4WORKDIR}/${config} ]; then
/bin/rm -f ${G4WORKDIR}/${config}
fi

/usr/bin/printf "#verbose \n-1 \n#rad \n" >> ${config}
/usr/bin/printf "#events \n" >> ${config}
/usr/bin/printf "%d\n" ${nevents} >> ${config}
if [ "x" == "x$JobID" ]; then
  echo "skip seed and jobid settings"
else
/usr/bin/printf "#randomSeed\n" >> ${config}
/usr/bin/printf "%d\n" ${seed}  >> ${config}
/usr/bin/printf "#jobID\n" >> ${config}
/usr/bin/printf "%d\n" ${JobID} >> ${config}
fi
/usr/bin/printf "//--------piplus_processes \n" >> ${config}
/usr/bin/printf "#particle \npi+ \n#isHARP \n#position(mm) \n0. 0. -100. \n#direction \n0. 0. 1. \n" >> ${config}

/usr/bin/printf "//-------- \n" >> ${config}

/usr/bin/printf "#target-geom(mm) \n0.0 3.15 160. G4Tubs \n" >> ${config}
/usr/bin/printf "#material \n" >> ${config}
/usr/bin/printf "%s\n" ${target} >> ${config}

/usr/bin/printf "#momentum(MeV/c) \n" >> ${config}
/usr/bin/printf "%d\n" ${momentum} >> ${config}

/usr/bin/printf "// --- \n#physicslist \n" >> ${config}
/usr/bin/printf "%s\n" ${physlist} >> ${config}
/usr/bin/printf "#run \n" >> ${config}

/usr/bin/printf "#exit\n" >> ${config}


./test23 ${config}
/bin/rm -f ${config}
