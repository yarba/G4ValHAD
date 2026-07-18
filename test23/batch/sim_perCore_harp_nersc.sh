#!/usr/bin/env bash
#

# setup workdir
#
if [ "x" == "x$G4WORKDIR" ] ; then
G4WORKDIR=${SLURM_SUBMIT_DIR}/..
else
    echo "Variable says: $G4WORKDIR"
    echo "Variable SLURM_SUBMIT_DIR says: $SLURM_SUBMIT_DIR"
fi

cd ${G4WORKDIR}

beam=${1}
target=${2}
momentum=${3}
physlist=${4}

EXP=HARP

echo "SLURM_NPROCS = ${SLURM_NPROCS}"
echo "SLURM_NTASKS = ${SLURM_NTASKS}"
nevents=$((1000000/${SLURM_NPROCS}))
echo " nevents = ${nevents} "

# JobID=1
JobID=$((1+${SLURM_PROCID}))
seed=$((1234+${JobID}))

bm=${beam}
if [ "x${beam}" == "xpiplus" ]; then
   bm="pi+"
fi
if [ "x${beam}" == "xpiminus" ]; then
   bm="pi-"
fi

config=${beam}.${target}.${momentum}.${EXP}.${physlist}

if [ "x" == "x$JobID" ]; then
   echo "Process entire statistics in one job"
else
 config=${beam}.${target}.${momentum}.${EXP}.${physlist}.${JobID}
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
/usr/bin/printf "//-------- \n" >> ${config}
/usr/bin/printf "#particle \n" >> ${config}
/usr/bin/printf "%s\n" ${bm} >> ${config}
/usr/bin/printf "#is%s\n" ${EXP} >> ${config}
/usr/bin/printf "#position(mm) \n0. 0. -100. \n#direction \n0. 0. 1. \n" >> ${config}

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

# gcc12.3.0
#
#export PATH=/global/common/software/m4599//geant4/local_settings_compiler:${PATH}
#. $HOME/products/spack-products/spack/share/spack/setup-env.sh

# gcc@13.3.0
#
export gcc_ver="13.3.0"
export SPACK_DIR=/global/common/software/m4599/geant4/products/spack-v0.19.0-products/spack
. ${SPACK_DIR}/share/spack/setup-env.sh
spack load gcc@${gcc_ver}

# this is still current, no matter what compiler and/or spack install location !
#
spack load root@6.26.06%gcc@${gcc_ver}

if [ "x${physlist}" == "xG4_HP_CFLUKAHI" ]; then
FLUKABIN=${CFS}/m4599/Users/${USER}/geant4/hadronic-models/geant4-local-builds/gcc${gcc_ver}/FLUKA/fluka4-5.0/bin
FLUKAHI=${CFS}/m4599/Users/${USER}/geant4/hadronic-models/geant4-local-builds/gcc${gcc_ver}/geant4-11-03-ref-04/examples/extended/hadronic/FlukaCern/FlukaInterface
export PATH=${FLUKABIN}:${PATH}
source ${FLUKAHI}/env_FLUKA_G4_interface.sh 
fi

./test23 ${config}

#if [ -e ${config} ]; then
#/bin/rm -f ${config}
#fi
