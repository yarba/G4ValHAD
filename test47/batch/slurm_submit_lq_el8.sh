#!/usr/bin/env bash
#

# EL8

sbatch -N 1 -n 8 -c 1 -p lq1_cpu --exclusive  -A g4 \
	slurm_master.sh test47_run_itep_lq_el8.sh proton # with-fluka.cern.4.4.0

sbatch -N 1 -n 8 -c 1 -p lq1_cpu --exclusive  -A g4 \
	slurm_master.sh test47_run_itep_lq_el8.sh piplus # with-fluka.cern.4.4.0

sbatch -N 1 -n 8 -c 1 -p lq1_cpu --exclusive  -A g4 \
	slurm_master.sh test47_run_itep_lq_el8.sh piminus # with-fluka.cern.4.4.0


