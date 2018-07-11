#!/usr/bin/env bash
#

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
    echo "Variable G4WORKDIR says: $G4WORKDIR"
    # ---> PBS/obsolete ---> echo "Variable PBS_O_WORKDIR says: $PBS_O_WORKDIR"
    echo "Variable SLURM_SUBMIT_DIR says: $SLURM_SUBMIT_DIR"
fi

cd ${G4WORKDIR}

# JobID=1
#JobID=${PBS_ARRAYID} # ---> would now be: SLURM_ARRAY_TASK_ID
#seed=$((1234+${JobID}))

./test75 test75.gamma

$ROOTSYS/bin/root -b -p -q GammaN.C\(\)
if [ "x" == "x$G4REGRESSION" ]; then
   echo "Regression testing is NOT requested"
else
   echo "G4REGRESSION says: $G4REGRESSION"
$ROOTSYS/bin/root -b -p -q GammaNRegre.C\(\)
fi
 
if [ "x" == "x$G4RELEASE" ] ; then
    echo "Variable G4RELEASE is not set"
else
#
. batch/copy_results.sh ${G4RELEASE}
#
fi
if [ "x" == "x$G4REGRESSION" ]; then
   echo "NO regression plots"
else
# move regression plots to results area
. batch/copy_regression.sh ${G4RELEASE}
#
fi
