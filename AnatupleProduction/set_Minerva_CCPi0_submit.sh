#!/bin/bash


## =============================================== ##
##  Setup CCPi0 in NX v22r1p1 for submission only  ##
## =============================================== ##

# Source Inextinguishable
source /cvmfs/minerva.opensciencegrid.org/minerva/software_releases/v22r1p1/setup.sh


# Main workspace
WORKSPACE="Minerva_CCPi0_Submit"
export WORKSPACE


# Set all packages inside that directory
export TOPDIR=$User_release_area$WORKSPACE
cd $TOPDIR

Package_list=(Ana/CCPi0
	      Ana/PlotUtils
	      Ana/MCReweight
	      Ana/AnaUtils
	      Ana/TruthMatcher
	      Rec/KludgeLibrary
	      Rec/ProngMaker
	      Rec/BlobFormation
	      Tools/ProductionScriptsLite
	      Tools/SystemTests)

Default_setup=/cmt/setup.sh

echo " Automatically setting installed packages... "
for i in ${Package_list[@]}; do
  echo " ...setting ${i} "
  source ${i}$Default_setup
done
echo " Done!"
echo " "


# Define other environment variables
export SRC=$CCPI0ROOT/src
export MAT=$CCPI0ROOT/Gonzalo/MAT
export CHW=$CCPI0ROOT/Gonzalo/ChainWrapper
export PROCESS_ANA=$PRODUCTIONSCRIPTSLITEROOT/ana_scripts
export PLOTUTILS_SCRIPTS=$PLOTUTILSROOT/scripts