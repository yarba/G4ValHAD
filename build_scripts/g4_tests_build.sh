#!/usr/bin/env bash

g4version=${1}

if [ "x" == "x$g4version" ]; then
echo "Geant4 version is not specified; exit"
exit 3
fi

if [ ! -d /g4/g4p/pbs/g4-had-validation/g4-releases/${g4version} ]; then
echo "release ${g4version} does NOT exist in /g4/g4p/pbs/g4-had-validation/g4-releases; exit"
exit 3
fi

echo "release area ${g4version} is found"

#./g4_set_prod.sh
cd /g4/g4p/pbs/g4-had-validation/build-scripts
source g4_set_prod.sh

if [ "x" == "x$G4INSTALL" ]; then
export G4INSTALL=/g4/g4p/pbs/g4-had-validation/g4-releases/${g4version}
fi

#  if [ ! -d ${G4INSTALL}/lib64 && ! -d ${G4INSTALL}/lib64 ];  then
#
# on cluck.fnal.gov, it's lib64
#
if [ ! -d ${G4INSTALL}/lib64 ];  then
echo "{g4version} has NOT been built; exit"
exit 3
fi

#
# we've passed (almost) all general checkups- now really build the tests
#

# find out if public or ref release, and "serial number" of the version 
#
public=yes
option=

NF=`echo ${g4version} | awk -F '-' '{print NF}'`

if [ ${NF} > 3 ]; then
status=`echo ${g4version} | awk -F '-' '{print $4}'`
if [ "x${status}" == "xref" ]; then
public=no
fi
fi

echo " public = ${public}"

release=`echo ${g4version} | awk -F '-' '{print $2}'`
cycle=`echo ${g4version} | awk -F '-' '{print $3}'`

if  [ "x${public}" == "xyes" ]; then
number=4${release}${cycle}
voption="-DUSE_G4PUBLIC=${number}"
elif [ "x${public}" == "xno" ]; then
ref=`echo ${g4version} | awk -F '-' '{print $NF}'`
number=4${release}${cycle}${ref} 
voption="-DUSE_G4REF=${number}"
fi

tstlist=(test23 test19 test47 test48 test75)

g4cmakedir=`ls -l  ${G4INSTALL}/lib64 | grep Geant4 | awk '{print $9}'`
echo "g4cmakedir is ${g4cmakedir}"

for (( i=0; i<${#tstlist[@]}; ++i )) do
cd ${G4INSTALL}/tests
#
# check if test has been checked out - otherwise print warning and skip
#
if [ ! -d ${tstlist[$i]} ]; then
echo "${tstlist[$i]} does NOT exist; you have to check it out from SVN; exit"
exit 3
else
if [ ! -d ${tstlist[$i]}-build ]; then
mkdir ${tstlist[$i]}-build
fi
cd ${tstlist[$i]}-build
cmake -DGeant4_DIR=${G4INSTALL}/lib64/${g4cmakedir} -DROOT_DIR=${ROOTSYS}/cmake -DCMAKE_CXX_COMPILER=${G4CXX} ${voption} ../${tstlist[$i]}
make
fi
done
