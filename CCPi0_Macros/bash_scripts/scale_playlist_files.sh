#!bin/bash


# ===============================================================================
#
# Perform POT-scaling of MC histograms per playlist.
# 
# This bash script runs the macro 'POTScalePlaylist.C' that gets the following info:
# -> Playlist
# -> Date of directory to store the files
# -> MC model
# -> Use of Truth tree?
# -> Use of systematics universes?
#
# In the same directory where the not-scaled files lie, one for each playlist,
# a new set of files are created, one for each playlist too, where the distributions
# have been POT-scaled accordingly.
#
# Additionally, for each playlist, a .txt file is created in BLUEARC
# with information of the MC and data POT of that playlist.
#
# ===============================================================================


echo
echo " POT-scale MC histograms per playlist "
echo " ==================================== "
echo
echo " Select among the following options: "
echo "   [1]: Event selection, migration matrix, efficiency num & den, and pre-tuning distributions "
echo "   [2]: Reconstruction cuts studies. "
echo "   [3]: Sideband studies. "
echo "   [5]: Fake data models for warping studies (MC-only). "
echo "   [6]: Alternative efficiency distributions (MC-only). "
echo "   [7]: Supporting distributions for different purposes. "
echo
read -p " Which option? " OPT_MACRO
echo



# MC model
# ========

read -p " Enter MC model: " OPT_MODEL
echo

if [ "$OPT_MODEL" != "v0" ] && [ "$OPT_MODEL" != "v1" ] &&
   [ "$OPT_MODEL" != "v1noNonResPi" ] && [ "$OPT_MODEL" != "v1noD2" ] && [ "$OPT_MODEL" != "v1noPionTune" ] &&
   [ "$OPT_MODEL" != "v2MINOS" ] && [ "$OPT_MODEL" != "v2JOINT" ] && [ "$OPT_MODEL" != "v2NU1PI" ] &&
   [ "$OPT_MODEL" != "v2NUNPI" ] && [ "$OPT_MODEL" != "v2NUPI0" ] && [ "$OPT_MODEL" != "v2MENU1PI" ]; then
    echo
    echo " ERROR: Bad model option, select correct model "
    echo
    return 1
fi



# Include systematics?
# ====================

read -p " Include systematics? [true/false]: " OPT_SYST

if [ "$OPT_SYST" != "true" ] && [ "$OPT_SYST" != "false" ]; then
    echo
    echo " ERROR: Enter either 'true' or 'false' "
    echo
    return 1
fi
echo



# Output files date
# =================

read -p " Enter directory date of ROOT files [YYYY-MM-DD]: " OPT_DATE
echo



# Create POT .txt file directory
# ==============================

if [ "$OPT_MACRO" == "1" ]; then
    OPT_POTDATE="${OPT_DATE}_${OPT_MODEL}"
    mkdir -p $BLUEARC/MAT/POTinfo/$OPT_POTDATE
fi



# Loop over playlists
# ===================

declare -a playlists=("minervame1A" "minervame1B" "minervame1C" "minervame1D"
                      "minervame1E" "minervame1F" "minervame1G" "minervame1L"
                      "minervame1M" "minervame1N" "minervame1O" "minervame1P")

                      
for playlist in "${playlists[@]}"; do

    if [ "$OPT_MACRO" == "1" ]; then
        root -l -b -q "loadLibs.C+" "EventSelection/POTScalePlaylist.C+(\"${playlist}\", \"${OPT_DATE}\", \"${OPT_MODEL}\", ${OPT_SYST})";
        
    elif [ "$OPT_MACRO" == "2" ]; then
        root -l -b -q "loadLibs.C+" "CutStudies/POTScalePlaylist.C+(\"${playlist}\", \"${OPT_DATE}\", \"${OPT_MODEL}\", ${OPT_SYST})";
    
    elif [ "$OPT_MACRO" == "3" ]; then
        root -l -b -q "loadLibs.C+" "SidebandStudies/POTScalePlaylist.C+(\"${playlist}\", \"${OPT_DATE}\", \"${OPT_MODEL}\", ${OPT_SYST})";
    
    elif [ "$OPT_MACRO" == "5" ]; then
        root -l -b -q "loadLibs.C+" "FakeDataModels/POTScalePlaylist.C+(\"${playlist}\", \"${OPT_DATE}\", \"${OPT_MODEL}\", ${OPT_SYST})";
        
    elif [ "$OPT_MACRO" == "7" ]; then
        root -l -b -q "loadLibs.C+" "SupportStudies/POTScalePlaylist.C+(\"${playlist}\", \"${OPT_DATE}\", \"${OPT_MODEL}\", ${OPT_SYST})";
        
    fi
done
