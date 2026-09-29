#!/bin/bash

## ================================ ##
##  Submit test jobs interactively  ##
## ================================ ##

# Monte Carlo or data
OPT_MC_OR_DATA=$1

if [ "$OPT_MC_OR_DATA" != "mc" ] && [ "$OPT_MC_OR_DATA" != "data" ]; then
  echo " Enter 'mc' or 'data' "
  return 1
fi


# =============
#  Monte Carlo
# =============

if [ "$OPT_MC_OR_DATA" = "mc" ]; then
  
  # Ask for inputs
  read -p " Playlist:      " OPT_PLAYLIST
  read -p " Run number:    " OPT_RUN
  read -p " Subrun:        " OPT_SUBRUN
  read -p " Ana tool:      " OPT_ANATOOL
  read -p " Detector conf: " OPT_DETCONF

  # Check if test directory already exists in SCRATCH; if so, delete it
  if [ -d "$SCRATCH/interactive_test/$WORKSPACE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_DETCONF/$OPT_PLAYLIST/$OPT_RUN/$OPT_SUBRUN" ]; then
    rm -rf $SCRATCH/interactive_test/$WORKSPACE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_DETCONF/$OPT_PLAYLIST/$OPT_RUN/$OPT_SUBRUN 
  fi
    
  # Submit test job
  echo " "
  cd $PROCESS_ANA
  python ProcessAna.py --mc --playlist $OPT_PLAYLIST --run $OPT_RUN --subrun $OPT_SUBRUN --ana_tool $OPT_ANATOOL --$OPT_DETCONF --inv Inextinguishable --kludge Inextinguishable --outdir $SCRATCH/interactive_test/$WORKSPACE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_DETCONF/$OPT_PLAYLIST/$OPT_RUN/$OPT_SUBRUN --interactive
  echo " "


# ======
#  Data
# ======

elif [ "$OPT_MC_OR_DATA" = "data" ]; then
  
  # Ask for inputs
  read -p " Playlist:   " OPT_PLAYLIST
  read -p " Run number: " OPT_RUN
  read -p " Subrun:     " OPT_SUBRUN
  read -p " Ana tool:   " OPT_ANATOOL

  # Check if test directory already exists in BlueArc; if so, delete it
  if [ -d "$SCRATCH/interactive_test/$WORKSPACE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_PLAYLIST/$OPT_RUN/$OPT_SUBRUN" ]; then
    rm -rf $SCRATCH/interactive_test/$WORKSPACE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_PLAYLIST/$OPT_RUN/$OPT_SUBRUN 
  fi
    
  # Submit test job
  echo " "
  cd $PROCESS_ANA
  python ProcessAna.py --data --playlist $OPT_PLAYLIST --run $OPT_RUN --subrun $OPT_SUBRUN --ana_tool $OPT_ANATOOL --inv Inextinguishable --kludge Inextinguishable --outdir $SCRATCH/interactive_test/$WORKSPACE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_PLAYLIST/$OPT_RUN/$OPT_SUBRUN --interactive
  echo " "

fi