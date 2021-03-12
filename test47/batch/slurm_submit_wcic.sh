#!/usr/bin/env bash
#

sbatch -N 1 -n 3 -c 1 -p cpu_gce --exclusive --qos=regular --time=16:00:00 -A g4v \
 	--export="G4RELEASE=${1}" \
	slurm_master.sh test47_run_itep_wcic.sh 

