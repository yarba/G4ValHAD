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

#
# cleanup logs/err from earlier rounds
#
/bin/rm -f *.sh.o*
/bin/rm -f *.sh.e*

# ---> test_list=`ls -l | grep run | awk '{print $9}'`
test_list=`ls -l | grep harp | awk '{print $9}'`

target=(C Be Ta)
momentum=(3000 5000 8000 12000)

for test in ${test_list} ; do
for (( i=0; i<${#target[@]}; ++i )) do
for (( j=0; j<${#momentum[@]}; ++j )) do
   echo "... Processing ... ${test} ... for ${1} for target ${target[$i]} and momentum ${momentum[$j]}"
#   qsub -q grunt -v G4RELEASE=${g4version},G4REGRESSION=${2} ${test}
# -->   qsub -q amd32_Geant4 -l nodes=1:amd32,walltime=60:00:00 -v G4RELEASE=${g4version},G4REGRESSION=${2},target=${target[$i]},momentum=${momentum[$j]} -A g4p ${test}
   qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v G4RELEASE=${g4version},G4REGRESSION=${2},target=${target[$i]},momentum=${momentum[$j]} -A g4p ${test}
done
done
done

# special case
#
# --> qsub -q amd32_Geant4 -l nodes=1:amd32,walltime=60:00:00 -v G4RELEASE=${g4version},G4REGRESSION=${2},target=Be,momentum=8900 -A g4p test19_run_harp_proton.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v G4RELEASE=${g4version},G4REGRESSION=${2},target=Be,momentum=8900 -A g4p test19_run_harp_proton.sh

# NA61 and NA49
#
# --> qsub -q amd32_Geant4 -l nodes=1:amd32,walltime=60:00:00 -v G4RELEASE=${g4version},G4REGRESSION=${2},target=${target[$i]} -A g4p test19_run_na61.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v G4RELEASE=${g4version},G4REGRESSION=${2},target=C -A g4p test19_run_na61.sh
# --> qsub -q amd32_Geant4 -l nodes=1:amd32,walltime=60:00:00 -v G4RELEASE=${g4version},G4REGRESSION=${2},target=${target[$i]} -A g4p test19_run_na49.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=48:00:00 -v G4RELEASE=${g4version},G4REGRESSION=${2},target=C -A g4p test19_run_na49.sh

test_sasm6e=`ls -l | grep SASM6E | awk '{print $9}'`
target_sasm6e=(C Cu Pb)

for test in ${test_sasm6e} ; do
for (( i=0; i<${#target_sasm6e[@]}; ++i )) do
   echo "... Processing ... ${test} ... for ${1} for target ${target_sasme6[$i]}"
   qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v App=${test},G4RELEASE=${g4version},target=${target_sasm6e[$i]},momentum=100000 -A g4p pbs_multiCore_master.sh
done
done
