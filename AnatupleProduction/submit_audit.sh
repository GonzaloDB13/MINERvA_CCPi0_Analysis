#!/bin/bash

## ====================================== ##
##  Submit audit of merged files to grid  ##
## ====================================== ##

# Monte Carlo or data
OPT_MC_OR_DATA=$1

if [ "$OPT_MC_OR_DATA" != "mc" ] && [ "$OPT_MC_OR_DATA" != "data" ]; then
  echo " Enter 'mc' or 'data' "
  return 1
fi

# Entire playlist or just run(s)?
read -p " Audit an entire playlist? [y/n] " OPT_AUDITPLAYLIST

if [ "$OPT_AUDITPLAYLIST" = "n" ]; then
  read -p " Audit a group of runs? [y/n]    " OPT_AUDITRUNS
fi


# =============
#  Monte Carlo
# =============

if [ "$OPT_MC_OR_DATA" = "mc" ]; then

  # Entire playlist
  # ---------------

  if [ "$OPT_AUDITPLAYLIST" = "y" ]; then

    # Ask for inputs
    read -p " Version <vXrYpZ>:        " OPT_VERSION
    read -p " Workspace <Minerva_XYZ>: " OPT_WORKSPACE
    read -p " Playlist:                " OPT_PLAYLIST
    read -p " Detector conf:           " OPT_DETCONF
    read -p " Ana tool:                " OPT_ANATOOL
    read -p " Date:                    " OPT_DATE
    read -p " Memory [Mb] (def = 500): " OPT_MEMORY
    read -p " Disk [Gb] (def = 10):    " OPT_DISK

    # Submit audit
    echo " "
    cd $PLOTUTILS_SCRIPTS
    python submitAuditToGrid.py --mc --release $OPT_VERSION --release_dir $OPT_WORKSPACE --tool $OPT_ANATOOL --unmerged_dir $WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_DETCONF/$OPT_PLAYLIST --merged_dir MergedFiles/$WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_DETCONF/$OPT_PLAYLIST --memory $OPT_MEMORY --disk $OPT_DISK  --scratch
    cd -
    echo " "

  # Group of runs
  # -------------

  elif [ "$OPT_AUDITPLAYLIST" = "n" ] && [ "$OPT_AUDITRUNS" = "y" ]; then

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

    #Submit audit
    echo " "
    cd $PLOTUTILS_SCRIPTS
    run_lower=$OPT_FIRSTRUN
    aux=1
    run_upper=`echo "$OPT_LASTRUN - $aux" | bc`
    for run in `seq ${run_lower} ${run_upper}`;
      do python submitAuditToGrid.py --mc --release $OPT_VERSION --release_dir $OPT_WORKSPACE --tool $OPT_ANATOOL --unmerged_dir $WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_DETCONF/$OPT_PLAYLIST/_${OPT_FIRSTRUN}_${OPT_LASTRUN} --merged_dir MergedFiles/$WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_DETCONF/$OPT_PLAYLIST/${run} --run ${run} --memory $OPT_MEMORY --disk $OPT_DISK --scratch;
    done
    cd -
    echo " "

  # Individual run
  # --------------

  elif [ "$OPT_AUDITPLAYLIST" = "n" ] && [ "$OPT_AUDITRUNS" = "n" ]; then

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

    #Submit audit
    echo " "
    cd $PLOTUTILS_SCRIPTS
    python submitAuditToGrid.py --mc --release $OPT_VERSION --release_dir $OPT_WORKSPACE --tool $OPT_ANATOOL --unmerged_dir $WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_DETCONF/$OPT_PLAYLIST --merged_dir MergedFiles/$WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_DETCONF/$OPT_PLAYLIST/$OPT_RUN --run $OPT_RUN --memory $OPT_MEMORY --disk $OPT_DISK --scratch
    cd -
    echo " "

  fi


# ======
#  Data
# ======

elif [ "$OPT_MC_OR_DATA" = "data" ]; then

  # Entire playlist
  # ---------------

  if [ "$OPT_AUDITPLAYLIST" = "y" ]; then

    # Ask for inputs
    read -p " Version <vXrYpZ>:        " OPT_VERSION
    read -p " Workspace <Minerva_XYZ>: " OPT_WORKSPACE
    read -p " Playlist:                " OPT_PLAYLIST
    read -p " Ana tool:                " OPT_ANATOOL
    read -p " Date:                    " OPT_DATE
    read -p " Memory [Mb] (def = 500): " OPT_MEMORY
    read -p " Disk [Gb] (def = 10):    " OPT_DISK

    # Submit audit
    echo " "
    cd $PLOTUTILS_SCRIPTS
    python submitAuditToGrid.py --data --release $OPT_VERSION --release_dir $OPT_WORKSPACE --tool $OPT_ANATOOL --unmerged_dir $WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_PLAYLIST --merged_dir MergedFiles/$WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_PLAYLIST --memory $OPT_MEMORY --disk $OPT_DISK  --scratch
    cd -
    echo " "

  # Group of runs
  # -------------

  elif [ "$OPT_AUDITPLAYLIST" = "n" ] && [ "$OPT_AUDITRUNS" = "y" ]; then

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

    #Submit audit
    echo " "
    cd $PLOTUTILS_SCRIPTS
    for run in `seq ${OPT_FIRSTRUN} ${OPT_LASTRUN}`;
      do python submitAuditToGrid.py --data --release $OPT_VERSION --release_dir $OPT_WORKSPACE --tool $OPT_ANATOOL --unmerged_dir $WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_PLAYLIST/_${OPT_FIRSTRUN}_${OPT_LASTRUN} --merged_dir MergedFiles/$WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_PLAYLIST/${run} --run ${run} --memory $OPT_MEMORY --disk $OPT_DISK --scratch;
    done
    cd -
    echo " "

  # Individual run
  # --------------

  elif [ "$OPT_AUDITPLAYLIST" = "n" ] && [ "$OPT_AUDITRUNS" = "n" ]; then

    # Ask for inputs
    read -p " Version <vXrYpZ>:        " OPT_VERSION
    read -p " Workspace <Minerva_XYZ>: " OPT_WORKSPACE
    read -p " Playlist:                " OPT_PLAYLIST
    read -p " Run:                     " OPT_RUN
    read -p " Ana tool:                " OPT_ANATOOL
    read -p " Date:                    " OPT_DATE
    read -p " Memory [Mb] (def = 500): " OPT_MEMORY
    read -p " Disk [Gb] (def = 10):    " OPT_DISK

    #Submit audit
    echo " "
    cd $PLOTUTILS_SCRIPTS
    python submitAuditToGrid.py --data --release $OPT_VERSION --release_dir $OPT_WORKSPACE --tool $OPT_ANATOOL --unmerged_dir $WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_PLAYLIST --merged_dir MergedFiles/$WORKSPACE/$OPT_DATE/$OPT_ANATOOL/$OPT_MC_OR_DATA/$OPT_PLAYLIST/$OPT_RUN --run $OPT_RUN --memory $OPT_MEMORY --disk $OPT_DISK --scratch
    cd -
    echo " "

  fi

fi