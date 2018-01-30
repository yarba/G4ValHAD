#!/usr/bin/env bash
#
# usage:
# source generate_upload_scripts.sh geant4-10.00-ref08 internal
#

g4version=${1}
status=${2}

source generate_upload_script_na61_models.sh proton C ${g4version} ${status}
source generate_upload_script_na61_regre.sh proton C ${g4version} ${status}

source generate_upload_script_na49_models.sh proton C ${g4version} ${status}
source generate_upload_script_na49_regre.sh proton C ${g4version} ${status}

