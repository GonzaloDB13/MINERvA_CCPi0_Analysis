#!bin/bash


echo
echo " Make x-section model histograms "
echo " =============================== "
echo


# Output files date
read -p " Enter directory date of input files [YYYY-MM-DD]: " OPT_DATE
echo


# Baseline MC models
echo " Enter baseline MC model(s) used in the reconstruction separated by space: "
read -a OPT_MC_MODELS

for mc_model in ${OPT_MC_MODELS[@]}; do
    if [ "$mc_model" != "v0" ] && [ "$mc_model" != "v1" ] &&
       [ "$mc_model" != "v1noNonResPi" ] && [ "$mc_model" != "v1noD2" ] && [ "$mc_model" != "v1noPionTune" ] &&
       [ "$mc_model" != "v2MINOS" ] && [ "$mc_model" != "v2JOINT" ] && [ "$momc_modeldel" != "v2NU1PI" ] &&
       [ "$mc_model" != "v2NUNPI" ] && [ "$mc_model" != "v2NUPI0" ] && [ "$mc_model" != "v2MENU1PI" ]; then
        echo
        echo " ERROR: Bad baseline MC model option, select correct model(s) "
        echo
        return 1
    fi
done
echo


# X-section MC models
echo " Enter x-section model(s) separated by space: "
read -a OPT_XSEC_MODELS

for xsec_model in ${OPT_XSEC_MODELS[@]}; do
    if [ "$xsec_model" != "v0" ] && [ "$xsec_model" != "v1" ] &&
       [ "$xsec_model" != "v1noNonResPi" ] && [ "$xsec_model" != "v1noD2" ] && [ "$xsec_model" != "v1noPionTune" ] &&
       [ "$xsec_model" != "v2MINOS" ] && [ "$xsec_model" != "v2JOINT" ] && [ "$xsec_model" != "v2NU1PI" ] &&
       [ "$xsec_model" != "v2NUNPI" ] && [ "$xsec_model" != "v2NUPI0" ] && [ "$xsec_model" != "v2MENU1PI" ]; then
        echo
        echo " ERROR: Bad fake data models option, select correct model(s) "
        echo
        return 1
    fi
done
echo


# Materials
echo " Enter analysis material(s) separated by space: "
read -a OPT_MATERIALS

for material in ${OPT_MATERIALS[@]}; do
    if [ "$material" != "lead" ] && [ "$material" != "iron" ]; then
        echo
        echo " ERROR: Bad material option, select correct material(s) "
        echo
        return 1
    fi
done
echo


# Background fit functions
echo " Enter background fit function(s) separated by space [Linear/Bilinear/etc...]: "
read -a OPT_FIT_FUNCTIONS

for fit_function in ${OPT_FIT_FUNCTIONS[@]}; do
    if [ "$fit_function" != "Scalar" ] &&
       [ "$fit_function" != "Linear" ] && [ "$fit_function" != "LinearAlt" ] &&
       [ "$fit_function" != "Bilinear" ] && [ "$fit_function" != "BilinearAlt" ]; then
        echo
        echo " ERROR: Bad fit function(s), select correct function(s) "
        echo
        return 1
    fi
done
echo


# Run script
for mc_model in ${OPT_MC_MODELS[@]}; do
    for xsec_model in ${OPT_XSEC_MODELS[@]}; do
        for material in ${OPT_MATERIALS[@]}; do
            for fit_function in ${OPT_FIT_FUNCTIONS[@]}; do
                root -b -l -q "loadLibs.C+" "XsectionModels/XsectionModels.C+(\"${OPT_DATE}\", \"${mc_model}\", \"${xsec_model}\", \"${material}\", \"${fit_function}\")";
            done
        done
    done
done
