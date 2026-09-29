#!/bin/bash

## ===================== ##
##  Submit jobs to grid  ##
## ===================== ##

# Monte Carlo or data
OPT_MC_OR_DATA=$1

if [ "$OPT_MC_OR_DATA" != "mc" ] && [ "$OPT_MC_OR_DATA" != "data" ]; then
  echo " Enter 'mc' or 'data' "
  return 1
fi

# Entire playlist or just run(s)?
read -p " Process an entire playlist? [y/n]: " OPT_Y_OR_N


# =============
#  Monte Carlo
# =============

if [ "$OPT_MC_OR_DATA" = "mc" ]; then
  
  # Entire playlist
  # ---------------

  if [ "$OPT_Y_OR_N" = "y" ]; then
    
    # Ask for inputs
    read -p " Playlist name: " OPT_PLAYLIST
    read -p " Ana tool:      " OPT_ANATOOL
    read -p " Detector conf: " OPT_DETCONF
    read -p " Date:          " OPT_DATE
    read -p " Memory [Mb]:   " OPT_MEMORY

    # Check if output directory already exists in scratch; if so, delete it
    # Format is < $SCRATCH/$WORKSPACE/date/ana_tool/mc/det_conf/playlist >
    if [ -d "$SCRATCH/$WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_DETCONF/$OPT_PLAYLIST" ]; then
      rm -rf $SCRATCH/$WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_DETCONF/$OPT_PLAYLIST
    fi

    #Submit
    echo " "
    cd $PROCESS_ANA
    python ProcessAna.py --mc --playlist $OPT_PLAYLIST --ana_tool $OPT_ANATOOL --$OPT_DETCONF --inv Inextinguishable --kludge Inextinguishable --outdir $SCRATCH/$WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_DETCONF/$OPT_PLAYLIST --memory ${OPT_MEMORY}Mb --os sl7
    cd -
    echo " "

  # Individual run or group of runs
  # -------------------------------

  elif [ "$OPT_Y_OR_N" = "n" ]; then

    # Ask for inputs
    read -p " Playlist name: " OPT_PLAYLIST
    #read -p " <run>/<runslist>: " OPT_RUNS_OR_RUNSLIST
    #read -p " Run number(s):    " OPT_RUNS
    read -p " Run number:    " OPT_RUN
    read -p " Subrun(s):     " OPT_SUBRUN
    read -p " Ana tool:      " OPT_ANATOOL
    read -p " Detector conf: " OPT_DETCONF
    read -p " Date:          " OPT_DATE
    read -p " Memory [Mb]:   " OPT_MEMORY

    # Perform check of existing directory
    if [ -d "$SCRATCH/$WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_DETCONF/$OPT_PLAYLIST" ]; then
      rm -rf $SCRATCH/$WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_DETCONF/$OPT_PLAYLIST
    fi

    # Submit
    echo " "
    cd $PROCESS_ANA
    python ProcessAna.py --mc --playlist $OPT_PLAYLIST --run $OPT_RUN --subrun $OPT_SUBRUN --ana_tool $OPT_ANATOOL --$OPT_DETCONF --inv Inextinguishable --kludge Inextinguishable --outdir $SCRATCH/$WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_DETCONF/$OPT_PLAYLIST --memory ${OPT_MEMORY}Mb --os sl7
    #python ProcessAna.py --mc --playlist $OPT_PLAYLIST --$OPT_RUNS_OR_RUNSLIST $OPT_RUNS --ana_tool $OPT_ANATOOL --$OPT_DETCONF --inv Inextinguishable --kludge Inextinguishable --outdir $SCRATCH/$WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_DETCONF/$OPT_PLAYLIST --memory ${OPT_MEMORY}Mb --os sl7
    cd -
    echo " "

  fi


# ======
#  Data
# ======

elif [ "$OPT_MC_OR_DATA" = "data" ]; then
  
  # Entire playlist
  # ---------------

  if [ "$OPT_Y_OR_N" = "y" ]; then
  
    # Ask for inputs
    read -p " Playlist name: " OPT_PLAYLIST
    read -p " Ana tool:      " OPT_ANATOOL
    read -p " Date:          " OPT_DATE
    read -p " Memory [Mb]:   " OPT_MEMORY
    
    # Check if output directory already exists in dCache; if so, delete it
    # Format is < $SCRATCH/$WORKSPACE/date/ana_tool/data/playlist >
    if [ -d "$SCRATCH/$WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_PLAYLIST" ]; then
      rm -rf $SCRATCH/$WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_PLAYLIST
    fi
    
    # Submit jobs
    echo " "
    cd $PROCESS_ANA
    python ProcessAna.py --data --playlist $OPT_PLAYLIST --ana_tool $OPT_ANATOOL --inv Inextinguishable --kludge Inextinguishable --outdir $SCRATCH/$WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_PLAYLIST --memory ${OPT_MEMORY}Mb --os sl7
    echo " "

  # Individual runs or group of runs
  # --------------------------------

  elif [ "$OPT_Y_OR_N" = "n" ]; then
    
    # Ask for inputs
    read -p " Playlist name: " OPT_PLAYLIST
    #read -p " <run>/<runslist> " OPT_RUNS_OR_RUNSLIST
    #read -p " Run number(s):   " OPT_RUNS
    read -p " Run number:    " OPT_RUN
    read -p " Subrun(s):     " OPT_SUBRUN
    read -p " Ana tool:      " OPT_ANATOOL
    read -p " Date:          " OPT_DATE
    read -p " Memory [Mb]:   " OPT_MEMORY

    # Perform check of existing directory
    if [ -d "$SCRATCH/$WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_PLAYLIST" ]; then
      rm -rf $SCRATCH/$WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_PLAYLIST
    fi

    # Submit jobs
    echo " "
    cd $PROCESS_ANA
    python ProcessAna.py --data --playlist $OPT_PLAYLIST --run $OPT_RUN --subrun $OPT_SUBRUN --ana_tool $OPT_ANATOOL --inv Inextinguishable --kludge Inextinguishable --outdir $SCRATCH/$WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_PLAYLIST --memory ${OPT_MEMORY}Mb --os sl7
    #python ProcessAna.py --data --playlist $OPT_PLAYLIST --$OPT_RUNS_OR_RUNSLIST $OPT_RUNS --ana_tool $OPT_ANATOOL --inv Inextinguishable --kludge Inextinguishable --outdir $SCRATCH/$WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_PLAYLIST --memory ${OPT_MEMORY}Mb --os sl7
    cd -
    echo " "

  fi

fi