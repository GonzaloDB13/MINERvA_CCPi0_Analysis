#!/bin/bash

## ================================= ##
##  Submit merging of files to grid  ##
## ================================= ##

# Monte Carlo or data
OPT_MC_OR_DATA=$1

if [ "$OPT_MC_OR_DATA" != "mc" ] && [ "$OPT_MC_OR_DATA" != "data" ]; then
  echo " Enter 'mc' or 'data' "
  return 1
fi


# Entire playlist or just run(s)?
read -p " Merge an entire playlist? [y/n]: " OPT_MERGEPLAYLIST

if [ "$OPT_MERGEPLAYLIST" = "n" ]; then
  read -p " Merge a group of runs? [y/n]:    " OPT_MERGERUNS
fi


# =============
#  Monte Carlo
# =============

if [ "$OPT_MC_OR_DATA" = "mc" ]; then
  
  # Entire playlist
  # ---------------

  if [ "$OPT_MERGEPLAYLIST" = "y" ]; then
    
    # Ask for inputs
    read -p " Version <vXrYpZ>:        " OPT_VERSION
    read -p " Workspace <Minerva_XYZ>: " OPT_WORKSPACE
    read -p " Playlist:                " OPT_PLAYLIST
    read -p " Detector conf:           " OPT_DETCONF
    read -p " Ana tool:                " OPT_ANATOOL
    read -p " Date:                    " OPT_DATE
    read -p " Memory [Mb] (def = 500): " OPT_MEMORY
    read -p " Disk [Gb] (def = 10):    " OPT_DISK

    #Submit merging
    echo " "
    cd $PLOTUTILS_SCRIPTS
    python submitMergeToGrid.py --mc --release $OPT_VERSION --release_dir $OPT_WORKSPACE --tool $OPT_ANATOOL --inputdir $WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_DETCONF/$OPT_PLAYLIST --outputdir MergedFiles/$WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_DETCONF/$OPT_PLAYLIST --memory $OPT_MEMORY --disk $OPT_DISK --scratch
    cd -
    echo " "

  # Group of runs
  # -------------

  elif [ "$OPT_MERGEPLAYLIST" = "n" ] && [ "$OPT_MERGERUNS" = "y" ]; then
    
    # Ask for inputs
    read -p " Version <vXrYpZ>:        " OPT_VERSION
    read -p " Workspace <Minerva_XYZ>: " OPT_WORKSPACE
    read -p " Playlist:                " OPT_PLAYLIST
    read -p " First run:               " OPT_FIRSTRUN
    read -p " Last run:                " OPT_LASTRUN
    read -p " Detector conf:           " OPT_DETCONF
    read -p " Ana tool:                " OPT_ANATOOL
    read -p " Date:                    " OPT_DATE
    read -p " Memory [Mb] (def = 500): " OPT_MEMORY
    read -p " Disk [Gb] (def = 10):    " OPT_DISK

    # Submit merging
    echo " "
    cd $PLOTUTILS_SCRIPTS
    run_lower=$OPT_FIRSTRUN
    aux=1
    run_upper=`echo "$OPT_LASTRUN - $aux" | bc`
    for run in `seq ${run_lower} ${run_upper}`;
      do python submitMergeToGrid.py --mc --release $OPT_VERSION --release_dir $OPT_WORKSPACE --tool $OPT_ANATOOL --inputdir $WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_DETCONF/$OPT_PLAYLIST/_${OPT_FIRSTRUN}_${OPT_LASTRUN} --outputdir MergedFiles/$WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_DETCONF/$OPT_PLAYLIST --run ${run} --memory $OPT_MEMORY --disk $OPT_DISK --scratch;
    done
    cd -
    echo " "

  # Individual run
  # --------------

  elif [ "$OPT_MERGEPLAYLIST" = "n" ] && [ "$OPT_MERGERUNS" = "n" ]; then
  
    # Ask for inputs
    read -p " Version <vXrYpZ>:        " OPT_VERSION
    read -p " Workspace <Minerva_XYZ>: " OPT_WORKSPACE
    read -p " Playlist:                " OPT_PLAYLIST
    read -p " Run:                     " OPT_RUN
    read -p " Detector conf:           " OPT_DETCONF
    read -p " Ana tool:                " OPT_ANATOOL
    read -p " Date:                    " OPT_DATE
    read -p " Memory [Mb] (def = 500): " OPT_MEMORY
    read -p " Disk [Gb] (def = 10):    " OPT_DISK

    # Submit merging
    echo " "
    cd $PLOTUTILS_SCRIPTS
    python submitMergeToGrid.py --mc --release $OPT_VERSION --release_dir $OPT_WORKSPACE --tool $OPT_ANATOOL --inputdir $WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_DETCONF/$OPT_PLAYLIST --outputdir MergedFiles/$WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_DETCONF/$OPT_PLAYLIST --run $OPT_RUN --memory $OPT_MEMORY --disk $OPT_DISK --scratch
    cd -
    echo " "

  fi


# ======
#  Data
# ======

elif [ "$OPT_MC_OR_DATA" = "data" ]; then
  
  # Entire playlist
  # ---------------

  if [ "$OPT_MERGEPLAYLIST" = "y" ]; then
    
    # Ask for inputs
    read -p " Version <vXrYpZ>:        " OPT_VERSION
    read -p " Workspace <Minerva_XYZ>: " OPT_WORKSPACE
    read -p " Playlist:                " OPT_PLAYLIST
    read -p " Ana tool:                " OPT_ANATOOL
    read -p " Date:                    " OPT_DATE
    read -p " Memory [Mb] (def = 500): " OPT_MEMORY
    read -p " Disk [Gb] (def = 10):    " OPT_DISK

    #Submit merging
    echo " "
    cd $PLOTUTILS_SCRIPTS
    python submitMergeToGrid.py --data --release $OPT_VERSION --release_dir $OPT_WORKSPACE --tool $OPT_ANATOOL --inputdir $WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_PLAYLIST --outputdir MergedFiles/$WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_PLAYLIST --memory $OPT_MEMORY --disk $OPT_DISK --scratch
    cd -
    echo " "

  # Group of runs
  # -------------

  elif [ "$OPT_MERGEPLAYLIST" = "n" ] && [ "$OPT_MERGERUNS" = "y" ]; then
    
    # Ask for inputs
    read -p " Version <vXrYpZ>:        " OPT_VERSION
    read -p " Workspace <Minerva_XYZ>: " OPT_WORKSPACE
    read -p " Playlist:                " OPT_PLAYLIST
    read -p " First run:               " OPT_FIRSTRUN
    read -p " Last run:                " OPT_LASTRUN
    read -p " Ana tool:                " OPT_ANATOOL
    read -p " Date:                    " OPT_DATE
    read -p " Memory [Mb] (def = 500): " OPT_MEMORY
    read -p " Disk [Gb] (def = 10):    " OPT_DISK

    # Submit merging
    echo " "
    cd $PLOTUTILS_SCRIPTS
    for run in `seq ${OPT_FIRSTRUN} ${OPT_LASTRUN}`;
      do python submitMergeToGrid.py --data --release $OPT_VERSION --release_dir $OPT_WORKSPACE --tool $OPT_ANATOOL --inputdir $WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_PLAYLIST/_${OPT_FIRSTRUN}_${OPT_LASTRUN} --outputdir MergedFiles/$WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_PLAYLIST --run ${run} --memory $OPT_MEMORY --disk $OPT_DISK --scratch;
    done
    cd -
    echo " "

  # Individual run
  # --------------
  
  elif [ "$OPT_MERGEPLAYLIST" = "n" ] && [ "$OPT_MERGERUNS" = "n" ]; then
  
    # Ask for inputs
    read -p " Version <vXrYpZ>:        " OPT_VERSION
    read -p " Workspace <Minerva_XYZ>: " OPT_WORKSPACE
    read -p " Playlist:                " OPT_PLAYLIST
    read -p " Run:                     " OPT_RUN
    read -p " Ana tool:                " OPT_ANATOOL
    read -p " Date:                    " OPT_DATE
    read -p " Memory [Mb] (def = 500): " OPT_MEMORY
    read -p " Disk [Gb] (def = 10):    " OPT_DISK

    # Submit merging
    echo " "
    cd $PLOTUTILS_SCRIPTS
    python submitMergeToGrid.py --data --release $OPT_VERSION --release_dir $OPT_WORKSPACE --tool $OPT_ANATOOL --inputdir $WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_PLAYLIST --outputdir MergedFiles/$WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_PLAYLIST --run $OPT_RUN --memory $OPT_MEMORY --disk $OPT_DISK --scratch
    cd -
    echo " "

  fi

fi