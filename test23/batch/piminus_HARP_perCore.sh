#!/usr/bin/env bash
#

#if [ "x" == "x$target" ] ; then
# bail out if NO TARGET !!!
#fi
#if [ "x" == "x$momentum" ] ; then
## bail out if NO MOMENTUM !!!
#fi

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

nevents=${1}

# JobID=1
#JobID=${PBS_ARRAYID}
JobID=${2}
seed=$((1234+${JobID}))

config=piminus.${target}.${momentum}.HARP.${physlist}

#if [ "x" == "x$PBS_ARRAYID" ]; then
if [ "x" == "x$JobID" ]; then
   echo "Process entire statisticas in one job"
else
config=piminus.${target}.${momentum}.HARP.${physlist}.${JobID}
fi

if [ -f ${G4WORKDIR}/${config} ]; then
/bin/rm -f ${G4WORKDIR}/${config}
fi

/usr/bin/printf "#verbose \n-1 \n#rad \n" >> ${config}
/usr/bin/printf "#events \n" >> ${config}
/usr/bin/printf "%d\n" ${nevents} >> ${config}
#if [ "x" == "x$PBS_ARRAYID" ]; then
if [ "x" == "x$JobID" ]; then
 echo "skip seed and jobid settings"
else
/usr/bin/printf "#randomSeed\n" >> ${config}
/usr/bin/printf "%d\n" ${seed}  >> ${config}
/usr/bin/printf "#jobID\n" >> ${config}
/usr/bin/printf "%d\n" ${JobID} >> ${config}
fi
/usr/bin/printf "//--------piminus_processes \n" >> ${config}
/usr/bin/printf "#particle \npi- \n#isHARP \n#position(mm) \n0. 0. -100. \n#direction \n0. 0. 1. \n" >> ${config}

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

./test23 ${config}
/bin/rm ${config}
