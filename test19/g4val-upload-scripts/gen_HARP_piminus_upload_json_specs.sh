#!/usr/bin/env bash
#

# NOTE: python v2_7_10 & numpy v1_9_2 must be setup
#       path to $ROOTSYS/lib must be added to PYTHONPATH
#       python uploader (by A.Dotti) must be installed:
#      git clone https://yarba@gitlab.cern.ch/PhysicsValidationDB/uploader.git

# MCDetails=( 'bertini=148' 'ftfp=150' 'inclxx=153' )
# BeamDetails=( '3.0GeV=46' '5.0GeV=47' '8.0GeV=48' '12.0GeV=49' )
BeamDetails=( '3.0GeV=CERN PS (pi-,3GeV)' '5.0GeV=CERN PS (pi-,5GeV)' '8.0GeV=CERN PS (pi-,8GeV)' '12.0GeV=CERN PS (pi-,12GeV)' )
TargetDetails=( 'Be=4' 'C=6' 'Ta=73' )

ModelDetails=( 'bertini=Bertini' 'ftfp=FTFP' 'inclxx=INCLXX' )

gdir=/g4/g4p/pbs/g4-had-validation/regression-test-files
g4version=${1}
g4vtag=${2}
access=${3}
if [ "x" == "x${access}" ]; then
   echo "ACCESS is not specified; setting it to \"public\""
   access=public
fi


beampart=piminus

if [ "x" == "x${g4version}" ]; then
   echo "please, provide geant4 version as the 1st input argument - it is mandatory"
   exit
fi

upload_dir=${gdir}/test19/${g4version}/g4val-upload-json
if [ ! -d "${upload_dir}" ]; then
/bin/mkdir ${upload_dir}
fi


#for (( i=0; i<${#MCDetails[@]}; ++i )) do
#model=`echo ${MCDetails[$i]} | awk -F '=' '{print $1}'`
#mid=`echo ${MCDetails[$i]} | awk -F '=' '{print $2}'`

for (( i=0; i<${#ModelDetails[@]}; ++i )) do
model=`echo ${ModelDetails[$i]} | awk -F '=' '{print $1}'`
mid=`echo ${ModelDetails[$i]} | awk -F '=' '{print $2}'`

for (( j=0; j<${#BeamDetails[@]}; ++j )) do
bmom=`echo ${BeamDetails[$j]} | awk -F '=' '{print $1}'`
bid=`echo ${BeamDetails[$j]} | awk -F '=' '{print $2}'`

for (( k=0; k<${#TargetDetails[@]}; ++k )) do
target=`echo ${TargetDetails[$k]} | awk -F '=' '{print $1}'`
tgtid=`echo ${TargetDetails[$k]} | awk -F '=' '{print $2}'`

if [ -e HARP-${beampart}${target}${bmom}-metadata-${model}.json ]; then
/bin/rm HARP-${beampart}${target}${bmom}-metadata-${model}.json
fi

# sed "s/XXX/$mid/; s/TGT/$tgtid/; s/BEAM/$bid/" HARP-metadata-template.json > HARP-${beampart}${target}${bmom}-metadata-${model}.json
sed "s/MODEL/$mid/; s/VTAG/$g4vtag/; s/TGT/$target/; s/BEAM/$bid/; s/ACCESS/$access/" HARP-metadata-template.json > HARP-${beampart}${target}${bmom}-metadata-${model}.json

#python ../../uploader/plot_histofiles.py -c convert -o HARP-${beampart}${target}${bmom}-${model}.json \
python ../../uploader/DoSSiERconverter.py -c convert -o HARP-${beampart}${target}${bmom}-${model}.json \
	--metadatafile HARP-${beampart}${target}${bmom}-metadata-${model}.json \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piplus_FW_0 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piplus_FW_1 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piplus_FW_2 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piplus_FW_3 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piplus_LA_0 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piplus_LA_1\
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piplus_LA_2 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piplus_LA_3 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piplus_LA_4 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piplus_LA_5 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piplus_LA_6 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piplus_LA_7 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piplus_LA_8 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piminus_FW_0 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piminus_FW_1 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piminus_FW_2 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piminus_FW_3 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piminus_LA_0 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piminus_LA_1 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piminus_LA_2 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piminus_LA_3 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piminus_LA_4 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piminus_LA_5 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piminus_LA_6 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piminus_LA_7 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piminus_LA_8 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piplus_mom_0 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piplus_mom_1 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piplus_mom_2 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piplus_mom_3 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piplus_mom_4 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piplus_mom_5 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piplus_mom_6 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piplus_mom_7 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piplus_mom_8 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piplus_mom_9 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piplus_mom_10 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piplus_mom_11 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piminus_mom_0 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piminus_mom_1 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piminus_mom_2 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piminus_mom_3 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piminus_mom_4 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piminus_mom_5 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piminus_mom_6 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piminus_mom_7 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piminus_mom_8 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piminus_mom_9 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piminus_mom_10 \
	${gdir}/test19/${g4version}/harp-histo/${beampart}${target}${bmom}${model}.root:piminus_mom_11 

/bin/mv HARP-${beampart}${target}${bmom}-${model}.json ${upload_dir}/.

/bin/rm HARP-${beampart}${target}${bmom}-metadata-${model}.json

done

done

done
