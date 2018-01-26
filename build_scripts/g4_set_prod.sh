#!/bin/bash -x

source /g4/g4p/pbs/g4-had-validation/externals/setup

setup root v6_10_04d -q e14:prof
setup xerces_c v3_1_4a -q e14:prof
setup cmake v3_9_2

export G4CXX=g++
export G4CC=gcc

# NOTE-2: the key is to execute G4's cmake (core or tests) with this flag: 
#         -DCMAKE_CXX_COMPILER=${G4CXX}

export LCG_EXT_TOOLS=/g4/g4p/pbs/g4-had-validation/lcgcmake-install
export JSONCPP_DIR=${LCG_EXT_TOOLS}/jsoncpp/1.7.2/x86_64-slc6-gcc49-opt
export CURL_DIR=${LCG_EXT_TOOLS}/curl/7.48.0/x86_64-slc6-gcc49-opt
export LD_LIBRARY_PATH=${JSONCPP_DIR}/lib:${CURL_DIR}/lib:${LD_LIBRARY_PATH}

# access to g4svn
#
export PATH=/home/g4p/bin:${PATH} 

