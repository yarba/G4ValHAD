#!/usr/bin/env bash
#

source /products/setup
setup root v5_34_01 -q "e2:prof"

# setup G4 datasets
#
source /home/g4p/pbs/g4-had-validation/env-setup/g4-datasets-setup.sh

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
JobID=${PBS_ARRAYID}
seed=$((1234+${JobID}))

printf "#verbose \n0 \n#rad \n" >> ITEP.piminus.${JobID}
printf "#events \n31250 \n" >> ITEP.piminus.${JobID}
printf "#randomSeed\n" >> ITEP.piminus.${JobID}
printf "%d\n" ${seed}  >> ITEP.piminus.${JobID}
printf "#jobID\n" >> ITEP.piminus.${JobID}
printf "%d\n" ${JobID} >> ITEP.piminus.${JobID}
printf "//--------Piplus_processes \n" >> ITEP.piminus.${JobID}
printf "#particle \npi- \n#isITEP \n#position(mm) \n0. 0. 0. \n#direction \n0. 0. 1. \n//-------- \n" >> ITEP.piminus.${JobID}

printf "//-------- \n" >> ITEP.piminus.${JobID}
printf "#material \nC \n" >> ITEP.piminus.${JobID}
printf "#momentum(MeV/c) \n1400.0 \n" >> ITEP.piminus.${JobID}
printf "// --- \n#generator \nbertini \n#run \n// --- \n#generator \nbinary \n#run \n" >> ITEP.piminus.${JobID}

printf "//-------- \n" >> ITEP.piminus.${JobID}
printf "#momentum(MeV/c) \n5000.0 \n" >> ITEP.piminus.${JobID}
printf "// --- \n#generator \nbertini \n#run \n// --- \n#generator \nftfp \n#run \n" >> ITEP.piminus.${JobID}

printf "//-------- \n" >> ITEP.piminus.${JobID}
printf "#material \nU \n" >> ITEP.piminus.${JobID}
printf "#momentum(MeV/c) \n1400.0 \n" >> ITEP.piminus.${JobID}
printf "// --- \n#generator \nbertini \n#run \n// --- \n#generator \nbinary \n#run \n" >> ITEP.piminus.${JobID}

printf "//-------- \n" >> ITEP.piminus.${JobID}
printf "#momentum(MeV/c) \n5000.0 \n" >> ITEP.piminus.${JobID}
printf "// --- \n#generator \nbertini \n#run \n// --- \n#generator \nftfp \n#run \n" >> ITEP.piminus.${JobID}

printf "#exit\n" >> ITEP.piminus.${JobID}


${G4EXE}/test47 ITEP.piminus.${JobID}
