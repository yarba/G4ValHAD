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
#
# somehow this does not work...
#./g4_set_prod.sh
#
# need to source the script
#
source g4_set_prod.sh

echo "G4CXX = ${G4CXX}"
#exit 0


if [ "x" == "x$G4INSTALL" ]; then
export G4INSTALL=/g4/g4p/pbs/g4-had-validation/g4-releases/${g4version}
fi

#g4logdir=/home/g4p/pbs/g4-had-validation/log-files
#if [ ! -d ${g4logdir}/${g4version} ]; then
#mkdir ${g4logdir}/${g4version}
#fi

cd /g4/g4p/pbs/g4-had-validation/g4-releases
mkdir ${g4version}-build

cd ${g4version}-build

#g4configtime=`date +%Y-%m-%d-%H-%M-%S` 
#g4configlog=${g4logdir}/${g4version}/${g4version}_config.log_${g4configtime}

# cmake -DCMAKE_INSTALL_PREFIX=${G4INSTALL} -DCMAKE_CXX_COMPILER=${G4CXX} ${G4INSTALL} 
cmake -DCMAKE_INSTALL_PREFIX=${G4INSTALL} -DGEANT4_USE_GDML=ON -DXERCESC_ROOT_DIR=${XERCESCROOT} -DCMAKE_CXX_COMPILER=${G4CXX} -DGEANT4_BUILD_CXXSTD=14 ${G4INSTALL} 

# extract datasets and generate setup script
#
script=g4-datasets-setup-${g4version}.sh
printf "#!/usr/bin/env bash \n" >> ${script}
printf " \n" >> ${script}
printf "#these datasets correspond to ${g4version} \n" >> ${script}
printf " \n" >> ${script}
#
g4data=/g4/g4p/download/g4data
#
dslist=(G4LEDATA G4LEVELGAMMADATA G4NEUTRONHPDATA G4RADIOACTIVEDATA G4ABLADATA G4REALSURFACEDATA G4NEUTRONXSDATA G4PIIDATA G4SAIDXSDATA G4ENSDFSTATEDATA)
#
for (( i=0; i<${#dslist[@]}; ++i )) do
dset=`grep ${dslist[$i]} geant4make.sh | awk -F '/' '{print $NF}'`
printf "export ${dslist[$i]}=${g4data}/${dset} \n" >> ${script}
done
#
printf " \n" >> ${script}
#
cp ${script} /g4/g4p/pbs/g4-had-validation/env-setup/.

make -j 8
make install

echo "Done building ${g4version}"

