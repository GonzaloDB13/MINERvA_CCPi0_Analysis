#!bin/bash


# ===============================================================================
#
# Merge playlist ROOT files into one corresponding to the full dataset.
# 
# This macro merges all of the data and MC (POT scaled) files of each playlist
# into one big file with distributions corresponding to such dataset.
# And finally performs bin width normalization on the overall full histogram.
#
# Similarly to when merging within the same playlist, input options include:
# -> Macro type whose files have to be merged
# -> MC or data?
# -> Model used (if MC)
# -> Use of systematics universes?
#
# ===============================================================================


echo
echo " Merging playlist ROOT files "
echo " =========================== "
echo
echo " Select among the following options: "
echo "   [1]: Event selection, migration matrix, efficiency num & den, and plastic/physics pre-tuning distributions. "
echo "   [2]: Reconstruction cuts studies. "
echo "   [3]: Sideband studies. "
echo "   [4]: Efficiency optimization as function of true muon angle (MC-only). "
echo "   [5]: Fake data models for warping studies (MC-only). "
echo "   [6]: Alternative efficiency distributions (MC-only). "
echo "   [7]: Supporting distributions for different purposes. "
echo
read -p " Which option? " OPT_MACRO
echo



# MC or data?
# ===========

read -p " Are these files MC or data? [mc/data]: " OPT_MC_DATA

if [ "$OPT_MC_DATA" != "mc" ] && [ "$OPT_MC_DATA" != "data" ]; then
    echo
    echo " ERROR: Enter either 'mc' or 'data' "
    echo
    return 1

elif [ "$OPT_MC_DATA" == "mc" ]; then
    LABEL_MC_DATA="MC_"
    echo

elif [ "$OPT_MC_DATA" == "data" ]; then
    LABEL_MC_DATA="Data_"
    echo

fi



# Macro name
# ==========

if [ "$OPT_MACRO" == "1" ]; then
    LABEL_MACRO_EVSEL="EventSelection_"
    
    if [ "$OPT_MC_DATA" == "mc" ]; then
        LABEL_MACRO_PLASTUN="BeforePlasticTuning_"
        LABEL_MACRO_PHYSTUN="BeforePhysicsTuning_"
        
    elif [ "$OPT_MC_DATA" == "data" ]; then
        LABEL_MACRO_PLASTUN="PlasticTuning_"
        LABEL_MACRO_PHYSTUN="PhysicsTuning_"
    fi
    
elif [ "$OPT_MACRO" == "2" ]; then
    LABEL_MACRO="CutStudies_"
    
elif [ "$OPT_MACRO" == "3" ]; then
    LABEL_MACRO="SidebandStudies_"
    
elif [ "$OPT_MACRO" == "4" ]; then
    LABEL_MACRO="EfficiencyOptimize_"
    
elif [ "$OPT_MACRO" == "5" ]; then
    LABEL_MACRO="FakeData_"
    
elif [ "$OPT_MACRO" == "6" ]; then
    LABEL_MACRO="EfficiencyAlternative_"
    
elif [ "$OPT_MACRO" == "7" ]; then
    LABEL_MACRO="SupportStudies_"
    
fi



# MC model
# ========

if [ "$OPT_MC_DATA" == "mc" ]; then
    read -p " Enter MC model: " OPT_MODEL
    echo
    
    if [ "$OPT_MODEL" != "v0" ] && [ "$OPT_MODEL" != "v1" ] &&
       [ "$OPT_MODEL" != "v1noNonResPi" ] && [ "$OPT_MODEL" != "v1noD2" ] && [ "$OPT_MODEL" != "v1noPionTune" ] &&
       [ "$OPT_MODEL" != "v2MINOS" ] && [ "$OPT_MODEL" != "v2JOINT" ] && [ "$OPT_MODEL" != "v2NU1PI" ] &&
       [ "$OPT_MODEL" != "v2NUNPI" ] && [ "$OPT_MODEL" != "v2NUPI0" ] && [ "$OPT_MODEL" != "v2MENU1PI" ]; then
        echo " ERROR: Bad model option, select correct model "
        return 1
    else
        LABEL_MODEL="MnvGENIE${OPT_MODEL}_"
    fi
    
else
    LABEL_MODEL=""
fi



# Include systematics?
# ====================

if [ "$OPT_MC_DATA" == "mc" ]; then
    read -p " Include systematics? [true/false]: " OPT_SYST

    if [ "$OPT_SYST" != "true" ] && [ "$OPT_SYST" != "false" ]; then
        echo
        echo " ERROR: Enter either 'true' or 'false' "
        echo
        return 1
        
    elif [ "$OPT_SYST" == "true" ]; then
        LABEL_SYST="WithSyst_"
        
    elif [ "$OPT_SYST" == "false" ]; then
        LABEL_SYST="NoSyst_"
        
    fi
    echo
    
else
    OPT_SYST="false"
    LABEL_SYST=""
fi



# Input/output directories
# ========================

read -p " Enter date of ROOT files [YYYY-MM-DD]: " OPT_DATE

if [ "$OPT_MC_DATA" == "mc" ]; then
    OPT_DIR_DATE="${OPT_DATE}_${OPT_MODEL}"
    
elif [ "$OPT_MC_DATA" == "data" ]; then
    OPT_DIR_DATE="${OPT_DATE}"
fi


if [ "$OPT_MACRO" == "1" ]; then
    
    if [ "$OPT_MC_DATA" == "mc" ]; then
        OPT_DIR_EVSEL_LEAD=$PNFS/MAT/EventSelection/mc/$OPT_DIR_DATE/lead
        OPT_DIR_EVSEL_IRON=$PNFS/MAT/EventSelection/mc/$OPT_DIR_DATE/iron
        
        OPT_DIR_PLASTUN_LEAD=$PNFS/MAT/PlasticTuning/mc/$OPT_DIR_DATE/lead/BeforeTuning
        OPT_DIR_PLASTUN_IRON=$PNFS/MAT/PlasticTuning/mc/$OPT_DIR_DATE/iron/BeforeTuning
        
        OPT_DIR_PHYSTUN_LEAD=$PNFS/MAT/PhysicsTuning/mc/$OPT_DIR_DATE/lead/BeforeTuning
        OPT_DIR_PHYSTUN_IRON=$PNFS/MAT/PhysicsTuning/mc/$OPT_DIR_DATE/iron/BeforeTuning
        
    elif [ "$OPT_MC_DATA" == "data" ]; then
        OPT_DIR_EVSEL_LEAD=$PNFS/MAT/EventSelection/data/$OPT_DIR_DATE/lead
        OPT_DIR_EVSEL_IRON=$PNFS/MAT/EventSelection/data/$OPT_DIR_DATE/iron
        
        OPT_DIR_PLASTUN_LEAD=$PNFS/MAT/PlasticTuning/data/$OPT_DIR_DATE/lead
        OPT_DIR_PLASTUN_IRON=$PNFS/MAT/PlasticTuning/data/$OPT_DIR_DATE/iron
        
        OPT_DIR_PHYSTUN_LEAD=$PNFS/MAT/PhysicsTuning/data/$OPT_DIR_DATE/lead
        OPT_DIR_PHYSTUN_IRON=$PNFS/MAT/PhysicsTuning/data/$OPT_DIR_DATE/iron
    fi
    
    
elif [ "$OPT_MACRO" == "2" ]; then
    
    if [ "$OPT_MC_DATA" == "mc" ]; then
        OPT_DIR_LEAD=$PNFS/MAT/CutStudies/mc/$OPT_DIR_DATE/lead
        OPT_DIR_IRON=$PNFS/MAT/CutStudies/mc/$OPT_DIR_DATE/iron
        
    elif [ "$OPT_MC_DATA" == "data" ]; then
        OPT_DIR_LEAD=$PNFS/MAT/CutStudies/data/$OPT_DIR_DATE/lead
        OPT_DIR_IRON=$PNFS/MAT/CutStudies/data/$OPT_DIR_DATE/iron
    fi
    
    
elif [ "$OPT_MACRO" == "3" ]; then
    
    if [ "$OPT_MC_DATA" == "mc" ]; then
        OPT_DIR_LEAD=$PNFS/MAT/SidebandStudies/mc/$OPT_DIR_DATE/lead
        OPT_DIR_IRON=$PNFS/MAT/SidebandStudies/mc/$OPT_DIR_DATE/iron
        
    elif [ "$OPT_MC_DATA" == "data" ]; then
        OPT_DIR_LEAD=$PNFS/MAT/SidebandStudies/data/$OPT_DIR_DATE/lead
        OPT_DIR_IRON=$PNFS/MAT/SidebandStudies/data/$OPT_DIR_DATE/iron
    fi
    
    
elif [ "$OPT_MACRO" == "4" ]; then
    
    if [ "$OPT_MC_DATA" == "mc" ]; then
        OPT_DIR_LEAD=$PNFS/MAT/EfficiencyStudies/$OPT_DIR_DATE/lead
        OPT_DIR_IRON=$PNFS/MAT/EfficiencyStudies/$OPT_DIR_DATE/iron
    fi
    
    
elif [ "$OPT_MACRO" == "5" ]; then
    
    if [ "$OPT_MC_DATA" == "mc" ]; then
        OPT_DIR_LEAD=$PNFS/MAT/FakeDataModels/$OPT_DIR_DATE/lead
        OPT_DIR_IRON=$PNFS/MAT/FakeDataModels/$OPT_DIR_DATE/iron
    fi
    
    
elif [ "$OPT_MACRO" == "7" ]; then
    
    if [ "$OPT_MC_DATA" == "mc" ]; then
        OPT_DIR_LEAD=$PNFS/MAT/SupportStudies/mc/$OPT_DIR_DATE/lead
        OPT_DIR_IRON=$PNFS/MAT/SupportStudies/mc/$OPT_DIR_DATE/iron
    
    elif [ "$OPT_MC_DATA" == "data" ]; then
        OPT_DIR_LEAD=$PNFS/MAT/SupportStudies/data/$OPT_DIR_DATE/lead
        OPT_DIR_IRON=$PNFS/MAT/SupportStudies/data/$OPT_DIR_DATE/iron
    fi
fi
echo



# Merge files for event selectionand pre-tuning distributions
# ===========================================================

if [ "$OPT_MACRO" == "1" ]; then
    
    # Event selection distributions
    # -----------------------------
    
    cd $OPT_DIR_EVSEL_LEAD
    echo " Merging EVENT SELECTION distributions of all playlists on LEAD to directory: " $PWD
    
    if [ "$OPT_MC_DATA" == "mc" ]; then
        madd ${LABEL_MC_DATA}${LABEL_MACRO_EVSEL}${LABEL_MODEL}${LABEL_SYST}POTScaled_AllPlaylists_lead.root ${LABEL_MC_DATA}${LABEL_MACRO_EVSEL}${LABEL_MODEL}${LABEL_SYST}POTScaled_*_lead.root
        
    elif [ "$OPT_MC_DATA" == "data" ]; then
        madd ${LABEL_MC_DATA}${LABEL_MACRO_EVSEL}AllPlaylists_lead.root ${LABEL_MC_DATA}${LABEL_MACRO_EVSEL}*_lead.root
    fi
    echo
    
    cd $OPT_DIR_EVSEL_IRON
    echo " Merging EVENT SELECTION distributions of all playlists on IRON to directory: " $PWD
    
    if [ "$OPT_MC_DATA" == "mc" ]; then
        madd ${LABEL_MC_DATA}${LABEL_MACRO_EVSEL}${LABEL_MODEL}${LABEL_SYST}POTScaled_AllPlaylists_iron.root ${LABEL_MC_DATA}${LABEL_MACRO_EVSEL}${LABEL_MODEL}${LABEL_SYST}POTScaled_*_iron.root
        
    elif [ "$OPT_MC_DATA" == "data" ]; then
        madd ${LABEL_MC_DATA}${LABEL_MACRO_EVSEL}AllPlaylists_iron.root ${LABEL_MC_DATA}${LABEL_MACRO_EVSEL}*_iron.root
    fi
    echo
    
    
    # Pre-plastic tuning distributions
    # --------------------------------
    
    cd $OPT_DIR_PLASTUN_LEAD
    echo " Merging PRE-PLASTIC TUNING distributions of all playlists on LEAD to directory: " $PWD
    
    if [ "$OPT_MC_DATA" == "mc" ]; then
        madd ${LABEL_MC_DATA}${LABEL_MACRO_PLASTUN}${LABEL_MODEL}${LABEL_SYST}POTScaled_AllPlaylists_lead.root ${LABEL_MC_DATA}${LABEL_MACRO_PLASTUN}${LABEL_MODEL}${LABEL_SYST}POTScaled_*_lead.root
        
    elif [ "$OPT_MC_DATA" == "data" ]; then
        madd ${LABEL_MC_DATA}${LABEL_MACRO_PLASTUN}AllPlaylists_lead.root ${LABEL_MC_DATA}${LABEL_MACRO_PLASTUN}*_lead.root
    fi
    echo
    
    cd $OPT_DIR_PLASTUN_IRON
    echo " Merging PRE-PLASTIC TUNING distributions of all playlists on IRON to directory: " $PWD
    
    if [ "$OPT_MC_DATA" == "mc" ]; then
        madd ${LABEL_MC_DATA}${LABEL_MACRO_PLASTUN}${LABEL_MODEL}${LABEL_SYST}POTScaled_AllPlaylists_iron.root ${LABEL_MC_DATA}${LABEL_MACRO_PLASTUN}${LABEL_MODEL}${LABEL_SYST}POTScaled_*_iron.root
        
    elif [ "$OPT_MC_DATA" == "data" ]; then
        madd ${LABEL_MC_DATA}${LABEL_MACRO_PLASTUN}AllPlaylists_iron.root ${LABEL_MC_DATA}${LABEL_MACRO_PLASTUN}*_iron.root
    fi
    echo
    
    
    # Pre-physics tuning distributions
    # --------------------------------
    
    cd $OPT_DIR_PHYSTUN_LEAD
    echo " Merging PRE-PHYSICS TUNING distributions of all playlists on LEAD to directory: " $PWD
    
    if [ "$OPT_MC_DATA" == "mc" ]; then
        madd ${LABEL_MC_DATA}${LABEL_MACRO_PHYSTUN}${LABEL_MODEL}${LABEL_SYST}POTScaled_AllPlaylists_lead.root ${LABEL_MC_DATA}${LABEL_MACRO_PHYSTUN}${LABEL_MODEL}${LABEL_SYST}POTScaled_*_lead.root
        
    elif [ "$OPT_MC_DATA" == "data" ]; then
        madd ${LABEL_MC_DATA}${LABEL_MACRO_PHYSTUN}AllPlaylists_lead.root ${LABEL_MC_DATA}${LABEL_MACRO_PHYSTUN}*_lead.root
    fi
    echo
    
    cd $OPT_DIR_PHYSTUN_IRON
    echo " Merging PRE-PHYSICS TUNING distributions of all playlists on IRON to directory: " $PWD
    
    if [ "$OPT_MC_DATA" == "mc" ]; then
        madd ${LABEL_MC_DATA}${LABEL_MACRO_PHYSTUN}${LABEL_MODEL}${LABEL_SYST}POTScaled_AllPlaylists_iron.root ${LABEL_MC_DATA}${LABEL_MACRO_PHYSTUN}${LABEL_MODEL}${LABEL_SYST}POTScaled_*_iron.root
        
    elif [ "$OPT_MC_DATA" == "data" ]; then
        madd ${LABEL_MC_DATA}${LABEL_MACRO_PHYSTUN}AllPlaylists_iron.root ${LABEL_MC_DATA}${LABEL_MACRO_PHYSTUN}*_iron.root
    fi
    echo
    
    echo " Done!"
    echo
    
    
    
# Merge files for cut studies
# ===========================

elif [ "$OPT_MACRO" == "2" ]; then
    
    # Distributions on LEAD
    cd $OPT_DIR_LEAD
    echo " Merging cut studies distributions of all playlists on LEAD to directory: " $PWD
    
    if [ "$OPT_MC_DATA" == "mc" ]; then
        madd ${LABEL_MC_DATA}${LABEL_MACRO}${LABEL_MODEL}${LABEL_SYST}POTScaled_AllPlaylists_lead.root ${LABEL_MC_DATA}${LABEL_MACRO}${LABEL_MODEL}${LABEL_SYST}POTScaled_*_lead.root
    
    elif [ "$OPT_MC_DATA" == "data" ]; then
        madd ${LABEL_MC_DATA}${LABEL_MACRO}AllPlaylists_lead.root ${LABEL_MC_DATA}${LABEL_MACRO}*_lead.root
    fi
    echo
    
    echo " Done!"
    echo
    
    
    # Distributions on IRON
    cd $OPT_DIR_IRON
    echo " Merging cut studies distributions of all playlists on IRON to directory: " $PWD
    
    if [ "$OPT_MC_DATA" == "mc" ]; then
        madd ${LABEL_MC_DATA}${LABEL_MACRO}${LABEL_MODEL}${LABEL_SYST}POTScaled_AllPlaylists_iron.root ${LABEL_MC_DATA}${LABEL_MACRO}${LABEL_MODEL}${LABEL_SYST}POTScaled_*_iron.root
    
     elif [ "$OPT_MC_DATA" == "data" ]; then
        madd ${LABEL_MC_DATA}${LABEL_MACRO}AllPlaylists_iron.root ${LABEL_MC_DATA}${LABEL_MACRO}*_iron.root
    fi
    echo
    
    echo " Done!"
    echo
    
    
    
# Merge files for sideband studies
# ================================

elif [ "$OPT_MACRO" == "3" ]; then
    
    # Distributions on LEAD
    cd $OPT_DIR_LEAD
    echo " Merging sideband studies distributions of all playlists on LEAD to directory: " $PWD
    
    if [ "$OPT_MC_DATA" == "mc" ]; then
        madd ${LABEL_MC_DATA}${LABEL_MACRO}${LABEL_MODEL}${LABEL_SYST}POTScaled_AllPlaylists_lead.root ${LABEL_MC_DATA}${LABEL_MACRO}${LABEL_MODEL}${LABEL_SYST}POTScaled_*_lead.root
    
    elif [ "$OPT_MC_DATA" == "data" ]; then
        madd ${LABEL_MC_DATA}${LABEL_MACRO}AllPlaylists_lead.root ${LABEL_MC_DATA}${LABEL_MACRO}*_lead.root
    fi
    echo
    
    echo " Done!"
    echo
    
    
    # Distributions on IRON
    cd $OPT_DIR_IRON
    echo " Merging sideband studies distributions of all playlists on IRON to directory: " $PWD
    
    if [ "$OPT_MC_DATA" == "mc" ]; then
        madd ${LABEL_MC_DATA}${LABEL_MACRO}${LABEL_MODEL}${LABEL_SYST}POTScaled_AllPlaylists_iron.root ${LABEL_MC_DATA}${LABEL_MACRO}${LABEL_MODEL}${LABEL_SYST}POTScaled_*_iron.root
    
     elif [ "$OPT_MC_DATA" == "data" ]; then
        madd ${LABEL_MC_DATA}${LABEL_MACRO}AllPlaylists_iron.root ${LABEL_MC_DATA}${LABEL_MACRO}*_iron.root
    fi
    echo
    
    echo " Done!"
    echo
    
    
    
# Merge files for efficiency studies
# ==================================

elif [ "$OPT_MACRO" == "4" ] || [ "$OPT_MACRO" == "6" ]; then
    
    # Distributions on LEAD
    cd $OPT_DIR_LEAD
    echo " Merging eff. studies distributions of all playlists on LEAD to directory: " $PWD
    
    if [ "$OPT_MC_DATA" == "mc" ]; then
        madd ${LABEL_MC_DATA}${LABEL_MACRO}${LABEL_MODEL}${LABEL_SYST}AllPlaylists_lead.root ${LABEL_MC_DATA}${LABEL_MACRO}${LABEL_MODEL}${LABEL_SYST}*_lead.root
    fi
    
    echo " Done!"
    echo
    
    
    # Distributions on IRON
    cd $OPT_DIR_IRON
    echo " Merging eff. studies distributions of all playlists on IRON to directory: " $PWD
    
    if [ "$OPT_MC_DATA" == "mc" ]; then
        madd ${LABEL_MC_DATA}${LABEL_MACRO}${LABEL_MODEL}${LABEL_SYST}AllPlaylists_iron.root ${LABEL_MC_DATA}${LABEL_MACRO}${LABEL_MODEL}${LABEL_SYST}*_iron.root
    fi
    
    echo " Done!"
    echo
    
    
    
# Merge files for fake data
# =========================

elif [ "$OPT_MACRO" == "5" ]; then
    
    # Distributions on LEAD
    cd $OPT_DIR_LEAD
    echo " Merging fake data distributions of all playlists on LEAD to directory: " $PWD
    
    if [ "$OPT_MC_DATA" == "mc" ]; then
        madd ${LABEL_MC_DATA}${LABEL_MACRO}${LABEL_MODEL}${LABEL_SYST}POTScaled_AllPlaylists_lead.root ${LABEL_MC_DATA}${LABEL_MACRO}${LABEL_MODEL}${LABEL_SYST}POTScaled_*_lead.root
    fi
    echo
    
    echo " Done!"
    echo
    
    
    # Distributions on IRON
    cd $OPT_DIR_IRON
    echo " Merging fake data distributions of all playlists on IRON to directory: " $PWD
    
    if [ "$OPT_MC_DATA" == "mc" ]; then
        madd ${LABEL_MC_DATA}${LABEL_MACRO}${LABEL_MODEL}${LABEL_SYST}POTScaled_AllPlaylists_iron.root ${LABEL_MC_DATA}${LABEL_MACRO}${LABEL_MODEL}${LABEL_SYST}POTScaled_*_iron.root
    fi
    echo
    
    echo " Done!"
    echo
    
    
    
# Merge files for support studies
# ===============================

elif [ "$OPT_MACRO" == "7" ]; then
    
    # Distributions on LEAD
    cd $OPT_DIR_LEAD
    echo " Merging support studies distributions of all playlists on LEAD to directory: " $PWD
    
    if [ "$OPT_MC_DATA" == "mc" ]; then
        madd ${LABEL_MC_DATA}${LABEL_MACRO}${LABEL_MODEL}${LABEL_SYST}POTScaled_AllPlaylists_lead.root ${LABEL_MC_DATA}${LABEL_MACRO}${LABEL_MODEL}${LABEL_SYST}POTScaled_*_lead.root
    
    elif [ "$OPT_MC_DATA" == "data" ]; then
        madd ${LABEL_MC_DATA}${LABEL_MACRO}AllPlaylists_lead.root ${LABEL_MC_DATA}${LABEL_MACRO}*_lead.root
    fi
    echo
    
    echo " Done!"
    echo
    
    
    # Distributions on IRON
    cd $OPT_DIR_IRON
    echo " Merging support studies distributions of all playlists on IRON to directory: " $PWD
    
    if [ "$OPT_MC_DATA" == "mc" ]; then
        madd ${LABEL_MC_DATA}${LABEL_MACRO}${LABEL_MODEL}${LABEL_SYST}POTScaled_AllPlaylists_iron.root ${LABEL_MC_DATA}${LABEL_MACRO}${LABEL_MODEL}${LABEL_SYST}POTScaled_*_iron.root
    
     elif [ "$OPT_MC_DATA" == "data" ]; then
        madd ${LABEL_MC_DATA}${LABEL_MACRO}AllPlaylists_iron.root ${LABEL_MC_DATA}${LABEL_MACRO}*_iron.root
    fi
    echo
    
    echo " Done!"
    echo
    
fi


# Return to $MAT directory
cd $CCPI0MACROS
