#!bin/bash


echo
echo " Plot event selection "
echo " ==================== "
echo


# Output files date
read -p " Enter directory date of input files [YYYY-MM-DD]: " OPT_DATE
echo


# MC models
echo " Enter baseline MC model(s) separated by space: "
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


# Include systematics?
read -p " Include systematics? [true/false]: " OPT_SYST

if [ "$OPT_SYST" != "true" ] && [ "$OPT_SYST" != "false" ]; then
    echo
    echo " ERROR: Enter either 'true' or 'false' "
    echo
    return 1
fi
echo


# Run script
for model in ${OPT_MODELS[@]}; do
    root -b -l -q "loadLibs.C+" "EventSelection/PlotEventSelection.C+(\"${OPT_DATE}\", \"${model}\", ${OPT_SYST})";
done
