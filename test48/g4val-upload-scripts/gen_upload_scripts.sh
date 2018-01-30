#!/usr/bin/env bash
#
# usage:
# source generate_upload_scripts.sh geant4-10.00-ref08 internal
#

g4version=${1}
status=${2}

./gen_upload_script_pion_models.sh ${g4version} ${status}
./gen_upload_script_pion_regre.sh ${g4version} ${status}

./gen_upload_script_kaon_models.sh ${g4version} ${status}
./gen_upload_script_kaon_regre.sh ${g4version} ${status}
 
./gen_upload_script_pbar_models.sh ${g4version} ${status}
./gen_upload_script_pbar_regre.sh ${g4version} ${status}

./gen_upload_script_muon_models.sh ${g4version} ${status}
./gen_upload_script_muon_regre.sh ${g4version} ${status}
