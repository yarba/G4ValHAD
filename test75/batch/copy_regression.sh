#!/usr/bin/env bash
#

release=${1}

if [ ! -d /g4/g4p/pbs/g4-had-validation/results/${release}/test75/regre ]; then
mkdir /g4/g4p/pbs/g4-had-validation/results/${release}/test75/regre
fi
cp *-regre.gif /g4/g4p/pbs/g4-had-validation/results/${release}/test75/regre/.
#
if [ ! -d /g4/g4p/pbs/g4-had-validation/regression-test-files/test75/${release} ]; then
mkdir /g4/g4p/pbs/g4-had-validation/regression-test-files/test75/${release}
fi
cp *.root /g4/g4p/pbs/g4-had-validation/regression-test-files/test75/${release}/.
