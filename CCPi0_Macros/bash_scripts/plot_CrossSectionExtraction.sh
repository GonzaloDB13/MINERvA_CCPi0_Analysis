#!bin/bash


echo
echo " Plot cross section extraction "
echo " ============================= "
echo


# Output files date
read -p " Enter directory date of input files [YYYY-MM-DD]: " OPT_DATE
echo


# MC models
echo " Enter MC baseline model(s) separated by space: "
read -a OPT_MODELS

for model in ${OPT_MODELS[@]}; do
    if [ "$model" != "v0" ] && [ "$model" != "v1" ] &&
       [ "$model" != "v1noNonResPi" ] && [ "$model" != "v1noD2" ] && [ "$model" != "v1noPionTune" ] &&
       [ "$model" != "v2MINOS" ] && [ "$model" != "v2JOINT" ] && [ "$model" != "v2NU1PI" ] &&
       [ "$model" != "v2NUNPI" ] && [ "$model" != "v2NUPI0" ] && [ "$model" != "v2MENU1PI" ]; then
        echo
        echo " ERROR: Bad model option, select correct model(s) "
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


# Plot unfolding and following steps?
read -p " Plot unfolding and following steps? [true/false]: " OPT_PLOT_UNFOLDING

if [ "$OPT_PLOT_UNFOLDING" != "true" ] && [ "$OPT_PLOT_UNFOLDING" != "false" ]; then
    echo
    echo " ERROR: Enter either 'true' or 'false' "
    echo
    return 1
fi
echo


# Run script
for model in ${OPT_MODELS[@]}; do
    for fit_function in ${OPT_FIT_FUNCTIONS[@]}; do
        root -b -l -q "loadLibs.C+" "CrossSectionExtraction/PlotCrossSectionExtraction.C+(\"${OPT_DATE}\", \"${model}\", \"${fit_function}\", ${OPT_PLOT_UNFOLDING})";
    done
done
