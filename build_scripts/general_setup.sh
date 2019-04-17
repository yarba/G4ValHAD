#!/bin/bash -x

source /g4/g4p/pbs/g4-had-validation/externals/setup

setup root v6_10_04d -q e14:prof
setup xerces_c v3_1_4a -q e14:prof
setup cmake v3_9_2
setup numpy v1_12_1 -q e14:p2713d:prof

# also setup more modern version of Git than the system one
setup git v2_13_3

# fragment from CERN/lcg/gcc4.9's setup.sh
# otherwise lcgcmake machinery picks up the default version
#
export FC=`which gfortran`
export CXX=`which g++`
export CC=`which gcc`

export G4CXX=g++
export G4CC=gcc

export LCG_EXT_TOOLS=/g4/g4p/pbs/g4-had-validation/lcgcmake-install
export JSONCPP_DIR=${LCG_EXT_TOOLS}/jsoncpp/1.7.2/x86_64-slc6-gcc49-opt
export CURL_DIR=${LCG_EXT_TOOLS}/curl/7.48.0/x86_64-slc6-gcc49-opt
export LD_LIBRARY_PATH=${JSONCPP_DIR}/lib:${CURL_DIR}/lib:${LD_LIBRARY_PATH}

# access to g4svn
#
export PATH=/home/g4p/bin:$PATH 

