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

config=muminus.${target}

# JobID=1
#JobID=${PBS_ARRAYID}
#seed=$((1234+${JobID}))


printf "#verbose \n-1 \n#rad \n" >> ${config}
printf "#events \n1000000 \n" >> ${config}

#printf "#randomSeed\n" >> ${config}
#printf "%d\n" ${seed}  >> ${config}
#printf "#jobID\n" >> ${config}
#printf "%d\n" ${JobID} >> ${config}

printf "//--------Muminus_processes \n" >> ${config}
printf "#particle \nmu- \n#position(mm) \n0. 0. 0. \n#direction \n0. 0. 1. \n//-------- \n" >> ${config}

printf "#material \n${target} \n" >> ${config}

printf "#momentum(MeV/c) \n0.0 \n" >> ${config}

printf "// --- \n" >> ${config}

# --> printf "generator \nstopping \n#run \n" >> ${config}
printf "#generator \ncaptureUpdate \n#run \n" >> ${config}

printf "#exit\n" >> ${config}


#./test48 test48.muminus
./test48 ${config}

$ROOTSYS/bin/root -b -p -q MuMinusModels.C\(\"${target}\"\)  
####$ROOTSYS/bin/root -b -p -q PlotGammaN.C\(\) 
 
if [ "x" == "x$G4RELEASE" ] ; then
    echo "Variable G4RELEASE is not set"
else
. batch/copy_results.sh ${G4RELEASE}
fi

/bin/rm -r ${config}
