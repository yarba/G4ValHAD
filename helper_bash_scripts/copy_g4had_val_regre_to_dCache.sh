#!/usr/bin/env bash

# this script has a non-negligible flaw:
# it doesn't properly handle cases when there're
# (sub)dirs and regular files at the same level/depth

dCache=/pnfs/geant4/persistent/g4p/g4-had-validation/regression-test-files/${1}
if [ ! -d ${dCache} ]; then
/bin/mkdir ${dCache}
fi

recursivedir() {
  store=${1}
  for d in *; do
    if [ -d ${d} ]; then
      if [ "$store" == "/"  ]; then
         store=
      fi
      store=${store}/${d}
      outputdir=${dCache}${store}
      if [ ! -d ${outputdir} ]; then
         echo " ${outputdir} does NOT exist; create it "
	 /bin/mkdir ${outputdir}
      fi
      ( cd ${d} && recursivedir "$store" ) # && pwd ) # the ( ... && ... ) is the essential part of recursive call
    fi
    if [ -f ${d} ]; then
        echo " ${d} --> outputdir = $outputdir "
	/usr/bin/dccp ${d} ${outputdir}/.
    fi
    store=`dirname "$store"`
  done
}

# cd /g4/g4p/pbs/g4-had-validation/regression-test-files/test19/geant4-10-04-ref-05
# cd /g4/g4p/pbs/g4-had-validation/regression-test-files/test19
#
cd ${1}
recursivedir

