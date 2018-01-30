#!/usr/bin/env bash
#

# NOTE: python v2_7_10 & numpy v1_9_2 must be setup
#       path to $ROOTSYS/lib must be added to PYTHONPATH
#       python uploader (by A.Dotti) must be installed:
#      git clone https://yarba@gitlab.cern.ch/PhysicsValidationDB/uploader.git

MCDetails=( 'bertini=148' 'inclxx=153' 'binary=149' 'ftfp=150'  )
BeamDetails=( '1.40GeV=66' '5.00GeV=73' )

gdir=/g4/g4p/pbs/g4-had-validation/regression-test-files
g4version=${1}

if [ "x" == "x${g4version}" ]; then
   echo "please, provide geant4 version as the 1st input argument - it is mandatory"
   exit
fi

upload_dir=${gdir}/test47/${g4version}/g4val-upload-json
if [ ! -d "${upload_dir}" ]; then
/bin/mkdir ${upload_dir}
fi

for (( i=0; i<${#MCDetails[@]}; ++i )) do
model=`echo ${MCDetails[$i]} | awk -F '=' '{print $1}'`
mid=`echo ${MCDetails[$i]} | awk -F '=' '{print $2}'`

for (( j=0; j<${#BeamDetails[@]}; ++j )) do
bmom=`echo ${BeamDetails[$j]} | awk -F '=' '{print $1}'`
bid=`echo ${BeamDetails[$j]} | awk -F '=' '{print $2}'`

if [ -e ITEP771-piminus-C-metadata-${model}.json ]; then
/bin/rm ITEP771-piminus-C-metadata-${model}.json
fi

sed "s/MODEL/$mid/; s/piXXX/piminus/; s/BEAM/$bid/" ITEP771-pion-C-metadata-template.json > ITEP771-piminus-C-metadata-${model}.json

python ../../uploader/plot_histofiles.py -c convert -o ITEP771-piminusC${bmom}-${model}.json \
	--metadatafile ITEP771-piminus-C-metadata-${model}.json \
	${gdir}/test47/${g4version}/piminusC${model}${bmom}.root:KEproton0piminusC${model}${bmom}\ 59.1 \
	${gdir}/test47/${g4version}/piminusC${model}${bmom}.root:KEproton0piminusC${model}${bmom}\ 89.0 \
	${gdir}/test47/${g4version}/piminusC${model}${bmom}.root:KEproton0piminusC${model}${bmom}119.0 \
	${gdir}/test47/${g4version}/piminusC${model}${bmom}.root:KEproton0piminusC${model}${bmom}159.6 \
	${gdir}/test47/${g4version}/piminusC${model}${bmom}.root:KEneutron0piminusC${model}${bmom}\ 59.1 \
	${gdir}/test47/${g4version}/piminusC${model}${bmom}.root:KEneutron0piminusC${model}${bmom}\ 89.0 \
	${gdir}/test47/${g4version}/piminusC${model}${bmom}.root:KEneutron0piminusC${model}${bmom}119.0 \
	${gdir}/test47/${g4version}/piminusC${model}${bmom}.root:KEneutron0piminusC${model}${bmom}159.6 

/bin/mv ITEP771-piminusC${bmom}-${model}.json ${upload_dir}/.

/bin/rm ITEP771-piminus-C-metadata-${model}.json

if [ -e ITEP771-piminus-U-metadata-${model}.json ]; then
/bin/rm ITEP771-piminus-U-metadata-${model}.json
fi

sed "s/MODEL/$mid/; s/piXXX/piminus/; s/BEAM/$bid/" ITEP771-pion-U-metadata-template.json > ITEP771-piminus-U-metadata-${model}.json

python ../../uploader/plot_histofiles.py -c convert -o ITEP771-piminusU${bmom}-${model}.json \
	--metadatafile ITEP771-piminus-U-metadata-${model}.json \
	${gdir}/test47/${g4version}/piminusU${model}${bmom}.root:KEproton0piminusU${model}${bmom}\ 59.1 \
	${gdir}/test47/${g4version}/piminusU${model}${bmom}.root:KEproton0piminusU${model}${bmom}\ 89.0 \
	${gdir}/test47/${g4version}/piminusU${model}${bmom}.root:KEproton0piminusU${model}${bmom}119.0 \
	${gdir}/test47/${g4version}/piminusU${model}${bmom}.root:KEproton0piminusU${model}${bmom}159.6 \
	${gdir}/test47/${g4version}/piminusU${model}${bmom}.root:KEneutron0piminusU${model}${bmom}\ 59.1 \
	${gdir}/test47/${g4version}/piminusU${model}${bmom}.root:KEneutron0piminusU${model}${bmom}\ 89.0 \
	${gdir}/test47/${g4version}/piminusU${model}${bmom}.root:KEneutron0piminusU${model}${bmom}119.0 \
	${gdir}/test47/${g4version}/piminusU${model}${bmom}.root:KEneutron0piminusU${model}${bmom}159.6 

/bin/mv ITEP771-piminusU${bmom}-${model}.json ${upload_dir}/.

/bin/rm ITEP771-piminus-U-metadata-${model}.json

if [ -e ITEP771-piminus-Cu-metadata-${model}.json ]; then
/bin/rm ITEP771-piminus-Cu-metadata-${model}.json
fi

sed "s/MODEL/$mid/; s/piXXX/piminus/; s/BEAM/$bid/" ITEP771-pion-Cu-Pb-metadata-template.json > ITEP771-piminus-Cu-Pb-metadata-${model}.json

python ../../uploader/plot_histofiles.py -c convert -o ITEP771-piminusCu${bmom}-${model}.json \
	--metadatafile ITEP771-piminus-Cu-Pb-metadata-${model}.json \
	${gdir}/test47/${g4version}/piminusCu${model}${bmom}.root:KEproton0piminusCu${model}${bmom}\ 59.1 \
	${gdir}/test47/${g4version}/piminusCu${model}${bmom}.root:KEproton0piminusCu${model}${bmom}\ 89.0 \
	${gdir}/test47/${g4version}/piminusCu${model}${bmom}.root:KEproton0piminusCu${model}${bmom}119.0 \
	${gdir}/test47/${g4version}/piminusCu${model}${bmom}.root:KEproton0piminusCu${model}${bmom}159.6 \
	${gdir}/test47/${g4version}/piminusCu${model}${bmom}.root:KEneutron0piminusCu${model}${bmom}\ 59.1 \
	${gdir}/test47/${g4version}/piminusCu${model}${bmom}.root:KEneutron0piminusCu${model}${bmom}\ 89.0 \
	${gdir}/test47/${g4version}/piminusCu${model}${bmom}.root:KEneutron0piminusCu${model}${bmom}119.0 \
	${gdir}/test47/${g4version}/piminusCu${model}${bmom}.root:KEneutron0piminusCu${model}${bmom}159.6 

/bin/mv ITEP771-piminusCu${bmom}-${model}.json ${upload_dir}/.

python ../../uploader/plot_histofiles.py -c convert -o ITEP771-piminusPb${bmom}-${model}.json \
	--metadatafile ITEP771-piminus-Cu-Pb-metadata-${model}.json \
	${gdir}/test47/${g4version}/piminusPb${model}${bmom}.root:KEproton0piminusPb${model}${bmom}\ 59.1 \
	${gdir}/test47/${g4version}/piminusPb${model}${bmom}.root:KEproton0piminusPb${model}${bmom}\ 89.0 \
	${gdir}/test47/${g4version}/piminusPb${model}${bmom}.root:KEproton0piminusPb${model}${bmom}119.0 \
	${gdir}/test47/${g4version}/piminusPb${model}${bmom}.root:KEproton0piminusPb${model}${bmom}159.6 \
	${gdir}/test47/${g4version}/piminusPb${model}${bmom}.root:KEneutron0piminusPb${model}${bmom}\ 59.1 \
	${gdir}/test47/${g4version}/piminusPb${model}${bmom}.root:KEneutron0piminusPb${model}${bmom}\ 89.0 \
	${gdir}/test47/${g4version}/piminusPb${model}${bmom}.root:KEneutron0piminusPb${model}${bmom}119.0 \
	${gdir}/test47/${g4version}/piminusPb${model}${bmom}.root:KEneutron0piminusPb${model}${bmom}159.6 

/bin/mv ITEP771-piminusPb${bmom}-${model}.json ${upload_dir}/.

/bin/rm ITEP771-piminus-Cu-Pb-metadata-${model}.json

done

done


