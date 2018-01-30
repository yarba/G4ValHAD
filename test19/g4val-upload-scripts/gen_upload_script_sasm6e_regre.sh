#!/usr/bin/env bash
#

g4version=${1}
status=${2}

testname=test19

momentum=100.0
beam=(piplus proton)
target=(C Cu Pb)
secondary=(antiproton kminus kplus piminus piplus proton)

models=(ftfp qgsp)
regression=(g4-9.6-p04 g4-10.2-p03 g4-10.3-p01)

script=${testname}-sasm6e-regre.xml

if [ -e ${script} ]; then
/bin/rm -f ${script}
fi

printf "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" >> ${script}
printf "\n" >> ${script}
printf "<testslist>\n" >> ${script}
printf "\n" >> ${script}

for (( i=0; i<${#beam[@]}; ++i )) do

bm=${beam[$i]}
if [ ${beam[$i]} == "piplus" ]; then
bm=pi+
fi

for (( j=0; j<${#target[@]}; ++j )) do

tg=${target[$j]}
if [ ${target[$j]} == "C" ]; then
tg=Carbon
fi
if [ ${target[$j]} == "Cu" ]; then
tg=Copper
fi
if [ ${target[$j]} == "Pb" ]; then
tg=Lead
fi

for (( k=0; k<${#secondary[@]}; ++k )) do

sec=${secondary[$k]}
if [ ${secondary[$k]} == "piplus" ]; then
sec=pi+
elif [ ${secondary[$k]} == "piminus" ]; then
sec=pi-
elif [ ${secondary[$k]} == "kplus" ]; then
sec=K+
elif [ ${secondary[$k]} == "kminus" ]; then
sec=K-
elif [ ${secondary[$k]} == "antiproton" ]; then
sec=anti_proton
fi

for  (( m=0; m<${#models[@]}; ++m )) do

printf "<tests>\n" >> ${script}

printf "<testdes>${testname}</testdes>\n" >> ${script}

printf "<g4version>${g4version}</g4version>\n" >> ${script}
printf "<observable>production cross section vs momentum of secondary</observable>\n" >> ${script}
printf "<reaction>${bm} on ${target}</reaction>\n" >> ${script}
printf "<beam>${bm}</beam>\n" >> ${script}
printf "<beammomentum>${momentum} GeV/c</beammomentum>\n" >> ${script}
printf "<beamenergy></beamenergy>\n" >> ${script}

printf "<target>${tg}</target>\n" >> ${script}

printf "<secondary>${sec}</secondary>\n" >> ${script}
printf "<status>${status}</status>\n" >> ${script}
printf "<score>\n" >> ${script}
printf "<type>expert</type>\n" >> ${script}
printf "<value>passed</value>\n" >> ${script}
printf "</score>\n" >> ${script}
printf "<imageblob>${beam[$i]}-${target[$j]}-${momentum}GeV-${secondary[$k]}-${models[$m]}-regre.gif</imageblob>\n" >> ${script}

printf "<taglist>\n" >> ${script}
printf "<tag>\n" >> ${script}
printf "<name>Model</name>\n" >> ${script}
printf "<value>${models[$m]}</value>\n" >> ${script}
printf "</tag>\n" >> ${script}
printf "<tag>\n" >> ${script}
printf "<name>Regression testing:</name>\n" >> ${script}

printf "<value>geant4 versions: " >> ${script}
for (( r=0; r<${#regression[@]}; ++r )) do
printf "${regression[$r]}," >> ${script}
done
printf "${g4version}</value>\n" >> ${script}

printf "</tag>\n" >> ${script}
printf "</taglist>\n" >> ${script}

printf "</tests>\n" >> ${script}

printf "\n" >> ${script}

done

done

done

done

printf "</testslist>\n" >> ${script}

