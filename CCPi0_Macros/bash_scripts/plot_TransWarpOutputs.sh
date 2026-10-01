#!bin/bash


echo
echo " Plot outputs from TransWarpExtract "
echo " ================================== "
echo


# Output files date
read -p " Enter directory date of input files [YYYY-MM-DD]: " OPT_DATE
echo


# Baseline MC models
echo " Enter baseline MC model(s) separated by space: "
read -a OPT_MC_MODELS

for mc_model in ${OPT_MC_MODELS[@]}; do
    if [ "$mc_model" != "v0" ] && [ "$mc_model" != "v1" ] &&
       [ "$mc_model" != "v1noNonResPi" ] && [ "$mc_model" != "v1noD2" ] && [ "$mc_model" != "v1noPionTune" ] &&
       [ "$mc_model" != "v2MINOS" ] && [ "$mc_model" != "v2JOINT" ] && [ "$mc_model" != "v2NU1PI" ] &&
       [ "$mc_model" != "v2NUNPI" ] && [ "$mc_model" != "v2NUPI0" ] && [ "$mc_model" != "v2MENU1PI" ]; then
        echo
        echo " ERROR: Bad baseline MC model option, select correct model(s) "
        echo
        return 1
    fi
done
echo


# Fake data MC models
echo " Enter fake data model(s) separated by space: "
read -a OPT_FD_MODELS

for fd_model in ${OPT_FD_MODELS[@]}; do
    if [ "$fd_model" != "v0" ] && [ "$fd_model" != "v1" ] &&
       [ "$fd_model" != "v1noNonResPi" ] && [ "$fd_model" != "v1noD2" ] && [ "$fd_model" != "v1noPionTune" ] &&
       [ "$fd_model" != "v2MINOS" ] && [ "$fd_model" != "v2JOINT" ] && [ "$fd_model" != "v2NU1PI" ] &&
       [ "$fd_model" != "v2NUNPI" ] && [ "$fd_model" != "v2NUPI0" ] && [ "$fd_model" != "v2MENU1PI" ]; then
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


# Run script
for mc_model in ${OPT_MC_MODELS[@]}; do
    for fd_model in ${OPT_FD_MODELS[@]}; do
        for material in ${OPT_MATERIALS[@]}; do
            root -b -l -q "loadLibs.C+" "WarpingStudies/PlotTransWarpOutputs.C+(\"${OPT_DATE}\", \"${mc_model}\", \"${fd_model}\", \"${material}\")";
        done
    done
done
