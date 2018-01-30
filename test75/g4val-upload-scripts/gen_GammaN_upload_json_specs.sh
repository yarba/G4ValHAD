#!/usr/bin/env bash
#

# NOTE: One needs a consistent setup of python, numpy, and ujson
#       so far the best way is get it with anaconda... although the version of Root is outdated
#       path to $ROOTSYS/lib must be added to PYTHONPATH
#       python uploader (by A.Dotti  & H.Wenzel) must be installed:
#      git clone https://yarba@gitlab.cern.ch/PhysicsValidationDB/uploader.git


model=Bertini

gdir=/g4/g4p/pbs/g4-had-validation/regression-test-files
g4version=${1}
g4tag=${2}

if [ "x" == "x${g4version}" ]; then
   echo "please, provide geant4 version - it is mandatory"
   exit
fi

upload_dir=${gdir}/test75/${g4version}/g4val-upload-json
if [ ! -d "${upload_dir}" ]; then
/bin/mkdir ${upload_dir}
fi

if [ -e Gamma300-Cu-metadata-${model}.json ]; then
/bin/rm Gamma300-Cu-metadata-${model}.json
fi
if [ -e Gamma668-Cu-metadata-${model}.json ]; then
/bin/rm Gamma668-Cu-metadata-${model}.json
fi
if [ -e Gamma668-Pb-metadata-${model}.json ]; then
/bin/rm Gamma668-Pb-metadata-${model}.json
fi

sed "s/VTAG/$g4tag/" Gamma300-Cu-metadata.json > Gamma300-Cu-metadata-${model}.json

python ../../uploader/DoSSiERconverter.py -c convert -o Gamma300-Cu-${model}.json \
	--metadatafile Gamma300-Cu-metadata-${model}.json \
	${gdir}/test75/${g4version}/gamma300MeVCu${model}.root:p45deg \
	${gdir}/test75/${g4version}/gamma300MeVCu${model}.root:p90deg \
	${gdir}/test75/${g4version}/gamma300MeVCu${model}.root:p135deg \
	${gdir}/test75/${g4version}/gamma300MeVCu${model}.root:p150deg 

/bin/mv Gamma300-Cu-${model}.json ${upload_dir}/.

/bin/rm Gamma300-Cu-metadata-${model}.json

sed "s/VTAG/$g4tag/" Gamma668-Cu-metadata.json > Gamma668-Cu-metadata-${model}.json

python ../../uploader/DoSSiERconverter.py -c convert -o Gamma668-Cu-${model}.json \
	--metadatafile Gamma668-Cu-metadata-${model}.json \
	${gdir}/test75/${g4version}/gamma668MeVCu${model}.root:pim28deg \
	${gdir}/test75/${g4version}/gamma668MeVCu${model}.root:pim44deg \
	${gdir}/test75/${g4version}/gamma668MeVCu${model}.root:pip28deg \
	${gdir}/test75/${g4version}/gamma668MeVCu${model}.root:pip44deg 

/bin/mv Gamma668-Cu-${model}.json ${upload_dir}/.

/bin/rm Gamma668-Cu-metadata-${model}.json

sed "s/VTAG/$g4tag/" Gamma668-Pb-metadata.json > Gamma668-Pb-metadata-${model}.json

python ../../uploader/DoSSiERconverter.py -c convert -o Gamma668-Pb-${model}.json \
	--metadatafile Gamma668-Pb-metadata-${model}.json \
	${gdir}/test75/${g4version}/gamma668MeVPb${model}.root:pim28deg \
	${gdir}/test75/${g4version}/gamma668MeVPb${model}.root:pim44deg \
	${gdir}/test75/${g4version}/gamma668MeVPb${model}.root:pip28deg \
	${gdir}/test75/${g4version}/gamma668MeVPb${model}.root:pip44deg 

/bin/mv Gamma668-Pb-${model}.json ${upload_dir}/.

/bin/rm Gamma668-Pb-metadata-${model}.json

