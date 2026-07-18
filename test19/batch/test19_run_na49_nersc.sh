#!/usr/bin/env bash
#

# --> export G4INSTALL=/work1/g4v/yarba_j/geant4-local-builds/gcc-8.2.0/geant4-${G4RELEASE}

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

# LQ/EL8
#module load gcc/11.4.0
#export MY_SPACK_DIR=/project/Geant4/yarba_j/products/spack-products/spack
#. $MY_SPACK_DIR/share/spack/setup-env.sh
#spack load root@6.26.06
#
#FLUKABIN=/project/Geant4/yarba_j/geant4-local-builds/el8+gcc-11.4.0/FLUKA/4.4.0/fluka4-4.0/bin
#FLUKAHI=/project/Geant4/yarba_j/geant4-local-builds/el8+gcc-11.4.0/geant4-11-03-ref-00/examples/extended/hadronic/FlukaCern/FlukaInterface
#
# NERSC/gcc@13.3.0
#
export gcc_ver="13.3.0"
export SPACK_DIR=/global/common/software/m4599/geant4/products/spack-v0.19.0-products/spack
. ${SPACK_DIR}/share/spack/setup-env.sh
spack load gcc@${gcc_ver}
#
#FLUKABIN=${CFS}/m4599/Users/${USER}/geant4/hadronic-models/geant4-local-builds/gcc${gcc_ver}/FLUKA/fluka4-5.0/bin
#FLUKAHI=${CFS}/m4599/Users/${USER}/geant4/hadronic-models/geant4-local-builds/gcc${gcc_ver}/geant4-11-04-mt/examples/extended/hadronic/FlukaCern/FlukaInterface
#
#export PATH=${FLUKABIN}:${PATH}
#source ${FLUKAHI}/env_FLUKA_G4_interface.sh 

./test19 test19.na49



