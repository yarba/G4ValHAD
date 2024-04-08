#!/usr/bin/env bash
#

#if [ "x" == "x$target" ] ; then
# bail out if NO TARGET !!!
#fi
#if [ "x" == "x$momentum" ] ; then
## bail out if NO MOMENTUM !!!
#fi

# SL7
# source /cvmfs/geant4-ib.opensciencegrid.org/products/setup
# setup root v6_24_06b -q e20:p399:prof

# setup G4 datasets
#
# --> no need --> source /wclustre/g4p/g4p/download/g4data/g4-datasets-${G4RELEASE}.sh

# setup workdir
#
if [ "x" == "x$G4WORKDIR" ] ; then
G4WORKDIR=${SLURM_SUBMIT_DIR}/..
else
    echo "Variable says: $G4WORKDIR"
    echo "Variable SLURM_SUBMIT_DIR says: $SLURM_SUBMIT_DIR"
fi

cd ${G4WORKDIR}

# ---> leftover from using pbs_multiCore_master.sh; 
# ---> now passed in as an explicit env.var. ---> nevents=${1}

# JobID=1
JobID=$((1+${SLURM_PROCID}))
seed=$((1234+${JobID}))

#bm=${beam}
bm=${1}
if [ "x${beam}" == "xpiplus" ]; then
   bm="pi+"
fi
if [ "x${beam}" == "xpiminus" ]; then
   bm="pi-"
fi

# config=${beam}.${target}.100000.SASM6E
config=${beam}.${2}.100000.SASM6E

nevents=${3}

if [ "x" == "x$JobID" ]; then
   echo "Process entire statistics in one job"
else
 config=${beam}.${target}.100000.SASM6E.${JobID}
fi

if [ -e ${G4WORKDIR}/${config} ]; then
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
/usr/bin/printf "//--------%s_processes \n" ${beam} >> ${config}
/usr/bin/printf "#particle \n" >> ${config}
/usr/bin/printf "%s\n" ${bm} >> ${config}
/usr/bin/printf "#isSASM6E \n#position(mm) \n0. 0. 0. \n#direction \n0. 0. 1. \n" >> ${config}

/usr/bin/printf "//-------- \n" >> ${config}

/usr/bin/printf "#material \n" >> ${config}
/usr/bin/printf "%s\n" ${target} >> ${config}

/usr/bin/printf "#momentum(MeV/c) \n" >> ${config}
/usr/bin/printf "100000 \n" >> ${config}

/usr/bin/printf "// --- \n#generator \n" >> ${config}
/usr/bin/printf "ftfp \n" >> ${config}
/usr/bin/printf "#run \n" >> ${config}
/usr/bin/printf "// --- \n#generator \n" >> ${config}
/usr/bin/printf "ftfp_tune3 \n" >> ${config}
/usr/bin/printf "#run \n" >> ${config}
/usr/bin/printf "// --- \n#generator \n" >> ${config}
/usr/bin/printf "qgsp \n" >> ${config}
/usr/bin/printf "#run \n" >> ${config}
/usr/bin/printf "// --- \n#generator \n" >> ${config}
/usr/bin/printf "qgsp-g4lund-str-fragm \n" >> ${config}
/usr/bin/printf "#run \n" >> ${config}

/usr/bin/printf "#exit\n" >> ${config}

# EL8

module load gcc/11.4.0

export HOME=/work1/g4p/g4p/products-el8
export SPACK_ROOT=/work1/g4p/g4p/products-el8/spack
. $SPACK_ROOT/share/spack/setup-env.sh
spack load root@6.26.06


./test19 ${config}

if [ -e ${config} ]; then
/bin/rm -f ${config}
fi

