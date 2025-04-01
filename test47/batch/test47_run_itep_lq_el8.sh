#!/usr/bin/env bash
#

# setup G4 datasets
#
# --> no need --> source /wclustre/g4p/g4p/download/g4data/g4-datasets-${G4RELEASE}.sh

echo "Variable SLURM_SUBMIT_DIR says: $SLURM_SUBMIT_DIR"

# setup workdir
#
if [ "x" == "x$G4WORKDIR" ] ; then
G4WORKDIR=${SLURM_SUBMIT_DIR}/..
else
    echo "Variable says: $G4WORKDIR"
    echo "Variable SLURM_SUBMIT_DIR says: $SLURM_SUBMIT_DIR"
fi

# --->

JobID=$((1+${SLURM_PROCID}))

beam=${1}

target_list=( C Cu Pb U )
mom_list=( 1400 5000 )
if [ "x${beam}" == "xproton" ]; then
mom_list=( 1400 7500 )
fi

ntgts=${#target_list[@]}
nmoms=${#mom_list[@]}

njobsmax=$((${ntgts}*${nmoms}))

# Note: see earlier that JobID = SLURM_PROCID + 1
#
if [ ${JobID} -gt ${njobsmax} ]; then
exit
fi

itgt=$((${SLURM_PROCID}/${nmoms}))
imom=$((${SLURM_PROCID} % ${nmoms}))
target=${target_list[$itgt]}
momentum=${mom_list[$imom]}

# --->

cd ${G4WORKDIR}

config=test47.itep.${beam}.${target}.${momentum}

if [ -e ${G4WORKDIR}/${config} ]; then
/bin/rm -f ${G4WORKDIR}/${config}
fi

bm=${beam}
if [ "x${beam}" == "xpiplus" ]; then
   bm="pi+"
fi
if [ "x${beam}" == "xpiminus" ]; then
   bm="pi-"
fi

mom=$((${momentum}/1000))

/usr/bin/printf "#verbose \n-1 \n#rad \n" >> ${config}
/usr/bin/printf "#events \n" >> ${config}
/usr/bin/printf "1000000 \n" >> ${config}

/usr/bin/printf "//--------%s_processes \n" ${beam} >> ${config}
/usr/bin/printf "#particle \n" >> ${config}
/usr/bin/printf "%s\n" ${bm} >> ${config}
/usr/bin/printf "#isITEP \n#position(mm) \n0. 0. 0. \n#direction \n0. 0. 1. \n" >> ${config}

/usr/bin/printf "//-------- \n" >> ${config}

/usr/bin/printf "#material \n" >> ${config}
/usr/bin/printf "%s\n" ${target} >> ${config}

/usr/bin/printf "#momentum(MeV/c) \n" >> ${config}
/usr/bin/printf "%d\n" ${momentum} >> ${config}

#/usr/bin/printf "// --- \n#generator \n" >> ${config}
#/usr/bin/printf "bertini \n" >> ${config}
#/usr/bin/printf "#run \n" >> ${config}
#
##/usr/bin/printf "// --- \n#generator \n" >> ${config}
##/usr/bin/printf "inclxx \n" >> ${config}
##/usr/bin/printf "#run \n" >> ${config}
#
if [ "${momentum}" -gt "3000" ]; then
/usr/bin/printf "// --- \n#generator \n" >> ${config}
/usr/bin/printf "ftfp \n" >> ${config}
/usr/bin/printf "#run \n" >> ${config}
/usr/bin/printf "// --- \n#generator \n" >> ${config}
/usr/bin/printf "ftfp_tune3 \n" >> ${config}
/usr/bin/printf "#run \n" >> ${config}
fi
if [ "x${2}" == "xwith-fluka.cern.4.4.0" ]; then
/usr/bin/printf "// --- \n#generator \n" >> ${config}
/usr/bin/printf "fluka.cern.4.4.0 \n" >> ${config}
/usr/bin/printf "#run \n" >> ${config}
fi

/usr/bin/printf "#exit\n" >> ${config}

module load gcc/11.4.0

export MY_SPACK_DIR=/project/Geant4/yarba_j/products/spack-products/spack
. $MY_SPACK_DIR/share/spack/setup-env.sh
spack load root@6.26.06

if [ "x${2}" == "xwith-fluka.cern.4.4.0" ]; then
FLUKABIN=/project/Geant4/yarba_j/geant4-local-builds/el8+gcc-11.4.0/FLUKA/4.4.0/fluka4-4.0/bin
FLUKAHI=/project/Geant4/yarba_j/geant4-local-builds/el8+gcc-11.4.0/geant4-11-03-ref-00/examples/extended/hadronic/FlukaCern/FlukaInterface
export PATH=${FLUKABIN}:${PATH}
source ${FLUKAHI}/env_FLUKA_G4_interface.sh 
fi

./test47 ${config}

#if [ -e ${config} ]; then
#/bin/rm -f ${config}
#fi


