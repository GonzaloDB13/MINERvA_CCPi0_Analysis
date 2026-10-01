#!bin/bash


# =============
#  GET OPTIONS
# =============

# Date
echo
read -p " Enter date of output directories [YYYY-MM-DD]: " OPT_DATE


# Baseline MC model
echo
read -p " Enter baseline MC model: " OPT_MODEL

if [ "$OPT_MODEL" != "v0" ] && [ "$OPT_MODEL" != "v1" ] &&
   [ "$OPT_MODEL" != "v1noNonResPi" ] && [ "$OPT_MODEL" != "v1noD2" ] && [ "$OPT_MODEL" != "v1noPionTune" ] &&
   [ "$OPT_MODEL" != "v2MINOS" ] && [ "$OPT_MODEL" != "v2JOINT" ] && [ "$OPT_MODEL" != "v2NU1PI" ] &&
   [ "$OPT_MODEL" != "v2NUNPI" ] && [ "$OPT_MODEL" != "v2NUPI0" ] && [ "$OPT_MODEL" != "v2MENU1PI" ]; then
    echo
    echo " ERROR: One of the baseline MC models is not valid!! "
    echo
    return 1
fi


# Ask if doing cut studies
echo
read -p " Doing cut studies? [y/n] " OPT_DO_CUT_STUDIES

if [ "$OPT_DO_CUT_STUDIES" != "y" ] && [ "$OPT_DO_CUT_STUDIES" != "n" ]; then
    echo
    echo " ERROR: Select either 'y' or 'n' "
    echo
    return 1
fi


# Ask if doing sideband studies
echo
read -p " Doing sideband studies? [y/n] " OPT_DO_SIDEBAND_STUDIES

if [ "$OPT_DO_SIDEBAND_STUDIES" != "y" ] && [ "$OPT_DO_SIDEBAND_STUDIES" != "n" ]; then
    echo
    echo " ERROR: Select either 'y' or 'n' "
    echo
    return 1
fi


# Ask if doing efficiency studies
echo
read -p " Doing efficiency studies? [y/n] " OPT_DO_EFF_STUDIES

if [ "$OPT_DO_EFF_STUDIES" != "y" ] && [ "$OPT_DO_EFF_STUDIES" != "n" ]; then
    echo
    echo " ERROR: Select either 'y' or 'n' "
    echo
    return 1
fi


# Ask is doing fake data models distributions from grid, and if so, get models
echo
read -p " Make directory of fake data models from grid? [y/n] " OPT_DO_FAKE_DATA_DISTR

if [ "$OPT_DO_FAKE_DATA_DISTR" != "y" ] && [ "$OPT_DO_FAKE_DATA_DISTR" != "n" ]; then
    echo
    echo " ERROR: Select either 'y' or 'n' "
    echo
    return 1

elif [ "$OPT_DO_FAKE_DATA_DISTR" == "y" ]; then
    echo
    echo " Enter fake data models separated by space: " 
    read -a OPT_FAKE_DATA_DIST_MODELS
    
    for model in ${OPT_FAKE_DATA_DIST_MODELS[@]}; do
        if [ "$model" != "v0" ] && [ "$model" != "v1" ] &&
           [ "$model" != "v1noNonResPi" ] && [ "$model" != "v1noD2" ] && [ "$model" != "v1noPionTune" ] &&
           [ "$model" != "v2MINOS" ] && [ "$model" != "v2JOINT" ] && [ "$model" != "v2NU1PI" ] &&
           [ "$model" != "v2NUNPI" ] && [ "$model" != "v2NUPI0" ] && [ "$model" != "v2MENU1PI" ]; then
            echo
            echo " ERROR: One/some of the fake data model(s) is/are not valid!! "
            echo
            return 1
        fi
    done
fi


# Ask is doing fake data models input/output files for warping studies, and if so, get models
echo
read -p " Make directory of fake data input/output files for warping studies? [y/n] " OPT_DO_FAKE_DATA_WARP

if [ "$OPT_DO_FAKE_DATA_WARP" != "y" ] && [ "$OPT_DO_FAKE_DATA_WARP" != "n" ]; then
    echo
    echo " ERROR: Select either 'y' or 'n' "
    echo
    return 1

elif [ "$OPT_DO_FAKE_DATA_WARP" == "y" ]; then
    echo
    echo " Enter fake data models separated by space: " 
    read -a OPT_FAKE_DATA_WARP_MODELS
    
    for model in ${OPT_FAKE_DATA_WARP_MODELS[@]}; do
        if [ "$OPT_MODEL" != "v0" ] && [ "$OPT_MODEL" != "v1" ] &&
           [ "$OPT_MODEL" != "v1noNonResPi" ] && [ "$OPT_MODEL" != "v1noD2" ] && [ "$OPT_MODEL" != "v1noPionTune" ] &&
           [ "$OPT_MODEL" != "v2MINOS" ] && [ "$OPT_MODEL" != "v2JOINT" ] && [ "$OPT_MODEL" != "v2NU1PI" ] &&
           [ "$OPT_MODEL" != "v2NUNPI" ] && [ "$OPT_MODEL" != "v2NUPI0" ] && [ "$OPT_MODEL" != "v2MENU1PI" ]; then
            echo
            echo " ERROR: One of the fake data models is not valid!! "
            echo
            return 1
        fi
    done
fi


# Ask is doing unfolding statistical studies
echo
read -p " Doing unfolding statistical studies? [y/n] " OPT_DO_UNFOLD_STAT_STUDIES

if [ "$OPT_DO_UNFOLD_STAT_STUDIES" != "y" ] && [ "$OPT_DO_UNFOLD_STAT_STUDIES" != "n" ]; then
    echo
    echo " ERROR: Select either 'y' or 'n' "
    echo
    return 1
fi


# Ask is doing x-section models
echo
read -p " Make directory of x-section models? [y/n] " OPT_DO_XSEC_MODELS

if [ "$OPT_DO_XSEC_MODELS" != "y" ] && [ "$OPT_DO_XSEC_MODELS" != "n" ]; then
    echo
    echo " ERROR: Select either 'y' or 'n' "
    echo
    return 1

elif [ "$OPT_DO_XSEC_MODELS" == "y" ]; then
    echo
    echo " Enter x-section models separated by space: " 
    read -a OPT_XSEC_MODELS
    
    for model in ${OPT_XSEC_MODELS[@]}; do
        if [ "$OPT_MODEL" != "v0" ] && [ "$OPT_MODEL" != "v1" ] &&
           [ "$OPT_MODEL" != "v1noNonResPi" ] && [ "$OPT_MODEL" != "v1noD2" ] && [ "$OPT_MODEL" != "v1noPionTune" ] &&
           [ "$OPT_MODEL" != "v2MINOS" ] && [ "$OPT_MODEL" != "v2JOINT" ] && [ "$OPT_MODEL" != "v2NU1PI" ] &&
           [ "$OPT_MODEL" != "v2NUNPI" ] && [ "$OPT_MODEL" != "v2NUPI0" ] && [ "$OPT_MODEL" != "v2MENU1PI" ] &&
           [ "$OPT_MODEL" != "GENIE3_02a" ] && [ "$OPT_MODEL" != "GENIE3_02b" ] &&
           [ "$OPT_MODEL" != "GENIE3_10a" ] && [ "$OPT_MODEL" != "GENIE3_10b" ] &&
           [ "$OPT_MODEL" != "NEUT_LFG" ]; then
            echo
            echo " ERROR: One of the x-section models is not valid!! "
            echo
            return 1
        fi
    done
fi



# ====================
#  CREATE DIRECTORIES
# ====================


# Create date + baseline model
OPT_OUTDIR_DATE="${OPT_DATE}_${OPT_MODEL}"
echo
echo " Creating directories.... "
echo


# Make POT directories
mkdir -p $BLUEARC/MAT/POTinfo/$OPT_OUTDIR_DATE


# Make fake data directories from grid
if [ "$OPT_DO_FAKE_DATA_DISTR" == "y" ]; then
    for model in ${OPT_FAKE_DATA_DIST_MODELS[@]}; do
    
        mkdir -p $PNFS/MAT/FakeDataModels/${OPT_DATE}_${model}/lead
        mkdir -p $PNFS/MAT/FakeDataModels/${OPT_DATE}_${model}/iron
        
        mkdir -p $BLUEARC/MAT/FakeDataModels/${OPT_DATE}_${model}/POTinfo
    done
fi


# Make unfolding statistical study directories
if [ "$OPT_DO_UNFOLD_STAT_STUDIES" == "y" ]; then
    mkdir -p $PNFS/MAT/UnfoldStatStudies/${OPT_OUTDIR_DATE}/lead/TransWarpOutputs
    mkdir -p $PNFS/MAT/UnfoldStatStudies/${OPT_OUTDIR_DATE}/lead/PlotMCSampleSizeScan
    
    mkdir -p $PNFS/MAT/UnfoldStatStudies/${OPT_OUTDIR_DATE}/iron/TransWarpOutputs
    mkdir -p $PNFS/MAT/UnfoldStatStudies/${OPT_OUTDIR_DATE}/iron/PlotMCSampleSizeScan
    
    mkdir -p $BLUEARC/MAT/UnfoldStatStudies/plots/${OPT_OUTDIR_DATE}/lead
    mkdir -p $BLUEARC/MAT/UnfoldStatStudies/plots/${OPT_OUTDIR_DATE}/iron
    
    mkdir -p $BLUEARC/MAT/UnfoldStatStudies/ProcessMCSampleSizeScan/${OPT_OUTDIR_DATE}/lead
    mkdir -p $BLUEARC/MAT/UnfoldStatStudies/ProcessMCSampleSizeScan/${OPT_OUTDIR_DATE}/iron
fi


# Loop over materials and make more diretories
declare -a materials=("lead" "iron")

for material in "${materials[@]}"; do

    # Cut studies
    # ===========
    
    if [ "$OPT_DO_CUT_STUDIES" == "y" ]; then
    
        mkdir -p $BLUEARC/MAT/CutStudies/Tables/$OPT_OUTDIR_DATE/${material}
        
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/08-Michel
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/09-Track
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/10-AngleScan
        
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/11-BlobAngleWRTMuon/SingleBlob
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/11-BlobAngleWRTMuon/MultiBlob
        
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/12-BlobDeviation/SingleBlob/ProjDeviation
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/12-BlobDeviation/SingleBlob/AngleDeviation
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/12-BlobDeviation/SingleBlob/AngleDeviationVSProjDeviation
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/12-BlobDeviation/MultiBlob/ProjDeviation
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/12-BlobDeviation/MultiBlob/AngleDeviation
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/12-BlobDeviation/MultiBlob/AngleDeviationVSProjDeviation
        
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/13-BlobEcaloVSdx/SingleBlob/Ecalo
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/13-BlobEcaloVSdx/SingleBlob/dx
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/13-BlobEcaloVSdx/SingleBlob/dEdxMean
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/13-BlobEcaloVSdx/SingleBlob/dEdxFront
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/13-BlobEcaloVSdx/SingleBlob/dEdxEnd
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/13-BlobEcaloVSdx/SingleBlob/EcaloVSdx
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/13-BlobEcaloVSdx/SingleBlob/dEdxMeanVSEcalo
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/13-BlobEcaloVSdx/SingleBlob/dEdxFrontVSEcalo
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/13-BlobEcaloVSdx/SingleBlob/dEdxEndVSEcalo
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/13-BlobEcaloVSdx/MultiBlob/Ecalo
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/13-BlobEcaloVSdx/MultiBlob/dx
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/13-BlobEcaloVSdx/MultiBlob/dEdxMean
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/13-BlobEcaloVSdx/MultiBlob/dEdxFront
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/13-BlobEcaloVSdx/MultiBlob/dEdxEnd
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/13-BlobEcaloVSdx/MultiBlob/EcaloVSdx
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/13-BlobEcaloVSdx/MultiBlob/dEdxMeanVSEcalo
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/13-BlobEcaloVSdx/MultiBlob/dEdxFrontVSEcalo
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/13-BlobEcaloVSdx/MultiBlob/dEdxEndVSEcalo
        
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/14-BlobdEdxEnd/SingleBlob/Ecalo
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/14-BlobdEdxEnd/SingleBlob/dx
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/14-BlobdEdxEnd/SingleBlob/dEdxMean
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/14-BlobdEdxEnd/SingleBlob/dEdxFront
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/14-BlobdEdxEnd/SingleBlob/dEdxEnd
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/14-BlobdEdxEnd/SingleBlob/EcaloVSdx
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/14-BlobdEdxEnd/SingleBlob/dEdxMeanVSEcalo
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/14-BlobdEdxEnd/SingleBlob/dEdxFrontVSEcalo
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/14-BlobdEdxEnd/SingleBlob/dEdxEndVSEcalo
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/14-BlobdEdxEnd/MultiBlob/Ecalo
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/14-BlobdEdxEnd/MultiBlob/dx
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/14-BlobdEdxEnd/MultiBlob/dEdxMean
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/14-BlobdEdxEnd/MultiBlob/dEdxFront
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/14-BlobdEdxEnd/MultiBlob/dEdxEnd
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/14-BlobdEdxEnd/MultiBlob/EcaloVSdx
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/14-BlobdEdxEnd/MultiBlob/dEdxMeanVSEcalo
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/14-BlobdEdxEnd/MultiBlob/dEdxFrontVSEcalo
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/14-BlobdEdxEnd/MultiBlob/dEdxEndVSEcalo
        
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/14-5-AfterEnergyCuts/SingleBlob/Ecalo
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/14-5-AfterEnergyCuts/SingleBlob/dx
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/14-5-AfterEnergyCuts/SingleBlob/dEdxMean
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/14-5-AfterEnergyCuts/SingleBlob/dEdxFront
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/14-5-AfterEnergyCuts/SingleBlob/dEdxEnd
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/14-5-AfterEnergyCuts/SingleBlob/EcaloVSdx
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/14-5-AfterEnergyCuts/SingleBlob/dEdxMeanVSEcalo
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/14-5-AfterEnergyCuts/SingleBlob/dEdxFrontVSEcalo
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/14-5-AfterEnergyCuts/SingleBlob/dEdxEndVSEcalo
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/14-5-AfterEnergyCuts/MultiBlob/Ecalo
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/14-5-AfterEnergyCuts/MultiBlob/dx
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/14-5-AfterEnergyCuts/MultiBlob/dEdxMean
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/14-5-AfterEnergyCuts/MultiBlob/dEdxFront
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/14-5-AfterEnergyCuts/MultiBlob/dEdxEnd
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/14-5-AfterEnergyCuts/MultiBlob/EcaloVSdx
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/14-5-AfterEnergyCuts/MultiBlob/dEdxMeanVSEcalo
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/14-5-AfterEnergyCuts/MultiBlob/dEdxFrontVSEcalo
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/14-5-AfterEnergyCuts/MultiBlob/dEdxEndVSEcalo
        
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/15-BlobMichel/SingleBlob/StartPoint
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/15-BlobMichel/SingleBlob/EndPoint
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/15-BlobMichel/MultiBlob/StartPoint
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/15-BlobMichel/MultiBlob/EndPoint
        
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/16-HadronicEnergy/SingleBlob/NoPi0RecoilE
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/16-HadronicEnergy/SingleBlob/RecoilE
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/16-HadronicEnergy/SingleBlob/Q2
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/16-HadronicEnergy/SingleBlob/W2
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/16-HadronicEnergy/SingleBlob/W
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/16-HadronicEnergy/MultiBlob/NoPi0RecoilE
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/16-HadronicEnergy/MultiBlob/RecoilE
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/16-HadronicEnergy/MultiBlob/Q2
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/16-HadronicEnergy/MultiBlob/W2
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/16-HadronicEnergy/MultiBlob/W
        
        mkdir -p $BLUEARC/MAT/CutStudies/plots/$OPT_OUTDIR_DATE/${material}/AllCuts
    fi
    
    
    # Sideband studies
    # ================
    
    if [ "$OPT_DO_SIDEBAND_STUDIES" == "y" ]; then
        
        mkdir -p $BLUEARC/MAT/SidebandStudies/plots/$OPT_OUTDIR_DATE/${material}/Configuration1
        mkdir -p $BLUEARC/MAT/SidebandStudies/plots/$OPT_OUTDIR_DATE/${material}/Configuration2
        mkdir -p $BLUEARC/MAT/SidebandStudies/plots/$OPT_OUTDIR_DATE/${material}/Configuration3
        mkdir -p $BLUEARC/MAT/SidebandStudies/plots/$OPT_OUTDIR_DATE/${material}/Configuration4
    fi
    
    
    # Efficiency studies
    # ==================
    
    if [ "$OPT_DO_EFF_STUDIES" == "y" ]; then
    
        mkdir -p $PNFS/MAT/EfficiencyStudies/$OPT_OUTDIR_DATE/${material}
        
        mkdir -p $BLUEARC/MAT/EfficiencyStudies/plots/$OPT_OUTDIR_DATE/${material}/EffNumeratorOptimize
        mkdir -p $BLUEARC/MAT/EfficiencyStudies/plots/$OPT_OUTDIR_DATE/${material}/EffDenominatorOptimize
        mkdir -p $BLUEARC/MAT/EfficiencyStudies/plots/$OPT_OUTDIR_DATE/${material}/EfficiencyOptimize
    fi
    
    
    # Event selection
    # ===============

    mkdir -p $PNFS/MAT/EventSelection/mc/$OPT_OUTDIR_DATE/${material}
    mkdir -p $PNFS/MAT/EventSelection/data/$OPT_DATE/${material}
    
    mkdir -p $BLUEARC/MAT/EventSelection/plots/$OPT_OUTDIR_DATE/${material}/EventSelection
    mkdir -p $BLUEARC/MAT/EventSelection/plots/$OPT_OUTDIR_DATE/${material}/EffNumerator
    mkdir -p $BLUEARC/MAT/EventSelection/plots/$OPT_OUTDIR_DATE/${material}/EffDenominator
    mkdir -p $BLUEARC/MAT/EventSelection/plots/$OPT_OUTDIR_DATE/${material}/Efficiency
    
    
    # Plastic tuning
    # ==============
    
    mkdir -p $PNFS/MAT/PlasticTuning/mc/$OPT_OUTDIR_DATE/${material}/BeforeTuning
    mkdir -p $PNFS/MAT/PlasticTuning/mc/$OPT_OUTDIR_DATE/${material}/AfterTuning
    
    mkdir -p $PNFS/MAT/PlasticTuning/data/$OPT_DATE/${material}
    
    mkdir -p $BLUEARC/MAT/PlasticTuning/plots/$OPT_OUTDIR_DATE/${material}/BeforeTuning/SigReg
    mkdir -p $BLUEARC/MAT/PlasticTuning/plots/$OPT_OUTDIR_DATE/${material}/BeforeTuning/ProtonBlobSB
    mkdir -p $BLUEARC/MAT/PlasticTuning/plots/$OPT_OUTDIR_DATE/${material}/BeforeTuning/PionBlobSB
    mkdir -p $BLUEARC/MAT/PlasticTuning/plots/$OPT_OUTDIR_DATE/${material}/BeforeTuning/HighWSB
    mkdir -p $BLUEARC/MAT/PlasticTuning/plots/$OPT_OUTDIR_DATE/${material}/AfterTuning/SigReg
    mkdir -p $BLUEARC/MAT/PlasticTuning/plots/$OPT_OUTDIR_DATE/${material}/AfterTuning/ProtonBlobSB
    mkdir -p $BLUEARC/MAT/PlasticTuning/plots/$OPT_OUTDIR_DATE/${material}/AfterTuning/PionBlobSB
    mkdir -p $BLUEARC/MAT/PlasticTuning/plots/$OPT_OUTDIR_DATE/${material}/AfterTuning/HighWSB
    
    mkdir -p $BLUEARC/MAT/PlasticTuning/ScaleFunctions/$OPT_OUTDIR_DATE/${material}
    
    
    # Physics tuning
    # ==============
    
    mkdir -p $PNFS/MAT/PhysicsTuning/mc/$OPT_OUTDIR_DATE/${material}/BeforeTuning
    mkdir -p $PNFS/MAT/PhysicsTuning/mc/$OPT_OUTDIR_DATE/${material}/AfterTuning
    
    mkdir -p $PNFS/MAT/PhysicsTuning/data/$OPT_DATE/${material}
    
    mkdir -p $BLUEARC/MAT/PhysicsTuning/plots/$OPT_OUTDIR_DATE/${material}/BeforeTuning/SigReg
    mkdir -p $BLUEARC/MAT/PhysicsTuning/plots/$OPT_OUTDIR_DATE/${material}/BeforeTuning/ProtonBlobSB
    mkdir -p $BLUEARC/MAT/PhysicsTuning/plots/$OPT_OUTDIR_DATE/${material}/BeforeTuning/PionBlobSB
    mkdir -p $BLUEARC/MAT/PhysicsTuning/plots/$OPT_OUTDIR_DATE/${material}/BeforeTuning/HighWSB
    mkdir -p $BLUEARC/MAT/PhysicsTuning/plots/$OPT_OUTDIR_DATE/${material}/AfterPlasticTuning/SigReg
    mkdir -p $BLUEARC/MAT/PhysicsTuning/plots/$OPT_OUTDIR_DATE/${material}/AfterPlasticTuning/ProtonBlobSB
    mkdir -p $BLUEARC/MAT/PhysicsTuning/plots/$OPT_OUTDIR_DATE/${material}/AfterPlasticTuning/PionBlobSB
    mkdir -p $BLUEARC/MAT/PhysicsTuning/plots/$OPT_OUTDIR_DATE/${material}/AfterPlasticTuning/HighWSB
    mkdir -p $BLUEARC/MAT/PhysicsTuning/plots/$OPT_OUTDIR_DATE/${material}/AfterPhysBackgrTuning/SigReg
    mkdir -p $BLUEARC/MAT/PhysicsTuning/plots/$OPT_OUTDIR_DATE/${material}/AfterPhysBackgrTuning/ProtonBlobSB
    mkdir -p $BLUEARC/MAT/PhysicsTuning/plots/$OPT_OUTDIR_DATE/${material}/AfterPhysBackgrTuning/PionBlobSB
    mkdir -p $BLUEARC/MAT/PhysicsTuning/plots/$OPT_OUTDIR_DATE/${material}/AfterPhysBackgrTuning/HighWSB
    mkdir -p $BLUEARC/MAT/PhysicsTuning/plots/$OPT_OUTDIR_DATE/${material}/AfterSignalTuning/SigReg
    mkdir -p $BLUEARC/MAT/PhysicsTuning/plots/$OPT_OUTDIR_DATE/${material}/AfterSignalTuning/ProtonBlobSB
    mkdir -p $BLUEARC/MAT/PhysicsTuning/plots/$OPT_OUTDIR_DATE/${material}/AfterSignalTuning/PionBlobSB
    mkdir -p $BLUEARC/MAT/PhysicsTuning/plots/$OPT_OUTDIR_DATE/${material}/AfterSignalTuning/HighWSB
    
    mkdir -p $BLUEARC/MAT/PhysicsTuning/ScaleFunctions/$OPT_OUTDIR_DATE/${material}
    
    
    # Pre cross-section extraction
    # ============================
    
    mkdir -p $PNFS/MAT/CrossSectionExtraction/$OPT_OUTDIR_DATE/${material}
    
    mkdir -p $BLUEARC/MAT/ExtractionInfo/$OPT_OUTDIR_DATE/${material}
    
    mkdir -p $BLUEARC/MAT/PreCrossSectionExtraction/plots/$OPT_OUTDIR_DATE/${material}/BeforeBackgrTuning
    mkdir -p $BLUEARC/MAT/PreCrossSectionExtraction/plots/$OPT_OUTDIR_DATE/${material}/AfterBackgrTuning
    
    
    # Background subtraction
    # ======================
    
    mkdir -p $BLUEARC/MAT/BackgroundSubtraction/plots/$OPT_OUTDIR_DATE/${material}/BackgrSubtracted
    
    
    # Warping studies
    # ===============
    
    mkdir -p $PNFS/MAT/WarpingStudies/$OPT_OUTDIR_DATE/${material}/TransWarpInputs
    mkdir -p $PNFS/MAT/WarpingStudies/$OPT_OUTDIR_DATE/${material}/TransWarpOutputs
    
    if [ "$OPT_DO_FAKE_DATA_WARP" == "y" ]; then
        for model in ${OPT_FAKE_DATA_WARP_MODELS[@]}; do
            mkdir -p $BLUEARC/MAT/WarpingStudies/plots/$OPT_OUTDIR_DATE/${material}/FakeData${model}
        done
    fi
    
    
    # Unfolding
    # =========
    
    mkdir -p $BLUEARC/MAT/Unfolding/plots/$OPT_OUTDIR_DATE/${material}/Folded
    mkdir -p $BLUEARC/MAT/Unfolding/plots/$OPT_OUTDIR_DATE/${material}/Unfolded
    mkdir -p $BLUEARC/MAT/Unfolding/plots/$OPT_OUTDIR_DATE/${material}/Covariance
    mkdir -p $BLUEARC/MAT/Unfolding/plots/$OPT_OUTDIR_DATE/${material}/Correlation
    
    
    # # Efficiency correction
    # # =====================
    
    mkdir -p $BLUEARC/MAT/EfficiencyCorrection/plots/$OPT_OUTDIR_DATE/${material}/EffCorrected
    mkdir -p $BLUEARC/MAT/EfficiencyCorrection/plots/$OPT_OUTDIR_DATE/${material}/Efficiency
    
    
    # Cross section
    # =============
    
    mkdir -p $BLUEARC/MAT/CrossSection/plots/$OPT_OUTDIR_DATE/${material}/CrossSection
    mkdir -p $BLUEARC/MAT/CrossSection/plots/$OPT_OUTDIR_DATE/${material}/ErrorGroups
    mkdir -p $BLUEARC/MAT/CrossSection/plots/$OPT_OUTDIR_DATE/${material}/Covariance
    mkdir -p $BLUEARC/MAT/CrossSection/plots/$OPT_OUTDIR_DATE/${material}/Correlation
    mkdir -p $BLUEARC/MAT/CrossSection/plots/$OPT_OUTDIR_DATE/${material}/Flux
    
    
    # X-section models
    # ================
    
    mkdir -p $PNFS/MAT/XsectionModels/$OPT_OUTDIR_DATE/${material}
    
    mkdir -p $BLUEARC/MAT/XsectionModels/Chi2Info/$OPT_OUTDIR_DATE/${material}
    
    if [ "$OPT_DO_XSEC_MODELS" == "y" ]; then
        for model in ${OPT_XSEC_MODELS[@]}; do
            mkdir -p $BLUEARC/MAT/XsectionModels/plots/$OPT_OUTDIR_DATE/${material}/${model}
        done
    fi
    
done