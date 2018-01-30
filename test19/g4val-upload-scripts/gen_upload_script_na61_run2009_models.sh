#!/usr/bin/env bash
#
# usage:
# source generate_upload_script_na61_models.sh proton C geant4-10.00-ref08 internal
#

beam=${1}
target=${2}
g4version=${3}
status=${4}

# g4version=geant4-10.00-ref07
# status=internal

testname=test19

momentum=(31)
secondary=(piplus piminus proton kplus kminus k0s lambda)
region=(p1 p2 p3)

script=${testname}-na61-run2009-${beam}-${target}-models.xml
if [ -e ${script} ]; then
/bin/rm -f ${script}
fi

tg=${target}
if [ ${target} = "C" ]; then
tg=Carbon
fi
if [ ${target} = "Be" ]; then
tg=Beryllium
fi

bm=${beam}
if [ ${beam} = "piplus" ]; then
bm=pi+
fi
if [ ${beam} = "piminus" ]; then
bm=pi-
fi

printf "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" >> ${script}
printf "\n" >> ${script}
printf "<testslist>\n" >> ${script}
printf "\n" >> ${script}

for (( i=0; i<${#momentum[@]}; ++i )) do

for (( j=0; j<${#secondary[@]}; ++j )) do

for (( k=0; k<${#region[@]}; ++k )) do

### echo "energy = ${energy[$i]} and secondary = ${secondary[$j]}"

sec=${secondary[$j]}
if [ ${secondary[$j]} == "piplus" ]; then
sec=pi+
elif [ ${secondary[$j]} == "piminus" ]; then
sec=pi-
elif [ ${secondary[$j]} == "kplus" ]; then
sec=K+
elif [ ${secondary[$j]} == "kminus" ]; then
sec=K-
elif [ ${secondary[$j]} == "k0s" ]; then
sec=K0s
elif [ ${secondary[$j]} == "lambda" ]; then
sec=Lambda
fi

if [ ${region[$k]} == "p3" ]; then
if [ ${secondary[$j]} == "kplus" ]; then
continue
fi
if [ ${secondary[$j]} == "kminus" ]; then
continue
fi
if [ ${secondary[$j]} == "k0s" ]; then
continue
fi
if [ ${secondary[$j]} == "lambda" ]; then
continue
fi
fi

printf "<tests>\n" >> ${script}

printf "<testdes>${testname}</testdes>\n" >> ${script}

printf "<g4version>${g4version}</g4version>\n" >> ${script}
printf "<observable>production cross section vs momentum of secondary, in different theta bins</observable>\n" >> ${script}
printf "<reaction>${bm} on ${target}</reaction>\n" >> ${script}
printf "<beam>${bm}</beam>\n" >> ${script}
printf "<beammomentum>${momentum[$i]} GeV/c</beammomentum>\n" >> ${script}
printf "<beamenergy></beamenergy>\n" >> ${script}

# printf "<target>${target}</target>\n" >> ${script}
printf "<target>${tg}</target>\n" >> ${script}


# printf "<secondary>${secondary[$j]}</secondary>\n" >> ${script}
printf "<secondary>${sec}</secondary>\n" >> ${script}
printf "<status>${status}</status>\n" >> ${script}
printf "<score>\n" >> ${script}
printf "<type>expert</type>\n" >> ${script}
printf "<value>passed</value>\n" >> ${script}
printf "</score>\n" >> ${script}
printf "<imageblob>${beam}-${target}-${momentum[$i]}GeV-${secondary[$j]}-Pub2015-models-${region[$k]}.gif</imageblob>\n" >> ${script}

printf "<taglist>\n" >> ${script}
printf "<tag>\n" >> ${script}
printf "<name>Model</name>\n" >> ${script}
printf "<value>FTF(P), QGS(P)</value>\n" >> ${script}
printf "</tag>\n" >> ${script}
printf "<tag>\n" >> ${script}
printf "<name>Comment</name>\n" >> ${script}
printf "<value>Both FTF and QGS models are backed with PreCompound model</value>\n" >> ${script}
printf "</tag>\n" >> ${script}
printf "</taglist>\n" >> ${script}

printf "</tests>\n" >> ${script}

printf "\n" >> ${script}

done

done

printf "\n" >> ${script}
 
done


printf "</testslist>\n" >> ${script}
