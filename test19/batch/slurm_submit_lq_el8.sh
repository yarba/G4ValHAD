#!/usr/bin/env bash
#

# EL8

sbatch -N 1 -n 12 -c 1 -p lq1_cpu --exclusive  -A g4 \
	slurm_master.sh test19_run_harp_lq_el8.sh proton # with-fluka.cern.4.4.0

sbatch -N 1 -n 12 -c 1 -p lq1_cpu --exclusive  -A g4 \
	slurm_master.sh test19_run_harp_lq_el8.sh piplus # with-fluka.cern.4.4.0

sbatch -N 1 -n 12 -c 1 -p lq1_cpu --exclusive  -A g4 \
	slurm_master.sh test19_run_harp_lq_el8.sh piminus # with-fluka.cern.4.4.0

#sbatch -N 1 -n 1 -c 1 -p lq1_cpu --exclusive  -A g4 \
#	slurm_master.sh test19_run_na61_lq_el8.sh proton

#sbatch -N 1 -n 1 -c 1 -p lq1_cpu --exclusive  -A g4 \
#	slurm_master.sh test19_run_na61_lq_el8.sh piplus

# sbatch -N 1 -n 1 -c 1 -p lq1_cpu --exclusive  -A g4 \
#	slurm_master.sh test19_run_na49_lq_el8.sh 

#target_sasm6e=(C Cu Pb)
#beam_sasm6e=(proton piplus)

#for (( i=0; i<${#target_sasm6e[@]}; ++i )) do
#for (( j=0; j<${#beam_sasm6e[@]}; ++j )) do

#sbatch -N 1 -n 16 -c 1 -p wc_cpu --exclusive --qos=regular --time=23:00:00 -A g4v \
#	slurm_master.sh test19_run_sasm6e_wcic.sh ${beam_sasm6e[$j]} ${target_sasm6e[$i]} 62500

#done
#done

