#!/bin/bash

## ================ ##
##  Samweb options  ##
## ================ ##

# Read options
read -p " Select either [prestage], or [check] my projects: " OPTION
#read -p " Want to [prestage], [check] all of my projects, or [show] playsets? " OPTION

if [ "$OPTION" != "prestage" ] && [ "$OPTION" != "check" ]; then
#if [ "$OPTION" != "prestage" ] && [ "$OPTION" != "check" ] && [ "$OPTION" != "show" ]; then
  echo " Enter any valid option! "
  echo
  return 1
fi


# ======== #
# Prestage #
# ======== #

if [ "$OPTION" = "prestage" ]; then

  read -p " [data] or [mc]:  " OPT1a
  read -p " Playlist:        " OPT1b

  if [ "$OPT1a" = "mc" ]; then
    read -p " Detector config: " OPT1c
    samweb prestage-dataset --parallel 10 --defname rodriges_${OPT1a}_reco_${OPT1b}_inextinguishable_${OPT1c}

  elif [ "$OPT1a" = "data" ]; then
    samweb prestage-dataset --parallel 10 --defname rodriges_${OPT1a}_reco_${OPT1b}_inextinguishable

  fi


# ============== #
# Check projects #
# ============== #

elif [ "$OPTION" = "check" ]; then
  samweb list-projects --user gonzalo


## ============= #
## Show playsets #
## ============= #

#elif [ "$OPTION" = "show" ]; then

  #cd /cvmfs/minerva.opensciencegrid.org/minerva/software_releases/v21r1p1/minerva/MINERVA/MINERVA_v21r1p1/Tools/ProductionScripts/production_tools

  #read -p " Show [all] of them, or just the [online] ones? " OPT3a

  #if [ "$OPT3a" != "all" ] && [ "$OPT3a" != "online" ]; then
    #echo " Enter a valid option "
    #return 1

  #elif [ "$OPT3a" = "all" ]; then
    #python samweb_reco_playlists.py show-datasets

  #elif [ "$OPT3a" = "online" ]; then
    #python samweb_reco_playlists.py show-online

  #fi

fi