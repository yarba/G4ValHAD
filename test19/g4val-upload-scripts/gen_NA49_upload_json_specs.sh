#!/usr/bin/env bash
#

# NOTE: python v2_7_10 & numpy v1_9_2 must be setup
#       path to $ROOTSYS/lib must be added to PYTHONPATH
#       python uploader (by A.Dotti) must be installed:
#      git clone https://yarba@gitlab.cern.ch/PhysicsValidationDB/uploader.git

# MCDetails=( 'FTFP=150' 'QGSP=151' 'QGSP_G4Lund=152' )
# MCDetails=( 'ftfp=150' 'qgsp=151' 'qgsp-g4lund-str-fragm=152' )

ModelDetails=( 'ftfp=FTFP' 'qgsp=QGSP' 'qgsp-g4lund-str-fragm=QGSP_G4Lund' )

gdir=/g4/g4p/pbs/g4-had-validation/regression-test-files
g4version=${1}
g4vtag=${2}
access=${3}
if [ "x" == "x${access}" ]; then
   echo "ACCESS is not specified; setting it to \"public\""
   access=public
fi

if [ "x" == "x${g4version}" ]; then
   echo "please, provide geant4 version - it is mandatory"
   exit
fi

upload_dir=${gdir}/test19/${g4version}/g4val-upload-json
if [ ! -d "${upload_dir}" ]; then
/bin/mkdir ${upload_dir}
fi


#for ((i=0; i<${#MCDetails[@]}; ++i )) do
#model=`echo ${MCDetails[$i]} | awk -F '=' '{print $1}'`
#mid=`echo ${MCDetails[$i]} | awk -F '=' '{print $2}'`

for (( i=0; i<${#ModelDetails[@]}; ++i )) do
model=`echo ${ModelDetails[$i]} | awk -F '=' '{print $1}'`
mid=`echo ${ModelDetails[$i]} | awk -F '=' '{print $2}'`

if [ -e NA49-proton-C-metadata-integrated-spectra-${model}.json ]; then
/bin/rm NA49-proton-C-metadata-integrated-spectra-${model}.json
fi

sed "s/MODEL/$mid/; s/VTAG/$g4vtag/; s/ACCESS/$access/" NA49-proton-C-metadata-integrated-spectra.json > NA49-proton-C-metadata-integrated-spectra-${model}.json

# python ../../uploader/plot_histofiles.py -c convert -o NA49-proton-C-integrated-spectra-${model}.json \
python ../../uploader/DoSSiERconverter.py -c convert -o NA49-proton-C-integrated-spectra-${model}.json \
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


if [ -e NA49-proton-C-metadata-ddiff-spectra-${model}-tmp.json ]; then
/bin/rm NA49-proton-C-metadata-ddiff-spectra-${model}-tmp.json
fi
if [ -e NA49-proton-C-metadata-ddiff-spectra-${model}.json ]; then
/bin/rm NA49-proton-C-metadata-ddiff-spectra-${model}.json
fi

sed "s/MODEL/$mid/; s/VTAG/$g4vtag/; s/ACCESS/$access/" NA49-proton-C-metadata-ddiff-spectra.json > NA49-proton-C-metadata-ddiff-spectra-${model}.json


python ../../uploader/DoSSiERconverter.py -c convert -o NA49-proton-C-proton-ddiff-spectra-${model}-tmp.json \
	--metadatafile NA49-proton-C-metadata-ddiff-spectra-${model}.json \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro0 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro1 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro2 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro3 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro4 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro5 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro6 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro7 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro8 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro9 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro10 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro11 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro12 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro13 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro14 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro15 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro16 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro17 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro18 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro19 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro20 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro21 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro22 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro23 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro24 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro25 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro26 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro27 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro28 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro29 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro30 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro31 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro32 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro33 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro34 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro35 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro36 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro37 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro38 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpro39 

python ../../uploader/DoSSiERconverter.py -c convert -o NA49-proton-C-piplus-ddiff-spectra-${model}-tmp.json \
	--metadatafile NA49-proton-C-metadata-ddiff-spectra-${model}.json \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpip0 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpip1 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpip2 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpip3 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpip4 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpip5 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpip6 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpip7 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpip8 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpip9 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpip10 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpip11 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpip12 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpip13 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpip14 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpip15 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpip16 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpip17 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpip18 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpip19 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpip20 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpip21 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpip22 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpip23 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpip24 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpip25 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpip26 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpip27 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpip28  

python ../../uploader/DoSSiERconverter.py -c convert -o NA49-proton-C-piminus-ddiff-spectra-${model}-tmp.json \
	--metadatafile NA49-proton-C-metadata-ddiff-spectra-${model}.json \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpim0 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpim1 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpim2 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpim3 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpim4 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpim5 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpim6 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpim7 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpim8 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpim9 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpim10 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpim11 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpim12 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpim13 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpim14 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpim15 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpim16 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpim17 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpim18 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpim19 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpim20 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpim21 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpim22 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpim23 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpim24 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpim25 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpim26 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpim27 \
	${gdir}/test19/${g4version}/na49-histo/protonC158.0GeV${model}-ReScaleNA49DDiff.root:pTpim28 

sed "s/0\.10625/0.1/" NA49-proton-C-proton-ddiff-spectra-${model}-tmp.json > NA49-proton-C-proton-ddiff-spectra-${model}.json
/bin/rm NA49-proton-C-proton-ddiff-spectra-${model}-tmp.json
sed "s/0\.3125/0.3/; s/0\.15625/0.15/; s/0\.05375/0.05/" NA49-proton-C-piplus-ddiff-spectra-${model}-tmp.json > NA49-proton-C-piplus-ddiff-spectra-${model}.json
/bin/rm NA49-proton-C-piplus-ddiff-spectra-${model}-tmp.json
sed "s/0\.3125/0.3/; s/0\.15625/0.15/; s/0\.05375/0.05/" NA49-proton-C-piminus-ddiff-spectra-${model}-tmp.json > NA49-proton-C-piminus-ddiff-spectra-${model}.json
/bin/rm NA49-proton-C-piminus-ddiff-spectra-${model}-tmp.json

/bin/mv NA49-proton-C-proton-ddiff-spectra-${model}.json ${upload_dir}/.
/bin/mv NA49-proton-C-piplus-ddiff-spectra-${model}.json ${upload_dir}/.
/bin/mv NA49-proton-C-piminus-ddiff-spectra-${model}.json ${upload_dir}/.

/bin/rm NA49-proton-C-metadata-ddiff-spectra-${model}.json

done
