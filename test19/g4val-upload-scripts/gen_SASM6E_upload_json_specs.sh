#!/usr/bin/env bash
#

# NOTE: python v2_7_10 & numpy v1_9_2 must be setup
#       path to $ROOTSYS/lib must be added to PYTHONPATH
#       python uploader (by A.Dotti) must be installed:
#      git clone https://yarba@gitlab.cern.ch/PhysicsValidationDB/uploader.git

# MCDetails=( 'FTFP=150' 'QGSP=151' 'QGSP_G4Lund=152' )
# corresponds to 10.3.p03
MCDetails=( 'ftfp=239' 'qgsp=240' 'qgsp-g4lund-str-fragm=241' )
BeamDetails=( 'piplus=10' 'proton=12' )
TargetDetails=( 'C=6' 'Cu=29' 'Pb=82' )

gdir=/g4/g4p/pbs/g4-had-validation/regression-test-files
g4version=${1}

if [ "x" == "x${g4version}" ]; then
   echo "please, provide geant4 version as the 1st input argument - it is mandatory"
   exit
fi

upload_dir=${gdir}/test19/${g4version}/g4val-upload-json
if [ ! -d "${upload_dir}" ]; then
/bin/mkdir ${upload_dir}
fi

bmom=100.0GeV

for (( i=0; i<${#MCDetails[@]}; ++i )) do
model=`echo ${MCDetails[$i]} | awk -F '=' '{print $1}'`
mid=`echo ${MCDetails[$i]} | awk -F '=' '{print $2}'`

for (( j=0; j<${#BeamDetails[@]}; ++j )) do
beampart=`echo ${BeamDetails[$j]} | awk -F '=' '{print $1}'`
bid=`echo ${BeamDetails[$j]} | awk -F '=' '{print $2}'`

for (( k=0; k<${#TargetDetails[@]}; ++k )) do
target=`echo ${TargetDetails[$k]} | awk -F '=' '{print $1}'`
tgtid=`echo ${TargetDetails[$k]} | awk -F '=' '{print $2}'`

if [ -e SASM6E-${beampart}${target}${bmom}-metadata-${model}.json ]; then
/bin/rm SASM6E-${beampart}${target}${bmom}-metadata-${model}.json
fi

sed "s/XXX/$mid/; s/TGT/$tgtid/; s/BEAM/$bid/" SASM6E-metadata-template.json > SASM6E-${beampart}${target}${bmom}-metadata-${model}.json

python ../../uploader/plot_histofiles.py -c convert -o SASM6E-${beampart}${target}${bmom}-${model}.json \
	--metadatafile SASM6E-${beampart}${target}${bmom}-metadata-${model}.json \
	${gdir}/test19/${g4version}/sasm6e-histo/${beampart}${target}${bmom}${model}.root:piplus_pt180 \
	${gdir}/test19/${g4version}/sasm6e-histo/${beampart}${target}${bmom}${model}.root:piplus_pt300 \
	${gdir}/test19/${g4version}/sasm6e-histo/${beampart}${target}${bmom}${model}.root:piplus_pt500 \
	${gdir}/test19/${g4version}/sasm6e-histo/${beampart}${target}${bmom}${model}.root:piminus_pt180 \
	${gdir}/test19/${g4version}/sasm6e-histo/${beampart}${target}${bmom}${model}.root:piminus_pt300 \
	${gdir}/test19/${g4version}/sasm6e-histo/${beampart}${target}${bmom}${model}.root:piminus_pt500 \
	${gdir}/test19/${g4version}/sasm6e-histo/${beampart}${target}${bmom}${model}.root:kplus_pt180 \
	${gdir}/test19/${g4version}/sasm6e-histo/${beampart}${target}${bmom}${model}.root:kplus_pt300 \
	${gdir}/test19/${g4version}/sasm6e-histo/${beampart}${target}${bmom}${model}.root:kplus_pt500 \
	${gdir}/test19/${g4version}/sasm6e-histo/${beampart}${target}${bmom}${model}.root:kminus_pt180 \
	${gdir}/test19/${g4version}/sasm6e-histo/${beampart}${target}${bmom}${model}.root:kminus_pt300 \
	${gdir}/test19/${g4version}/sasm6e-histo/${beampart}${target}${bmom}${model}.root:kminus_pt500 \
	${gdir}/test19/${g4version}/sasm6e-histo/${beampart}${target}${bmom}${model}.root:proton_pt180 \
	${gdir}/test19/${g4version}/sasm6e-histo/${beampart}${target}${bmom}${model}.root:proton_pt300 \
	${gdir}/test19/${g4version}/sasm6e-histo/${beampart}${target}${bmom}${model}.root:proton_pt500 \
	${gdir}/test19/${g4version}/sasm6e-histo/${beampart}${target}${bmom}${model}.root:antiproton_pt180 \
	${gdir}/test19/${g4version}/sasm6e-histo/${beampart}${target}${bmom}${model}.root:antiproton_pt300 \
	${gdir}/test19/${g4version}/sasm6e-histo/${beampart}${target}${bmom}${model}.root:antiproton_pt500 

/bin/mv SASM6E-${beampart}${target}${bmom}-${model}.json ${upload_dir}/.

/bin/rm SASM6E-${beampart}${target}${bmom}-metadata-${model}.json

done

done

done

