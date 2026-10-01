#!/bin/bash


echo
echo " Run TransWarpExtract "
echo " ==================== "
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


# Fake data MC models
echo " Enter fake data model(s) separated by space: "
read -a OPT_FD_MODELS

for fd_model in ${OPT_FD_MODELS[@]}; do
    if [ "$fd_model" != "v0" ] && [ "$fd_model" != "v1" ] &&
       [ "$fd_model" != "v1noNonResPi" ] && [ "$fd_model" != "v1noD2" ] && [ "$fd_model" != "v1noPionTune" ] &&
       [ "$fd_model" != "v2MINOS" ] && [ "$fd_model" != "v2JOINT" ] && [ "$fd_model" != "v2NU1PI" ] &&
       [ "$fd_model" != "v2NUNPI" ] && [ "$fd_model" != "v2NUPI0" ] && [ "$fd_model" != "v2MENU1PI" ]; then
        echo
        echo " ERROR: Bad fake data model(s) option, select correct model(s) "
        echo
        return 1
    fi
done
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
OUTPUT_TOPDIR="/pnfs/minerva/persistent/users/gonzalo/MAT/WarpingStudies/${OPT_DATE}_${OPT_MC_MODEL}/${OPT_MATERIAL}/TransWarpOutputs"


# Other options
DIMENSIONS="1"
ITERATIONS="0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,18,20,25"
N_STAT_UNIVERSES="1000"
MAX_CHI2="100"
STEP_CHI2="1"
DATA_POT_NORM="1"



# Run TransWarp iteratively
# =========================

for fd_model in ${OPT_FD_MODELS[@]}; do

    input_file="${INPUT_TOPDIR}/BaselineMC_${OPT_MC_MODEL}_FakeData_${fd_model}_${OPT_MATERIAL}.root"
    
    output_file="${OUTPUT_TOPDIR}/Warping_MC_${OPT_MC_MODEL}_FakeData_${fd_model}_${OPT_MATERIAL}.root"
    
    echo
    echo " Verifying options: "
    echo
    echo -e "\t Baseline MC input file: ${input_file}"
    echo -e "\t Fake data input file:   ${input_file}"
    echo -e "\t Output file:            ${output_file}"
    echo
    echo -e "\t Analysis dimensions:    " $DIMENSIONS
    echo -e "\t Number of iterations:   " $ITERATIONS
    echo -e "\t Num. of stat universes: " $N_STAT_UNIVERSES
    echo -e "\t Maximum chi2:     " $MAX_CHI2
    echo -e "\t Size of chi2 bin: " $STEP_CHI2
    echo -e "\t POT of data norm: " $DATA_POT_NORM
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
                        -u $N_STAT_UNIVERSES \
                        -c $MAX_CHI2 \
                        -C $STEP_CHI2 \
                        -P $DATA_POT_NORM
done
