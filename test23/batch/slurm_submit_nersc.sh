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
# --> pl0=(ShieldingM ftfp_bert)
pl0=(G4_HP_CFLUKAHI)

for (( p=0; p<${#pl0[@]}; ++p )) do

sbatch -N 1 -n 100 -c 2 --constraint cpu -A m4599 --exclusive --qos=regular --time=6:00:00 \
	slurm_master.sh sim_perCore_harp_nersc.sh proton C 8000 ${pl0[$p]}

sbatch -N 1 -n 100 -c 2 --constraint cpu -A m4599 --exclusive --qos=regular --time=6:00:00\
	slurm_master.sh sim_perCore_harp_nersc.sh proton Ta 8000 ${pl0[$p]}

sbatch -N 1 -n 100 -c 2 --constraint cpu -A m4599 --exclusive --qos=regular --time=6:00:00 \
	slurm_master.sh sim_perCore_harp_nersc.sh piplus C 8000 ${pl0[$p]}

sbatch -N 1 -n 100 -c 2 --constraint cpu -A m4599 --exclusive --qos=regular --time=6:00:00 \
	slurm_master.sh sim_perCore_harp_nersc.sh piplus Ta 8000 ${pl0[$p]}

done

