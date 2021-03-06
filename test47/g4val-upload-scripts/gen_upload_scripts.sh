#!/usr/bin/env bash
#
# usage:
# source generate_upload_scripts.sh geant4-10.00-ref08 internal
#

g4version=${1}
status=${2}

./gen_upload_script_itep_models.sh ${g4version} ${status}
./gen_upload_script_itep_regre.sh ${g4version} ${status}
./gen_upload_script_pbarprod_models.sh ${g4version} ${status}
./gen_upload_script_pbarprod_regre.sh ${g4version} ${status}

