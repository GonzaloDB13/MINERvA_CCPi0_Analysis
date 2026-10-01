#!/bin/bash


echo
echo " Run unfolding statistical warping "
echo " ================================= "
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
read -p " Enter number of Poisson universes " OPT_N_UNIVERSES
echo



# Input histograms
# ================

# Input top directory
INPUT_TOPDIR="/pnfs/minerva/persistent/users/gonzalo/MAT/WarpingStudies/${OPT_DATE}_${OPT_MC_MODEL}/${OPT_MATERIAL}/TransWarpInputs"


# Baseline MC inputs histograms
MIGRATION_HISTO="h_migration_MuonPt"
RECO_TRUTH_HISTO="h_reco_truth_MuonPt"
RECO_HISTO="h_reco_MuonPt"


# Fake data input histograms
DATA_TRUTH_HISTO="h_data_truth_MuonPt"
DATA_HISTO="h_data_MuonPt"



# Other options
# =============

# Output top directory
OUTPUT_TOPDIR="/pnfs/minerva/persistent/users/gonzalo/MAT/UnfoldStatStudies/${OPT_DATE}_${OPT_MC_MODEL}/${OPT_MATERIAL}/TransWarpOutputs"


# Other options
DIMENSIONS="1"
ITERATIONS="1,2,3,4,5,6,7,8,9,10"
# N_STAT_UNIVERSES="1000"
MAX_CHI2="50"
STEP_CHI2="1"
DATA_POT_NORM="1"


# Scaling factors
STAT_FACTORS=(10 11 11.1345 12 13)


# Run TransWarp iteratively
# =========================

for factor in ${STAT_FACTORS[@]}; do

    input_file="${INPUT_TOPDIR}/BaselineMC_${OPT_MC_MODEL}_FakeData_${OPT_MC_MODEL}_${OPT_MATERIAL}.root"
    
    output_file="${OUTPUT_TOPDIR}/UnfoldStatWarping_StatFactor${factor}_${OPT_MATERIAL}.root"
    
    echo
    echo " Verifying options: "
    echo
    echo -e "\t Baseline MC input file: " ${input_file}
    echo -e "\t Fake data input file:   " ${input_file}
    echo -e "\t Output file:            " ${output_file}
    echo
    echo -e "\t Analysis dimensions:    " $DIMENSIONS
    echo -e "\t Number of iterations:   " $ITERATIONS
    echo -e "\t Num. of stat universes: " ${OPT_N_UNIVERSES}
    echo -e "\t Maximum chi2:     " $MAX_CHI2
    echo -e "\t Size of chi2 bin: " $STEP_CHI2
    echo -e "\t POT of data norm: " $DATA_POT_NORM
    echo
    echo -e "\t Statistical scale factor: " ${factor}
    echo
    # read -p " Press 'y' to proceed: " OPT_PROCEED
    # if [ "$OPT_PROCEED" != "y" ]; then
    #     echo
    #     echo " Don't proceed! Check your inputs "
    #     echo
    #     return 1
    # fi
    # echo
    # echo " Running TransWarpExtraction... "
    # echo
    
    TransWarpExtraction -o $output_file \
                        -d $DATA_HISTO \
                        -D $input_file \
                        -i $DATA_TRUTH_HISTO \
                        -I $input_file \
                        -m $MIGRATION_HISTO \
                        -M $input_file \
                        -r $RECO_HISTO \
                        -R $input_file \
                        -t $RECO_TRUTH_HISTO \
                        -T $input_file \
                        -z $DIMENSIONS \
                        -n $ITERATIONS \
                        -u $OPT_N_UNIVERSES \
                        -c $MAX_CHI2 \
                        -C $STEP_CHI2 \
                        -P $DATA_POT_NORM \
                        -f ${factor}
done
