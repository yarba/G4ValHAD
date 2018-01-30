#!/usr/bin/env bash
#

# NOTE: python v2_7_10 & numpy v1_9_2 must be setup
#       path to $ROOTSYS/lib must be added to PYTHONPATH
#       python uploader (by A.Dotti) must be installed:
#      git clone https://yarba@gitlab.cern.ch/PhysicsValidationDB/uploader.git

# MCDetails=( 'FTFP=150' 'QGSP=151' 'QGSP_G4Lund=152' )
MCDetails=( 'ftfp=150' 'qgsp=151' 'qgsp-g4lund-str-fragm=152' )

gdir=/g4/g4p/pbs/g4-had-validation/regression-test-files
g4version=${1}

if [ "x" == "x${g4version}" ]; then
   echo "please, provide geant4 version - it is mandatory"
   exit
fi

upload_dir=${gdir}/test19/${g4version}/g4val-upload-json
if [ ! -d "${upload_dir}" ]; then
/bin/mkdir ${upload_dir}
fi


for ((i=0; i<${#MCDetails[@]}; ++i )) do
model=`echo ${MCDetails[$i]} | awk -F '=' '{print $1}'`
mid=`echo ${MCDetails[$i]} | awk -F '=' '{print $2}'`

if [ -e NA49-proton-C-metadata-integrated-spectra-${model}.json ]; then
/bin/rm NA49-proton-C-metadata-integrated-spectra-${model}.json
fi

sed "s/XXX/$mid/" NA49-proton-C-metadata-integrated-spectra.json > NA49-proton-C-metadata-integrated-spectra-${model}.json

python ../../uploader/plot_histofiles.py -c convert -o NA49-proton-C-integrated-spectra-${model}.json \
	--metadatafile NA49-proton-C-metadata-integrated-spectra-${model}.json \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}.root:piplus_dNdxF \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}.root:piplus_pT \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}.root:piminus_dNdxF \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}.root:piminus_pT \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}.root:proton_dNdxF  \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}.root:proton_pT  \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}.root:antiproton_dNdxF  \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}.root:antiproton_pT  \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}.root:neutron_dNdxF  

/bin/mv NA49-proton-C-integrated-spectra-${model}.json ${upload_dir}/.

/bin/rm NA49-proton-C-metadata-integrated-spectra-${model}.json

done
