#!/usr/bin/env bash
#
# usage:
# source generate_upload_script_models.sh geant4-10.00-ref08 internal
#

g4version=${1}
status=${2}

# g4version=geant4-10.00-ref07
# status=internal

testname=test75

beam=gamma

target=(Cu Cu Pb)
momentum=(300 668 668)
secondary=(p pions pions)

script=${testname}-upload-models.xml

printf "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" >> ${script}
printf "\n" >> ${script}
printf "<testslist>\n" >> ${script}
printf "\n" >> ${script}

for (( i=0; i<${#momentum[@]}; ++i )) do

printf "<tests>\n" >> ${script}

printf "<testdes>${testname}</testdes>\n" >> ${script}

printf "<g4version>${g4version}</g4version>\n" >> ${script}
if [ ${momentum[$i]} = "300" ]; then
printf "<observable>kinetic energy of seconadry proton</observable>\n" >> ${script}
printf "<secondary>proton</secondary>\n" >> ${script}
else
printf "<observable>momentum of secondary pion</observable>\n" >> ${script}
if [ ${target[$i]} = "Cu" ]; then
printf "<secondary>pi+ or pi-</secondary>\n" >> ${script}
else
printf "<secondary>pi+</secondary>\n" >> ${script}
fi
fi
printf "<reaction>${beam} on ${target[$i]}</reaction>\n" >> ${script}
printf "<beam>${beam}</beam>\n" >> ${script}
printf "<beammomentum>${momentum[$i]} MeV/c</beammomentum>\n" >> ${script}
printf "<beamenergy></beamenergy>\n" >> ${script}
printf "<target>${target[$i]}</target>\n" >> ${script}
printf "<status>${status}</status>\n" >> ${script}
printf "<score>\n" >> ${script}
printf "<type>expert</type>\n" >> ${script}
printf "<value>passed</value>\n" >> ${script}
printf "</score>\n" >> ${script}

printf "<imageblob>${beam}-${momentum[$i]}-${target[$i]}-${secondary[$i]}-models.gif</imageblob>\n" >> ${script}

printf "<taglist>\n" >> ${script}
printf "<tag>\n" >> ${script}
printf "<name>Model</name>\n" >> ${script}
printf "<value>Bertini</value>\n" >> ${script}
printf "</tag>\n" >> ${script}
printf "</taglist>\n" >> ${script}
printf "</tests>\n" >> ${script}
printf "\n" >> ${script}

done

printf "</testslist>\n" >> ${script}
