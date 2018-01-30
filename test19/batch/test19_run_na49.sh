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
    echo "Variable G4WORKDIR says: $G4WORKDIR"
    echo "Variable PBS_O_WORKDIR says: $PBS_O_WORKDIR"
fi

cd ${G4WORKDIR}

# JobID=1
#JobID=${PBS_ARRAYID}
#seed=$((1234+${JobID}))

./test19 test19.na49

export G4INSTALL=/g4/g4p/pbs/g4-had-validation/g4-releases/${G4RELEASE}

if [ -d ${G4INSTALL}/tests/DbReader/lib ]; then
export LD_LIBRARY_PATH=${G4INSTALL}/tests/DbReader/lib:${LD_LIBRARY_PATH}
if [ -e ${G4INSTALL}/tests/DbReader/lib/libDbReader.so ]; then
# ***
$ROOTSYS/bin/root -b -p -q NA49Models.C 
# ***
else
echo "DbReader package exists but is NOT built; missing library "
fi
else
echo " DbReader is NOT installed"
fi

if [ "x" == "x$G4REGRESSION" ]; then
   echo "Regression testing is NOT requested"
else
   echo "G4REGRESSION says: $G4REGRESSION"
$ROOTSYS/bin/root -b -p -q NA49Regre.C\(\)
fi
 
if [ "x" == "x$G4RELEASE" ] ; then
    echo "Variable G4RELEASE is not set"
else
#
. batch/copy_results.sh ${G4RELEASE} proton
#
if [ "x" == "x$G4REGRESSION" ]; then
   echo "NO regression plots"
else
# move regression plots to results area
. batch/copy_regression.sh ${G4RELEASE} proton na49-histo
#
fi
#
fi
