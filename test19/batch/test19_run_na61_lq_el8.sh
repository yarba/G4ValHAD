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

cd ${G4WORKDIR}


# EL8

module load gcc/11.4.0

export MY_SPACK_DIR=/project/Geant4/yarba_j/products/spack-products/spack
. $MY_SPACK_DIR/share/spack/setup-env.sh
spack load root@6.26.06

FLUKABIN=/project/Geant4/yarba_j/geant4-local-builds/el8+gcc-11.4.0/FLUKA/4.4.0/fluka4-4.0/bin
FLUKAHI=/project/Geant4/yarba_j/geant4-local-builds/el8+gcc-11.4.0/geant4-11-03-ref-00/examples/extended/hadronic/FlukaCern/FlukaInterface
export PATH=${FLUKABIN}:${PATH}
source ${FLUKAHI}/env_FLUKA_G4_interface.sh 

beam=${1}
./test19 test19.na61.${beam}



