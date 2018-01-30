#!/usr/bin/env bash
#

#source /products/setup
#setup root v5_34_01 -q "e2:prof"
source /g4/g4p/pbs/g4-had-validation/build-scripts/g4_set_prod.sh

# setup G4 datasets
#
#source /home/g4p/pbs/g4-had-validation/env-setup/g4-datasets-setup-${G4RELEASE}.sh
source /g4/g4p/pbs/g4-had-validation/env-setup/g4-datasets-setup-${G4RELEASE}.sh


# setup workdir
#
if [ "x" == "x$G4WORKDIR" ] ; then
G4WORKDIR=${PBS_O_WORKDIR}/.. 
else
    echo "Variable says: $G4WORKDIR"
    echo "Variable PBS_O_WORKDIR says: $PBS_O_WORKDIR"
fi

cd ${G4WORKDIR}

# JobID=1
#JobID=${PBS_ARRAYID}
#seed=$((1234+${JobID}))

./test47 test47pim.itep

# $ROOTSYS/bin/root -b -p -q Plot.C\($index\)  
$ROOTSYS/bin/root -b -p -q ITEPModels.C\(\"piminus\"\) 
if [ "x" == "x$G4REGRESSION" ]; then
   echo "Regression testing is NOT requested"
else
   echo "G4REGRESSION says: $G4REGRESSION"
$ROOTSYS/bin/root -b -p -q ITEPRegre.C\(\"piminus\"\)
fi

if [ "x" == "x$G4RELEASE" ] ; then
    echo "Variable G4RELEASE is not set"
else
#
. barch/copy_results.sh ${G4RELEASE} piminus
#
if [ "x" == "x$G4REGRESSION" ]; then
   echo "NO regression plots"
else
# move regression plots to results area
. batch/copy_regression.sh ${G4RELEASE} piminus
#
fi
#
fi
