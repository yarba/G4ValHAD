#!/usr/bin/env bash
#

sbatch -N 1 -n 12 -c 1 -p cpu_gce --exclusive --qos=regular --time=16:00:00 -A g4v \
 	--export="G4RELEASE=${1},beam=proton" \
	slurm_master.sh test19_run_harp_wcic.sh 

#sbatch -N 1 -n 12 -c 1 -p cpu_gce --exclusive --qos=regular --time=16:00:00 -A g4v \
# 	--export="G4RELEASE=${1},beam=piplus" \
#	slurm_master.sh test19_run_harp_wcic.sh 

#sbatch -N 1 -n 12 -c 1 -p cpu_gce --exclusive --qos=regular --time=16:00:00 -A g4v \
# 	--export="G4RELEASE=${1},beam=piminus" \
#	slurm_master.sh test19_run_harp_wcic.sh 

