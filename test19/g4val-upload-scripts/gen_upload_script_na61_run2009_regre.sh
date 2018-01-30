#!/usr/bin/env bash
#

beam=${1}
target=${2}
g4version=${3}
status=${4}

#g4version=geant4-10.00-ref07
#status=internal

testname=test19

momentum=(31)
secondary=(piplus piminus proton kplus kminus k0s lambda)
region=(p1 p2 p3)
# models=(ftfp qgsp qgsp-g4lund-str-fragm)
models=(ftfp qgsp)

regression=(g4-9.6-p04 g4-10.2-p03 g4-10.3-p01)

script=${testname}-na61-run2009-${beam}-${target}-regre.xml
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

for  (( m=0; m<${#models[@]}; ++m )) do

### echo "energy = ${energy[$i]} and secondary = ${secondary[$j]}"

sec=${secondary[$j]}
if [ ${secondary[$j]} = "piplus" ]; then
sec=pi+
elif [ ${secondary[$j]} = "piminus" ]; then
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
printf "<imageblob>${beam}-${target}-${momentum[$i]}GeV-${secondary[$j]}-Pub2015-${models[$m]}-regre-${region[$k]}.gif</imageblob>\n" >> ${script}

printf "<taglist>\n" >> ${script}
printf "<tag>\n" >> ${script}
printf "<name>Model</name>\n" >> ${script}
printf "<value>${models[$m]}</value>\n" >> ${script}
printf "</tag>\n" >> ${script}
printf "<tag>\n" >> ${script}
printf "<name>Regression testing:</name>\n" >> ${script}
#
# printf "<value>geant4 versions: g4-9.6-p04,g4-10.0-p04,g4-10.1,${g4version}</value>\n" >> ${script}
printf "<value>geant4 versions: " >> ${script}
for (( r=0; r<${#regression[@]}; ++r )) do
printf "${regression[$r]}," >> ${script}
done
printf "${g4version}</value>\n" >> ${script}
#
printf "</tag>\n" >> ${script}
printf "</taglist>\n" >> ${script}

printf "</tests>\n" >> ${script}

printf "\n" >> ${script}

done

done

done
 
done

printf "</testslist>\n" >> ${script}

# mv ${script} /home/g4p/pbs/g4-had-validation/results/${g4version}/${testname}/regre
