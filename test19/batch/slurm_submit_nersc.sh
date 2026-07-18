#!/usr/bin/env bash
#

# NERSC

sbatch -N 1 -n 4 -c 2 --constraint cpu -A m4599 --exclusive --qos=regular --time=12:00:00 \
	slurm_master.sh test19_run_harp_nersc.sh proton # with-fluka.cern.4.5.0

#sbatch -N 1 -n 12 -c 2 --constraint cpu -A m4599 --exclusive --qos=regular --time=12:00:00 \
#	slurm_master.sh test19_run_harp_nersc.sh piplus # with-fluka.cern.4.5.0

#sbatch -N 1 -n 12 -c 2 --constraint cpu -A m4599 --exclusive --qos=regular --time=12:00:00 \
#	slurm_master.sh test19_run_harp_nersc.sh piminus # with-fluka.cern.4.5.0

#sbatch -N 1 -n 1 -c 2 --constraint cpu -A m4599 --exclusive --qos=regular --time=3:00:00 \
#	slurm_master.sh test19_run_na61_nersc.sh proton

#sbatch -N 1 -n 1 -c 2 --constraint cpu -A m4599 --exclusive --qos=regular --time=3:00:00 \
#	slurm_master.sh test19_run_na61_nersc.sh piplus

#sbatch -N 1 -n 1 -c 2 --constraint cpu -A m4599 --exclusive --qos=regular --time=3:00:00 \
#	slurm_master.sh test19_run_na49_nersc.sh 

#target_sasm6e=(C Cu Pb)
#beam_sasm6e=(proton piplus)

#for (( i=0; i<${#target_sasm6e[@]}; ++i )) do
#for (( j=0; j<${#beam_sasm6e[@]}; ++j )) do

#sbatch -N 1 -n 16 -c 1 -p wc_cpu --exclusive --qos=regular --time=23:00:00 -A g4v \
#	slurm_master.sh test19_run_sasm6e_wcic.sh ${beam_sasm6e[$j]} ${target_sasm6e[$i]} 62500

#done
#done

