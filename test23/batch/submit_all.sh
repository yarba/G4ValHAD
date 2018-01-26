#!/usr/bin/env bash
#

# 1st input arg (version) is mandatory;
# 2nd one (regression testing flag) is optional

g4version=${1}

if [ "x" == "x$g4version" ]; then
echo " you must specify Geant4 release/version; exit"
exit 3
fi

#source /products/setup
source /g4/g4p/pbs/g4-had-validation/build-scripts/g4_set_prod.sh

if [ `echo ${PATH} | grep pbs` ]; then
echo "PBS is already set"
else
export PATH=${PATH}:/usr/local/pbs/bin
fi

harp_list=`ls -l | grep HARP_perCore | awk '{print $9}'`

echo " harp_list = ${harp_list} ... "

# nevents=1000000
# --> nevents=31250
#
# --> split="-t 1-32"
# split=


for test in ${harp_list} ; do

qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v  App=${test},G4RELEASE=${g4version},target=C,momentum=3000,physlist=NuBeam  -A g4p pbs_multiCore_master.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v  App=${test},G4RELEASE=${g4version},target=C,momentum=5000,physlist=NuBeam  -A g4p pbs_multiCore_master.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v  App=${test},G4RELEASE=${g4version},target=C,momentum=8000,physlist=NuBeam  -A g4p pbs_multiCore_master.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v  App=${test},G4RELEASE=${g4version},target=C,momentum=12000,physlist=NuBeam -A g4p pbs_multiCore_master.sh

qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v  App=${test},G4RELEASE=${g4version},target=Be,momentum=3000,physlist=NuBeam  -A g4p pbs_multiCore_master.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v  App=${test},G4RELEASE=${g4version},target=Be,momentum=5000,physlist=NuBeam  -A g4p pbs_multiCore_master.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v  App=${test},G4RELEASE=${g4version},target=Be,momentum=8000,physlist=NuBeam  -A g4p pbs_multiCore_master.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v  App=${test},G4RELEASE=${g4version},target=Be,momentum=12000,physlist=NuBeam -A g4p pbs_multiCore_master.sh

qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v  App=${test},G4RELEASE=${g4version},target=C,momentum=3000,physlist=ftfp_bert  -A g4p pbs_multiCore_master.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v  App=${test},G4RELEASE=${g4version},target=C,momentum=5000,physlist=ftfp_bert  -A g4p pbs_multiCore_master.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v  App=${test},G4RELEASE=${g4version},target=C,momentum=8000,physlist=ftfp_bert  -A g4p pbs_multiCore_master.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v  App=${test},G4RELEASE=${g4version},target=C,momentum=12000,physlist=ftfp_bert -A g4p pbs_multiCore_master.sh

qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v  App=${test},G4RELEASE=${g4version},target=Be,momentum=3000,physlist=ftfp_bert  -A g4p pbs_multiCore_master.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v  App=${test},G4RELEASE=${g4version},target=Be,momentum=5000,physlist=ftfp_bert  -A g4p pbs_multiCore_master.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v  App=${test},G4RELEASE=${g4version},target=Be,momentum=8000,physlist=ftfp_bert  -A g4p pbs_multiCore_master.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v  App=${test},G4RELEASE=${g4version},target=Be,momentum=12000,physlist=ftfp_bert -A g4p pbs_multiCore_master.sh

done

#
# Hold off for now - apparently, there's some issue here as most of the jobs get killed (memory use limit ?),
# and those remaining run out of 48 hours of the requested wall time 
#
# Nov.9, 2015 - try to resume these tests since with 4.10.1.ref10 they went fine
#
# Apr.13, 2016 - after some more hiccups in 10.2 cycle (in radioactive decays...), Shielding(M) seem to be back on track; resume tests as of 4.10.2.ref03
#
qsub -q amd32_g4val -l nodes=1:slow,walltime=48:00:00 -v App=proton_HARP_perCore.sh,G4RELEASE=${g4version},target=Ta,momentum=8000,physlist=Shielding  -A g4p pbs_multiCore_master.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=48:00:00 -v App=proton_HARP_perCore.sh,G4RELEASE=${g4version},target=Ta,momentum=8000,physlist=ShieldingM -A g4p pbs_multiCore_master.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=48:00:00 -v App=proton_HARP_perCore.sh,G4RELEASE=${g4version},target=Ta,momentum=8000,physlist=qgsp_bert -A g4p pbs_multiCore_master.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=48:00:00 -v App=proton_HARP_perCore.sh,G4RELEASE=${g4version},target=Ta,momentum=8000,physlist=ftfp_bert -A g4p pbs_multiCore_master.sh

qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v  App=proton_HARP_perCore.sh,G4RELEASE=${g4version},target=Be,momentum=8900,physlist=NuBeam    -A g4p pbs_multiCore_master.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v  App=proton_HARP_perCore.sh,G4RELEASE=${g4version},target=Be,momentum=8900,physlist=ftfp_bert -A g4p pbs_multiCore_master.sh

qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v App=proton_NA61_perCore.sh,G4RELEASE=${g4version},target=C,momentum=31000,physlist=NuBeam    -A g4p pbs_multiCore_master.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v App=proton_NA61_perCore.sh,G4RELEASE=${g4version},target=C,momentum=31000,physlist=ftfp_bert -A g4p pbs_multiCore_master.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v App=proton_NA61_perCore.sh,G4RELEASE=${g4version},target=C,momentum=31000,physlist=qgsp_bert -A g4p pbs_multiCore_master.sh

qsub -q amd32_g4val -l nodes=1:slow,walltime=48:00:00 -v App=proton_NA49_perCore.sh,G4RELEASE=${g4version},target=C,momentum=158000,physlist=NuBeam    -A g4p pbs_multiCore_master.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=48:00:00 -v App=proton_NA49_perCore.sh,G4RELEASE=${g4version},target=C,momentum=158000,physlist=ftfp_bert -A g4p pbs_multiCore_master.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=48:00:00 -v App=proton_NA49_perCore.sh,G4RELEASE=${g4version},target=C,momentum=158000,physlist=qgsp_bert -A g4p pbs_multiCore_master.sh



