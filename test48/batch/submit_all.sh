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


# qsub -q grunt -v G4RELEASE=${g4version},G4REGRESSION=${2} test48_run_piminus.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=48:00:00 -v G4RELEASE=${g4version},G4REGRESSION=${2} -A g4p test48_run_piminus.sh

# NOTE: as of Oct. 2014 (geant4-10-00-ref-08), validation of mu- capture is suspended
#       because usage of the new mu- capture model (G4MuonMinusCapture) may result
#       in a stuck job, sujcet to a choice of startup random seed; infinite loop suspected

# all targets and models in 1 job
#
#qsub -q grunt -v G4RELEASE=${1},G4REGRESSION=${2} test48_run_muminus_chain.sh

# separate targets
#
#qsub -q grunt -v G4RELEASE=${g4version},G4REGRESSION=${2},target=Al test48_run_muminus.sh
#qsub -q grunt -v G4RELEASE=${g4version},G4REGRESSION=${2},target=Si test48_run_muminus.sh
#qsub -q grunt -v G4RELEASE=${g4version},G4REGRESSION=${2},target=Ca test48_run_muminus.sh
#qsub -q grunt -v G4RELEASE=${g4version},G4REGRESSION=${2},target=Fe test48_run_muminus.sh
#qsub -q grunt -v G4RELEASE=${g4version},G4REGRESSION=${2},target=Ag test48_run_muminus.sh
#qsub -q grunt -v G4RELEASE=${g4version},G4REGRESSION=${2},target=I  test48_run_muminus.sh
#qsub -q grunt -v G4RELEASE=${g4version},G4REGRESSION=${2},target=Au test48_run_muminus.sh
#qsub -q grunt -v G4RELEASE=${g4version},G4REGRESSION=${2},target=Pb test48_run_muminus.sh
#qsub -q grunt -v G4RELEASE=${g4version},G4REGRESSION=${2},target=S  test48_run_muminus.sh
#
target=(Al Si Ca Fe Ag I Au Pb S)
for (( k=0; k<${#target[@]}; ++k )) do
   qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v G4RELEASE=${g4version},target=${target[$k]},G4REGRESSION=${2} -A g4p test48_run_muminus.sh
done



#test_list=`ls -l | grep run | awk '{print $9}'`
#for test in ${test_list} ; do
#  echo "... Processing ... ${test} ... for ${1}"
##   qsub -q grunt -v ${1} ${test}
#  qsub -l nodes=1,walltime=48:00:00 -q grunt -v G4RELEASE=${g4version},G4REGRESSION=${2} ${test}
#done
