#!/bin/bash


echo
echo " Run Dan's ProcessMCSampleSizeScan.py "
echo " ==================================== "
echo


# General options
# ===============

# Date
read -p " Enter directory date of input files [YYYY-MM-DD]: " OPT_DATE
echo


# Baseline MC model
read -p " Enter baseline MC model: " OPT_MC_MODEL
echo

if [ "$OPT_MC_MODEL" != "v0" ] && [ "$OPT_MC_MODEL" != "v1" ] &&
   [ "$OPT_MC_MODEL" != "v1noNonResPi" ] && [ "$OPT_MC_MODEL" != "v1noD2" ] && [ "$OPT_MC_MODEL" != "v1noPionTune" ] &&
   [ "$OPT_MC_MODEL" != "v2MINOS" ] && [ "$OPT_MC_MODEL" != "v2JOINT" ] && [ "$OPT_MC_MODEL" != "v2NU1PI" ] &&
   [ "$OPT_MC_MODEL" != "v2NUNPI" ] && [ "$OPT_MC_MODEL" != "v2NUPI0" ] && [ "$OPT_MC_MODEL" != "v2MENU1PI" ]; then
    echo
    echo " ERROR: Bad baseline MC model option, select correct model "
    echo
    return 1
fi


# Material
read -p " Enter material: " OPT_MATERIAL
echo

if [ "$OPT_MATERIAL" != "lead" ] && [ "$OPT_MATERIAL" != "iron" ]; then
    echo " ERROR: Select either 'lead' or 'iron' "
    echo
    return 1
fi


# Number of Poisson universes
read -p " Enter number of Poisson universes: " OPT_N_UNIVERSES
echo


# Uncertainty factors
read -p " Enter uncertainty factor to test: " OPT_UNCFACTOR
echo


# Run macro for each stat factor from TransWarp
STAT_FACTORS=(10 11 11.1345 12 13)

for factor in ${STAT_FACTORS[@]}; do

    python UnfoldStatStudies/ProcessMCSampleSizeScan.py --date=${OPT_DATE} --model=${OPT_MC_MODEL} --material=${OPT_MATERIAL} --n_uni=${OPT_N_UNIVERSES} --iters=1,2,3,4,5,6,7,8,9,10 --input=$PNFS/MAT/UnfoldStatStudies/${OPT_DATE}_${OPT_MC_MODEL}/${OPT_MATERIAL}/TransWarpOutputs/UnfoldStatWarping_StatFactor${factor}_${OPT_MATERIAL}.root --uncfactor=${OPT_UNCFACTOR} --f_option_used_transwarp=${factor}
    
done
