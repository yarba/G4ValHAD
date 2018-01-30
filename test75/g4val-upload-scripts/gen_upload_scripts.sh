#!/usr/bin/env bash
#
# usage:
# source gen_upload_scripts.sh geant4-10.00-ref08 internal
#

g4version=${1}
status=${2}

./gen_upload_script_models.sh ${g4version} ${status}
./gen_upload_script_regre.sh ${g4version} ${status}

