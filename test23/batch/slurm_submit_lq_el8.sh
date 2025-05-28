#!/usr/bin/env bash
#
# cleanup logs/err from earlier rounds
#
# ---> not now ---> /bin/rm -f slurm*.out
#
# NOTE-1 : upon migration to EL8 there's no longer a possibility
#          to pass in an env.var. (--export) AND use "module load"
#          as in the later the system will report "no command module";
#          so it's either one or the other...
#          which leads to re-making into input arguments or the likes
#          whatever used to be passed via the --export option
#
# NOTE-2 : --qos=regular does NOT work on LQ1 (for whatever reason...)
#
# NOTE-3 : also remove --time=23:00:00
#          can't remember what's the default time but maybe 23hrs is too much...


# --> pl0=(ShieldingM Shielding ftfp_bert qgsp_bert)
pl0=(ShieldingM ftfp_bert)  # -->  G4_HP_CFLUKAHI)
# --> pl0=( G4_HP_CFLUKAHI )

for (( p=0; p<${#pl0[@]}; ++p )) do

sbatch -N 1 -n 40 -c 1 -p lq1_cpu -A g4 --exclusive \
	slurm_master.sh sim_perCore_harp_lq_el8.sh proton C 3000 ${pl0[$p]}

sbatch -N 1 -n 40 -c 1 -p lq1_cpu -A g4 --exclusive  \
	slurm_master.sh sim_perCore_harp_lq_el8.sh piplus C 3000 ${pl0[$p]}

sbatch -N 1 -n 40 -c 1 -p lq1_cpu -A g4 --exclusive \
	slurm_master.sh sim_perCore_harp_lq_el8.sh proton Ta 3000 ${pl0[$p]}

sbatch -N 1 -n 40 -c 1 -p lq1_cpu -A g4 --exclusive  \
	slurm_master.sh sim_perCore_harp_lq_el8.sh piplus Ta 3000 ${pl0[$p]}

sbatch -N 1 -n 40 -c 1 -p lq1_cpu -A g4 --exclusive \
	slurm_master.sh sim_perCore_harp_lq_el8.sh proton C 8000 ${pl0[$p]}

sbatch -N 1 -n 40 -c 1 -p lq1_cpu -A g4 --exclusive  \
	slurm_master.sh sim_perCore_harp_lq_el8.sh piplus C 8000 ${pl0[$p]}

sbatch -N 1 -n 40 -c 1 -p lq1_cpu -A g4 --exclusive \
	slurm_master.sh sim_perCore_harp_lq_el8.sh proton Ta 8000 ${pl0[$p]}

sbatch -N 1 -n 40 -c 1 -p lq1_cpu -A g4 --exclusive  \
	slurm_master.sh sim_perCore_harp_lq_el8.sh piplus Ta 8000 ${pl0[$p]}

done

